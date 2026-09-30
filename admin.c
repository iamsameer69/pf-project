#include <stdio.h>

int Admin() {
    int password1 = 0;
    int password2 = -1; 
    
    FILE *admin = fopen("admin.txt", "r");

    if (admin == NULL) {
        // File doesn't exist, meaning they are a new user
        admin = fopen("admin.txt", "w");
        if (admin == NULL) {
            printf("Error creating file!\n");
            return -1;
        }

        // Loop runs until password1 matches password2
        while (password1 != password2) {
            printf("\n--- Welcome ---\nAs you are a new user, please create your password:\n");
            printf("Enter password (numbers only): ");
            scanf("%d", &password1); 

            printf("Confirm your password: ");
            scanf("%d", &password2); 

            if (password1 != password2) {
                printf("\nPasswords do not match! Please try again.\n");
            }
        }

        // They match! Write to the file
        fprintf(admin, "%d", password1);
        printf("\nPassword Updated Successfully!\n");
        
        fclose(admin); 
    } else {
        // The file already exists (Old user)
        int stored_password;
        int entered_password = -1;

        // 1. Read the correct password from the file ONCE
        fscanf(admin, "%d", &stored_password); // Fixed: Changed 'file' to 'admin'

        printf("\n--- Welcome Back! ---\n");

        // 2. Loop until the user enters the correct password
        while (entered_password != stored_password) {
            printf("Enter Password: ");
            scanf("%d", &entered_password); // Fixed: Added comma and &

            if (entered_password != stored_password) {
                printf("Incorrect password! Try again.\n\n");
            }
        }

        printf("\nAccess Granted! Welcome Admin.\n");
        fclose(admin); 
    }

    printf("")
    return 0;    
}
