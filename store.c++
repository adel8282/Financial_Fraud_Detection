// main.cpp or store.c++
#include <iostream>  // For console input/output
#include <fstream>   // For file input/output (std::ifstream)
#include <sstream>   // For parsing lines (std::stringstream)
#include <string>    // For string manipulation (std::string, std::stod)
#include <chrono>    // For performance measurement (timing)
#include <algorithm> // For std::min

#include "TransactionArray.h" // Your custom array structure
#include "Transaction.h"      // Your Transaction class (TransactionArray.h already includes it, but harmless to include again)


// Function to parse a single CSV line into a Transaction object
// IMPORTANT: This function MUST accurately parse your CSV file's column order.
// Double-check the order of columns in your actual financial_fraud_detection.csv file.
Transaction parseCsvLine(const std::string& line) {
    std::stringstream ss(line);
    std::string segment;

    // Declare variables to hold parsed data
    std::string transaction_id;
    std::string timestamp;
    std::string sender_account;
    std::string receiver_account;
    double amount;
    std::string transaction_type;
    std::string merchant_category;
    std::string location;
    std::string device_used;
    bool is_fraud;
    std::string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    double velocity_score;
    double geo_anomaly_score;
    std::string payment_channel;
    std::string ip_address;
    std::string device_hash;

    try {
        // transaction_id
        std::getline(ss, transaction_id, ',');
        // timestamp
        std::getline(ss, timestamp, ',');
        // sender_account
        std::getline(ss, sender_account, ',');
        // receiver_account
        std::getline(ss, receiver_account, ',');
        // amount
        std::getline(ss, segment, ',');
        amount = std::stod(segment);
        // transaction_type
        std::getline(ss, transaction_type, ',');
        // merchant_category
        std::getline(ss, merchant_category, ',');
        // location
        std::getline(ss, location, ',');
        // device_used
        std::getline(ss, device_used, ',');
        // is_fraud (Assuming "1" for true, "0" for false in CSV)
        std::getline(ss, segment, ',');
        is_fraud = (segment == "1" || segment == "True" || segment == "true"); // Be robust with boolean parsing
        // fraud_type
        std::getline(ss, fraud_type, ',');
        // time_since_last_transaction
        std::getline(ss, segment, ',');
        time_since_last_transaction = std::stod(segment);
        // spending_deviation_score
        std::getline(ss, segment, ',');
        spending_deviation_score = std::stod(segment);
        // velocity_score
        std::getline(ss, segment, ',');
        velocity_score = std::stod(segment);
        // geo_anomaly_score
        std::getline(ss, segment, ',');
        geo_anomaly_score = std::stod(segment);
        // payment_channel
        std::getline(ss, payment_channel, ',');
        // ip_address
        std::getline(ss, ip_address, ',');
        // device_hash (This should be the last one, no comma after it)
        std::getline(ss, device_hash);


        return Transaction(transaction_id, timestamp, sender_account, receiver_account,
                           amount, transaction_type, merchant_category, location,
                           device_used, is_fraud, fraud_type,
                           time_since_last_transaction, spending_deviation_score,
                           velocity_score, geo_anomaly_score,
                           payment_channel, ip_address, device_hash);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing line: '" << line << "' - " << e.what() << std::endl;
        return Transaction(); // Return a default-constructed (empty/invalid) Transaction
    }
}


int main() {
    // Instantiate your custom array data structure
    TransactionArray myTransactions(10000); // Start with a reasonable initial capacity

    // Open the CSV file
    std::ifstream file("financial_fraud_detection.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open financial_fraud_detection.csv. Make sure it's in the same directory as your executable." << std::endl;
        return 1; // Indicate error
    }

    std::string line;
    // Read and discard the header line
    std::getline(file, line);

    auto startLoad = std::chrono::high_resolution_clock::now(); // Start timer for loading
    int recordsLoaded = 0;

    // Read data line by line
    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        Transaction transaction = parseCsvLine(line);
        // Basic validation: skip if transaction_id is empty, implying parsing error
        if (!transaction.transaction_id.empty()) {
             myTransactions.addTransaction(transaction);
             recordsLoaded++;
        } else {
             std::cerr << "Skipped malformed transaction due to parsing error on line: '" << line << "'" << std::endl;
        }
    }
    file.close();

    auto endLoad = std::chrono::high_resolution_clock::now(); // End timer for loading
    std::chrono::duration<double> loadDuration = endLoad - startLoad;
    std::cout << "Loaded " << recordsLoaded << " transactions in " << loadDuration.count() << " seconds." << std::endl;

    // --- Functional Requirements ---

    // 1. Display all financial transactions on the console output (Displaying first 10 for brevity)
    std::cout << "\n--- Displaying All Transactions (First 10 for brevity) ---" << std::endl;
    for (int i = 0; i < std::min(10, myTransactions.getSize()); ++i) {
        myTransactions[i].display();
    }
    if (myTransactions.getSize() > 10) {
        std::cout << "... and " << myTransactions.getSize() - 10 << " more." << std::endl;
    }


    // 2. Sort financial transactions by location in ascending order
    std::cout << "\n--- Sorting Transactions by Location ---" << std::endl;
    auto startSort = std::chrono::high_resolution_clock::now();
    myTransactions.sortTransactionsByLocation(); // Call your Merge Sort
    auto endSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sortDuration = endSort - startSort;
    std::cout << "Sorting completed in " << sortDuration.count() << " seconds." << std::endl;

    // Display sorted transactions (first 10 for brevity)
    std::cout << "\n--- Displaying Sorted Transactions by Location (First 10) ---" << std::endl;
    for (int i = 0; i < std::min(10, myTransactions.getSize()); ++i) {
        myTransactions[i].display();
    }


    // 3. Search the specific transaction type
    std::string searchType = "Online Purchase"; // Example transaction type, adjust as per your data
    std::cout << "\n--- Searching for transaction type: '" << searchType << "' ---" << std::endl;
    auto startSearch = std::chrono::high_resolution_clock::now();
    TransactionArray foundTransactions = myTransactions.searchTransactionsByType(searchType); // Call your Linear Search
    auto endSearch = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchDuration = endSearch - startSearch;
    std::cout << "Search completed in " << searchDuration.count() << " seconds." << std::endl;
    std::cout << "Found " << foundTransactions.getSize() << " transactions of type '" << searchType << "'" << std::endl;
    // Display found transactions (first 5 for brevity)
    for (int i = 0; i < std::min(5, foundTransactions.getSize()); ++i) {
        foundTransactions[i].display();
    }


    // 4. Store financial transactions separately according to payment channel
    std::string filterChannel = "Online"; // Example payment channel, adjust as per your data
    std::cout << "\n--- Filtering transactions by payment channel: '" << filterChannel << "' ---" << std::endl;
    TransactionArray filteredChannelTransactions = myTransactions.filterByPaymentChannel(filterChannel);
    std::cout << "Found " << filteredChannelTransactions.getSize() << " transactions via '" << filterChannel << "' channel." << std::endl;
    // Display filtered transactions (first 5 for brevity)
    for (int i = 0; i < std::min(5, filteredChannelTransactions.getSize()); ++i) {
        filteredChannelTransactions[i].display();
    }


    // 5. Generate JSON format from the specific data collection
    std::cout << "\n--- Generating JSON (First 5 records from filtered transactions for brevity) ---" << std::endl;
    // Create a temporary TransactionArray with a few records to demonstrate JSON
    TransactionArray smallJsonSet;
    for (int i = 0; i < std::min(5, filteredChannelTransactions.getSize()); ++i) {
        smallJsonSet.addTransaction(filteredChannelTransactions[i]);
    }
    std::string jsonOutput = smallJsonSet.generateJson();
    std::cout << jsonOutput << std::endl;

    // Remember to compare time and memory efficiency in your documentation
    // This program already logs execution time for load, sort, search.

    return 0;
}