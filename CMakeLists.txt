cmake_minimum_required(VERSION 3.16)
project(TheBigBangTheoryGame)

set(CMAKE_CXX_STANDARD 14)

add_executable(TheBigBangTheoryGame main.cpp Client/Udp/UdpClient.h
                                     Client/Udp/UdpClient.cpp
                                     Server/Udp/UdpServer.h
                                     Server/Udp/UdpServer.cpp GameSession/GameSession.h GameSession/GameSession.cpp GameParameters/GameParameters.h MessageTypes.h)

target_link_libraries(TheBigBangTheoryGame pthread)

include_directories(GameParameters GameSession ${CMAKE_SOURCE_DIR})