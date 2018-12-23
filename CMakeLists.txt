cmake_minimum_required(VERSION 2.8)
project(onesoft)
set(PROJECT_VERSION 0.1)

add_compile_options(-Wall -Wextra -Wpedantic -O0 -ggdb)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include/mongocxx/v_noabi)
include_directories(/usr/local/include/bsoncxx/v_noabi)
include_directories(/usr/include/libmongoc-1.0)
include_directories(/usr/include/libbson-1.0)
include_directories(Business/inc)
include_directories(Database/inc)
include_directories(Message/inc)
include_directories(MessageBroker/inc)

link_directories(/usr/local/lib)

add_library(message SHARED
    Message/src/Message.cpp
    Message/src/Proxy.cpp
    Message/src/Socket.cpp
    Message/src/ClientBuilder.cpp
    Message/inc/Client.h
    Message/inc/Message.h
    Message/inc/Proxy.h
    Message/inc/Socket.h
    Message/inc/ClientBuilder.h)
set_target_properties(message PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER Message/inc/Client.h
    PUBLIC_HEADER Message/inc/Message.h
    PUBLIC_HEADER Message/inc/Socket.h
    PUBLIC_HEADER Message/inc/Proxy.h)

add_library(business SHARED
    Business/src/Transaction.cpp
    Business/src/ConversionHelper.cpp
    Business/src/DateHourQuery.cpp
    Business/inc/Transaction.h
    Business/inc/ConversionHelper.h
    Business/inc/DateHourQuery.h)
set_target_properties(business PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER Business/inc/Transaction.h
    PUBLIC_HEADER Business/inc/ConversionHelper.h
    PUBLIC_HEADER Business/inc/DateHourQuery.h)

add_library(database SHARED
    Database/src/Database.cpp
    Database/inc/Database.h)
set_target_properties(database PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER Database/inc/Database.h)

add_executable(messageBroker messageBroker.cpp)
target_link_libraries(messageBroker
    message
    zmqpp
    zmq
    boost_program_options)

add_executable(server server.cpp)
target_link_libraries(server
    mongocxx
    bsoncxx
    business
    database
    message
    zmqpp
    zmq
    boost_serialization
    boost_program_options)

add_executable(subscribed_client subscribed_client.cpp)
target_link_libraries(subscribed_client
    business
    message
    zmqpp
    zmq
    boost_serialization
    boost_program_options)

add_executable(push_client push_client.cpp)
target_link_libraries(push_client
    business
    message
    zmqpp
    zmq
    boost_serialization
    boost_program_options)

add_executable(query_client query_client.cpp)
target_link_libraries(query_client
    business
    message
    zmqpp
    zmq
    boost_serialization
    boost_program_options)
