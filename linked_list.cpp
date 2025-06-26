#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>  // setw for styling
#include "json.hpp" // to export results in JSON

using namespace std;
using json = nlohmann::json;

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

struct ChannelLists
{
    Node *card;
    Node *ach;
    Node *wire_transfer;
    Node *upi;

    ChannelLists() : card(nullptr), ach(nullptr), wire_transfer(nullptr), upi(nullptr) {}
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
// helper function to find the middle of the linked list
Node *findMiddle(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    Node *low = head;
    Node *high = head;
    Node *prev = nullptr;

    // Use two pointers to find middle
    while (high != nullptr && high->next != nullptr)
    {
        prev = low;
        low = low->next;
        high = high->next->next;
    }

    // Split the list by breaking the connection
    prev->next = nullptr;
    return low;
}

// helper function to merge two sorted linked lists
Node *merge(Node *left, Node *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    Node *dummy = new Node(Transaction());
    Node *tail = dummy;

    while (left != nullptr && right != nullptr)
    {
        if (left->data.location <= right->data.location)
        {
            tail->next = left;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    if (left != nullptr)
        tail->next = left;
    else
        tail->next = right;

    Node *result = dummy->next;
    delete dummy;
    return result;
}

// Merge sort main function
Node *mergeSortByLocation(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    Node *middle = findMiddle(head);
    Node *right = middle->next;
    middle->next = nullptr;

    Node *left = mergeSortByLocation(head);
    right = mergeSortByLocation(right);

    return merge(left, right);
}

// Read CSV and insert into linked list
void readCSV(string filename, ChannelLists &channels)
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

        // Insert into appropriate channel list
        if (t.payement_channel == "card")
            insert(channels.card, t);
        else if (t.payement_channel == "ACH")
            insert(channels.ach, t);
        else if (t.payement_channel == "wire_transfer")
            insert(channels.wire_transfer, t);
        else if (t.payement_channel == "UPI")
            insert(channels.upi, t);
    }
    file.close();
}

Node *selectPaymentChannel(ChannelLists &channels)
{
    cout << "\nSelect Payment Channel:" << endl;
    cout << "1. Card" << endl;
    cout << "2. ACH" << endl;
    cout << "3. Wire Transfer" << endl;
    cout << "4. UPI" << endl;
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        return channels.card;
    case 2:
        return channels.ach;
    case 3:
        return channels.wire_transfer;
    case 4:
        return channels.upi;
    default:
        return nullptr;
    }
}

size_t calculateMemoryUsage(Node *head)
{
    size_t memory = 0;
    Node *current = head;
    while (current != nullptr)
    {
        memory += sizeof(Node) + sizeof(Transaction);
        current = current->next;
    }
    return memory;
}

void showPerformanceMetrics(const string &operation, long long timeMs, double memoryMB, size_t spaceUsed)
{
    cout << "\n+------------------------------- PERFORMANCE METRICS -------------------------------+" << endl;
    cout << "| Operation         | " << setw(60) << left << operation << "|\n";
    cout << "|-------------------+--------------------------------------------------------------|" << endl;
    cout << "| Execution Time    | " << setw(60) << left << (to_string(timeMs) + " ms") << "|\n";
    cout << "| Memory Usage      | " << setw(60) << left << (to_string(memoryMB) + " MB") << "|\n";
    cout << "| Space Used        | " << setw(60) << left << (to_string(spaceUsed) + " bytes") << "|\n";
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << "Press Enter to exit: ";
    cin.get(); // Wait for Enter
    return;
}

void exportSearchResultsToJson(Node *head, const string &type, long long timeMs, double memoryMB, size_t spaceUsed)
{
    json j;
    j["operation"] = "search";
    j["search_type"] = type;
    j["execution_time_ms"] = timeMs;
    j["memory_usage_mb"] = memoryMB;
    j["space_used_bytes"] = spaceUsed;

    json transactions = json::array();
    Node *current = head;
    while (current != nullptr)
    {
        if (current->data.transaction_type == type)
        {
            json transaction;
            transaction["id"] = current->data.id;
            transaction["amount"] = current->data.amount;
            transaction["location"] = current->data.location;
            transaction["is_fraud"] = current->data.is_fraud;
            transaction["transaction_type"] = current->data.transaction_type;
            transactions.push_back(transaction);
        }
        current = current->next;
    }
    j["matches"] = transactions;

    std::ofstream file("search_results.json");
    file << std::setw(4) << j << std::endl;
}

void exportSortResultsToJson(Node *head, long long timeMs, double memoryMB, size_t spaceUsed)
{
    json j;
    j["operation"] = "location_sort";
    j["execution_time_ms"] = timeMs;
    j["memory_usage_mb"] = memoryMB;
    j["space_used_bytes"] = spaceUsed;

    json transactions = json::array();
    Node *current = head;
    while (current != nullptr)
    {
        json transaction;
        transaction["id"] = current->data.id;
        transaction["location"] = current->data.location;
        transactions.push_back(transaction);
        current = current->next;
    }
    j["sorted_transactions"] = transactions;

    std::ofstream file("sort_results.json");
    file << std::setw(4) << j << std::endl;
}

int main()
{
    ChannelLists channels;
    readCSV("financial_fraud_detection_dataset.csv", channels);

    while (true)
    {
        cout << "\n+------------------- MENU -------------------+" << endl;
        cout << "| 1. Search by Transaction Type             |" << endl;
        cout << "| 2. Sort Transactions by Location            |" << endl;
        cout << "| 0. Exit                                   |" << endl;
        cout << "+-------------------------------------------+" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        cin.ignore();
        if (choice == 0)
        {
            cout << "Exiting program." << endl;
            return 0;
        }

        Node *selectedChannel = selectPaymentChannel(channels);
        if (!selectedChannel)
        {
            cout << "Invalid channel selection!" << endl;
            continue;
        }

        if (choice == 1)
        {
            // // ------ Sohaib ------
            string type;
            cout << "Enter transaction type to search: ";
            getline(cin >> ws, type);

            // timer//
            auto start = chrono::high_resolution_clock::now();

            searchByTransactionType(selectedChannel, type);

            // timer//
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

            size_t memoryUsed = calculateMemoryUsage(selectedChannel);
            double memoryMB = static_cast<double>(memoryUsed) / (1024 * 1024);

            // Export as JSON
            exportSearchResultsToJson(selectedChannel, type, duration.count(), memoryMB, memoryUsed);

            showPerformanceMetrics("Search", duration.count(), memoryMB, memoryUsed);
            // // ------------------------
        }
        else if (choice == 2)
        {
            auto start = chrono::high_resolution_clock::now();

            if (selectedChannel == channels.card)
                channels.card = mergeSortByLocation(selectedChannel);
            else if (selectedChannel == channels.ach)
                channels.ach = mergeSortByLocation(selectedChannel);
            else if (selectedChannel == channels.wire_transfer)
                channels.wire_transfer = mergeSortByLocation(selectedChannel);
            else if (selectedChannel == channels.upi)
                channels.upi = mergeSortByLocation(selectedChannel);

            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            size_t memoryUsed = calculateMemoryUsage(selectedChannel);
            double memoryMB = static_cast<double>(memoryUsed) / (1024 * 1024);

            // Export as JSON
            exportSortResultsToJson(selectedChannel, duration.count(), memoryMB, memoryUsed);

            showPerformanceMetrics("Location Sort", duration.count(), memoryMB, memoryUsed);
        }
    }
}
