cmake_minimum_required(VERSION 3.15.4)

project(cool_chat)

set(SERVER_SOURCES
    src/server.cpp
    src/list.cpp
    src/hash_table.cpp
)

set(CLIENT_SOURCES
    src/client.cpp
)

set(TEST_LIST_SOURCES
    test/test_list.cpp
    src/list.cpp
)

set(TEST_HASHTABLE_SOURCES
    test/test_hash_table.cpp
    src/list.cpp
    src/hash_table.cpp
)

add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})
add_executable(test_list ${TEST_LIST_SOURCES})
add_executable(test_hash_table ${TEST_HASHTABLE_SOURCES})

target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(test_list
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(test_hash_table
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)