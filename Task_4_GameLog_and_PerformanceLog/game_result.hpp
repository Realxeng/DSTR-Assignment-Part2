#include "game_result.hpp"
#include <iostream>

GameResult::GameResult(int mID, std::string pname, std::string oname, std::string res, int sc)
    : matchID(mID), playerName(pname), opponentName(oname), result(res), score(sc), next(nullptr) {}

GameHistory::GameHistory() : head(nullptr) {}

GameHistory::~GameHistory() {
    clearHistory();
}

// Add a new game result to the history
void GameHistory::addResult(int matchID, std::string playerName, std::string opponentName, std::string result, int score) {
    GameResult* newResult = new GameResult(matchID, playerName, opponentName, result, score);
    newResult->next = head;
    head = newResult;
}

// Display all results
void GameHistory::displayHistory() const {
    GameResult* current = head;
    while (current != nullptr) {
        std::cout << "Match ID: " << current->matchID << std::endl;
        std::cout << "Player: " << current->playerName << " vs " << current->opponentName << std::endl;
        std::cout << "Result: " << current->result << ", Score: " << current->score << std::endl;
        std::cout << "-------------------------" << std::endl;
        current = current->next;
    }
}

// Display history for a specific player
void GameHistory::displayPlayerHistory(const std::string& playerName) const {
    GameResult* current = head;
    bool found = false;
    while (current != nullptr) {
        if (current->playerName == playerName) {
            std::cout << "Match ID: " << current->matchID << std::endl;
            std::cout << "Opponent: " << current->opponentName << std::endl;
            std::cout << "Result: " << current->result << ", Score: " << current->score << std::endl;
            std::cout << "-------------------------" << std::endl;
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        std::cout << "No history found for player: " << playerName << std::endl;
    }
}

// Clear the whole history
void GameHistory::clearHistory() {
    while (head != nullptr) {
        GameResult* temp = head;
        head = head->next;
        delete temp;
    }
}