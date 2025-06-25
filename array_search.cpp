#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <sys/resource.h>

using namespace std;

const int MAX_STRING_LENGTH = 100;
const int INITIAL_CAPACITY = 1000;

// Struct to represent a transaction
struct Transaction {
    char transaction_id[MAX_STRING_LENGTH];
    char timestamp[MAX_STRING_LENGTH];
    char sender_account[MAX_STRING_LENGTH];
    char receiver_account[MAX_STRING_LENGTH];
    double amount;
    char transaction_type[MAX_STRING_LENGTH];
    char merchant_category[MAX_STRING_LENGTH];
    char location[MAX_STRING_LENGTH];
    char device_used[MAX_STRING_LENGTH];
    int is_fraud;
    char fraud_type[MAX_STRING_LENGTH];
    double time_since_last_transaction;
    double spending_deviation_score;
    double velocity_score;
    double geo_anomaly_score;
    char payment_channel[MAX_STRING_LENGTH];
    char ip_address[MAX_STRING_LENGTH];
    char device_hash[MAX_STRING_LENGTH];
};

// Helper to split CSV line
void splitCSVLine(char* line, char fields[18][MAX_STRING_LENGTH]) {
    int fieldIndex = 0;
    char* token = strtok(line, ",");
    while (token != NULL && fieldIndex < 18) {
        strncpy(fields[fieldIndex], token, MAX_STRING_LENGTH - 1);
        fields[fieldIndex][MAX_STRING_LENGTH - 1] = '\0';
        token = strtok(NULL, ",");
        fieldIndex++;
    }
}

// Dynamically load transactions
int loadTransactions(const char* filename, Transaction*& transactions) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Cannot open file.\n";
        return 0;
    }

    char line[1000];
    int size = 0;
    int capacity = INITIAL_CAPACITY;
    transactions = new Transaction[capacity];

    // Skip header
    file.getline(line, 1000);

    while (file.getline(line, 1000)) {
        if (size >= capacity) {
            // Expand array
            capacity *= 2;
            Transaction* newArray = new Transaction[capacity];
            for (int i = 0; i < size; ++i) {
                newArray[i] = transactions[i];
            }
            delete[] transactions;
            transactions = newArray;
        }

        char fields[18][MAX_STRING_LENGTH];
        splitCSVLine(line, fields);

        strncpy(transactions[size].transaction_id, fields[0], MAX_STRING_LENGTH);
        strncpy(transactions[size].timestamp, fields[1], MAX_STRING_LENGTH);
        strncpy(transactions[size].sender_account, fields[2], MAX_STRING_LENGTH);
        strncpy(transactions[size].receiver_account, fields[3], MAX_STRING_LENGTH);
        transactions[size].amount = atof(fields[4]);
        strncpy(transactions[size].transaction_type, fields[5], MAX_STRING_LENGTH);
        strncpy(transactions[size].merchant_category, fields[6], MAX_STRING_LENGTH);
        strncpy(transactions[size].location, fields[7], MAX_STRING_LENGTH);
        strncpy(transactions[size].device_used, fields[8], MAX_STRING_LENGTH);
        transactions[size].is_fraud = atoi(fields[9]);
        strncpy(transactions[size].fraud_type, fields[10], MAX_STRING_LENGTH);
        transactions[size].time_since_last_transaction = atof(fields[11]);
        transactions[size].spending_deviation_score = atof(fields[12]);
        transactions[size].velocity_score = atof(fields[13]);
        transactions[size].geo_anomaly_score = atof(fields[14]);
        strncpy(transactions[size].payment_channel, fields[15], MAX_STRING_LENGTH);
        strncpy(transactions[size].ip_address, fields[16], MAX_STRING_LENGTH);
        strncpy(transactions[size].device_hash, fields[17], MAX_STRING_LENGTH);

        size++;
    }

    file.close();
    return size;
}

// Linear search
void searchByTransactionType(Transaction* transactions, int size, const char* targetType) {
    cout << "\nResults for transaction_type = " << targetType << ":\n";
    int found = 0;
    for (int i = 0; i < size; ++i) {
        if (strcmp(transactions[i].transaction_type, targetType) == 0) {
            cout << "Transaction ID: " << transactions[i].transaction_id
                 << ", Amount: " << transactions[i].amount
                 << ", Sender: " << transactions[i].sender_account
                 << ", Receiver: " << transactions[i].receiver_account << "\n";
            found++;
        }
    }
    if (found == 0) {
        cout << "No transactions found.\n";
    }
}

int main() {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    Transaction* transactions = nullptr;
    int count = loadTransactions("financial_fraud_detection_dataset.csv", transactions);
    if (count == 0) return 1;

    char searchType[MAX_STRING_LENGTH];
    cout << "Enter transaction_type to search for: ";
    cin.getline(searchType, MAX_STRING_LENGTH);

    searchByTransactionType(transactions, count, searchType);

    delete[] transactions;
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long memory_kb = usage.ru_maxrss;
#ifdef __APPLE__
    memory_kb /= 1024;
#endif
    double memory_mb = memory_kb / 1024.0;
    long space_bytes = memory_kb * 1024;

    cout << "\n+--------------- PERFORMANCE METRICS ------------------\n";
    cout << "| Operation         | Search Transactions             |\n";
    cout << "|-------------------+----------------------------------\n";
    cout << "| Execution Time    | " << duration.count() << " ms\n";
    cout << "| Memory Usage      | " << memory_mb << " MB\n";
    cout << "| Space Used        | " << space_bytes << " bytes\n";
    cout << "+-----------------------------------------------------+\n";

    cout << "Press ENTER to exit...";
    cin.get();  // Wait for Enter key
    return 0;
}