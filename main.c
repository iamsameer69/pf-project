//main.c
#include <stdio.h>
#include"admin.c"

int main() {
    int choice;

    while (1) {
        printf("\nWelcome to Login Portal\n");
        printf("1. Admin\n");
        printf("2. Billing Operator\n");
        printf("3. Exit Program\n");
        printf("Enter your Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Admin Module\n");
            admin();
        }
        else if (choice == 2) {
            printf("Billing Operator Module\n");
        }
        else if (choice == 3) {
            printf("Saving data back to files...\n");
            printf("Exiting Program, Good Bye!\n");
            break; // Exits the loop and ends the program
        }
        else {
            printf("Incorrect Choice\n");
        }
    }

    return 0;
}
