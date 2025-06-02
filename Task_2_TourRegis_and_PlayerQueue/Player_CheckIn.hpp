/**
 * Player Check-In Queue
 * This class manages the check-in of players who have registered for a tournament.
 * ----------------------------------------------------------
 * Array with Circular Queue
 * 
 * isFull() - Checks if the queue is full.
 * isEmpty() - Checks if the queue is empty.
 * getCount() - Returns the current number of players in the queue.
 * enqueue() - Adds a player to the end of the queue.
 * removeById() - Removes a player from the queue by their ID.
 * peekAt() - Returns the player at a specific index in the queue.
 * displayQueue() - Displays the current check-in queue.
 */

#ifndef PLAYER_CHECKIN_HPP
#define PLAYER_CHECKIN_HPP

#include "Player_Registration.hpp"
#include <iostream>
using namespace std;

class CheckInQueue 
{
    playerNode** arr;
    int capacity;
    int frontIndex;    
    int rearIndex;
    int count;

public:
    CheckInQueue(int cap)
    {
        capacity = cap;
        arr = new playerNode*[capacity];
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    ~CheckInQueue()
    {
        delete[] arr;
        cout << "CheckInQueue destroyed" << endl;
    }

    bool isFull()
    {
        if (count == capacity)
        {
            cout << "CheckInQueue is full." << endl;
            return true;
        }
        return false;
    }

    bool isEmpty()
    {
        if (count == 0)
        {
            cout << "CheckInQueue is empty." << endl;
            return true;
        }
        return false;
    }

    int getCount()
    {
        return count;
    }

    bool enqueue(playerNode* player)
    {
        if (isFull())
        {
            cout << "CheckInQueue is full. Cannot add player." << endl;
            return false;
        }
        rearIndex = (rearIndex + 1) % capacity;
        arr[rearIndex] = player;
        count++;
        cout << "Player: " << player->name << " with ID " << player->id << " was added to the Check In Queue." << endl; 
        return true;
    }

    bool removeById(int playerId)
    {
        if (isEmpty())
        {
            cout << "CheckInQueue is empty. Cannot remove player." << endl;
            return false;
        }
        int index = frontIndex;
        for (int i = 0; i < count; ++i)
        {
            if (arr[index]->id == playerId)
            {
                int cur = index;
                while (cur != rearIndex)
                {
                    int next = (cur + 1) % capacity;
                    arr[cur] = arr[next];
                    cur = next;
                }
                rearIndex = (rearIndex - 1 + capacity) % capacity;
                count--;
                cout << "Player: " << arr[index]->name << " with ID " << playerId << " was removed from the Check In Queue." << endl;
                return true;
            }
            index = (index + 1) % capacity;
        }
        cout << "Player with ID " << playerId << " not found in the Check In Queue." << endl;
        return false; 
    }  

    playerNode* peekAt(int index)
    {
        if (index < 0 || index >= count)
        {
            cout << "Index out of bounds." << endl;
            return nullptr;
        }
        return arr[(frontIndex + index) % capacity];
    }
    
    void displayQueue()
    {
        cout << "Checked-in [" << count << "]: " << endl;
        for (int i = 0; i < count; ++i)
        {
            playerNode* player = peekAt(i);
            cout << "PlayerID(" << player->id << ") " << player->name << endl;
        }
        cout << endl;
    }

};

#endif