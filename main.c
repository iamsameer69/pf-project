// main.c
#include <stdio.h>
#include <stdlib.h>
#include "admin.c"
#include "operator.c"

void clear_screen(int clear) {
    if (clear == 1) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
}

int main() {
    int choice;
    while (1) {
        clear_screen(1);
        printf("\n========================================\n");
        printf("       PHARMACY MANAGEMENT SYSTEM       \n");
        printf("========================================\n");
        printf("             MAIN LOGIN PORTAL          \n");
        printf("========================================\n");
        printf(" 1. Admin Portal                        \n");
        printf(" 2. Billing Operator Portal             \n");
        printf(" 3. Exit Program                        \n");
        printf("========================================\n");
        printf("Enter your Choice (1-3): ");

        if (scanf("%d", &choice) != 1) {
            printf("\n[!] Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear invalid input buffer
            printf("Press Enter to continue...");
            getchar();
            continue;
        }

        if (choice == 1) {
            printf("\n[+] Loading Admin Module...\n");
            Admin();
            
        }
        else if (choice == 2) {
            printf("\n[+] Loading Billing Operator Module...\n");
            Operator();
        }
        else if (choice == 3) {
            printf("\n========================================\n");
            printf(" [+] Saving data back to files...\n");
            printf(" Exiting Program. Good Bye!\n");
            printf("========================================\n");
            break; // Exits the loop and ends the program
        }
        else {
            printf("\n[!] Incorrect Choice. Please select 1, 2, or 3.\n");
        }
    }

    return 0;
}
