cmake_minimum_required(VERSION 3.21)
project(ClientServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(ClientServer main.cpp Client/Udp/UdpClient.h
                                     Client/Udp/UdpClient.cpp
                                     Server/Udp/UdpServer.h
                                     Server/Udp/UdpServer.cpp)

target_link_libraries(ClientServer pthread)