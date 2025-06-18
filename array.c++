#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MAX_TRANSACTIONS = 10000;

struct Transaction {
    char transaction_id[50];
    char location[50];
    char transaction_type[50];
    char payment_channel[50];
    double amount;
};

// Arrays for each payment channel
Transaction online[MAX_TRANSACTIONS];
Transaction mobile[MAX_TRANSACTIONS];
Transaction branch[MAX_TRANSACTIONS];
int onlineCount = 0, mobileCount = 0, branchCount = 0;

// Function to split a CSV line into fields
void parseCSVLine(char* line, Transaction& t) {
    int col = 0, start = 0;
    char field[256];
    bool inQuotes = false;
    int len = strlen(line);

    for (int i = 0, j = 0; i <= len; i++) {
        char c = line[i];

        if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
            inQuotes = !inQuotes;
        } else if ((c == ',' || c == '\0') && !inQuotes) {
            int fieldLen = i - start;
            if (fieldLen >= 255) fieldLen = 255;
            strncpy(field, &line[start], fieldLen);
            field[fieldLen] = '\0';

            switch (col) {
                case 0: strncpy(t.transaction_id, field, 49); break;
                case 4: t.amount = atof(field); break;
                case 5: strncpy(t.transaction_type, field, 49); break;
                case 7: strncpy(t.location, field, 49); break;
                case 15: strncpy(t.payment_channel, field, 49); break;
            }

            col++;
            start = i + 1;
        }
    }
}

// Load and store transactions
void loadTransactions(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    char line[512];
    file.getline(line, 512); // Skip header

    while (file.getline(line, 512)) {
        Transaction t;
        parseCSVLine(line, t);

        if (strcmp(t.payment_channel, "online") == 0) {
            online[onlineCount++] = t;
        } else if (strcmp(t.payment_channel, "mobile") == 0) {
            mobile[mobileCount++] = t;
        } else if (strcmp(t.payment_channel, "branch") == 0) {
            branch[branchCount++] = t;
        }
    }

    file.close();
}

// Merge sort helper
int compareLocation(const Transaction &a, const Transaction &b) {
    return strcmp(a.location, b.location);
}

void merge(Transaction arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareLocation(L[i], R[j]) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Transaction arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + ('a' - 'A');
    }
}

// Linear search
void linearSearch(Transaction arr[], int size, const char* type) {
    bool found = false;
    cout << "\nSearch results for transaction type: " << type << endl;

    char tempType[50];
    strncpy(tempType, type, 49);
    toLowerCase(tempType);

    for (int i = 0; i < size; i++) {
        char currentType[50];
        strncpy(currentType, arr[i].transaction_type, 49);
        toLowerCase(currentType);

        if (strcmp(currentType, tempType) == 0) {
            found = true;
            cout << arr[i].transaction_id << " | "
                 << arr[i].transaction_type << " | "
                 << arr[i].location << " | "
                 << arr[i].payment_channel << " | $"
                 << arr[i].amount << endl;
        }
    }

    if (!found) {
        cout << "No matching transactions found." << endl;
    }
}

// Main
int main() {
    loadTransactions("financial_fraud_detection.csv");

    // Sort each channel's data by location
    mergeSort(online, 0, onlineCount - 1);
    mergeSort(mobile, 0, mobileCount - 1);
    mergeSort(branch, 0, branchCount - 1);

    // Show sorted results (example: online)
    cout << "\nSorted Online Transactions by Location:\n";
    for (int i = 0; i < (onlineCount < 5 ? onlineCount : 5); i++) {
        cout << online[i].transaction_id << " | "
             << online[i].location << " | "
             << online[i].transaction_type << " | $"
             << online[i].amount << endl;
    }

    // Prompt user for a transaction type to search
    char userSearchType[50];
    cout << "\nEnter a transaction type to search (e.g., deposit, withdrawal, transfer, payment): ";
    cin.getline(userSearchType, 50);

    cout << "\nSearching in ONLINE transactions:";
    linearSearch(online, onlineCount, userSearchType);

    cout << "\nSearching in MOBILE transactions:";
    linearSearch(mobile, mobileCount, userSearchType);

    cout << "\nSearching in BRANCH transactions:";
    linearSearch(branch, branchCount, userSearchType);

    return 0;
}