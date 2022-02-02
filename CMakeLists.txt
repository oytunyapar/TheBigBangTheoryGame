cmake_minimum_required(VERSION 3.21)
project(ClientServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(ClientServer main.cpp Client/Udp/UdpClient.h
                                     Client/Udp/UdpClient.cpp
                                     Server/Udp/UdpServer.h
                                     Server/Udp/UdpServer.cpp GameSession/GameSession.h GameSession/GameSession.cpp GameParameters/GameParameters.h MessageTypes.h)

target_link_libraries(ClientServer pthread)

include_directories(GameParameters GameSession ${CMAKE_SOURCE_DIR})