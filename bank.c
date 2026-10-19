
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "bank.dat"

typedef struct
{
    int accountNumber;
    char name[50];
    double balance;
} BankAccount;

int loadAccount(BankAccount *account)
{
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
        return 0;

    int success = fread(account, sizeof(BankAccount), 1, fp) == 1;
    fclose(fp);

    return success;
}

int saveAccount(const BankAccount *account)
{
    FILE *fp = fopen(FILE_NAME, "wb");

    if (fp == NULL)
    {
        printf("Error saving account data!\n");
        return 0;
    }

    int success = fwrite(account, sizeof(BankAccount), 1, fp) == 1;

    if (fclose(fp) != 0)
        success = 0;

    if (!success)
        printf("Error writing account data!\n");

    return success;
}

void createAccount(BankAccount *account)
{
    printf("\nEnter Account Number: ");

    if (scanf("%d", &account->accountNumber) != 1 ||
        account->accountNumber <= 0)
    {
        printf("Invalid account number!\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter Account Holder Name: ");

    if (scanf(" %49[^\n]", account->name) != 1)
    {
        printf("Invalid name!\n");
        exit(EXIT_FAILURE);
    }

    account->balance = 0.0;

    if (saveAccount(account))
        printf("Account created successfully!\n");
}

void deposit(BankAccount *account)
{
    double amount;

    printf("Enter amount to deposit: ");

    if (scanf("%lf", &amount) != 1)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Amount must be greater than zero.\n");
        return;
    }

    account->balance += amount;

    if (saveAccount(account))
        printf("Deposit successful!\nNew Balance: %.2f\n",
               account->balance);
    else
        account->balance -= amount;
}

void withdraw(BankAccount *account)
{
    double amount;

    printf("Enter amount to withdraw: ");

    if (scanf("%lf", &amount) != 1)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Amount must be greater than zero.\n");
        return;
    }

    if (amount > account->balance)
    {
        printf("Insufficient balance!\n");
        return;
    }

    account->balance -= amount;

    if (saveAccount(account))
        printf("Withdrawal successful!\nNew Balance: %.2f\n",
               account->balance);
    else
        account->balance += amount;
}

void balanceEnquiry(const BankAccount *account)
{
    printf("\n----- ACCOUNT DETAILS -----\n");
    printf("Account Number: %d\n", account->accountNumber);
    printf("Account Holder: %s\n", account->name);
    printf("Available Balance: %.2f\n", account->balance);
}

int main(void)
{
    BankAccount account;
    int choice;

    if (!loadAccount(&account))
    {
        printf("No saved account found. Create a new account.\n");
        createAccount(&account);
    }

    do
    {
        printf("\n===== BANKING SYSTEM =====\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Balance Enquiry\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }

        switch (choice)
        {
            case 1:
                deposit(&account);
                break;

            case 2:
                withdraw(&account);
                break;

            case 3:
                balanceEnquiry(&account);
                break;

            case 4:
                printf("Thank you for using our banking system!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}
