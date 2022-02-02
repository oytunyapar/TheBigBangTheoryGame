//
// Created by oytun on 31.01.2022.
//
#pragma once

#include <cstdint>
#include <thread>
#include <string>
#include <functional>
#include <vector>

#include <netinet/in.h>

#include <map>
#include <chrono>
#include <queue>

#include "GameSession.h"
#include "MessageTypes.h"

using namespace std;

class UdpServer
{
public:
    UdpServer(const uint16_t port);
    ~UdpServer();

private:
    uint16_t m_port;
    static const auto m_max_udp_packet_size = 65000;
    char m_buffer[m_max_udp_packet_size];
    vector<char> m_reply_buffer;

    int m_socket_fd;
    sockaddr_in m_client_address;
    socklen_t m_client_address_length = sizeof(sockaddr_in);

    queue<pair<uint32_t,uint16_t>> m_waiting_queue;
    map<pair<uint32_t,uint16_t>, chrono::milliseconds> m_player_timeouts;

    uint32_t m_last_game_session_id = 0;
    map<pair<uint32_t,uint16_t>, uint32_t> m_game_session_ids;
    map<uint32_t, GameSession> m_game_sessions;

    chrono::milliseconds m_player_timeout;

    bool m_server_thread_active;
    std::thread m_server_thread;

    void processRequest(vector<char>& data, pair<uint32_t,uint16_t> sender);

    void joinClient(pair<uint32_t,uint16_t> sender);
    void leaveClient(pair<uint32_t,uint16_t> sender);
    void gameResult(GameResultObject result);

    bool write(const char* data, size_t data_size, pair<uint32_t,uint16_t>& sender);
    void createSocket();
    void bindSocket();
    void createServerApplication();

    bool bufferToVector(vector<char>& data);
    bool vectorToBuffer(const vector<char>& data);

};
