#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Structure Definition ---
typedef struct {
    int accNumber;
    char name[50];
    float balance;
} Account;

// --- Function Prototypes ---
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void viewTransactionHistory();
int getAccountIndex(int id);
void logTransaction(int id, const char *message, float amount, float newBalance);

const char *FILE_NAME = "accounts.dat";

int main() {
    int choice;
    
    while (1) {
        printf("\n=== Banking Management System ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Transaction History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: viewTransactionHistory(); break;
            case 6: 
                printf("Exiting... Thank you for banking with us!\n");
                exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// --- 1. Create Account ---
void createAccount() {
    Account acc;
    FILE *fp = fopen(FILE_NAME, "ab+"); // Append/Binary/Read mode
    
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number (Integer): ");
    scanf("%d", &acc.accNumber);

    // Check if account already exists
    Account temp;
    rewind(fp); // Go to start of file to check duplicates
    while (fread(&temp, sizeof(Account), 1, fp)) {
        if (temp.accNumber == acc.accNumber) {
            printf("Error: Account number %d already exists!\n", acc.accNumber);
            fclose(fp);
            return;
        }
    }

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", acc.name); // Read string with spaces
    acc.balance = 0.0; // Initial balance is 0

    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);
    
    // Create the history log file for this user
    logTransaction(acc.accNumber, "Account Created", 0, 0);
    printf("Account created successfully!\n");
}

// --- 2. Deposit Money ---
void depositMoney() {
    int id, found = 0;
    float amount;
    Account acc;
    FILE *fp = fopen(FILE_NAME, "rb+"); // Read/Write Binary

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNumber == id) {
            found = 1;
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount.\n");
            } else {
                acc.balance += amount;
                
                // Move file pointer back to update the record
                fseek(fp, -sizeof(Account), SEEK_CUR);
                fwrite(&acc, sizeof(Account), 1, fp);
                
                printf("Deposited $%.2f. New Balance: $%.2f\n", amount, acc.balance);
                logTransaction(id, "Deposit", amount, acc.balance);
            }
            break;
        }
    }
    
    if (!found) printf("Account not found!\n");
    fclose(fp);
}

// --- 3. Withdraw Money ---
void withdrawMoney() {
    int id, found = 0;
    float amount;
    Account acc;
    FILE *fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNumber == id) {
            found = 1;
            printf("Current Balance: $%.2f\n", acc.balance);
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount > acc.balance) {
                printf("Insufficient funds!\n");
            } else if (amount <= 0) {
                printf("Invalid amount.\n");
            } else {
                acc.balance -= amount;
                
                fseek(fp, -sizeof(Account), SEEK_CUR);
                fwrite(&acc, sizeof(Account), 1, fp);
                
                printf("Withdrawn $%.2f. New Balance: $%.2f\n", amount, acc.balance);
                logTransaction(id, "Withdrawal", amount, acc.balance);
            }
            break;
        }
    }

    if (!found) printf("Account not found!\n");
    fclose(fp);
}

// --- 4. Check Balance ---
void checkBalance() {
    int id, found = 0;
    Account acc;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- Check Balance ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNumber == id) {
            found = 1;
            printf("\nAccount ID: %d\n", acc.accNumber);
            printf("Holder Name: %s\n", acc.name);
            printf("Current Balance: $%.2f\n", acc.balance);
            break;
        }
    }

    if (!found) printf("Account not found!\n");
    fclose(fp);
}

// --- 5. Transaction History ---
void viewTransactionHistory() {
    int id;
    char filename[50];
    char line[100];
    
    printf("\n--- Transaction History ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &id);
    
    // Construct filename: e.g., "101_log.txt"
    sprintf(filename, "%d_log.txt", id);
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No history found for account %d.\n", id);
        return;
    }
    
    printf("\nHistory for Account %d:\n", id);
    printf("--------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("--------------------------------------------------\n");
    fclose(fp);
}

// --- Helper: Log Transaction ---
void logTransaction(int id, const char *message, float amount, float newBalance) {
    char filename[50];
    sprintf(filename, "%d_log.txt", id);
    
    FILE *fp = fopen(filename, "a"); // Append mode
    if (fp == NULL) return;
    
    if (amount == 0) {
        fprintf(fp, "[%s] - Balance: $%.2f\n", message, newBalance);
    } else {
        fprintf(fp, "[%s] Amount: $%.2f | Balance: $%.2f\n", message, amount, newBalance);
    }
    
    fclose(fp);
}