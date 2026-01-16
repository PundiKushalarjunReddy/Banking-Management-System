#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main() {
    int choice;
    //A simple menu loop to keep the program running
    while(1){
        printf("\n=== Banking System User Menu ===\n");
        printf("\n1. Create New Account\n");
        printf("\n2. Deposit Money\n");
        printf("\n3. Withdraw Money\n");
        printf("\n4. View Account Details\n");
        printf("\n5. Check Balance\n ");
        printf("\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice){
            case 1:
             create_account();
                break;
            case 2:
                // deposit_money();
                
                break;
            case 3:
                // withdraw_money();
                
                break;
            case 4:
                // view_account_details();
                
                break;
            case 5:
                // check_balance();
                
                break;
            case 6:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            case 99: { // Hidden Option
               int adminID;
              char adminPass[20];
    
              printf("\n[SECURE ACCESS]: Enter Admin ID: ");
              scanf("%d", &adminID);
              printf("[SECURE ACCESS]: Enter Secret Code: ");
              scanf("%s", adminPass);

              // Hardcoded credentials for the simulation
              if (adminID == 777 && strcmp(adminPass, "mogul123") == 0) {
                  printf("\n--- ADMINISTRATIVE ACCESS GRANTED ---");
                   // view_all_accounts(); // This function will be built tomorrow
             } else {
                  printf("\nInvalid Selection.\n"); // Hidden in plain sight
                                         break;}
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
typedef struct{
    int acc_no;
    char holder_name[100];
    double balance;
    int is_active;
} Account;
int create_account() {
    Account new_acc;
    FILE *fp = fopen("records.dat", "ab"); //'ab' means append at the end in binary

    if (fp == NULL)
{
    printf("System Error: Could not access database. \n");
    return 1;
}
printf("\n--- Create New Account ---\n");
printf("Enter Account Number: ");
scanf("%d", &new_acc.acc_no);
 // Using %[^\n] to read string with spaces in names
 printf("Enter Account Holder Name: ");
 getchar(); // Clear the newline left by scanf
 scanf("%[^\n]s", new_acc.holder_name);
 new_acc.balance=0.0;//new acc starts as zero
 new_acc.is_active=1;//Mark as active
 // THE COMMIT: Writing 1 instance of 'Account' size to the file
 fwrite(&new_acc, sizeof(Account), 1, fp);
 fclose(fp);
 printf("\nAccount created and saved successfully!\n");
 return 0;
}