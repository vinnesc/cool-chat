cmake_minimum_required(VERSION 3.15.4)

project(cool_chat)

set(SERVER_SOURCES
    src/server.c
    src/list.c
    src/hash_table.c
)

set(CLIENT_SOURCES
    src/client.c
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