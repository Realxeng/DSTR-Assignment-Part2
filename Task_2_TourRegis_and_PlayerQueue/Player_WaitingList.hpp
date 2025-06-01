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
        return player; // Return the dequeued player node
    }

    void displayQueue()
    {
        cout << queueName << " with " << size << " players waiting: " << endl;
        waitNode* current = front;
        while (current != nullptr)
        {
            int queueNum = 1;
            cout << queueNum << ". PlayerID(" << current->data->id << ") " << current->data->name << endl;
            current = current->next;
        }
        cout << endl;
    }
        
};

#endif