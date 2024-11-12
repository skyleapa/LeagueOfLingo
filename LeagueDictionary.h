#ifndef LEAGUE_DICTIONARY_H
#define LEAGUE_DICTIONARY_H

#include <string>
#include <unordered_map>
#include <functional>
#include "RiotApi.h"

class LeagueDictionary {
public:
    LeagueDictionary();
    void addEntry(const std::string& keyword, const std::string& definition);
    std::string lookup(const std::string& keyword, RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const;
    void showAllEntries() const;

private:
    std::unordered_map<std::string, std::string> dictionary;
    std::unordered_map<std::string, std::function<std::string(RiotApi&, const std::string&, const std::string& gameName, const std::string& tagLine)>> queryHandlers;

    std::string csQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const;
    std::string ccQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const;
    std::string fedQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const;
    std::string intingQueryHandler(RiotApi& riotApi, const std::string& puuid, const std::string& gameName, const std::string& tagLine) const;
};

#endif // LEAGUE_DICTIONARY_H
