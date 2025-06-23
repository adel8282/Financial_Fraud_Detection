#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

struct Transaction
{
    string id;
    string timestamp;
    string sender_account;
    string reciver_amount;
    float amount;
    string transaction_type;
    string merchant_category;
    string location;
    string device_used;
    bool is_fraud;
    string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    double velocity_score;
    double geo_anomaly_score;
    string payement_channel;
    string ip_address;
    string device_hash;
};

// Node structure
struct Node
{
    Transaction data;
    Node *next;
    Node(Transaction t)
    {
        data = t;
        next = nullptr;
    }
};

// Insert at head
void insert(Node *&head, Transaction t)
{
    Node *newNode = new Node(t);
    newNode->next = head;
    head = newNode;
}

// Sohaib
// Search by transaction type (linear search)
void searchByTransactionType(Node *head, string type)
{
    Node *current = head;
    int matchCount = 0;

    while (current != nullptr)
    {
        if (current->data.transaction_type == type)
        {
            matchCount++;
            // comment this block during test
            // cout << "ID: " << current->data.id << "\n";
            // cout << "Amount: " << current->data.amount << "\n";
            // cout << "Location: " << current->data.location << "\n";
            // cout << "Is Fraud: " << (current->data.is_fraud ? "Yes" : "No") << "\n";
            // cout << "Transaction Type: " << current->data.transaction_type << "\n";
        }
        current = current->next;
    }

    if (matchCount == 0)
        cout << "No transactions found with type: " << type << endl;
    else
        cout << matchCount << " transactions found with type: " << type << endl;
}

// Mahmood
// Helper function to find middle node
Node *findMiddle(Node *head)
{
    if (!head || !head->next)
        return head;

    Node *low = head, *high = head->next;
    while (high && high->next)
    {
        low = low->next;
        high = high->next->next;
    }
    return low;
}

// Helper function to merge two sorted lists
Node *merge(Node *left, Node *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    Node *result = nullptr;
    if (left->data.amount <= right->data.amount)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }
    return result;
}

// Merge sort function
Node *mergeSort(Node *head)
{
    // Base case
    if (!head || !head->next)
        return head;

    // Find middle and split list
    Node *middle = findMiddle(head);
    Node *secondHalf = middle->next;
    middle->next = nullptr;

    // Recursively sort both halves
    Node *left = mergeSort(head);
    Node *right = mergeSort(secondHalf);

    // Merge sorted halves
    return merge(left, right);
}

// Read CSV and insert into linked list
void readCSV(string filename, Node *&head)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        Transaction t;
        getline(ss, t.id, ',');
        getline(ss, t.timestamp, ',');
        getline(ss, t.sender_account, ',');
        getline(ss, t.reciver_amount, ',');

        getline(ss, token, ',');
        t.amount = token.empty() ? 0.0f : stof(token);
        getline(ss, t.transaction_type, ',');
        getline(ss, t.merchant_category, ',');
        getline(ss, t.location, ',');
        getline(ss, t.device_used, ',');

        getline(ss, token, ',');
        t.is_fraud = (token == "1" || token == "true");
        getline(ss, t.fraud_type, ',');

        getline(ss, token, ',');
        t.time_since_last_transaction = token.empty() ? 0.0 : stod(token);
        getline(ss, token, ',');
        t.spending_deviation_score = token.empty() ? 0.0 : stod(token);
        getline(ss, token, ',');
        t.velocity_score = token.empty() ? 0.0 : stod(token);
        getline(ss, token, ',');
        t.geo_anomaly_score = token.empty() ? 0.0 : stod(token);
        getline(ss, t.payement_channel, ',');
        getline(ss, t.ip_address, ',');
        getline(ss, t.device_hash, ',');

        insert(head, t);
    }

    file.close();
}

int main()
{
    Node *head = nullptr;
    readCSV("financial_fraud_detection_dataset.csv", head);

    // // ------ Sohaib ------
    // string type;
    // cout << "Enter transaction type to search: ";
    // getline(cin >> ws, type);

    // // timer//
    // auto start = chrono::high_resolution_clock::now();

    // searchByTransactionType(head, type);

    // // timer//
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> duration = end - start;

    // cout << "\nTime taken for search: " << duration.count() << " seconds" << endl;
    // // ------------------------

    // ------ Mahmood ------
    auto start = chrono::high_resolution_clock::now();
    head = mergeSort(head);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    cout << "\nTime taken for merge sort: " << duration.count() << " seconds" << endl;
    // ------------------------
}
