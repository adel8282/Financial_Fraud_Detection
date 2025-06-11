// TransactionArray.h
#ifndef TRANSACTION_ARRAY_H
#define TRANSACTION_ARRAY_H

#include "Transaction.h" // Include your Transaction class
#include <stdexcept>     // For std::out_of_range

class TransactionArray {
private:
    Transaction* transactions; // Pointer to the dynamically allocated array
    int currentSize;           // Current number of transactions stored
    int arrayCapacity;         // Total capacity of the array

    // Private helper method to double the array's capacity
    void resize(); // Declaration only, implementation in .cpp

    // Private recursive function for Merge Sort
    // Declared here, but definition (actual code) will be in TransactionArray.cpp
    // or a separate helper. For now, declare it static if you want to keep it in .cpp
    // without making it a member function. Or make it a non-member helper.
    // For simplicity, let's declare it as a helper function that will be defined in the .cpp
    // You could also make it a static member of TransactionArray if you prefer.
    // For clarity, I'll put it as a non-member helper function in the .cpp, so no declaration here.

public:
    // Constructor
    TransactionArray(int initialCapacity = 100);

    // Destructor to free dynamically allocated memory
    ~TransactionArray();

    // Add a transaction to the array
    void addTransaction(const Transaction& transaction);

    // Get a transaction by index (for sorting/searching algorithms)
    Transaction& getTransaction(int index);
    const Transaction& getTransaction(int index) const; // const version

    // Get the current number of transactions
    int getSize() const { return currentSize; }

    // Display all transactions in the array
    void displayAllTransactions() const;

    // Sorting methods (declaration)
    void sortTransactionsByLocation(); // This will call your Merge Sort implementation

    // Searching methods (declaration)
    TransactionArray searchTransactionsByType(const std::string& type) const;

    // Method to filter by payment channel
    TransactionArray filterByPaymentChannel(const std::string& channel) const;

    // Method to generate JSON (declaration)
    std::string generateJson() const;

    // Overload [] operator for easier access
    Transaction& operator[](int index);
    const Transaction& operator[](int index) const;
};

#endif // TRANSACTION_ARRAY_H