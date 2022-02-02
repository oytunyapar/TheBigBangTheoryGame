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

using CallbackType = function<void(vector<char>)>;

class UdpClient
{
public:
    UdpClient(uint16_t port, string ip4_address);
    ~UdpClient();

    bool write(const char* data, size_t data_size);
    void setReadCallback(CallbackType callback);

private:
    static const auto m_max_udp_packet_size = 65000;
    char m_receive_buffer[m_max_udp_packet_size];
    int m_socket_fd;
    CallbackType m_callback;
    sockaddr_in m_server_address;

    bool m_reader_thread_active;
    std::thread m_reader_thread;
};
