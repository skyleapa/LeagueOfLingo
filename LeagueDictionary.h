//
// Created by Michelle Wang on 2024-11-11.
//

#ifndef LEAGUEDICTIONARY_H
#define LEAGUEDICTIONARY_H
#include <string>


class LeagueDictionary {
private:
    std::unordered_map<std::string, std::string> dictionary;

public:
    LeagueDictionary();
    void addEntry(const std::string& keyword, const std::string& definition);
    std::string lookup(const std::string& keyword) const;
    void showAllEntries() const;
};


#endif //LEAGUEDICTIONARY_H
