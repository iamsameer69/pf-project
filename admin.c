#include <stdio.h>

int Admin() {
    int password1 = 0;
    int password2 = -1; 
    int password3 = 0;
    int password4 = -1;
    int feature = 0;
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
        while (password3 != password4)
        {
            printf("\nAlso Create Password Also for operator as well\n");
            printf("\nEnter password (numbers only): ");
            scanf("%d", &password3); 

            printf("Confirm your password: ");
            scanf("%d", &password4); 

            if (password3 != password4) {
                printf("\nPasswords do not match! Please try again.\n");
            } 
        }
        

        // They match! Write to the file
        fprintf(admin, "%d \n%d", password1,password3);
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
        
        FILE *pharmacy = fopen("pharmacy.txt", "r");
        if (pharmacy == NULL) {
            printf("Error creating file!\n");
            return -1;
        }

        while(feature < 5){
            printf("\nEnter what you want to do\n1.Add New Medicine\n2.Update/Delete Medicine Record\n3.View Low-Stock & Expiry Alerts\n4.View Total Revenue & Sales Report\n5.Logout ");
            scanf("%d",&feature);
            switch(feature){
                case 1:{
                    pharmacy = fopen("pharmacy.txt", "w");   
                    fclose(pharmacy);
                    break;
                }
                case 2:{
                    pharmacy = fopen("pharmacy.txt", "w");   
                    fclose(pharmacy);
                    break;
                }
                case 3:{
                    pharmacy = fopen("pharmacy.txt", "r");
                    fclose(pharmacy);
                    break;
                }
                case 4:{
                    
                    break;
                }
                default:{
                    break;
                }
            }
            
        }
        
    }
    
    return 0;    
}
