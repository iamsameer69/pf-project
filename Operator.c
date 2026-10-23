#include <stdio.h>
#include <string.h>

int Operator() {
    int stored_password;
    int entered_password = -1;
    int choice;
    int search_id;
    int id, price, stock;
    char name[256], date[50];

    FILE *admin = fopen("admin.txt", "r");
    if (admin == NULL) return 1; 
    
    // Reading password (kept your double fscanf if your file requires it)
    fscanf(admin, "%d", &stored_password);
    fscanf(admin, "%d", &stored_password);
    fclose(admin);

    while (stored_password != entered_password) {
        printf("--- Welcome Back ---\nEnter your passkey: ");
        scanf("%d", &entered_password);
    }
    
    printf("\n\nWelcome Back Operator\n");

    FILE *inventory = fopen("pharmacy.txt", "r");
    if (inventory == NULL) return 1;

    while (1) {
        printf("\nEnter your choice \n1: Create Bill\n2: Search Inventory\n3: Exit\nChoice: ");
        
        // FIXED: Use scanf for keyboard input instead of fscanf
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1: // Create Bill
                printf("Create bill feature goes here.\n");
                while(1){

                printf("Enter Medicine ID to create bill(to stop press 0): ");
                if (scanf("%d", &search_id) != 1) {
                    printf("Invalid input.\n");
                    while(getchar() != '\n'); // Clear buffer
                    break;
                }

                // Open file fresh for searching
                FILE *inventory = fopen("pharmacy.txt", "r");
                if (inventory == NULL) {
                    printf("Error: Could not open pharmacy.txt\n");
                    break;
                }

                int found = 0;
                // Matches file structure: ID Name Price Stock Date
                while (fscanf(inventory, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5) {
                    if (id == search_id) {
                        int pcs;
                        printf("\nEnter pcs buyer bought");
                        scanf("%d",&pcs);
                        printf("\n--- Item Found ---\n");
                        printf("ID       : %d\n", id);
                        printf("Name     : %s\n", name);
                        printf("Price    : %d\n", price * pcs);
                        printf("Date     : %s\n", date);
                        printf("------------------\n");
                        found = 1;
                        // No break here in case the ID appears multiple times
                    }
                }
                if ( !found && search_id!= 0) {
                    printf("\n[!] Medicine with ID %d not found.\n", search_id);
                }

                if(search_id == 0) break;

                }
                break;

            case 2: // Search Inventory (Moved your search logic here!)
                printf("Enter Medicine ID to search: ");
                if (scanf("%d", &search_id) != 1) {
                    printf("Invalid input.\n");
                    while(getchar() != '\n'); // Clear buffer
                    break;
                }

                // Open file fresh for searching
                FILE *inventory = fopen("pharmacy.txt", "r");
                if (inventory == NULL) {
                    printf("Error: Could not open pharmacy.txt\n");
                    break;
                }

                int found = 0;
                // Matches file structure: ID Name Price Stock Date
                while (fscanf(inventory, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5) {
                    if (id == search_id) {
                        printf("\n--- Item Found ---\n");
                        printf("ID       : %d\n", id);
                        printf("Name     : %s\n", name);
                        printf("Stock    : %d\n", stock);
                        printf("Price    : %d\n", price);
                        printf("Date     : %s\n", date);
                        printf("------------------\n");
                        found = 1;
                        // No break here in case the ID appears multiple times
                    }
                }

                if (!found) {
                    printf("\n[!] Medicine with ID %d not found.\n", search_id);
                }

                break;

            case 3: // Exit
                printf("Exiting system...\n");
                fclose(inventory);
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    fclose(inventory);
    return 0;
}
