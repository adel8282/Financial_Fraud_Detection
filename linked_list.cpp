#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> // Add this for setw

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
// Helper function to find the middle of the linked list
Node *findMiddle(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    Node *slow = head;
    Node *fast = head;
    Node *prev = nullptr;

    // Use two pointers to find middle
    while (fast != nullptr && fast->next != nullptr)
    {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    // Split the list by breaking the connection
    prev->next = nullptr;
    return slow;
}

// Merge function to merge two sorted linked lists
Node *merge(Node *left, Node *right)
{
    // Create a dummy node to simplify merging
    Node *dummy = new Node(Transaction());
    Node *tail = dummy;

    // Merge the two lists by comparing transaction amounts
    while (left != nullptr && right != nullptr)
    {
        if (left->data.amount <= right->data.amount)
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

    // Append remaining nodes
    if (left != nullptr)
        tail->next = left;
    else
        tail->next = right;

    // Get the merged list (skip dummy node)
    Node *result = dummy->next;
    delete dummy;
    return result;
}

// Main merge sort function
Node *mergeSort(Node *head)
{
    // Base case: if list is empty or has only one node
    if (head == nullptr || head->next == nullptr)
        return head;

    // Find the middle and split the list
    Node *middle = findMiddle(head);

    // Recursively sort both halves
    Node *left = mergeSort(head);
    Node *right = mergeSort(middle);

    // Merge the sorted halves
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

int main()
{
    Node *head = nullptr;
    readCSV("financial_fraud_detection_dataset.csv", head);

    while (true)
    {
        cout << "\n+------------------- MENU -------------------+" << endl;
        cout << "| 1. Search by Transaction Type             |" << endl;
        cout << "| 2. Sort Transactions by Amount            |" << endl;
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
        else if (choice == 1)
        {
            // // ------ Sohaib ------
            string type;
            cout << "Enter transaction type to search: ";
            getline(cin >> ws, type);

            // timer//
            auto start = chrono::high_resolution_clock::now();

            searchByTransactionType(head, type);

            // timer//
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

            size_t memoryUsed = calculateMemoryUsage(head);
            double memoryMB = static_cast<double>(memoryUsed) / (1024 * 1024);

            showPerformanceMetrics("Search", duration.count(), memoryMB, memoryUsed);
            // // ------------------------
        }
        else if (choice == 2)
        {
            // ------ Mahmood ------
            auto start = chrono::high_resolution_clock::now();
            head = mergeSort(head);
            auto end = chrono::high_resolution_clock::now();

            // Calculate metrics
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            size_t memoryUsed = calculateMemoryUsage(head);
            double memoryMB = static_cast<double>(memoryUsed) / (1024 * 1024);

            showPerformanceMetrics("Merge Sort", duration.count(), memoryMB, memoryUsed);
            // ------------------------
        }
    }
}
