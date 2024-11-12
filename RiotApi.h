#ifndef RIOTAPI_H
#define RIOTAPI_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

class RiotApi {
private:
    std::string apiKey;
    std::unordered_map<std::string, nlohmann::json> cache;
    std::string cacheDir;

    std::string makeApiRequest(const std::string& url);
    void loadCacheFromDisk();
    void saveCacheToDisk(const std::string& matchId, const nlohmann::json& data);

public:
    explicit RiotApi(const std::string& apiKey, const std::string& cacheDirectory = "../riot_cache");
    nlohmann::json getMatchData(const std::string& matchId);
    nlohmann::json getPlayerMatches(const std::string& puuid, const std::string& gameName, const std::string& tagLine, const std::string& region);

    void cacheMatchData(const std::string& matchId, const nlohmann::json& data);
};

#endif // RIOTAPI_H
