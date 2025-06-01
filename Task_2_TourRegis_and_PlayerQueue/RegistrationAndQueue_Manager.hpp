#ifndef REGISTRATIONANDQUEUE_MANAGER_HPP
#define REGISTRATIONANDQUEUE_MANAGER_HPP

#include "Player_Registration.hpp"
#include "Player_WaitingList.hpp"
#include "Player_CheckIn.hpp"

class RegistrationManager
{
    PlayerRegistration regQueue;
    WaitingListQueue waitQueue;
    CheckInQueue checkQueue;

    playerNode* finalists;
    int capacity;
    int finalistCount;

    int findFinalistIndex(int playerId)
    {
        for (int i = 0; i < finalistCount; ++i)
        {
            if (finalists[i].id == playerId)
                return i;
        }
        return -1;
    }
public:
    RegistrationManager(int cap)
        : regQueue("Tournament Registration"),
          waitQueue("Waiting List"),
          checkQueue(cap)
    {
        capacity = cap;
        finalists = new playerNode[capacity];
        finalistCount = 0;
    }

    ~RegistrationManager()
    {
        delete[] finalists;

        cout << "RegistrationManager destroyed" << endl;
    }

    void registerPlayer(int id, string name, bool isEarlyBird, bool isWildcard)
    {
        if (regQueue.getSize() < capacity)
        {
            regQueue.registerPlayer(id, name, isEarlyBird, isWildcard);
        }
        else
        {
            cout << "Registration queue is full. Player cannot be registered." << endl;
            cout << "Player : " << name << " with ID " << id << " will be added to the waiting list." << endl;
            waitQueue.enqueue(regQueue.registerPlayer(id, name, isEarlyBird, isWildcard));
        }
    }

    void finaliseList()
    {
        finalistCount = 0;
        while (!regQueue.isEmpty() && finalistCount < capacity)
        {
            finalists[finalistCount++] = *regQueue.popFront();
        }
        // move leftovers to waiting list
        while (!regQueue.isEmpty())
        {
            waitQueue.enqueue(regQueue.popFront());
        }
    }

    bool checkIn(int playerId)
    {
        int index = findFinalistIndex(playerId);
        if (index == -1) 
        {
            cout << "Player with ID " << playerId << " not found in finalists." << endl;
            return false;
        }
        for (int i = 0; i < checkQueue.getCount(); ++i)
        {
            if (checkQueue.peekAt(i)->id == playerId)
            {
                cout << "Player with ID " << playerId << " is already checked in." << endl;
                return false; // already checked in
            }
        }
        return checkQueue.enqueue(&finalists[index]);
    }    
    bool withdraw(int playerId)
    {
        checkQueue.removeById(playerId);
        int index = findFinalistIndex(playerId);
        if (index == -1) 
        {
            cout << "Player with ID " << playerId << " not found in finalists." << endl;
            return false;
        }
        for (int i = index; i < finalistCount - 1; ++i)
        {
            finalists[i] = finalists[i + 1];
        }
        finalistCount--;
        if (!waitQueue.isEmpty())
        {
            playerNode* replacement = waitQueue.dequeue();
            finalists[finalistCount++] = *replacement;
            checkQueue.enqueue(replacement);
            cout << "Player with ID " << playerId << " has been replaced by player with ID " << replacement->id << "." << endl;
            return true;
        }
        cout << "Player with ID " << playerId << " has been withdrawn." << endl;
        cout << "No replacement available from waiting list." << endl;
        return false;
    }
    void showAll()
    {
        cout << "--- STATUS ---" << endl;
        cout << "Finalists [" << finalistCount << "]: "<< endl;
        for (int i = 0; i < finalistCount; ++i)
        {
            cout << i + 1 << ". PlayerID (" << finalists[i].id << ") " << finalists[i].name << endl;
        }
        cout << endl;
        checkQueue.display();
        waitQueue.displayQueue();
        cout << "--------------" << endl;
    }
};

#endif