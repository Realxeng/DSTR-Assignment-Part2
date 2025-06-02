/**
 * Player Registration Queue
 * This class manages the registration of players for a tournament.
 * ----------------------------------------------------------
 * Doubly-Linked List with Priority Queue Behavior:
 *     Wildcard > Early-Bird > Regular
 * 
 * insertAfter() - Inserts a new player node after a specified position.
 * isEmpty() - Checks if the queue is empty.
 * getSize() - Returns the current size of the queue.
 * registerPlayer() - Registers a new player and places them in the correct position based on their status.
 * popFront() - Dequeue the player at the front of the queue. (Highest priority player)
 * removeById() - Removes a player from the queue by their ID.
 * displayQueue() - Displays the current registration queue.
 */

#ifndef PLAYER_REGISTRATION_HPP
#define PLAYER_REGISTRATION_HPP

#include <string>
#include <iostream>
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
    int size;

    void insertAfter(playerNode* lastPosition, playerNode* newNode)
    {
        if (lastPosition == nullptr)
        {
            if (isEmpty())
            {
                front = rear = newNode;
            }
            else
            {
                newNode->next = front;
                newNode->prev = nullptr;
                front->prev = newNode;
                front = newNode;
            }
        }
        else
        {
            newNode->next = lastPosition->next;
            newNode->prev = lastPosition;
            if (lastPosition->next != nullptr)
            {
                lastPosition->next->prev = newNode;
            }
            lastPosition->next = newNode;
            if (lastPosition == rear)
            {
                rear = newNode; 
            }
        }
    }

public:
    PlayerRegistration(string queueName)
    {
        front = nullptr;
        rear = nullptr;
        this->queueName = queueName;
        size = 0;
    }

    ~PlayerRegistration()
    {
        playerNode* current = front;
        while (current != nullptr)
        {
            playerNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        cout << queueName << " removed" << endl;
    }

    bool isEmpty()
    {
        if (front == nullptr)
        {
            return true;
        }
        return false;
    }

    int getSize()
    {
        return size;
    }

    playerNode* registerPlayer(int id, string name, bool isEarlyBird, bool isWildcard)
    {
        playerNode* newNode = new playerNode;
        newNode->id = id;
        newNode->name = name;
        newNode->isEarlyBird = isEarlyBird;
        newNode->isWildcard = isWildcard;
        newNode->next = nullptr;
        newNode->prev = nullptr;


        if (isEmpty())
        {
            front = rear = newNode;
            size = 1;
        }
        else if (isWildcard)
        {
            playerNode* current = front;
            playerNode* last = nullptr;
            while (current && current->isWildcard)
            {
                last = current;
                current = current->next;
            }

            insertAfter(last, newNode);
            size++;
        }
        else if (isEarlyBird)
        {
            playerNode* current = front;
            playerNode* last = nullptr;
            while (current && (current->isWildcard || current->isEarlyBird))
            {
                last = current;
                current = current->next;
            }

            insertAfter(last, newNode);
            size++;
        }
        else
        {
            newNode->prev = rear;
            newNode->next = nullptr;
            rear->next = newNode;
            rear = newNode;
            size++;
        }
        
        cout << "Player: " << name << " with ID " << id << " was added to the Registration Queue." << endl;
        return newNode; 
    }

    playerNode* popFront()  // remove player from the front of the queue
    {
        if (isEmpty())
        {
            cout << "Registration Queue is empty." << endl;
            return nullptr; // Queue is empty
        }
        playerNode* node = front;
        front = front->next;
        if (front != nullptr)
        {
            front->prev = nullptr;
        }
        else
        {
            rear = nullptr; // Queue is now empty
        }
        size--;
        cout << "Player: " << node->name << " with ID " << node->id << " was removed from the Registration Queue." << endl;
        return node; // Return the removed player node
    }

    bool removeById(int playerId)
    {
        playerNode* current = front;
        while (current != nullptr)
        {
            if (current->id == playerId)
            {
                if (current->prev != nullptr)
                {
                    current->prev->next = current->next;
                }
                else{
                    front = current->next; 
                }
                
                if (current->next != nullptr)
                {
                    current->next->prev = current->prev;
                }
                else
                {
                    rear = current->prev; 
                }

                cout << "Player: " << current->name << " with ID " << playerId << " was removed from the Registration Queue." << endl;
                delete current;
                size--;
                return true;
            }
            current = current->next;
        }
        cout << "Player with ID " << playerId << " not found in the queue." << endl;
        return false; 
    }

    void displayQueue()
    {
        cout << queueName << " with " << size << " players registered: " << endl;
        playerNode* current = front;
        int queueNum = 1;
        while (current != nullptr)
        {
            cout << queueNum << ". PlayerID(" << current->id << ") " << current->name << " (Early Bird: " << (current->isEarlyBird ? "Yes" : "No") 
                 << ", Wildcard: " << (current->isWildcard ? "Yes" : "No") << ")" << endl;
            current = current->next;
            queueNum++;
        }
    }
    
};

#endif