// TransactionArray.cpp
#include "TransactionArray.h" // Include its own header, which pulls in Transaction.h
#include <iostream>           // For display methods and debugging
#include <string>             // For std::string
#include <sstream>            // For JSON generation
#include <algorithm>          // For std::min (used in main.cpp for display limits, not strictly needed here)


// --- Private Helper Functions for Merge Sort ---
// These are not members of TransactionArray, but helpers used by it.
void merge(Transaction* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] < R[j]) { // Uses overloaded operator< for comparison
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L; // Free temp memory
    delete[] R; // Free temp memory
}

// Private recursive function for Merge Sort
void mergeSort(Transaction* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Avoid overflow for large l and r
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
// --- End Private Helper Functions for Merge Sort ---


// --- TransactionArray Class Implementations ---

// Private resize method
void TransactionArray::resize() {
    int newCapacity = arrayCapacity * 2;
    if (newCapacity == 0) newCapacity = 10; // Handle initial empty array case
    Transaction* newTransactions = new Transaction[newCapacity];
    for (int i = 0; i < currentSize; ++i) {
        newTransactions[i] = transactions[i];
    }
    delete[] transactions; // Free the old array memory
    transactions = newTransactions;
    arrayCapacity = newCapacity;
    // std::cout << "Array resized to capacity: " << arrayCapacity << std::endl; // For debugging
}

// Constructor
TransactionArray::TransactionArray(int initialCapacity) : currentSize(0), arrayCapacity(initialCapacity) {
    if (initialCapacity <= 0) initialCapacity = 100; // Ensure a positive initial capacity
    transactions = new Transaction[arrayCapacity];
}

// Destructor
TransactionArray::~TransactionArray() {
    delete[] transactions;
}

// Add a transaction
void TransactionArray::addTransaction(const Transaction& transaction) {
    if (currentSize == arrayCapacity) {
        resize(); // Resize if capacity is reached
    }
    transactions[currentSize++] = transaction;
}

// Get a transaction by index (non-const version)
Transaction& TransactionArray::getTransaction(int index) {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("TransactionArray: Index out of bounds (non-const)");
    }
    return transactions[index];
}

// Get a transaction by index (const version)
const Transaction& TransactionArray::getTransaction(int index) const {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("TransactionArray: Index out of bounds (const)");
    }
    return transactions[index];
}

// Display all transactions
void TransactionArray::displayAllTransactions() const {
    if (currentSize == 0) {
        std::cout << "No transactions to display." << std::endl;
        return;
    }
    std::cout << "--- All Transactions (" << currentSize << ") ---" << std::endl;
    for (int i = 0; i < currentSize; ++i) {
        transactions[i].display(); // Use the display method of Transaction
    }
    std::cout << "-----------------------------------" << std::endl;
}

// Sort financial transactions by location in ascending order
void TransactionArray::sortTransactionsByLocation() {
    if (currentSize <= 1) return; // No need to sort if 0 or 1 element
    mergeSort(transactions, 0, currentSize - 1); // Pass the internal array and its bounds
}

// Search for specific transaction type using Linear Search
TransactionArray TransactionArray::searchTransactionsByType(const std::string& type) const {
    TransactionArray results; // Create a new array to store matching results
    // Iterate through all transactions and add matches to results
    for (int i = 0; i < currentSize; ++i) {
        if (transactions[i].getTransactionType() == type) {
            results.addTransaction(transactions[i]);
        }
    }
    return results;
}

// Store financial transactions separately according to payment channel
TransactionArray TransactionArray::filterByPaymentChannel(const std::string& channel) const {
    TransactionArray filteredResults;
    for (int i = 0; i < currentSize; ++i) {
        if (transactions[i].getPaymentChannel() == channel) {
            filteredResults.addTransaction(transactions[i]);
        }
    }
    return filteredResults;
}

// Generate the JSON format from the specific data collection
std::string TransactionArray::generateJson() const {
    std::stringstream ss;
    ss << "[\n";
    for (int i = 0; i < currentSize; ++i) {
        ss << "  {\n";
        ss << "    \"transaction_id\": \"" << transactions[i].transaction_id << "\",\n";
        ss << "    \"timestamp\": \"" << transactions[i].timestamp << "\",\n";
        ss << "    \"sender_account\": \"" << transactions[i].sender_account << "\",\n";
        ss << "    \"receiver_account\": \"" << transactions[i].receiver_account << "\",\n";
        ss << "    \"amount\": " << transactions[i].amount << ",\n";
        ss << "    \"transaction_type\": \"" << transactions[i].transaction_type << "\",\n";
        ss << "    \"merchant_category\": \"" << transactions[i].merchant_category << "\",\n";
        ss << "    \"location\": \"" << transactions[i].location << "\",\n";
        ss << "    \"device_used\": \"" << transactions[i].device_used << "\",\n";
        ss << "    \"is_fraud\": " << (transactions[i].is_fraud ? "true" : "false") << ",\n";
        ss << "    \"fraud_type\": \"" << transactions[i].fraud_type << "\",\n";
        ss << "    \"time_since_last_transaction\": " << transactions[i].time_since_last_transaction << ",\n";
        ss << "    \"spending_deviation_score\": " << transactions[i].spending_deviation_score << ",\n";
        ss << "    \"velocity_score\": " << transactions[i].velocity_score << ",\n";
        ss << "    \"geo_anomaly_score\": " << transactions[i].geo_anomaly_score << ",\n";
        ss << "    \"payment_channel\": \"" << transactions[i].payment_channel << "\",\n";
        ss << "    \"ip_address\": \"" << transactions[i].ip_address << "\",\n";
        ss << "    \"device_hash\": \"" << transactions[i].device_hash << "\"\n";
        ss << "  }" << (i == currentSize - 1 ? "" : ",") << "\n";
    }
    ss << "]\n";
    return ss.str();
}

// Overload [] operator for easier access (non-const)
Transaction& TransactionArray::operator[](int index) {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Array index out of bounds (non-const)");
    }
    return transactions[index];
}

// Overload [] operator for easier access (const)
const Transaction& TransactionArray::operator[](int index) const {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Array index out of bounds (const)");
    }
    return transactions[index];
}