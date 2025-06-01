#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
using namespace std;

/**
 * Basic player record used across the registration system.
 * No STL containers besides std::string are used (permitted).
 */
struct Player {
    int id;         
    string name;    
    bool isEarlyBird; // registered before early‑bird cutoff (priority in queue over normal players)
    bool isWildcard;  // special invite or flag by organisers (Jump to front of queue)

};

#endif