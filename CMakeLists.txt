cmake_minimum_required(VERSION 3.29)
project(LeagueOfLingo)

set(CMAKE_CXX_STANDARD 20)

add_executable(LeagueOfLingo main.cpp
        RiotApi.cpp
        RiotApi.h
        LeagueDictionary.cpp
        LeagueDictionary.h)

find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(LeagueOfLingo PRIVATE nlohmann_json::nlohmann_json)

find_package(CURL REQUIRED)
target_link_libraries(LeagueOfLingo PRIVATE CURL::libcurl)

include_directories(${CMAKE_SOURCE_DIR}/include)