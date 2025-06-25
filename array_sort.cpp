#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <iomanip> // for setw

using namespace std;
using namespace std::chrono;

//Function to remove unwanted whitespace from strings
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Structure for transaction data
struct Transaction {
    string type;
    double amount;
    string nameOrig;
    string nameDest;
    string paymentChannel;
    string location;
};

// Dynamic array for transactions
template <typename T>
class DynamicArray {
private:
    T* data;
    int current_size;
    int current_capacity;

    void resize() {
        current_capacity = current_capacity + (current_capacity / 2);
        if (current_capacity == 0) current_capacity = 10;
        T* newData = new T[current_capacity];
        for (int i = 0; i < current_size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray(int initialCapacity = 10) : current_size(0), current_capacity(initialCapacity) {
        data = new T[current_capacity];
    }
    ~DynamicArray() {
        delete[] data;
    }
    void add(const T& item) {
        if (current_size == current_capacity) resize();
        data[current_size++] = item;
    }
    T& operator[](int index) {
        if (index < 0 || index >= current_size) throw out_of_range("Index out of bounds");
        return data[index];
    }
    int size() const { return current_size; }
    bool isEmpty() const { return current_size == 0; }
};

// Mergesort function 
void merge(DynamicArray<Transaction>& arr, int l, int mid, int r) {
    int n1 = mid - l + 1;
    int n2 = r - mid;
    Transaction* a = new Transaction[n1];
    Transaction* b = new Transaction[n2];

    for (int i = 0; i < n1; i++) a[i] = arr[l + i];
    for (int i = 0; i < n2; i++) b[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (a[i].location <= b[j].location) arr[k++] = a[i++];
        else arr[k++] = b[j++];
    }
    while (i < n1) arr[k++] = a[i++];
    while (j < n2) arr[k++] = b[j++];

    delete[] a;
    delete[] b;
}

void mergesort(DynamicArray<Transaction>& arr, int l, int r) {
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergesort(arr, l, mid);
        mergesort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }
}

// Normalize the payment channel 
string normalizeChannel(string channel) {
    transform(channel.begin(), channel.end(), channel.begin(), ::tolower);
    if (channel == "card") return "Card";
    if (channel == "ach") return "ACH";
    if (channel == "upi") return "UPI";
    if (channel == "wire" || channel == "wire_transfer" || channel == "bank_transfer" || channel == "wire transfer") return "Bank Transfer";
    return "Unknown";
}

// Display transactions
void displayTransactions(const string& title, DynamicArray<Transaction>& arr) {
    cout << "\n--- " << title << " ---" << endl;
    if (arr.isEmpty()) {
        cout << "No transactions to display." << endl;
        return;
    }
    cout << "Total transactions: " << arr.size() << endl;
    cout << "---------------------------------------------------------" << endl;
    for (int i = 0; i < arr.size(); ++i) {
        cout << "Location: " << arr[i].location
             << " | Channel: " << arr[i].paymentChannel
             << " | Type: " << arr[i].type
             << " | Amount: " << arr[i].amount << endl;
    }
    cout << "---------------------------------------------------------" << endl;
}

// Loading data from CSV file
void loadTransactions(const string& filename,
                      DynamicArray<Transaction>& card,
                      DynamicArray<Transaction>& ach,
                      DynamicArray<Transaction>& upi,
                      DynamicArray<Transaction>& bankTransfer) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    string line;
    getline(file, line); 
    int count = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        Transaction t;

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

            getline(ss, field, ','); // ip_address - skip
            getline(ss, field, ','); // device_hash - skip

            if (t.paymentChannel == "Card") card.add(t);
            else if (t.paymentChannel == "ACH") ach.add(t);
            else if (t.paymentChannel == "UPI") upi.add(t);
            else if (t.paymentChannel == "Bank Transfer") bankTransfer.add(t);

            count++;
        } catch (...) {
            // skip malformed lines
        }
    }

    cout << "Loaded and processed " << count << " transactions." << endl;
    file.close();
}

// Show performance metrics
void showPerformanceMetrics(const string& operation, long long timeMs, double memoryMB, size_t spaceUsed) {
    cout << "\n+------------------------------- PERFORMANCE METRICS -------------------------------+" << endl;
    cout << "| Operation         | " << setw(60) << left << operation << "|\n";
    cout << "|-------------------+--------------------------------------------------------------|" << endl;
    cout << "| Execution Time    | " << setw(60) << left << (to_string(timeMs) + " ms") << "|\n";
    cout << "| Memory Usage      | " << setw(60) << left << (to_string(memoryMB) + " MB") << "|\n";
    cout << "| Space Used        | " << setw(60) << left << (to_string(spaceUsed) + " bytes") << "|\n";
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << "Press Enter to exit: ";
    cin.ignore();
    cin.get(); // Wait for Enter
    exit(0);   // Exit program immediately
}

// Menu selection
DynamicArray<Transaction>* selectChannel(DynamicArray<Transaction>& card,
                                          DynamicArray<Transaction>& ach,
                                          DynamicArray<Transaction>& upi,
                                          DynamicArray<Transaction>& bank,
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

// Main function
int main() {
    srand(time(0));
    DynamicArray<Transaction> cardTransactions;
    DynamicArray<Transaction> achTransactions;
    DynamicArray<Transaction> upiTransactions;
    DynamicArray<Transaction> bankTransferTransactions;

    cout << "Loading data from file, please wait...\n";

    loadTransactions("/Users/suhanashabhik/Documents/DS/Assignment/financial_fraud_detection_dataset.csv",
                     cardTransactions, achTransactions, upiTransactions, bankTransferTransactions);

    int choice = 0;
    while (choice != 3) {
        cout << "\n========== FINANCIAL DATA SORTER ==========" << endl;
        cout << "1. Sort Transactions by Location" << endl;
        cout << "2. Display Transactions by Channel" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        string channelName;
        DynamicArray<Transaction>* selected = nullptr;

        switch (choice) {
            case 1:
                selected = selectChannel(cardTransactions, achTransactions, upiTransactions, bankTransferTransactions, channelName);
                if (selected && !selected->isEmpty()) {
                    auto startSort = high_resolution_clock::now();
                    mergesort(*selected, 0, selected->size() - 1);
                    auto endSort = high_resolution_clock::now();
                    displayTransactions("Sorted " + channelName + " Transactions", *selected);
                    auto sortDuration = duration_cast<milliseconds>(endSort - startSort);

                    size_t spaceUsed = selected->size() * sizeof(Transaction);
                    double memoryMB = spaceUsed / (1024.0 * 1024.0);
                    showPerformanceMetrics("Search Transactions", sortDuration.count(), memoryMB, spaceUsed);
                } else {
                    cout << "No transactions in this channel to sort.\n";
                }
                break;

            case 2:
                selected = selectChannel(cardTransactions, achTransactions, upiTransactions, bankTransferTransactions, channelName);
                if (selected) {
                    displayTransactions(channelName + " Transactions (Unsorted)", *selected);
                }
                break;

            case 3:
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}