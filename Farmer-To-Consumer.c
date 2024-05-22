#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for farmer and product
typedef struct {
    int farmer_id;
    char f_name[50];
    char location[50];
    int product_count;
    int product_id[100];
    char p_name[100][50];
    float p_price[100];
    float p_quantity[100];
} Farmer;

// Define array to store farmers
Farmer farmers[100];
int farmer_count = 0;

// Function to add a farmer to the database
void addFarmer() {
    FILE *file = fopen("farmers.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter farmer's name: ");
    scanf("%s", farmers[farmer_count].f_name);
    printf("Enter farmer's location: ");
    scanf("%s", farmers[farmer_count].location);
    farmers[farmer_count].farmer_id = farmer_count + 1;
    farmers[farmer_count].product_count = 0;
    printf("Your Farmer id is %d\n", farmers[farmer_count].farmer_id);

    // Write to file
    fprintf(file, "%d %s %s\n", farmers[farmer_count].farmer_id, farmers[farmer_count].f_name, farmers[farmer_count].location);

    fclose(file);

    farmer_count++;
}

// Function to add a product to the database
void addProduct() {
    if (farmer_count == 0) {
        printf("Please add a farmer first.\n");
        return;
    }

    int farmer_id;
    printf("Enter farmer ID to add product: ");
    scanf("%d", &farmer_id);

    int found = 0;
    for (int i = 0; i < farmer_count; i++) {
        if (farmers[i].farmer_id == farmer_id) {
            FILE *file = fopen("products.txt", "a");
            if (file == NULL) {
                printf("Error opening file!\n");
                return;
            }

            printf("Enter product name: ");
            scanf("%s", farmers[i].p_name[farmers[i].product_count]);
            printf("Enter product price: ");
            scanf("%f", &farmers[i].p_price[farmers[i].product_count]);
            printf("Enter product quantity (in kgs): ");
            scanf("%f", &farmers[i].p_quantity[farmers[i].product_count]);
            farmers[i].product_id[farmers[i].product_count] = farmers[i].product_count + 1;
            farmers[i].product_count++;
            printf("Product added successfully.\n");

            // Write to file
            fprintf(file, "%d %s %.2f %.2f\n", farmers[i].farmer_id, farmers[i].p_name[farmers[i].product_count - 1],
                    farmers[i].p_price[farmers[i].product_count - 1], farmers[i].p_quantity[farmers[i].product_count - 1]);

            fclose(file);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Farmer ID not found.\n");
}

// Function to read farmers' data from file
void readFarmersFromFile() {
    FILE *file = fopen("farmers.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%d %s %s", &farmers[farmer_count].farmer_id, farmers[farmer_count].f_name, farmers[farmer_count].location) != EOF) {
        farmers[farmer_count].product_count = 0;
        farmer_count++;
    }

    fclose(file);
}

// Function to read products' data from file
void readProductsFromFile() {
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int farmer_id;
    char p_name[50];
    float p_price, p_quantity;

    while (fscanf(file, "%d %s %f %f", &farmer_id, p_name, &p_price, &p_quantity) != EOF) {
        for (int i = 0; i < farmer_count; i++) {
            if (farmers[i].farmer_id == farmer_id) {
                strcpy(farmers[i].p_name[farmers[i].product_count], p_name);
                farmers[i].p_price[farmers[i].product_count] = p_price;
                farmers[i].p_quantity[farmers[i].product_count] = p_quantity;
                farmers[i].product_id[farmers[i].product_count] = farmers[i].product_count + 1;
                farmers[i].product_count++;
                break;
            }
        }
    }

    fclose(file);
}

// Function to display all products
void displayProducts() {
    printf("\nAvailable Products:\n");
    printf("+----+----------------------+-----------+-----------+--------------+\n");
    printf("| ID |        Product       |   Price   | Quantity  |   Sold by    |\n");
    printf("+----+----------------------+-----------+-----------+--------------+\n");
    for (int i = 0; i < farmer_count; i++) {
        for (int j = 0; j < farmers[i].product_count; j++) {
            if (farmers[i].p_quantity[j] > 0) {
                printf("| %2d | %-20s | $%8.2f | %9.2f | %-12s |\n",
                       farmers[i].product_id[j], farmers[i].p_name[j], farmers[i].p_price[j], farmers[i].p_quantity[j], farmers[i].f_name);
            }
        }
    }
    printf("+----+----------------------+-----------+-----------+--------------+\n");
}

// Function to display all farmers
void displayFarmers() {
    printf("\nAvailable Farmers:\n");
    printf("+----+----------------------+-----------+\n");
    printf("| ID |         Name         | Location  |\n");
    printf("+----+----------------------+-----------+\n");
    for (int i = 0; i < farmer_count; i++) {
        printf("| %2d | %-20s | %-9s |\n", farmers[i].farmer_id, farmers[i].f_name, farmers[i].location);
    }
    printf("+----+----------------------+-----------+\n");
}

// Function to filter products based on farmer's name, product name, or location
void filterProducts(int choice, char filter[50], int prev_choice, char prev_filter[50], int prev_prev_choice, char prev_prev_filter[50], int prompt_displayed) {
    printf("\nFiltered Products:\n");
    printf("+----+----------------------+-----------+-----------+--------------+\n");
    printf("| ID |        Product       |   Price   | Quantity  |   Sold by    |\n");
    printf("+----+----------------------+-----------+-----------+--------------+\n");
    for (int i = 0; i < farmer_count; i++) {
        for (int j = 0; j < farmers[i].product_count; j++) {
            int match = 0;
            if ((prev_choice == 0 || (prev_choice == 1 && strcmp(farmers[i].f_name, prev_filter) == 0) ||
                (prev_choice == 2 && strcmp(farmers[i].p_name[j], prev_filter) == 0) ||
                (prev_choice == 3 && strcmp(farmers[i].location, prev_filter) == 0)) &&
                (prev_prev_choice == 0 || (prev_prev_choice == 1 && strcmp(farmers[i].f_name, prev_prev_filter) == 0) ||
                (prev_prev_choice == 2 && strcmp(farmers[i].p_name[j], prev_prev_filter) == 0) ||
                (prev_prev_choice == 3 && strcmp(farmers[i].location, prev_prev_filter) == 0))) {
                if (choice == 1 && strcmp(farmers[i].f_name, filter) == 0)
                    match = 1;
                else if (choice == 2 && strcmp(farmers[i].p_name[j], filter) == 0)
                    match = 1;
                else if (choice == 3 && strcmp(farmers[i].location, filter) == 0)
                    match = 1;
            }

            if (match && farmers[i].p_quantity[j] > 0) {
                printf("| %2d | %-20s | $%8.2f | %9.2f | %-12s |\n",
                       farmers[i].product_id[j], farmers[i].p_name[j], farmers[i].p_price[j], farmers[i].p_quantity[j], farmers[i].f_name);
            }
        }
    }
    printf("+----+----------------------+-----------+-----------+--------------+\n");

    // Prompt user for further filtering based on remaining one criteria
    if (!prompt_displayed) {
        if (choice == 1) {
            char option;
            printf("Do you want to further filter these results based on the remaining one criteria (y/n)? ");
            scanf(" %c", &option);

            if (option == 'y' || option == 'Y') {
                int new_choice;
                char new_filter[50];

                if (prev_choice == 2) {
                    new_choice = 3;
                    strcpy(new_filter, prev_prev_filter);
                } else if (prev_choice == 3) {
                    new_choice = 2;
                    strcpy(new_filter, prev_prev_filter);
                } else {
                    printf("\nFilter by:\n");
                    printf("1. Product Name\n");
                    printf("2. Location\n");
                    printf("Enter your choice: ");
                    scanf("%d", &new_choice);

                    printf("Enter the %s to further filter: ", (new_choice == 1) ? "product name" : "location");
                    scanf("%s", new_filter);
                }

                filterProducts(new_choice, new_filter, choice, filter, prev_choice, prev_filter, 1);
            }
        } else if (choice == 2) {
            char option;
            printf("Do you want to further filter these results based on the remaining one criteria (y/n)? ");
            scanf(" %c", &option);

            if (option == 'y' || option == 'Y') {
                int new_choice;
                char new_filter[50];

                if (prev_choice == 1) {
                    new_choice = 3;
                    strcpy(new_filter, prev_prev_filter);
                } else if (prev_choice == 3) {
                    new_choice = 1;
                    strcpy(new_filter, prev_prev_filter);
                } else {
                    printf("\nFilter by:\n");
                    printf("1. Farmer Name\n");
                    printf("2. Location\n");
                    printf("Enter your choice: ");
                    scanf("%d", &new_choice);

                    printf("Enter the %s to further filter: ", (new_choice == 1) ? "farmer name" : "location");
                    scanf("%s", new_filter);
                }

                filterProducts(new_choice, new_filter, choice, filter, prev_choice, prev_filter, 1);
            }
        } else if (choice == 3) {
            char option;
            printf("Do you want to further filter these results based on the remaining one criteria (y/n)? ");
            scanf(" %c", &option);

            if (option == 'y' || option == 'Y') {
                int new_choice;
                char new_filter[50];

                if (prev_choice == 1) {
                    new_choice = 2;
                    strcpy(new_filter, prev_prev_filter);
                } else if (prev_choice == 2) {
                    new_choice = 1;
                    strcpy(new_filter, prev_prev_filter);
                } else {
                    printf("\nFilter by:\n");
                    printf("1. Farmer Name\n");
                    printf("2. Product Name\n");
                    printf("Enter your choice: ");
                    scanf("%d", &new_choice);

                    printf("Enter the %s to further filter: ", (new_choice == 1) ? "farmer name" : "product name");
                    scanf("%s", new_filter);
                }

                filterProducts(new_choice, new_filter, choice, filter, prev_choice, prev_filter, 1);
            }
        }
    }
}

// Function to allow consumer to buy a product
void buyProduct() {
    int farmer_id, product_id;
    printf("Enter the ID of the farmer you want to buy from: ");
    scanf("%d", &farmer_id);
    printf("Enter the ID of the product you want to buy: ");
    scanf("%d", &product_id);

    int farmer_index = -1;
    for (int i = 0; i < farmer_count; i++) {
        if (farmers[i].farmer_id == farmer_id) {
            farmer_index = i;
            break;
        }
    }

    if (farmer_index != -1 && product_id >= 1 && product_id <= farmers[farmer_index].product_count) {
        float quantity_to_buy;
        printf("Enter the quantity you want to buy (in kgs): ");
        scanf("%f", &quantity_to_buy);

        if (quantity_to_buy <= 0 || quantity_to_buy > farmers[farmer_index].p_quantity[product_id - 1]) {
            printf("Invalid quantity.\n");
            return;
        }

        float total_price = quantity_to_buy * farmers[farmer_index].p_price[product_id - 1];
        printf("You have purchased %.2f kgs of %s from %s for $%.2f\n", quantity_to_buy, farmers[farmer_index].p_name[product_id - 1], farmers[farmer_index].f_name, total_price);

        farmers[farmer_index].p_quantity[product_id - 1] -= quantity_to_buy;
        if (farmers[farmer_index].p_quantity[product_id - 1] <= 0) {
            // Remove product if quantity is zero
            for (int i = product_id - 1; i < farmers[farmer_index].product_count - 1; i++) {
                strcpy(farmers[farmer_index].p_name[i], farmers[farmer_index].p_name[i + 1]);
                farmers[farmer_index].p_price[i] = farmers[farmer_index].p_price[i + 1];
                farmers[farmer_index].p_quantity[i] = farmers[farmer_index].p_quantity[i + 1];
            }
            farmers[farmer_index].product_count--;
        }
    } else {
        printf("Invalid farmer ID or product ID\n");
    }
}

int main() {
    // Read data from file into memory
    readFarmersFromFile();
    readProductsFromFile();

    int choice;
    do {
        printf("\n1. Add Farmer\n2. Add Product\n3. Display Products\n4. Display Farmers\n5. Filter Products\n6. Buy Product\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addFarmer();
                break;
            case 2:
                addProduct();
                break;
            case 3:
                displayProducts();
                break;
            case 4:
                displayFarmers();
                break;
            case 5: {
                int filter_choice;
                printf("\nFilter products by:\n");
                printf("1. Farmer's Name\n");
                printf("2. Product Name\n");
                printf("3. Location\n");
                printf("Enter your choice: ");
                scanf("%d", &filter_choice);

                char filter[50];
                printf("Enter the %s to filter: ", (filter_choice == 1) ? "farmer's name" : (filter_choice == 2) ? "product name" : "location");
                scanf("%s", filter);

                filterProducts(filter_choice, filter, 0, "", 0, "", 0);
                break;
            }
            case 6:
                buyProduct();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
