#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

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
        // File doesn't exist: First-time setup
        admin = fopen("admin.txt", "w");
        if (admin == NULL)
        {
            printf("Error creating file!\n");
            return -1;
        }

        while (password1 != password2)
        {
            printf("\n--- Welcome ---\nAs you are a new user, please create your Admin password:\n");
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
            printf("\nCreate Password for Operator as well:\n");
            printf("Enter password (numbers only): ");
            scanf("%d", &password3);

            printf("Confirm your password: ");
            scanf("%d", &password4);

            if (password3 != password4)
            {
                printf("\nPasswords do not match! Please try again.\n");
            }
        }

        fprintf(admin, "%d\n%d\n", password1, password3);
        printf("\nPasswords Created Successfully! Logging you in as Admin...\n");
        fclose(admin);
    }
    else
    {
        // File already exists: Authenticate Admin
        int stored_password = 0;
        int entered_password = -1;

        if (fscanf(admin, "%d", &stored_password) != 1)
        {
            printf("Error reading admin credentials.\n");
            fclose(admin);
            return -1;
        }
        fclose(admin);

        printf("\n--- Welcome Back! ---\n");

        while (entered_password != stored_password)
        {
            printf("Enter Password: ");
            if (scanf("%d", &entered_password) != 1)
            {
                clear_input_buffer();
                entered_password = -1;
            }

            if (entered_password != stored_password)
            {
                printf("Incorrect password! Try again.\n\n");
            }
        }

        printf("\nAccess Granted! Welcome Admin.\n");
    }

    FILE *pharmacy = NULL;

    while (feature != 5)
    {
        printf("\n================ ADMIN MENU ================\n");
        printf("1. Add New Medicine\n");
        printf("2. Update/Delete Medicine Record\n");
        printf("3. View Stock Alerts & Date Reports\n");
        printf("4. View Total Revenue & Sales Report\n");
        printf("5. Logout\n");
        printf("Choice: ");

        if (scanf("%d", &feature) != 1)
        {
            feature = 0;
        }
        clear_input_buffer();

        switch (feature)
        {
        case 1:
        {
            pharmacy = fopen("pharmacy.txt", "a");
            if (pharmacy == NULL)
            {
                printf("Error opening pharmacy.txt!\n");
                break;
            }

            while (1)
            {
                printf("\nEnter Medicine Record in this format:\n");
                printf("Medicine_ID Medicine_Name Medicine_Cost Medicine_Stock Expiry_Date(DD/MM/YYYY)\n");
                printf("(Type 'stop' and press Enter to finish)\n\n");

                if (fgets(add_medicine, sizeof(add_medicine), stdin) == NULL)
                {
                    break;
                }

                // Remove trailing newline
                add_medicine[strcspn(add_medicine, "\r\n")] = 0;

                if (strcmp(add_medicine, "stop") == 0 || strcmp(add_medicine, "Stop") == 0)
                {
                    break;
                }

                // Validate format before writing to prevent corrupted rows
                int check_id, check_price, check_stock;
                char check_name[256], check_date[50];
                if (sscanf(add_medicine, "%d %255s %d %d %49s", &check_id, check_name, &check_price, &check_stock, check_date) == 5)
                {
                    fprintf(pharmacy, "%d %s %d %d %s\n", check_id, check_name, check_price, check_stock, check_date);
                    printf("[+] Record added.\n");
                }
                else
                {
                    printf("[!] Invalid format. Please follow: ID Name Cost Stock DD/MM/YYYY\n");
                }
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
                            printf("\nWhat field do you want to change?\n");
                            printf("1. Update Stock\n2. Update Price\n3. Update Expiry Date\n4. Exit Update Fields\nChoice: ");
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
                                printf("Current Expiry Date is %s. Enter new Date (DD/MM/YYYY): ", date);
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
                        should_delete = 1;
                    }
                }

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
            int view3 = 0;
            printf("\n--- View Stock Alerts & Date Reports ---\n");
            printf("1. Stock Report\n2. Expiry Date Report\nChoice: ");
            scanf("%d", &view3);

            switch (view3)
            {
            case 1:
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

                printf("\nEnter stock threshold number: ");
                scanf("%d", &target_stock);

                printf("\nSelect filter logic condition:\n");
                printf("1. Stock LESS than or equal to %d (<=)\n", target_stock);
                printf("2. Stock GREATER than or equal to %d (>=)\n", target_stock);
                printf("3. Stock EXACTLY equal to %d (==)\n", target_stock);
                printf("Choice: ");
                scanf("%d", &match_condition);

                printf("\n========================================================================\n");
                printf("                         FILTERED STOCK REPORT                          \n");
                printf("========================================================================\n");
                printf("%-10s %-20s %-10s %-10s %-12s\n", "ID", "Medicine Name", "Price", "Stock", "Expiry");
                printf("------------------------------------------------------------------------\n");

                while (fscanf(pharmacy, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
                {
                    int is_match = 0;
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
                        printf("%-10d %-20s %-10d %-10d %-12s\n", id, name, price, stock, date);
                        record_found = 1;
                    }
                }
                fclose(pharmacy);

                if (!record_found)
                {
                    printf("  No medicine items matched your search criteria.\n");
                }
                printf("========================================================================\n");
                break;
            }

            case 2:
            {
                int year_check = 0, month_check = 0, day_check = 0;
                int id, price, stock;
                char name[256], date[50];
                int match_condition = 0;
                int record_found = 0;

                pharmacy = fopen("pharmacy.txt", "r");
                if (pharmacy == NULL)
                {
                    printf("No inventory data found. Add data using Option 1 first.\n");
                    break;
                }

                printf("\nEnter date threshold year (e.g., 2028): ");
                scanf("%d", &year_check);

                printf("Enter date threshold month (1-12): ");
                scanf("%d", &month_check);

                printf("Enter date threshold day (1-31): ");
                scanf("%d", &day_check);

                printf("\nSelect date filter condition:\n");
                printf("1. Expiring ON or BEFORE %02d/%02d/%04d (<=)\n", day_check, month_check, year_check);
                printf("2. Expiring ON or AFTER  %02d/%02d/%04d (>=)\n", day_check, month_check, year_check);
                printf("3. Expiring EXACTLY ON   %02d/%02d/%04d (==)\n", day_check, month_check, year_check);
                printf("Choice: ");
                scanf("%d", &match_condition);

                long target_date = (long)year_check * 10000 + month_check * 100 + day_check;

                printf("\n========================================================================\n");
                printf("                      FILTERED EXPIRY DATE REPORT                       \n");
                printf("========================================================================\n");
                printf("%-10s %-20s %-10s %-10s %-12s\n", "ID", "Medicine Name", "Price", "Stock", "Expiry");
                printf("------------------------------------------------------------------------\n");

                while (fscanf(pharmacy, "%d %255s %d %d %49s", &id, name, &price, &stock, date) == 5)
                {
                    int med_day = 0, med_month = 0, med_year = 0;
                    if (sscanf(date, "%d/%d/%d", &med_day, &med_month, &med_year) != 3)
                    {
                        continue;
                    }

                    long med_date = (long)med_year * 10000 + med_month * 100 + med_day;

                    int is_match = 0;
                    if (match_condition == 1 && med_date <= target_date)
                    {
                        is_match = 1;
                    }
                    else if (match_condition == 2 && med_date >= target_date)
                    {
                        is_match = 1;
                    }
                    else if (match_condition == 3 && med_date == target_date)
                    {
                        is_match = 1;
                    }

                    if (is_match)
                    {
                        printf("%-10d %-20s %-10d %-10d %-12s\n", id, name, price, stock, date);
                        record_found = 1;
                    }
                }
                fclose(pharmacy);

                if (!record_found)
                {
                    printf("  No medicine items matched your date criteria.\n");
                }
                printf("========================================================================\n");
                break;
            }

            default:
                printf("Invalid report choice.\n");
                break;
            }
            break;
        }

        case 4:
        {
            FILE *sale = fopen("sale.txt", "r");
            if (sale == NULL)
            {
                printf("\nNo sales data found (sale.txt does not exist yet).\n");
                break;
            }

            int visual = 0;
            printf("\n--- Total Revenue & Sales Report ---\n");
            printf("1. Sheet (Tabular Report)\n");
            printf("2. Graph (Monthly Revenue Bar Chart)\n");
            printf("Choice: ");
            scanf("%d", &visual);

            if (visual == 1)
            {
                int report_format = 0;
                printf("\nSelect Sheet Format:\n");
                printf("1. Whole Year Report\n");
                printf("2. Monthly Report\n");
                printf("3. Daily Report\n");
                printf("Choice: ");
                scanf("%d", &report_format);

                int target_day = 0, target_month = 0, target_year = 0;

                if (report_format == 1)
                {
                    printf("Enter Year (e.g., 2026): ");
                    scanf("%d", &target_year);
                }
                else if (report_format == 2)
                {
                    printf("Enter Year (e.g., 2026): ");
                    scanf("%d", &target_year);
                    printf("Enter Month (1-12): ");
                    scanf("%d", &target_month);
                }
                else if (report_format == 3)
                {
                    printf("Enter Year (e.g., 2026): ");
                    scanf("%d", &target_year);
                    printf("Enter Month (1-12): ");
                    scanf("%d", &target_month);
                    printf("Enter Day (1-31): ");
                    scanf("%d", &target_day);
                }
                else
                {
                    printf("Invalid report format.\n");
                    fclose(sale);
                    break;
                }

                int id, price, qty;
                char name[256], time_str[50], date_str[50];
                int record_found = 0;
                long total_revenue = 0;
                int total_units = 0;

                printf("\n=====================================================================================\n");
                printf("                               SALES & REVENUE REPORT                                \n");
                printf("=====================================================================================\n");
                printf("%-8s %-20s %-8s %-10s %-12s %-12s %-10s\n", "ID", "Medicine Name", "Qty", "UnitPrice", "Unit Price", "Date", "Time");
                printf("-------------------------------------------------------------------------------------\n");

                while (fscanf(sale, "%d %255s %d %d %49s %49s", &id, name, &qty, &price, date_str, time_str) == 6)
                {
                    int s_day = 0, s_month = 0, s_year = 0;
                    
                    // Fixed: correctly parsing YYYY-MM-DD from sale.txt
                    if (sscanf(date_str, "%d-%d-%d", &s_year, &s_month, &s_day) != 3)
                    {
                        continue;
                    }

                    int is_match = 0;
                    if (report_format == 1 && s_year == target_year)
                    {
                        is_match = 1;
                    }
                    else if (report_format == 2 && s_year == target_year && s_month == target_month)
                    {
                        is_match = 1;
                    }
                    else if (report_format == 3 && s_year == target_year && s_month == target_month && s_day == target_day)
                    {
                        is_match = 1;
                    }

                    if (is_match)
                    {
                        int subtotal = price / qty;
                        total_revenue += price;
                        total_units += qty;
                        printf("%-8d %-20s %-8d %-10d %-12d %-12s %-10s\n", id, name, qty, price, subtotal, date_str, time_str);
                        record_found = 1;
                    }
                }

                printf("-------------------------------------------------------------------------------------\n");
                if (!record_found)
                {
                    printf("  No sales records matched your selected date criteria.\n");
                }
                else
                {
                    printf("Total Units Sold : %d\n", total_units);
                    printf("Total Revenue    : %ld\n", total_revenue);
                }
                printf("=====================================================================================\n");
            }
            else if (visual == 2)
            {
                int target_year = 0;
                printf("Enter Year for Monthly Revenue Graph (e.g., 2026): ");
                scanf("%d", &target_year);

                long monthly_rev[12] = {0};
                long max_rev = 0;
                int id, price, qty;
                char name[256], time_str[50], date_str[50];
                const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

                while (fscanf(sale, "%d %255s %d %d %49s %49s", &id, name, &qty, &price, date_str, time_str) == 6)
                {
                    int s_day = 0, s_month = 0, s_year = 0;
                    
                    // Fixed: correctly parsing YYYY-MM-DD here as well
                    if (sscanf(date_str, "%d-%d-%d", &s_year, &s_month, &s_day) == 3)
                    {
                        if (s_year == target_year && s_month >= 1 && s_month <= 12)
                        {
                            monthly_rev[s_month - 1] += (long)qty * price;
                            if (monthly_rev[s_month - 1] > max_rev)
                            {
                                max_rev = monthly_rev[s_month - 1];
                            }
                        }
                    }
                }

                printf("\n========================================================================\n");
                printf("                 MONTHLY REVENUE GRAPH (%d)                            \n", target_year);
                printf("========================================================================\n");

                for (int i = 0; i < 12; i++)
                {
                    printf("%-4s | ", months[i]);
                    int bar_len = (max_rev > 0) ? (int)((monthly_rev[i] * 40) / max_rev) : 0;
                    for (int b = 0; b < bar_len; b++)
                    {
                        printf("#");
                    }
                    printf(" (%ld)\n", monthly_rev[i]);
                }
                printf("========================================================================\n");
            }
            else
            {
                printf("Invalid display option.\n");
            }

            fclose(sale);
            break;
        }

        case 5:
            printf("\nLogging out of Admin panel...\n");
            break;

        default:
            printf("\nInvalid menu choice. Please select 1 to 5.\n");
            break;
        }
    }

    return 0;
}
