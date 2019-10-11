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

set(TEST_LIST_SOURCES
    test/test_list.c
    src/list.c
)

set(TEST_HASHTABLE_SOURCES
    test/test_hash_table.c
    src/list.c
    src/hash_table.c
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