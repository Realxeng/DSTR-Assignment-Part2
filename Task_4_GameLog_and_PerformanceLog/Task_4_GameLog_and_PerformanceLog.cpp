#include <iostream>
#include <cstring>

// --- Struct to hold match result details ---
struct MatchResult {
    int matchID;            
    char player1[50];       //Name of player 
    char player2[50];       
    int player1Score;       //Score of player 
    int player2Score;      
    char winner[50];        //winner name / draw
};

// --- Stack class to store recent match results ---
// Uses a dynamically allocated array to implement a fixed-size stack.
class MatchStack {
    MatchResult* stack;     // Dynamic array to hold MatchResult elements
    int top;                // Index of the current top element (-1 when empty)
    int capacity;           // Maximum capacity of the stack

public:
    // Constructor: allocate array and initialize top index
    MatchStack(int size = 10) {
        capacity = size;
        stack = new MatchResult[capacity];
        top = -1;
    }

    // Destructor: free allocated memory
    ~MatchStack() {
        delete[] stack;
    }

    // Push a new match result onto the stack
    // If full, remove the oldest (bottom) result by shifting elements down
    bool push(const MatchResult& result) {
        if (isFull()) {
            // Shift all elements down by one to discard bottom/oldest
            for (int i = 1; i < capacity; ++i) {
                stack[i - 1] = stack[i];
            }
            // Replace top element with new result
            stack[top] = result;
            return true;
        }
        else {
            // Stack not full - just add to top
            stack[++top] = result;
            return true;
        }
    }

    //Pop top match result from stack, returns false if empty
    bool pop(MatchResult& result) {
        if (isEmpty())
            return false;
        result = stack[top--];
        return true;
    }

    //Peek at top match result without removing, returns false if empty
    bool peek(MatchResult& result) {
        if (isEmpty())
            return false;
        result = stack[top];
        return true;
    }

    // Check if stack is empty
    bool isEmpty() const {
        return (top == -1);
    }

    // Check if stack is full
    bool isFull() const {
        return (top == capacity - 1);
    }

    // Print all recent match results in stack from most recent to oldest
    void printRecent() const {
        if (isEmpty()) {
            std::cout << "No recent results available.\n";
            return;
        }
        std::cout << "\nRecent Results (Most recent first):\n";
        for (int i = top; i >= 0; --i) {
            std::cout << "Match ID: " << stack[i].matchID
                << " | " << stack[i].player1 << " vs " << stack[i].player2
                << " | " << stack[i].player1Score << ":" << stack[i].player2Score
                << " | Winner: " << stack[i].winner << "\n";
        }
    }
};

// --- Node structure for singly linked list ---
// Holds match result data and pointer to next node
struct Node {
    MatchResult data;
    Node* next;
};

// --- Linked list class to store full match history ---
// Allows dynamic storage of an unbounded number of matches and provides search and print functionalities.
class MatchHistory {
    Node* head;             //Pointer to head of linked list

public:
    // Constructor initializes head pointer
    MatchHistory() : head(nullptr) { }

    // Destructor frees all nodes to avoid memory leaks
    ~MatchHistory() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    // Add a new match result to end of linked list
    void addResult(const MatchResult& result) {
        Node* newNode = new Node;
        newNode->data = result;
        newNode->next = nullptr;

        if (head == nullptr) {
            // If list empty, new node becomes head
            head = newNode;
        }
        else {
            // Otherwise append to end
            Node* curr = head;
            while (curr->next != nullptr)
                curr = curr->next;
            curr->next = newNode;
        }
    }

    // Print all match results in the history list
    void printAllResults() const {
        if (head == nullptr) {
            std::cout << "No match history found.\n";
            return;
        }
        std::cout << "\n--- Full Match History ---\n";
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << "Match ID: " << curr->data.matchID
                << " | " << curr->data.player1 << " vs " << curr->data.player2
                << " | " << curr->data.player1Score << ":" << curr->data.player2Score
                << " | Winner: " << curr->data.winner << "\n";
            curr = curr->next;
        }
    }

    // Search and print all matches where the specified player participated
    void searchByPlayer(const char* playerName) const {
        bool found = false;
        Node* curr = head;
        std::cout << "\nResults for player: " << playerName << "\n";
        while (curr != nullptr) {
            if (strcmp(curr->data.player1, playerName) == 0 || strcmp(curr->data.player2, playerName) == 0) {
                found = true;
                std::cout << "Match ID: " << curr->data.matchID
                    << " | " << curr->data.player1 << " vs " << curr->data.player2
                    << " | " << curr->data.player1Score << ":" << curr->data.player2Score
                    << " | Winner: " << curr->data.winner << "\n";
            }
            curr = curr->next;
        }
        if (!found) std::cout << "No results found for this player.\n";
    }

    // Search and print match matching given match ID
    void searchByMatchID(int matchID) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data.matchID == matchID) {
                std::cout << "\nMatch ID: " << curr->data.matchID
                    << " | " << curr->data.player1 << " vs " << curr->data.player2
                    << " | " << curr->data.player1Score << ":" << curr->data.player2Score
                    << " | Winner: " << curr->data.winner << "\n";
                return;
            }
            curr = curr->next;
        }
        std::cout << "Match ID " << matchID << " not found.\n";
    }

    // --- Internal struct to hold player win counts for tallying ---
    struct PlayerWin {
        char name[50];
        int wins;
    };

    // Count wins for each player and print the top N winners sorted by win count descending
    void printTopWinners(int topN = 5) const {
        if (head == nullptr) {
            std::cout << "No match history to calculate winners.\n";
            return;
        }

        const int MAX_PLAYERS = 100; // Maximum distinct players supported
        PlayerWin players[MAX_PLAYERS];
        int playerCount = 0;

        // Iterate over all matches, tally wins (exclude draws)
        Node* curr = head;
        while (curr != nullptr) {
            if (strcmp(curr->data.winner, "Draw") != 0) {
                // Check if winner already in players list
                int idx = -1;
                for (int i = 0; i < playerCount; i++) {
                    if (strcmp(players[i].name, curr->data.winner) == 0) {
                        idx = i;
                        break;
                    }
                }
                if (idx == -1 && playerCount < MAX_PLAYERS) {
                    // New player found, add to list
                    strcpy(players[playerCount].name, curr->data.winner);
                    players[playerCount].wins = 1;
                    playerCount++;
                }
                else if (idx != -1) {
                    // Existing player, increment win count
                    players[idx].wins++;
                }
            }
            curr = curr->next;
        }

        if (playerCount == 0) {
            std::cout << "No wins recorded yet.\n";
            return;
        }

        // Sort players by descending wins using simple selection sort
        for (int i = 0; i < playerCount - 1; i++) {
            int maxIdx = i;
            for (int j = i + 1; j < playerCount; j++) {
                if (players[j].wins > players[maxIdx].wins) {
                    maxIdx = j;
                }
            }
            if (maxIdx != i) {
                PlayerWin temp = players[i];
                players[i] = players[maxIdx];
                players[maxIdx] = temp;
            }
        }

        // Print top N winners or all if less than N
        std::cout << "\n--- Top " << (topN < playerCount ? topN : playerCount) << " Winners ---\n";
        for (int i = 0; i < topN && i < playerCount; i++) {
            std::cout << i + 1 << ". " << players[i].name << " - " << players[i].wins << " wins\n";
        }
    }
};

// --- Function to input match details from user ---
void inputMatch(MatchResult& result) {
    std::cout << "Enter Match ID: ";
    std::cin >> result.matchID;
    std::cin.ignore();

    std::cout << "Enter Player 1 Name: ";
    std::cin.getline(result.player1, 50);

    std::cout << "Enter Player 2 Name: ";
    std::cin.getline(result.player2, 50);

    std::cout << "Enter Player 1 Score: ";
    std::cin >> result.player1Score;
    std::cout << "Enter Player 2 Score: ";
    std::cin >> result.player2Score;
    std::cin.ignore();

    // Determine winner or draw based on scores
    if (result.player1Score > result.player2Score)
        strcpy(result.winner, result.player1);
    else if (result.player2Score > result.player1Score)
        strcpy(result.winner, result.player2);
    else
        strcpy(result.winner, "Draw");
}

// --- Main program: Provides a menu to interact with the match logging system ---
int main() {
    const int RECENT_SIZE = 5; // store last 5 matches in recent stack
    MatchStack recentResults(RECENT_SIZE);
    MatchHistory fullHistory;

    int choice;
    do {
        // Menu screen displayed to user
        std::cout << "\n==== APUEC Game Result Logging ====\n";
        std::cout << "1. Log new match result\n";
        std::cout << "2. Show recent results\n";
        std::cout << "3. Show all match history\n";
        std::cout << "4. Search by player name\n";
        std::cout << "5. Search by match ID\n";
        std::cout << "6. Show top 5 winners\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // clear input buffer to avoid issues with getline

        switch (choice) {
        case 1: {
            MatchResult res;
            inputMatch(res);               // get match details from user
            recentResults.push(res);       // add to recent matches stack
            std::cout << "Added to recent results.\n";
            fullHistory.addResult(res);    // add to full history linked list
            std::cout << "Added to match history.\n";
            break;
        }
        case 2: {
            recentResults.printRecent();   // display recent match results
            break;
        }
        case 3: {
            fullHistory.printAllResults(); // display full match history
            break;
        }
        case 4: {
            char searchName[50];
            std::cout << "Enter player name to search: ";
            std::cin.getline(searchName, 50);
            fullHistory.searchByPlayer(searchName); // search by player
            break;
        }
        case 5: {
            int searchID;
            std::cout << "Enter match ID to search: ";
            std::cin >> searchID;
            std::cin.ignore();
            fullHistory.searchByMatchID(searchID); // search by match ID
            break;
        }
        case 6: {
            fullHistory.printTopWinners(5); // print top 5 winners by wins
            break;
        }
        case 7:
            std::cout << "Exiting...\n";  // exit message
            break;
        default:
            std::cout << "Invalid option. Try again.\n"; // error for invalid choice
        }
    } while (choice != 7);

    return 0;
}