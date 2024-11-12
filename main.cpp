#include <iostream>
#include "RiotApi.h"
#include "LeagueDictionary.h"

void showMenu() {
    std::cout << "\n--- League Dictionary ---" << std::endl;
    std::cout << "1. Lookup a keyword" << std::endl;
    std::cout << "2. Add a new keyword" << std::endl;
    std::cout << "3. Show all keywords" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    try {
        RiotApi riotApi(";)");
        std::string puuid = "lpm4glcFwzpMNPDlAo7EPjtzAn84v5HlGxScVciLiq0GO8959Y4ZWEl42WcWF0NDg-78Gj5jCJYTKA";

        std::string playerName = "skyleapa";
        std::string tagLine = "NA1";
        std::string region = "americas";
        auto matchList = riotApi.getPlayerMatches(puuid, playerName, tagLine, region);

        std::cout << "Recent matches for " << playerName << ":\n";
        for (const auto& matchId : matchList) {
            auto matchData = riotApi.getMatchData(matchId);
            // std::cout << "match id: " << matchId << "\n";
            // std::cout << "match data: " << matchData.dump(4) << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
    // LeagueDictionary dictionary;
    // int choice;
    //
    // do {
    //     showMenu();
    //     std::cin >> choice;
    //
    //     std::cin.ignore(); // clear newline character
    //
    //     switch (choice) {
    //         case 1: {
    //             std::string keyword;
    //             std::cout << "Enter keyword: ";
    //             std::getline(std::cin, keyword);
    //             std::cout << "Definition: " << dictionary.lookup(keyword) << std::endl;
    //             break;
    //         }
    //         case 2: {
    //             std::string keyword, definition;
    //             std::cout << "Enter keyword: ";
    //             std::getline(std::cin, keyword);
    //             std::cout << "Enter definition: ";
    //             std::getline(std::cin, definition);
    //             dictionary.addEntry(keyword, definition);
    //             break;
    //         }
    //         case 3:
    //             dictionary.showAllEntries();
    //         break;
    //         case 4:
    //             std::cout << "Exiting the dictionary tool. Goodbye!" << std::endl;
    //         break;
    //         default:
    //             std::cout << "Invalid choice. Please try again." << std::endl;
    //     }
    // } while (choice != 4);
    //
    // return 0;
}