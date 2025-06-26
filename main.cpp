#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <sys/resource.h>
#include <sstream>
#include <iomanip>

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

struct SimpleTransaction {
    string type;
    double amount;
    string nameOrig;
    string nameDest;
    string paymentChannel;
    string location;
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
    auto startSearch = chrono::high_resolution_clock::now();
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
    // Insert performance metrics display code here
    auto endSearch = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endSearch - startSearch);
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
    cout << "Press ENTER to go to the main menu...";
    cin.get();
    if (found == 0) {
        cout << "No transactions found.\n";
    }
}

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray() : capacity(100), length(0) {
        data = new T[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void add(const T& item) {
        if (length == capacity) {
            resize();
        }
        data[length++] = item;
    }

    size_t size() const {
        return length;
    }

    bool isEmpty() const {
        return length == 0;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }
};

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

string normalizeChannel(const string& channel) {
    string lower;
    for (char c : channel) lower += tolower(c);
    if (lower.find("card") != string::npos) return "Card";
    if (lower.find("ach") != string::npos) return "ACH";
    if (lower.find("upi") != string::npos) return "UPI";
    if (lower.find("bank") != string::npos) return "Bank Transfer";
    return channel;
}

void merge(DynamicArray<SimpleTransaction>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    SimpleTransaction* L = new SimpleTransaction[n1];
    SimpleTransaction* R = new SimpleTransaction[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].location <= R[j].location) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergesort(DynamicArray<SimpleTransaction>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void displayTransactions(const string& title, const DynamicArray<SimpleTransaction>& arr) {
    cout << "\n" << title << ":\n";
    cout << left << setw(15) << "Type" << setw(10) << "Amount" << setw(15) << "NameOrig" << setw(15) << "NameDest" << setw(20) << "PaymentChannel" << setw(15) << "Location" << "\n";
    cout << string(90, '-') << "\n";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << left << setw(15) << arr[i].type
             << setw(10) << fixed << setprecision(2) << arr[i].amount
             << setw(15) << arr[i].nameOrig
             << setw(15) << arr[i].nameDest
             << setw(20) << arr[i].paymentChannel
             << setw(15) << arr[i].location << "\n";
        if (i >= 19) break; // limit output to first 20 for readability
    }
}

void showPerformanceMetrics(const string& operation, long milliseconds, double memoryMB, size_t spaceBytes) {
    cout << "\n+--------------- PERFORMANCE METRICS ------------------\n";
    cout << "| Operation         | " << operation << string(30 - operation.size(), ' ') << "|\n";
    cout << "|-------------------+----------------------------------\n";
    cout << "| Execution Time    | " << milliseconds << " ms\n";
    cout << "| Memory Usage      | " << memoryMB << " MB\n";
    cout << "| Space Used        | " << spaceBytes << " bytes\n";
    cout << "+-----------------------------------------------------+\n";
}

void loadSuhanaTransactions(const string& filename,
                             DynamicArray<SimpleTransaction>& card,
                             DynamicArray<SimpleTransaction>& ach,
                             DynamicArray<SimpleTransaction>& upi,
                             DynamicArray<SimpleTransaction>& bankTransfer) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        SimpleTransaction t;

        try {
            getline(ss, field, ','); // 0 transaction_id - skip
            getline(ss, field, ','); // 1 timestamp - skip
            getline(ss, field, ','); t.nameOrig = field;
            getline(ss, field, ','); t.nameDest = field;
            getline(ss, field, ','); t.amount = stod(field);
            getline(ss, field, ','); t.type = field;
            getline(ss, field, ','); // merchant_category - skip
            getline(ss, field, ','); t.location = field;

            for (int i = 0; i < 7; ++i) getline(ss, field, ',');
            getline(ss, field, ','); t.paymentChannel = normalizeChannel(trim(field));

            if (t.paymentChannel == "Card") card.add(t);
            else if (t.paymentChannel == "ACH") ach.add(t);
            else if (t.paymentChannel == "UPI") upi.add(t);
            else if (t.paymentChannel == "Bank Transfer") bankTransfer.add(t);
        } catch (...) {
            // skip malformed lines
        }
    }

    file.close();
}

DynamicArray<SimpleTransaction>* selectChannelSuhana(DynamicArray<SimpleTransaction>& card,
                                                     DynamicArray<SimpleTransaction>& ach,
                                                     DynamicArray<SimpleTransaction>& upi,
                                                     DynamicArray<SimpleTransaction>& bank,
                                                     string& name) {
    cout << "\nSelect a Payment Channel:\n";
    cout << "1. Card\n2. ACH\n3. UPI\n4. Bank Transfer\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: name = "Card"; return &card;
        case 2: name = "ACH"; return &ach;
        case 3: name = "UPI"; return &upi;
        case 4: name = "Bank Transfer"; return &bank;
        default: cout << "Invalid choice.\n"; name = "Invalid"; return nullptr;
    }
}

int main() {
    using namespace std::chrono;
    cout << "Loading data from file, please wait..." << endl;
    cout.flush();
    auto start = high_resolution_clock::now();

    Transaction* transactions = nullptr;
    int count = loadTransactions("financial_fraud_detection_dataset.csv", transactions);
    if (count == 0) return 1;

    // Prepare for Suhana's sort
    DynamicArray<SimpleTransaction> cardTransactions;
    DynamicArray<SimpleTransaction> achTransactions;
    DynamicArray<SimpleTransaction> upiTransactions;
    DynamicArray<SimpleTransaction> bankTransferTransactions;

    loadSuhanaTransactions("financial_fraud_detection_dataset.csv",
                           cardTransactions, achTransactions, upiTransactions, bankTransferTransactions);

    int mainChoice = 0;
    while (mainChoice != 3) {
        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. Array Search (Adel)" << endl;
        cout << "2. Array Sort by Location (Suhana)" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> mainChoice;
        cin.ignore();

        if (mainChoice == 1) {
            char searchType[MAX_STRING_LENGTH];
            cout << "Enter transaction_type to search for: ";
            cin.getline(searchType, MAX_STRING_LENGTH);
            searchByTransactionType(transactions, count, searchType);
        } else if (mainChoice == 2) {
            string channelName;
            DynamicArray<SimpleTransaction>* selected = selectChannelSuhana(
                cardTransactions, achTransactions, upiTransactions, bankTransferTransactions, channelName);
            if (selected && !selected->isEmpty()) {
                auto start = high_resolution_clock::now();
                mergesort(*selected, 0, selected->size() - 1);
                auto end = high_resolution_clock::now();
                displayTransactions("Sorted " + channelName + " Transactions", *selected);
                auto duration = duration_cast<milliseconds>(end - start);
                size_t spaceUsed = selected->size() * sizeof(SimpleTransaction);
                double memoryMB = spaceUsed / (1024.0 * 1024.0);
                showPerformanceMetrics("Sort Transactions", duration.count(), memoryMB, spaceUsed);
                cout << "Press ENTER to go to the main menu...";
                cin.ignore();
                cin.get();
            } else {
                cout << "No transactions in this channel to sort.\n";
            }
        }
    }

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

    return 0;
}