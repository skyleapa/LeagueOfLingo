#include "RiotApi.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

RiotApi::RiotApi(const std::string& apiKey, const std::string& cacheDirectory)
    : apiKey(apiKey), cacheDir(cacheDirectory) {
    if (!fs::exists(cacheDir)) {
        fs::create_directory(cacheDir);
    }
    loadCacheFromDisk();
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    out->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string RiotApi::makeApiRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("X-Riot-Token: " + apiKey).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return response;
}

nlohmann::json RiotApi::getMatchData(const std::string& matchId) {
    if (cache.find(matchId) != cache.end()) {
        return cache[matchId];  // return cached data so dont need to re request from API
    }

    std::string url = "https://americas.api.riotgames.com/lol/match/v5/matches/" + matchId;
    std::string response = makeApiRequest(url);
    nlohmann::json data = nlohmann::json::parse(response);

    cacheMatchData(matchId, data);
    saveCacheToDisk(matchId, data);
    return data;
}

nlohmann::json RiotApi::getPlayerMatches(const std::string& puuid, const std::string& gameName, const std::string& tagLine, const std::string& region) {
    std::string playerPuuid = puuid;

    if (playerPuuid.empty()) {
        std::string url = "https://" + region + ".api.riotgames.com/riot/account/v1/accounts/by-riot-id/" + gameName + "/" + tagLine;
        std::string response = makeApiRequest(url);
        nlohmann::json playerData = nlohmann::json::parse(response);

        playerPuuid = playerData["puuid"];
    }

    // specify how many matches based on the count at end
    std::string url = "https://" + region + ".api.riotgames.com/lol/match/v5/matches/by-puuid/" + playerPuuid + "/ids?start=0&count=15";
    std::string response = makeApiRequest(url);

    return nlohmann::json::parse(response);
}


void RiotApi::cacheMatchData(const std::string& matchId, const nlohmann::json& data) {
    cache[matchId] = data;
}

void RiotApi::saveCacheToDisk(const std::string& matchId, const nlohmann::json& data) {
    std::ofstream outFile(cacheDir + "/" + matchId + ".json");
    if (outFile.is_open()) {
        outFile << data.dump(4);
        outFile.close();
    }
}

// pulling from our disk
void RiotApi::loadCacheFromDisk() {
    for (const auto& entry : fs::directory_iterator(cacheDir)) {
        if (entry.path().extension() == ".json") {
            std::ifstream inFile(entry.path());
            if (inFile.is_open()) {
                nlohmann::json data;
                inFile >> data;
                std::string matchId = entry.path().stem().string();
                cache[matchId] = data;
                inFile.close();
            }
        }
    }
}
