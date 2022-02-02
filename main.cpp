#include <iostream>
#include <chrono>
#include "Client/Udp/UdpClient.h"
#include "Server/Udp/UdpServer.h"

using namespace std;

int main() {
    const uint16_t port = 50000;

    const auto buffer_size = 2;
    char* buffer = new char[buffer_size];

    UdpServer udpServer(port);
    UdpClient firstPlayer(port,"127.0.0.1");
    UdpClient secondPlayer(port,"127.0.0.1");

    bool firstGameStarted = false;
    char firstResult;
    auto firstPlayerRead = [&firstGameStarted, &firstResult](vector<char> data){
        if(data[0] == GAME_STARTED)
        {
            firstGameStarted = true;
        }
        else if(data[0] == RESULT)
        {
            firstResult = data[1];
        }
    };
    firstPlayer.setReadCallback(firstPlayerRead);

    bool secondGameStarted = false;
    char secondResult;
    auto secondPlayerRead = [&secondGameStarted, &secondResult](vector<char> data){
        if(data[0] == GAME_STARTED)
        {
            secondGameStarted = true;
        }
        else if(data[0] == RESULT)
        {
            secondResult = data[1];
        }
    };
    secondPlayer.setReadCallback(secondPlayerRead);

    this_thread::sleep_for(chrono::seconds (1));

    buffer[0] = JOIN;
    firstPlayer.write(buffer, 1);
    secondPlayer.write(buffer, 1);

    this_thread::sleep_for(chrono::seconds (1));

    if(firstGameStarted && secondGameStarted)
    {
        cout << "Game started" << endl;
        buffer[0] = CHOICE;
        buffer[1] = SCISSORS;
        firstPlayer.write(buffer, 2);

        buffer[0] = CHOICE;
        buffer[1] = PAPER;
        secondPlayer.write(buffer, 2);

        this_thread::sleep_for(chrono::seconds (1));

        if(firstResult == WIN)
        {
            cout << "First wins" << endl;
        }

        if(secondResult == LOST)
        {
            cout << "Second lost" << endl;
        }

        cout << "Game started" << endl;
        buffer[0] = CHOICE;
        buffer[1] = SCISSORS;
        firstPlayer.write(buffer, 2);

        buffer[0] = CHOICE;
        buffer[1] = SPOCK;
        secondPlayer.write(buffer, 2);

        this_thread::sleep_for(chrono::seconds (1));

        if(firstResult == LOST)
        {
            cout << "First lost" << endl;
        }

        if(secondResult == WIN)
        {
            cout << "Second wins" << endl;
        }

        buffer[0] = LEAVE;
        secondPlayer.write(buffer, 1);

        this_thread::sleep_for(chrono::seconds (1));

        if(firstResult == WIN)
        {
            cout << "First win" << endl;
        }
    }

    delete buffer;

    return 0;
}
