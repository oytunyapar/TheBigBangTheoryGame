//
// Created by oytun on 31.01.2022.
//
#include "UdpServer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <iostream>
#include <system_error>

using namespace std;

UdpServer::UdpServer(const uint16_t port):
        m_socket_fd(0),
        m_port(port),
        m_server_thread_active(false)
{
    memset(&m_client_address, 0, sizeof(m_client_address));
    m_reply_buffer.reserve(m_max_udp_packet_size);
    createSocket();
    bindSocket();
    createServerApplication();
}

UdpServer::~UdpServer()
{
    m_server_thread_active = false;

    if(m_socket_fd)
    {
        close(m_socket_fd);
    }

    if(m_server_thread.joinable())
    {
        m_server_thread.join();
    }
}

void UdpServer::createSocket()
{
    m_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if ( m_socket_fd < 0 ) {
        throw system_error(ENOTSOCK,std::generic_category(), "Socket could not be opened.");
    }
}

void UdpServer::bindSocket()
{
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(m_port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    auto bind_result = bind(m_socket_fd, (const struct sockaddr *)&server_address,sizeof(server_address));
    if (  bind_result < 0 )
    {
        throw system_error(ENOTSOCK,std::generic_category(), "Socket bind operation failed.");
    }
}

bool UdpServer::write(const char* data, size_t data_size, pair<uint32_t,uint16_t>& client)
{
    if(data_size > m_max_udp_packet_size || m_client_address_length <= 0)
    {
        return false;
    }

    sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));

    client_address.sin_family = AF_INET;
    client_address.sin_port = client.second;
    client_address.sin_addr.s_addr = client.first;

    auto sent_size = sendto(m_socket_fd, (const char *)data, data_size,
                            MSG_CONFIRM, (const struct sockaddr *) &client,
                            sizeof(client));

    return data_size == sent_size;
}

bool UdpServer::bufferToVector(vector<char>& data)
{
    auto data_size = data.size();

    if(data_size > m_max_udp_packet_size || data_size == 0)
    {
        return false;
    }

    for (int counter = 0; counter < data_size; counter++) {
        data[counter] = m_buffer[counter];
    }

    return true;
}

bool UdpServer::vectorToBuffer(const vector<char>& data)
{
    auto data_size = data.size();

    if(data_size > m_max_udp_packet_size || data_size == 0)
    {
        return false;
    }

    for (int counter = 0; counter < data_size; counter++) {
        m_buffer[counter] = data[counter];
    }

    return true;
}

void UdpServer::processRequest(vector<char>& data, pair<uint32_t,uint16_t> sender)
{
    const auto data_size = data.size();

    if(data_size == 1)
    {
        if(data[0] == JOIN)
        {
            joinClient(sender);
        }
        else if (data[0] == LEAVE)
        {
            leaveClient(sender);
        }
    }
    else if (data_size == 2)
    {
        if (data[0] == CHOICE)
        {
            auto result= m_game_sessions[m_game_session_ids[sender]].playerChoice(sender, static_cast<GameChoices>(data[1]));
            gameResult(result);
        }
    }
}

void UdpServer::joinClient(pair<uint32_t,uint16_t> sender)
{
    if(m_player_timeouts.find(sender) == m_player_timeouts.end())
    {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        m_player_timeouts.emplace(sender, millis);

        m_waiting_queue.emplace(sender);

        if(m_waiting_queue.size() > 1)
        {
            auto first_player = m_waiting_queue.front();
            m_waiting_queue.pop();
            auto second_player = m_waiting_queue.front();
            m_waiting_queue.pop();

            GameSession gameSession(first_player, second_player);
            m_game_session_ids.emplace(first_player,m_last_game_session_id);
            m_game_session_ids.emplace(second_player,m_last_game_session_id);
            m_game_sessions.emplace(m_last_game_session_id, gameSession);

            char gameStarted = GAME_STARTED;
            write(&gameStarted, 1, first_player);
            write(&gameStarted, 1, second_player);
        }
    }
}

void UdpServer::leaveClient(pair<uint32_t,uint16_t> sender)
{
    if(m_player_timeouts.find(sender) != m_player_timeouts.end())
    {
        m_player_timeouts.erase(sender);
    }
}

void UdpServer::gameResult(GameResultObject result)
{

}

void UdpServer::createServerApplication()
{
    if(m_server_thread_active)
    {
        return;
    }

    m_server_thread_active = true;

    auto thread_function = [this]()
    {
        while(m_server_thread_active) {
            auto received_size = recvfrom(m_socket_fd, (char *) m_buffer, m_max_udp_packet_size,
                                          MSG_DONTWAIT, (struct sockaddr *) &m_client_address, &m_client_address_length);

            if (received_size > 0) {
                cout << "IP:" << inet_ntoa(m_client_address.sin_addr) << " PORT:" << ntohs(m_client_address.sin_port) << endl;

                vector<char> data(received_size);
                pair<uint32_t,uint16_t> sender{m_client_address.sin_addr.s_addr, m_client_address.sin_port};
                if (bufferToVector(data)) {
                    processRequest(data, sender);
                }
            }

            this_thread::sleep_for(chrono::milliseconds(10));
        }
    };

    m_server_thread = thread(thread_function);
}
