cmake_minimum_required(VERSION 3.5)
project(untitled)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ")

set(SOURCE_FILES main.cpp Network/Server.cpp Network/Server.h Network/Socket.cpp Network/Socket.h Network/Connection.cpp Network/Connection.h View/CommandLine.cpp View/CommandLine.h utils/PipeFactory.cpp utils/PipeFactory.h utils/InputPipe.cpp utils/InputPipe.h utils/OutputPipe.cpp utils/OutputPipe.h Network/TCP.cpp Network/TCP.h Security/Security.cpp Security/Security.h)
add_executable(untitled ${SOURCE_FILES})

target_link_libraries(untitled /usr/lib/libcrypto++.a)