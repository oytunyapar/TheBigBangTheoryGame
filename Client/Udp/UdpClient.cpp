//
// Created by oytun on 31.01.2022.
//
#include "UdpClient.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <chrono>
#include <system_error>

using namespace std;

UdpClient::UdpClient(uint16_t port, string ip4_address):
    m_socket_fd(0),
    m_server_address{0,0,0,0},
    m_reader_thread_active(false)
{
    m_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if ( m_socket_fd < 0 ) {
        throw system_error(ENOTSOCK,std::generic_category(), "Socket could not be opened.");
    }

    memset(&m_server_address, 0, sizeof(m_server_address));
    m_server_address.sin_family = AF_INET;
    m_server_address.sin_port = htons(port);
    m_server_address.sin_addr.s_addr = inet_addr(ip4_address.c_str());
}

UdpClient::~UdpClient()
{
    m_reader_thread_active = false;

    if(m_socket_fd)
    {
        close(m_socket_fd);
    }

    if(m_reader_thread.joinable())
    {
        m_reader_thread.join();
    }
}

bool UdpClient::write(const char* data, size_t data_size)
{
    if(data == nullptr || data_size > m_max_udp_packet_size || data_size <= 0)
    {
        return false;
    }

    auto sent_size = sendto(m_socket_fd, (const char *)data, data_size,
           MSG_CONFIRM, (const struct sockaddr *) &m_server_address,
           sizeof(m_server_address));

    return data_size == sent_size;
}

void UdpClient::setReadCallback(CallbackType callback)
{
    if(m_reader_thread_active)
    {
        return;
    }

    m_reader_thread_active = true;
    m_callback = callback;

    auto thread_function = [this]()
    {
        socklen_t address_length = 0;
        while(m_reader_thread_active) {
            auto received_size = recvfrom(m_socket_fd, (char *) m_receive_buffer, m_max_udp_packet_size,
                                          MSG_DONTWAIT, (struct sockaddr *) &m_server_address, &address_length);

            if(received_size > 0)
            {
                vector<char> data(received_size);

                for (int counter = 0; counter < received_size; counter++) {
                    data[counter] = m_receive_buffer[counter];
                }

                m_callback(std::move(data));
            }

            this_thread::sleep_for(chrono::milliseconds(10));
        }
    };

    m_reader_thread = thread(thread_function);
}
