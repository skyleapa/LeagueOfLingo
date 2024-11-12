#include "LeagueDictionary.h"
#include "RiotApi.h"
#include <iostream>

void showMenu() {
    std::cout << "Welcome to League of Lingo! Select an option:\n"
              << "1. Lookup a term\n"
              << "2. Add a new term\n"
              << "3. Show all entries\n"
              << "4. Exit\n"
              << "Enter your choice: ";
}

int main() {
    const char* apiKey = std::getenv("RIOT_API_KEY");
    RiotApi riotApi(apiKey);

    LeagueDictionary dictionary;
    int choice;

    // this is my puuid (skyleapa)
    std::string puuid = "lpm4glcFwzpMNPDlAo7EPjtzAn84v5HlGxScVciLiq0GO8959Y4ZWEl42WcWF0NDg-78Gj5jCJYTKA";
    std::string gameName = "skyleapa";
    std::string tagLine = "NA1";
    do {
        showMenu();
        std::cin >> choice;

        std::cin.ignore(); // clear newline character

        switch (choice) {
            case 1: {
                showMenu();
                std::string keyword;
                std::cout << "Enter keyword: ";
                std::getline(std::cin, keyword);
                std::cout << "Definition: " << dictionary.lookup(keyword, riotApi, puuid, gameName, tagLine) << std::endl;
                break;
            }
            case 2: {
                std::string keyword, definition;
                std::cout << "Enter keyword: ";
                std::getline(std::cin, keyword);
                std::cout << "Enter definition: ";
                std::getline(std::cin, definition);
                dictionary.addEntry(keyword, definition);
                break;
            }
            case 3:
                dictionary.showAllEntries();
            break;
            case 4:
                std::cout << "Closing LeagueOfLingo. Goodbye!" << std::endl;
            break;
            default:
                std::cout << "That's not a valid option, please try again" << std::endl;
        }
    } while (choice != 4);

    return 0;
}
