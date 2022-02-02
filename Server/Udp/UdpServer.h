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

using namespace std;

using ServerApplicationType = function<void(vector<char>, vector<char>&)>;

class UdpServer
{
public:
    UdpServer(const uint16_t port, ServerApplicationType serverApplication);
    ~UdpServer();

private:
    uint16_t m_port;
    static const auto m_max_udp_packet_size = 65000;
    char m_buffer[m_max_udp_packet_size];
    vector<char> m_reply_buffer;

    int m_socket_fd;
    ServerApplicationType m_callback;
    sockaddr_in m_client_address;
    socklen_t m_client_address_length = sizeof(sockaddr_in);

    bool m_server_thread_active;
    std::thread m_server_thread;

    bool write(const char* data, size_t data_size);
    void createSocket();
    void bindSocket();
    void createServerApplication();

    bool bufferToVector(vector<char>& data);
    bool vectorToBuffer(const vector<char>& data);

};
