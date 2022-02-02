#include <iostream>
#include <chrono>
#include "Client/Udp/UdpClient.h"
#include "Server/Udp/UdpServer.h"

using namespace std;

int main() {
    const uint16_t port = 50000;

    auto server_application = [](vector<char> received_data, vector<char>& reply)
    {
        auto received_data_size = received_data.size();
        auto reply_capacity = reply.capacity();
        if(received_data_size > 0 && reply_capacity > 0)
        {
            if(received_data[0] == 'a')
            {
                reply.emplace_back('b');
            }
            else if (received_data[0] == 'b')
            {
                reply.emplace_back('a');
            }
        }
    };

    UdpServer udpServer(port);
    UdpClient udpClient(port,"127.0.0.1");

    const auto buffer_size = 10;
    char* buffer = new char[buffer_size];
    buffer[0] = 'a';
    buffer[1] = 'b';

    auto clientRead = [](vector<char> data){ cout << "Reply received:" << data[0] << endl;};
    udpClient.setReadCallback(clientRead);

    this_thread::sleep_for(chrono::seconds (1));

    udpClient.write(buffer, buffer_size);

    this_thread::sleep_for(chrono::seconds (1));

    buffer[0] = 'b';
    buffer[1] = 'a';
    udpClient.write(buffer, buffer_size);
    this_thread::sleep_for(chrono::seconds (1));

    buffer[0] = 'c';
    buffer[1] = 'a';
    udpClient.write(buffer, buffer_size);
    this_thread::sleep_for(chrono::seconds (1));

    delete buffer;

    return 0;
}
