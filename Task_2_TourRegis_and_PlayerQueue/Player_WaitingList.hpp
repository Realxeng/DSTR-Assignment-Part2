/**
 * Player Waiting List Queue
 * This class manages a waiting list for players who registered after the tournament cap was reached.
 * ----------------------------------------------------------
 * Singly-Linked List with FIFO (Queue) 
 * 
 * isEmpty() - Checks if the waiting list is empty.
 * enqueue() - Adds a player to the end of the waiting list.
 * dequeue() - Removes a player from the front of the waiting list.
 * displayQueue() - Displays the current waiting list.
 */

#ifndef PLAYER_WAITINGLIST_HPP
#define PLAYER_WAITINGLIST_HPP

#include "Player_Registration.hpp"
#include <iostream>
#include <string>
using namespace std;

struct waitNode {
    playerNode* data;
    waitNode* next;
};

class WaitingListQueue {
    waitNode* front;
    waitNode* rear;
    string queueName;
    int size;

public:
    WaitingListQueue(string name)
    {
        front = nullptr;
        rear = nullptr;
        queueName = name;
        size = 0;
    }

    ~WaitingListQueue()
    {
        waitNode* current = front;
        while (current != nullptr)
        {
            waitNode* nextNode = current->next;
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

    void enqueue(playerNode* player)
    {
        waitNode* newNode = new waitNode;
        newNode->data = player;
        newNode->next = nullptr;

        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
        cout << "Player: " << player->name << " added to waiting list." << endl;
    }

    playerNode* dequeue()
    {
        if (isEmpty())
        {
            cout << "Waiting list is empty." << endl;
            return nullptr;
        }
        
        waitNode* node = front;
        playerNode* player = node->data;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr; // Queue is now empty
        }
        delete node;
        size--;
        cout << "Player: " << player->name << " removed from waiting list." << endl;
        return player; // Return the dequeued player node
    }

    void displayQueue()
    {
        cout << queueName << " with " << size << " players waiting: " << endl;
        waitNode* current = front;
        int queueNum = 1;
        while (current != nullptr)
        {
            cout << queueNum << ". PlayerID(" << current->data->id << ") " << current->data->name << endl;
            current = current->next;
            queueNum++;
        }
        cout << endl;
    }
        
};

#endif