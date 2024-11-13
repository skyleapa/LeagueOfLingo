# Welcome to the League Of Lingo!

As a way to familiarize myself with a role at Riot Games, I've built a league dictionary with C++ and Qt that gets real life examples from my own games!

## Inspiration
I have friends that are first time players of league, and one of their struggles is understanding league lingo. To help them learn the different terms used
in game, I've built League of Lingo, which allows you search for a specific keyword and receive a definition. Along with that, the dictionary makes a request to the Riot API,
which queries for past matches from a specified player (Me!). The dictionary then searches through the match data, and looks for an example that fulfills a set of criteria
for that term. For example, to identify an "inting" example, we look for players that have a low kda score, and low total damage dealt. The examples will vary as I play more games,
so the definitions stay fresh!

## How it was built
This application was built entirely in C++. The frontend framework is using Qt Widgets in MainWindow.cpp, and the API calls are made using LibCurl in RiotApi.cpp.

## Installation
1. Use CLion or your preferred IDE
2. The libraries used are:
   - nlohmann_json
   - libcurl
   - qt
   I installed them through homebrew
3. Clone the repository and press start!
