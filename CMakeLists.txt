cmake_minimum_required(VERSION 3.29)
project(LeagueOfLingo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(LeagueOfLingo main.cpp
        RiotApi.cpp
        RiotApi.h
        LeagueDictionary.cpp
        LeagueDictionary.h
        MainWindow.cpp
        MainWindow.h)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(LeagueOfLingo PRIVATE nlohmann_json::nlohmann_json)

find_package(CURL REQUIRED)
target_link_libraries(LeagueOfLingo PRIVATE CURL::libcurl)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(LeagueOfLingo PRIVATE Qt6::Widgets)
