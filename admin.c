#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Admin()
{
    int password1 = 0;
    int password2 = -1;
    int password3 = 0;
    int password4 = -1;
    int feature = 0;
    char add_medicine[512];
    FILE *admin = fopen("admin.txt", "r");

    if (admin == NULL)
    {
        // File doesn't exist, meaning they are a new user
        admin = fopen("admin.txt", "w");
        if (admin == NULL)
        {
            printf("Error creating file!\n");
            return -1;
        }

        // Loop runs until password1 matches password2
        while (password1 != password2)
        {
            printf("\n--- Welcome ---\nAs you are a new user, please create your password:\n");
            printf("Enter password (numbers only): ");
            scanf("%d", &password1);

            printf("Confirm your password: ");
            scanf("%d", &password2);

            if (password1 != password2)
            {
                printf("\nPasswords do not match! Please try again.\n");
            }
        }

        while (password3 != password4)
        {
            printf("\nAlso Create Password for operator as well\n");
            printf("\nEnter password (numbers only): ");
            scanf("%d", &password3);

            printf("Confirm your password: ");
            scanf("%d", &password4);

            if (password3 != password4)
            {
                printf("\nPasswords do not match! Please try again.\n");
            }
        }

        // They match! Write to the file
        fprintf(admin, "%d \n%d", password1, password3);
        printf("\nPassword Updated Successfully!\n");

        fclose(admin);
    }
    else
    {
        // The file already exists (Old user)
        int stored_password;
        int entered_password = -1;

        // Read the correct password from the file
        fscanf(admin, "%d", &stored_password);

        printf("\n--- Welcome Back! ---\n");

        // Loop until the user enters the correct password
        while (entered_password != stored_password)
        {
            printf("Enter Password: ");
            scanf("%d", &entered_password);

            if (entered_password != stored_password)
            {
                printf("Incorrect password! Try again.\n\n");
            }
        }

        printf("\nAccess Granted! Welcome Admin.\n");
        fclose(admin);

        FILE *pharmacy = NULL;

        while (feature < 5)
        {
            printf("\nEnter what you want to do\n1. Add New Medicine\n2. Update/Delete Medicine Record\n3. View Stock Alerts & Reports\n4. View Total Revenue & Sales Report\n5. Logout\nChoice: ");
            if (scanf("%d", &feature) != 1)
            {
                feature = 0;
            }

            // Clear the input buffer to remove the leftover newline from scanf
            while (getchar() != '\n')
                ;

            switch (feature)
            {
            case 1:
            {
                pharmacy = fopen("pharmacy.txt", "a");
                if (pharmacy == NULL)
                {
                    printf("Error opening file!\n");
                    break;
                }

                while (1)
                {
                    printf("\nEnter Medicine Record in this format \nMedicin_ID Medicine_Name Medicine_Cost Medicine_Stock Medicine_Expiry_Date(day/month/year) \nto stop type stop and Enter \n\n");

                    if (fgets(add_medicine, sizeof(add_medicine), stdin) == NULL)
                    {
                        break;
                    }

                    // Remove the trailing newline character left by fgets
                    add_medicine[strcspn(add_medicine, "\r\n")] = 0;

                    // Check if the user typed stop or Stop to break the loop
                    if (strcmp(add_medicine, "stop") == 0 || strcmp(add_medicine, "Stop") == 0)
                    {
                        break;
                    }

                    // Write the valid record into the file
                    fprintf(pharmacy, "%s\n", add_medicine);
                }
                fclose(pharmacy);
                break;
            }
            case 2:
            {
                int search_id, id, price, stock;
                char name[256], date[50];
                int found = 0;
                int should_delete = 0;

                // Open the original file to read, and a temporary file to write
                pharmacy = fopen("pharmacy.txt", "r");
                FILE *temp = fopen("temp.txt", "w");

                if (pharmacy == NULL || temp == NULL)
                {
                    printf("Error opening inventory files. Ensure pharmacy.txt exists.\n");
                    if (pharmacy)
                        fclose(pharmacy);
                    if (temp)
                        fclose(temp);
                    break;
                }

                printf("Enter Medicine ID to update or delete: ");
                scanf("%d", &search_id);

                // Read line-by-line directly through streaming variables (No item limit!)
                while (fscanf(pharmacy, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
                {
                    if (id == search_id)
                    {
                        found = 1;

                        printf("\n--- Item Found ---\n");
                        printf("ID    : %d\n", id);
                        printf("Name  : %s\n", name);
                        printf("Price : %d\n", price);
                        printf("Stock : %d\n", stock);
                        printf("Expiry: %s\n", date);
                        printf("------------------\n");

                        int action = 0;
                        printf("\nChoose Action:\n1. Update Fields\n2. Delete Entire Record\nChoice: ");
                        scanf("%d", &action);

                        if (action == 1)
                        {
                            int running = 1;
                            while (running)
                            {
                                int choice = 0;
                                printf("\nWhat field do you want to change?\n1. Update Stock \n2. Update Price \n3. Update Expiry Date\n4. Exit Update Fields \nChoice: ");
                                scanf("%d", &choice);

                                switch (choice)
                                {
                                case 1:
                                    printf("Current stock is %d. Enter new stock: ", stock);
                                    scanf("%d", &stock);
                                    break;
                                case 2:
                                    printf("Current price is %d. Enter new Price: ", price);
                                    scanf("%d", &price);
                                    break;
                                case 3:
                                    printf("Current Expiry Date is %s. Enter new Date: ", date);
                                    scanf("%49s", date);
                                    break;
                                case 4:
                                    printf("Exiting Fields Modification Sub-Menu.\n");
                                    running = 0;
                                    break;
                                default:
                                    printf("Invalid field choice.\n");
                                    break;
                                }
                            }
                        }
                        else if (action == 2)
                        {
                            should_delete = 1; // Mark to drop row
                        }
                    }

                    // If marked for deletion, skip writing this current row into temp file
                    if (should_delete && id == search_id)
                    {
                        continue;
                    }

                    fprintf(temp, "%d %s %d %d %s\n", id, name, price, stock, date);
                }

                fclose(pharmacy);
                fclose(temp);

                if (!found)
                {
                    printf("Medicine ID %d not found in database.\n", search_id);
                    remove("temp.txt");
                }
                else
                {
                    remove("pharmacy.txt");
                    rename("temp.txt", "pharmacy.txt");

                    if (should_delete)
                    {
                        printf("[+] Item completely removed and saved permanently.\n");
                    }
                    else
                    {
                        printf("[+] Item modifications updated and saved permanently.\n");
                    }
                }
                break;
            }
            case 3:
            {
                int id, price, stock;
                char name[256], date[50];
                int target_stock = 0;
                int match_condition = 0;
                int record_found = 0;

                pharmacy = fopen("pharmacy.txt", "r");
                if (pharmacy == NULL)
                {
                    printf("No inventory data found. Add data using Option 1 first.\n");
                    break;
                }

                // Ask the user for their custom filtering bounds
                printf("\nEnter stock threshold number: ");
                scanf("%d", &target_stock);

                printf("\nSelect filter logic condition:\n");
                printf("1. See items with stock LESSER than or equal to %d (<=)\n", target_stock);
                printf("2. See items with stock GREATER than or equal to %d (>=)\n", target_stock);
                printf("3. See items with stock EXACTLY equal to %d (==)\n", target_stock);
                printf("Choice: ");
                scanf("%d", &match_condition);

                printf("\n======================================================================\n");
                printf("                  FILTERED STOCK REPORT                \n");
                printf("========================================================================\n");
                printf("%-10s %-20s %-10s %-10s %-12s\n", "ID", "Medicine Name","Price", "Stock", "Expiry");
                printf("------------------------------------------------------------------------\n");
                while (fscanf(pharmacy, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
                {
                    int is_match = 0;
                    // Match condition validation branch logic
                    if (match_condition == 1 && stock <= target_stock)
                    {
                        is_match = 1;
                    }
                    else if (match_condition == 2 && stock >= target_stock)
                    {
                        is_match = 1;
                    }
                    else if (match_condition == 3 && stock == target_stock)
                    {
                        is_match = 1;
                    }
                    if (is_match)
                    {
                        printf("%-10d %-20s %-10d %-10d %-12s\n", id, name,price, stock, date);
                        record_found = 1;
                    }
                }
                fclose(pharmacy);
                if (!record_found)
                {
                    printf("  No medicine items matched your search criteria.\n");
                }
                printf("=======================================================================\n");
                break;
            }
            case 4:
            {
                printf("\nTotal Revenue & Sales report feature under construction.\n");
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
    return 0;
}
