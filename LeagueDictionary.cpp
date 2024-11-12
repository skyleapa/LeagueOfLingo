#include "LeagueDictionary.h"

#include <iostream>

LeagueDictionary::LeagueDictionary() {
    // TODO: tracking map data
    // dictionary["Gank"] = "An ambush of an enemy champion.";
    dictionary["CS"] = "Creep Score, the number of minions killed.";
    dictionary["Dragon"] = "A powerful neutral monster offering team-wide buffs.";
    dictionary["CC"] = "crowd control";

    dictionary["Inting"] = "A powerful neutral monster offering team-wide buffs.";
    dictionary["Smurfing"] = "A powerful neutral monster offering team-wide buffs.";
    dictionary["Fed"] = "A powerful neutral monster offering team-wide buffs.";
    dictionary["Carry"] = "A powerful neutral monster offering team-wide buffs.";
}

void LeagueDictionary::addEntry(const std::string& keyword, const std::string& definition) {
    dictionary[keyword] = definition;
    std::cout << "Entry added: " << keyword << " -> " << definition << std::endl;
}

std::string LeagueDictionary::lookup(const std::string& keyword) const {
    auto it = dictionary.find(keyword);
    if (it != dictionary.end()) {
        // here make api call to find an example scenario from match data
        return it->second;
    } else {
        return "The league term is not in our dictionary :(";
    }
}

void LeagueDictionary::showAllEntries() const {
    for (const auto& entry : dictionary) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }
}
