cmake_minimum_required(VERSION 3.15.4)

project(xion)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS -pthread)

set(SERVER_SOURCES
    src/Server.cpp
    src/ClientModel.cpp
    src/ClientHandler.cpp
    src/ServerController.cpp
    src/SocketBase.cpp
    src/SocketLinux.cpp
    src/Command.cpp
    src/commands/QuitCommand.cpp
    src/commands/NameCommand.cpp
    src/commands/WhisperCommand.cpp
)

add_executable(server ${SERVER_SOURCES})

target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)