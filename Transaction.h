// Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>    // For std::string
#include <iostream>  // For std::cout in the display() method

class Transaction {
public:
    std::string transaction_id;
    std::string timestamp;
    std::string sender_account;
    std::string receiver_account;
    double amount;
    std::string transaction_type;
    std::string merchant_category;
    std::string location; // Used for sorting
    std::string device_used;
    bool is_fraud;
    std::string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    double velocity_score;
    double geo_anomaly_score;
    std::string payment_channel; // Used for filtering
    std::string ip_address;
    std::string device_hash;

    // Default constructor
    Transaction() : transaction_id(""), timestamp(""), sender_account(""), receiver_account(""),
                    amount(0.0), transaction_type(""), merchant_category(""), location(""),
                    device_used(""), is_fraud(false), fraud_type(""),
                    time_since_last_transaction(0.0), spending_deviation_score(0.0),
                    velocity_score(0.0), geo_anomaly_score(0.0),
                    payment_channel(""), ip_address(""), device_hash("") {}

    // Parameterized constructor
    Transaction(std::string id, std::string ts, std::string sender, std::string receiver,
                double amt, std::string type, std::string merchant, std::string loc,
                std::string device, bool fraud, std::string ftype,
                double time_last, double spending_dev, double velocity, double geo_anom,
                std::string channel, std::string ip, std::string hash)
        : transaction_id(id), timestamp(ts), sender_account(sender), receiver_account(receiver),
          amount(amt), transaction_type(type), merchant_category(merchant), location(loc),
          device_used(device), is_fraud(fraud), fraud_type(ftype),
          time_since_last_transaction(time_last), spending_deviation_score(spending_dev),
          velocity_score(velocity), geo_anomaly_score(geo_anom),
          payment_channel(channel), ip_address(ip), device_hash(hash) {}

    // Getter methods for required functionalities
    std::string getLocation() const { return location; }
    std::string getTransactionType() const { return transaction_type; }
    std::string getPaymentChannel() const { return payment_channel; }
    std::string getTransactionId() const { return transaction_id; }
    double getAmount() const { return amount; }

    // Overload < operator for sorting by location in ascending order
    bool operator<(const Transaction& other) const {
        return location < other.location;
    }

    // Overload == operator for potential search/comparison by ID
    bool operator==(const Transaction& other) const {
        return transaction_id == other.transaction_id;
    }

    // Helper to display details (you can customize what's shown)
    void display() const {
        std::cout << "ID: " << transaction_id
                  << ", Time: " << timestamp
                  << ", Amt: " << amount
                  << ", Type: " << transaction_type
                  << ", Location: " << location
                  << ", Channel: " << payment_channel
                  << ", Fraud: " << (is_fraud ? "Yes" : "No")
                  << std::endl;
    }
};

#endif // TRANSACTION_H