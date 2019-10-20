cmake_minimum_required(VERSION 3.15.4)

project(cool_chat)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS -pthread)


set(SERVER_SOURCES
    src/Server.cpp
    src/ClientModel.cpp
    src/ClientHandler.cpp
    src/ServerController.cpp
    src/SocketBase.cpp
    src/SocketLinux.cpp
)

set(CLIENT_SOURCES
    src/Client.cpp
)

add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})

target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
