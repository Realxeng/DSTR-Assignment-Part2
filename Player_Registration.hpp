#ifndef PLAYER_REGISTRATION_HPP
#define PLAYER_REGISTRATION_HPP

#include <string>
using namespace std;

struct playerNode
{
    int id;         
    string name;    
    bool isEarlyBird; // registered before early‑bird cutoff (priority in queue over normal players)
    bool isWildcard;  // special invite or flag by organisers (Jump to front of queue)

    playerNode* next;
    playerNode* prev;
};

class PlayerRegistration 
{
    playerNode* front;
    playerNode* rear;
    string queueName;
    int size = 0;

    void insertAfter(playerNode* pos, playerNode* node);
public:
    PlayerRegistration(string queueName = "Registration Queue");
    ~PlayerRegistration();
    bool isEmpty();
    int getSize();
    void registerPlayer(int id, string name, bool isEarlyBird, bool isWildcard);
    playerNode* popFront();
    bool removeById(int playerId);
    void displayQueue();
    

};

#endif