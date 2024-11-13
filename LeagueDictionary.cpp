#include "LeagueDictionary.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

LeagueDictionary::LeagueDictionary() {
    // TODO: tracking map data from matches to determine jungle ganks
    dictionary["Gank (In progress)"] = "An ambush of an enemy champion/teammate, coming from the jungle to help lane.";
    // TODO: play a game to get dragon data
    dictionary["Dragon"] = "A powerful neutral monster offering team-wide buffs. There are a variety of different dragon types, such as Cloud, Flame and Chemtech.";
    dictionary["CS"] = "Creep Score, the number of minions or neutral monsters killed.";
    dictionary["CC"] = "Crowd control, describing an ability or spell that temporarily impedes an enemy's ability to fight.";

    dictionary["Inting"] = "Intentionally feeding, or dying repeatedly in the game. Often done maliciously or due to poor play.";
    dictionary["Fed"] = "A player who has accumulated many kills and has become very strong, often 'carrying' their team.";

    queryHandlers["CS"] = [this](RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) {
        return this->csQueryHandler(riotApi, puuid, gameName, tagLine);
    };
    queryHandlers["CC"] = [this](RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) {
        return this->ccQueryHandler(riotApi, puuid, gameName, tagLine);
    };
    queryHandlers["Fed"] = [this](RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) {
        return this->fedQueryHandler(riotApi, puuid, gameName, tagLine);
    };
    queryHandlers["Inting"] = [this](RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) {
        return this->intingQueryHandler(riotApi, puuid, gameName, tagLine);
    };
    queryHandlers["Dragon"] = [this](RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) {
        return this->dragonQueryHandler(riotApi, puuid, gameName, tagLine);
    };
}

void LeagueDictionary::addEntry(const std::string& keyword, const std::string& definition) {
    dictionary[keyword] = definition;
    std::cout << "Entry added: " << keyword << " -> " << definition << std::endl;
}

std::string LeagueDictionary::lookup(const std::string& keyword, RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto it = queryHandlers.find(keyword);
    if (it != queryHandlers.end()) {
        return it->second(riotApi, puuid, gameName, tagLine);
    }

    return "The league term is not in our dictionary or under construction :(";
}

std::string LeagueDictionary::formatGameTimestamp(long long timestamp) const {
    std::time_t gameStartTime = timestamp / 1000;
    std::tm* timeInfo = std::gmtime(&gameStartTime);

    std::ostringstream dateStream;
    dateStream << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S UTC");
    return dateStream.str();
}

std::string LeagueDictionary::csQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto matchList = riotApi.getPlayerMatches(puuid, gameName, tagLine, "americas");

    for (const auto& matchId : matchList) {
        auto matchData = riotApi.getMatchData(matchId);

        for (const auto& participant : matchData["info"]["participants"]) {
            // finding match information for myself
            if (participant["puuid"] == puuid) {
                int csScore = participant["totalMinionsKilled"].get<int>() + participant["neutralMinionsKilled"].get<int>();
                std::string champion = participant["championName"].get<std::string>();

                long long gameStartTimestamp = matchData["info"]["gameStartTimestamp"].get<long long>();
                std::string formattedTimestamp = formatGameTimestamp(gameStartTimestamp);

                return dictionary.at("CS") + "\n\nExample: On " + formattedTimestamp +
                       ", the player " + gameName + " playing " + champion +
                       " achieved a CS score of " + std::to_string(csScore) +
                       ", meaning that they killed that many minions/neutral monsters!.";
            }
        }
    }
    return dictionary.at("CS") + "\nNo examples found for CS";
}

std::string LeagueDictionary::ccQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto matchList = riotApi.getPlayerMatches(puuid, gameName, tagLine, "americas");

    for (const auto& matchId : matchList) {
        auto matchData = riotApi.getMatchData(matchId);

        for (const auto& participant : matchData["info"]["participants"]) {
            // finding match information for the player
            if (participant["puuid"] == puuid) {
                int ccScore = participant["totalTimeCCDealt"].get<int>();
                std::string champion = participant["championName"].get<std::string>();

                long long gameStartTimestamp = matchData["info"]["gameStartTimestamp"].get<long long>();
                std::string formattedTimestamp = formatGameTimestamp(gameStartTimestamp);

                return dictionary.at("CC") + "\n\nExample: On " + formattedTimestamp +
                       ", the player " + gameName + " playing " + champion +
                       " dealt " + std::to_string(ccScore) +
                       " seconds of crowd control during the game.";
            }
        }
    }
    return dictionary.at("CC") + "\nNo examples found for CC";
}

std::string LeagueDictionary::fedQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto matchList = riotApi.getPlayerMatches(puuid, gameName, tagLine, "americas");

    for (const auto& matchId : matchList) {
        auto matchData = riotApi.getMatchData(matchId);

        double highestDamage = 0;
        std::string fedPlayerName;
        std::string fedChampion;
        int fedKills = 0, fedDeaths = 0, fedAssists = 0;
        double bestKDA = 0;
        long long fedPlayerTimestamp = 0;

        for (const auto& participant : matchData["info"]["participants"]) {
            int kills = participant["kills"].get<int>();
            int deaths = participant["deaths"].get<int>();
            int assists = participant["assists"].get<int>();
            double damageDealt = participant["totalDamageDealtToChampions"].get<double>();
            std::string participantName = participant["summonerName"].get<std::string>();
            std::string champion = participant["championName"].get<std::string>();

            double kda = deaths > 0 ? static_cast<double>(kills + assists) / deaths : kills + assists;

            // constantly update the best player based on data
            if (kda > 3.0 && damageDealt > highestDamage) {
                highestDamage = damageDealt;
                bestKDA = kda;
                fedPlayerName = participantName;
                fedChampion = champion;
                fedKills = kills;
                fedDeaths = deaths;
                fedAssists = assists;

                fedPlayerTimestamp = matchData["info"]["gameStartTimestamp"].get<long long>();
            }
        }

        if (!fedPlayerName.empty()) {
            std::string formattedTimestamp = formatGameTimestamp(fedPlayerTimestamp);

            return dictionary.at("Fed") + "\n\nExample: On " + formattedTimestamp + ", the player " +
                   fedPlayerName + " playing " + fedChampion +
                   " achieved " + std::to_string(fedKills) + " kills, " +
                   std::to_string(fedDeaths) + " deaths, " +
                   std::to_string(fedAssists) + " assists with a KDA of " +
                   std::to_string(bestKDA) + " and dealt " +
                   std::to_string(static_cast<int>(highestDamage)) + " damage, carrying their team!";
        }
    }

    return dictionary.at("Fed") + "\nNo examples found for fed";
}

std::string LeagueDictionary::intingQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto matchList = riotApi.getPlayerMatches(puuid, gameName, tagLine, "americas");

    for (const auto& matchId : matchList) {
        auto matchData = riotApi.getMatchData(matchId);

        double lowestDamage = std::numeric_limits<double>::max();
        std::string intingPlayerName;
        std::string intingChampion;
        int intingKills = 0, intingDeaths = 0, intingAssists = 0;
        double worstKDA = std::numeric_limits<double>::max();
        long long intingPlayerTimestamp = 0;

        for (const auto& participant : matchData["info"]["participants"]) {
            int kills = participant["kills"].get<int>();
            int deaths = participant["deaths"].get<int>();
            int assists = participant["assists"].get<int>();
            double damageDealt = participant["totalDamageDealtToChampions"].get<double>();
            std::string participantName = participant["summonerName"].get<std::string>();
            std::string champion = participant["championName"].get<std::string>();

            double kda = deaths > 0 ? static_cast<double>(kills + assists) / deaths : kills + assists;

            // checking for a low kda and a lot of deaths
            if (kda < 1.0 && damageDealt < lowestDamage && deaths >= 7) {
                lowestDamage = damageDealt;
                worstKDA = kda;
                intingPlayerName = participantName;
                intingChampion = champion;
                intingKills = kills;
                intingDeaths = deaths;
                intingAssists = assists;

                intingPlayerTimestamp = matchData["info"]["gameStartTimestamp"].get<long long>();
            }
        }

        if (!intingPlayerName.empty()) {
            std::string formattedTimestamp = formatGameTimestamp(intingPlayerTimestamp);

            return dictionary.at("Inting") + "\n\nExample: On " + formattedTimestamp + ", the player " +
                   intingPlayerName + " playing " + intingChampion +
                   " had " + std::to_string(intingKills) + " kills, " +
                   std::to_string(intingDeaths) + " deaths, and " +
                   std::to_string(intingAssists) + " assists, with a KDA of " +
                   std::to_string(worstKDA) + " and only " +
                   std::to_string(static_cast<int>(lowestDamage)) + " damage dealt, severely underperforming :(.";
        }
    }

    return dictionary.at("Inting") + "\nNo examples found for inting.";
}

std::string LeagueDictionary::dragonQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const {
    auto matchList = riotApi.getPlayerMatches(puuid, gameName, tagLine, "americas");

    for (const auto& matchId : matchList) {
        auto matchData = riotApi.getMatchData(matchId);

        // dragon kills are found in "events" data
        for (const auto& event : matchData["info"]["events"]) {
            if (event["type"] == "BUILDING_KILL" && event["killerId"].is_number()) {
                int killerId = event["killerId"].get<int>();
                std::string eventName = event["eventType"].get<std::string>();

                if (eventName == "Dragon") {
                    for (const auto& participant : matchData["info"]["participants"]) {
                        if (participant["participantId"] == killerId) {
                            std::string champion = participant["championName"].get<std::string>();
                            std::string playerName = participant["summonerName"].get<std::string>();

                            long long gameStartTimestamp = matchData["info"]["gameStartTimestamp"].get<long long>();
                            std::string formattedTimestamp = formatGameTimestamp(gameStartTimestamp);

                            return dictionary.at("Dragon") + "\n\nExample: On " + formattedTimestamp +
                                   ", the player " + playerName + " playing " + champion +
                                   " was part of the team that killed the Dragon.";
                        }
                    }
                }
            }
        }
    }

    return dictionary.at("Dragon") + "\nNo examples found for dragons";
}

// this was used for the terminal version
void LeagueDictionary::showAllEntries() const {
    for (const auto& entry : dictionary) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }
}

std::vector<std::string> LeagueDictionary::getAllTerms() const {
    std::vector<std::string> terms;
    for (const auto& entry : dictionary) {
        terms.push_back(entry.first);
    }
    return terms;
}