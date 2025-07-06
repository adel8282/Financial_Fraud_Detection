# 💵Financial Fraud Detection System

A C++-based system to detect and analyze fraudulent transactions using both Array and Linked List implementations. It allows users to search and sort transaction data, providing performance metrics like execution time, memory usage, and space usage.

STL is not used; all data structures are implemented manually.

---

## 📋 Features

### **Array - Adel (Search)**
	- Search transactions by type using Linear Search.
	- Separate arrays for each payment channel (Card, ACH, Wire Transfer, UPI).
	- JSON export of matched results.
	- Performance metrics: time, memory, space.

---

### **Array - Suhana (Sort)**
	- Sort transactions by location using Merge Sort.
	- Sorting is done per payment channel.
	- JSON export of sorted data.
	- Performance metrics: time, memory, space.

---

### **Linked List - Sohaib (Search)**
	- Search transactions by type using Linear Search.
	- Linked lists created for each payment channel.
	- JSON export of matched results.
	- Performance metrics: time, memory, space.

---

### **Linked List - Mahmood (Sort)**
	- Sort transactions by location using Merge Sort.
	- Sorting is done on linked list nodes.
	- JSON export of sorted data.
	- Performance metrics: time, memory, space.

 ## 🧾 CSV Fields Used (18 Total)
	- transaction_id, timestamp, sender_account, reciver_amount, amount
	- transaction_type, merchant_category, location, device_used
	- is_fraud, fraud_type, time_since_last_transaction, spending_deviation_score
	- velocity_score, geo_anomaly_score, payement_channel, ip_address, device_hash

 ## 📂 JSON Output Files
	- search_results_array.json
	- search_results_list.json
	- sort_results_array.json
	- sort_results_list.json

### Each includes: transaction details, search/sort type, execution time, memory, space usage.



## 🛠️ Tech Summary
	- Language: C++
	- Concepts: Dynamic Memory, Manual Array & Linked List Handling, Merge Sort, Linear Search, Performance Profiling, JSON Export
	- Libraries: nlohmann/json
