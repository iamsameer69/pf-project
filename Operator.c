#include <stdio.h>
#include <string.h>
#include <time.h>

// Helper function to update stock in pharmacy.txt
void update_stock(int target_id, int pcs_sold)
{
    FILE *src = fopen("pharmacy.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!src || !temp)
    {
        if (src) fclose(src);
        if (temp) fclose(temp);
        return;
    }

    int id, price, stock;
    char name[256], date[50];

    while (fscanf(src, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
    {
        if (id == target_id)
        {
            stock -= pcs_sold;
        }
        fprintf(temp, "%d %s %d %d %s\n", id, name, price, stock, date);
    }

    fclose(src);
    fclose(temp);
    remove("pharmacy.txt");
    rename("temp.txt", "pharmacy.txt");
}

int Operator()
{
    int stored_password;
    int entered_password = -1;
    int choice;
    int search_id;
    int id, price, stock;
    char name[256], date[50];

    FILE *admin = fopen("admin.txt", "r");
    if (admin == NULL)
    {
        printf("\n========================================\n");
        printf(" [!] Error: Authentication database not found.\n");
        printf("========================================\n");
        return 1;
    }

    fscanf(admin, "%d", &stored_password);
    if (fscanf(admin, "%d", &stored_password) != 1)
    {
        printf("\n========================================\n");
        printf(" [!] Error: Operator password not setup yet.\n");
        printf("========================================\n");
        fclose(admin);
        return 1;
    }
    fclose(admin);

    // --- LOGIN SCREEN ---
    printf("\n========================================\n");
    printf("         OPERATOR LOGIN PORTAL          \n");
    printf("========================================\n");

    while (stored_password != entered_password)
    {
        printf("Enter Operator Passkey: ");
        if (scanf("%d", &entered_password) != 1)
        {
            printf("[!] Invalid input. Numbers only.\n");
            while (getchar() != '\n')
                ;
        }
        else if (stored_password != entered_password)
        {
            printf("[!] Incorrect passkey! Try again.\n\n");
        }
    }

    printf("\n========================================\n");
    printf("     ACCESS GRANTED! WELCOME OPERATOR   \n");
    printf("========================================\n");

    while (1)
    {
        // --- MAIN MENU BOX ---
        printf("\n========================================\n");
        printf("              OPERATOR MENU             \n");
        printf("========================================\n");
        printf(" 1. Create Bill & Process Sale          \n");
        printf(" 2. Search Inventory Record             \n");
        printf(" 3. Logout / Exit                       \n");
        printf("========================================\n");
        printf("Select Choice (1-3): ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\n[!] Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1: // Create Bill
        {
            int grand_total = 0;

            FILE *bill_file = fopen("bill.txt", "w");
            if (bill_file)
            {
                time_t now = time(NULL);
                struct tm *local = localtime(&now);
                char time_buf[80];
                strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", local);

                fprintf(bill_file, "========================================\n");
                fprintf(bill_file, "         PHARMACY OFFICIAL RECEIPT      \n");
                fprintf(bill_file, "========================================\n");
                fprintf(bill_file, " Date & Time : %s\n", time_buf);
                fprintf(bill_file, "----------------------------------------\n");
                fprintf(bill_file, "%-6s %-15s %-5s %-10s\n", "ID", "Name", "Qty", "Total");
                fprintf(bill_file, "----------------------------------------\n");
            }

            printf("\n========================================\n");
            printf("          BILLING SESSION STARTED       \n");
            printf("========================================\n");

            while (1)
            {
                printf("Enter Medicine ID (Press 0 to Finish): ");
                if (scanf("%d", &search_id) != 1)
                {
                    printf("[!] Invalid input.\n");
                    while (getchar() != '\n')
                        ;
                    break;
                }

                if (search_id == 0)
                {
                    break;
                }

                FILE *inventory_file = fopen("pharmacy.txt", "r");
                if (inventory_file == NULL)
                {
                    printf("[!] Error: Could not open pharmacy.txt\n");
                    break;
                }

                int found = 0;
                int pcs = 0;
                while (fscanf(inventory_file, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
                {
                    if (id == search_id)
                    {
                        found = 1;
                        printf(" -> Enter quantity needed [Available Stock: %d]: ", stock);
                        if (scanf("%d", &pcs) != 1 || pcs <= 0 || pcs > stock)
                        {
                            printf(" [!] Invalid quantity or insufficient stock available.\n");
                            while (getchar() != '\n')
                                ;
                            pcs = 0;
                            break;
                        }

                        int item_total = price * pcs;
                        grand_total += item_total;

                        printf("\n----------------------------------------\n");
                        printf(" [+] ITEM SUCCESSFULLY ADDED TO BILL\n");
                        printf("----------------------------------------\n");
                        printf(" ID       : %d\n", id);
                        printf(" Name     : %s\n", name);
                        printf(" Qty      : %d\n", pcs);
                        printf(" Subtotal : %d\n", item_total);
                        printf(" Expiry   : %s\n", date);
                        printf("----------------------------------------\n");

                        // Log to permanent sales history (sale.txt)
                        FILE *sale = fopen("sale.txt", "a");
                        if (sale != NULL)
                        {
                            time_t now = time(NULL);
                            struct tm *local = localtime(&now);
                            char buffer[80];
                            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);

                            fprintf(sale, "%d %s %d %d %s\n", id, name, pcs, item_total, buffer);
                            fclose(sale);
                        }

                        // Save item to bill.txt
                        if (bill_file)
                        {
                            fprintf(bill_file, "%-6d %-15s %-5d %-10d\n", id, name, pcs, item_total);
                        }
                        break;
                    }
                }
                fclose(inventory_file);

                if (found && pcs > 0)
                {
                    update_stock(search_id, pcs);
                }
                else if (!found)
                {
                    printf(" [!] Medicine with ID %d not found in inventory.\n\n", search_id);
                }
            }

            if (bill_file)
            {
                fprintf(bill_file, "----------------------------------------\n");
                fprintf(bill_file, " GRAND TOTAL : %d\n", grand_total);
                fprintf(bill_file, "========================================\n");
                fprintf(bill_file, "      THANK YOU FOR YOUR PURCHASE!      \n");
                fprintf(bill_file, "========================================\n");
                fclose(bill_file);
            }

            int printing = 0;
            printf("\nDo you want to print receipt on screen? (1 = Yes, 0 = No): ");
            scanf("%d", &printing);
            
            if (printing == 1)
            {
                FILE *read_bill = fopen("bill.txt", "r");
                if (read_bill)
                {
                    char line[256];
                    printf("\n");
                    while (fgets(line, sizeof(line), read_bill))
                    {
                        printf("%s", line);
                    }
                    fclose(read_bill);
                }
            }
            printf("\n[+] Billing completed successfully. Saved to bill.txt\n");
            break;
        }

        case 2: // Search Inventory
        {
            printf("\n========================================\n");
            printf("            INVENTORY LOOKUP            \n");
            printf("========================================\n");
            printf("Enter Medicine ID to search: ");
            
            if (scanf("%d", &search_id) != 1)
            {
                printf("[!] Invalid input.\n");
                while (getchar() != '\n')
                    ;
                break;
            }

            FILE *inventory_file = fopen("pharmacy.txt", "r");
            if (inventory_file == NULL)
            {
                printf("[!] Error: Could not open pharmacy.txt\n");
                break;
            }

            int found = 0;
            while (fscanf(inventory_file, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
            {
                if (id == search_id)
                {
                    printf("\n========================================\n");
                    printf("             RECORD MATCH FOUND         \n");
                    printf("========================================\n");
                    printf(" ID       : %d\n", id);
                    printf(" Name     : %s\n", name);
                    printf(" Unit Price: %d\n", price);
                    printf(" Stock Qty: %d units\n", stock);
                    printf(" Expiry   : %s\n", date);
                    printf("========================================\n");
                    found = 1;
                    break;
                }
            }
            fclose(inventory_file);

            if (!found)
            {
                printf("\n[!] Medicine with ID %d not found in inventory.\n", search_id);
            }
            break;
        }

        case 3: // Exit
            printf("\n========================================\n");
            printf(" Exiting Operator System. Goodbye!\n");
            printf("========================================\n");
            break;

        default:
            printf("\n[!] Invalid choice. Please select between 1 and 3.\n");
            break;
        }
    }
    return 0;
}
