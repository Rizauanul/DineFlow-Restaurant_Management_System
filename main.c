#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

// Structure for food items
typedef struct {
    int id;
    char name[50];
    float price;
} FoodItem;

// Structure for order items
typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} OrderItem;

// Linked list node for food menu
typedef struct Node {
    FoodItem food;
    struct Node* next;
} Node;

// Linked list node for order menu
typedef struct OrderNode {
    OrderItem order;
    struct OrderNode* next;
} OrderNode;

// Function prototypes
void loadScreen();
void clearScreen();
void adminSection();
void customerSection();
void viewTotalSales();
void addNewItem();
void deleteItem();
void displayOrderMenu();
void placeOrder();
void viewOrderedItems();
void deleteOrderItem();
void payBill();
void displayFoodMenu();
void saveOrderMenu(OrderNode* orderMenu);
void saveBill(OrderNode* orderMenu);
void loadOrderMenu(OrderNode** orderMenu);
void info();
void printLine();

// Global variables
char adminUsername[20] = "admin";
char adminPassword[20] = "password";
float totalSales = 0.0;

int main() {
    loadScreen();

    int choice;
    OrderNode* orderMenu = NULL;  // Initialize the order menu
    float totalSales = 0.0;  // Initialize total sales

    do {
        clearScreen();
        printf("\n\t\t\t\tRestaurant Management System\n\n");
        printf("\t\t\t\t1. Admin Section\n");
        printf("\t\t\t\t2. Customer Section\n");
        printf("\t\t\t\t3. About\n");
        printf("\t\t\t\t4. Exit\n");
        printf("\n\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminSection();
                break;
            case 2:
                customerSection(&orderMenu, &totalSales); // Pass orderMenu and totalSales
                break;
            case 3:
                info();
                break;    
            case 4:
                printf("\n\t\t\t\tThank you for using the Restaurant Management System!\n");
                exit(0);
            default:
                printf("\n\t\t\t\tInvalid choice! Please try again.\n");
                sleep(2);
        }
    } while (1);

    return 0;
}


void loadScreen() {
    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\tLoading Restaurant Management System\n\n\n");
    printf("\t\t\t\t\t");
    for (int i = 0; i < 4; i++) {
        printf("|||   ");
        fflush(stdout);
        Sleep(500); 
    }
}


void clearScreen() {
    system("cls");
}

void adminSection() {
    char username[20], password[20];
    clearScreen();
    printf("\n\t\t\t\tAdmin Section\n\n");
    printf("\t\t\t\tEnter username: ");
    scanf("%s", username);
    printf("\t\t\t\tEnter password: ");
    scanf("%s", password);

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        int choice;
        do {
            clearScreen();
            printf("\n\t\t\t\tAdmin Section\n\n");
            printf("\t\t\t\t1. View Bill\n");
            printf("\t\t\t\t2. Add New Item in Order Menu\n");
            printf("\t\t\t\t3. Delete Item from Order Menu\n");
            printf("\t\t\t\t4. Display Order Menu\n");
            printf("\t\t\t\t5. Back to Main Menu\n");
            printf("\n\t\t\t\tEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    viewTotalSales();
                    break;
                case 2:
                    addNewItem();
                    break;
                case 3:
                    deleteItem();
                    break;
                case 4:
                    displayOrderMenu();
                    break;
                case 5:
                    return;
                default:
                    printf("\n\t\t\t\tInvalid choice! Please try again.\n");
                    sleep(2);
            }
        } while (1);
    } else {
        printf("\n\t\t\t\tInvalid username or password! Access denied.\n");
        sleep(2);
    }
}

void customerSection(OrderNode** orderMenu, float* totalSales) {
    int choice;
    do {
        clearScreen();
        printf("\n\t\t\t\tCustomer Section\n\n");
        printf("\t\t\t\t1. Place Your Order\n");
        printf("\t\t\t\t2. View Your Ordered Items and Price\n");
        printf("\t\t\t\t3. Delete an Item from Order\n");
        printf("\t\t\t\t4. Pay Your Bill\n");
        printf("\t\t\t\t5. Display Food Menu\n");
        printf("\t\t\t\t6. Back to Main Menu\n");
        printf("\n\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                placeOrder(orderMenu);
                break;
            case 2:
                viewOrderedItems(*orderMenu); // Pass orderMenu
                break;
            case 3:
                deleteOrderItem(orderMenu); // Pass orderMenu
                break;
            case 4:
                payBill(orderMenu, totalSales); // Pass orderMenu and totalSales
                break;
            case 5:
                displayFoodMenu();
                break;
            case 6:
                return;
            default:
                printf("\n\t\t\t\tInvalid choice! Please try again.\n");
                sleep(2);
        }
    } while (1);
}

void viewTotalSales() {
     clearScreen();
    printf("\n\t\t\t\tView Paid Orders\n\n");

    FILE* file = fopen("bills.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tNo paid orders found.\n");
        sleep(2);
        return;
    }

    printf("\t\t\t\tID\tName\t\tPrice\t\tQuantity\tTotal\n");
    while (1) {
        int id, quantity;
        float price, total;
        char name[50];
        
        if (fscanf(file, "%d %s %f %d", &id, name, &price, &quantity) != 4) {
            break; // End of file reached
        }

        total = price * quantity;
        printf("\t\t\t\t%d\t%s\t\t%.2f\t\t%d\t\t%.2f\n", id, name, price, quantity, total);
    }

    fclose(file);
    sleep(5);
}

void addNewItem() {
    clearScreen();
    printf("\n\t\t\t\tAdd New Item in Order Menu\n\n");

    FILE* file = fopen("order_menu.txt", "a");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    FoodItem food;
    printf("\t\t\t\tEnter item ID: ");
    scanf("%d", &food.id);
    printf("\t\t\t\tEnter item name: ");
    scanf("%s", food.name);
    printf("\t\t\t\tEnter item price: ");
    scanf("%f", &food.price);

    fprintf(file, "%d %s %.2f\n", food.id, food.name, food.price);
    fclose(file);

    printf("\n\t\t\t\tItem added successfully!\n");
    sleep(2);
}

void deleteItem() {
    clearScreen();
    printf("\n\t\t\t\tDelete Item from Order Menu\n\n");

    int id;
    printf("\t\t\t\tEnter item ID to delete: ");
    scanf("%d", &id);

    FILE* file = fopen("order_menu.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        fclose(file);
        return;
    }

    int found = 0;
    FoodItem food;
    while (fscanf(file, "%d %s %f", &food.id, food.name, &food.price) != EOF) {
        if (food.id != id) {
            fprintf(tempFile, "%d %s %.2f\n", food.id, food.name, food.price);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("order_menu.txt");
    rename("temp.txt", "order_menu.txt");

    if (found) {
        printf("\n\t\t\t\tItem deleted successfully!\n");
    } else {
        printf("\n\t\t\t\tItem not found!\n");
    }

    sleep(2);
}

void displayOrderMenu() {
    clearScreen();
    printf("\n\t\t\t\tOrder Menu\n\n");

    FILE* file = fopen("order_menu.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    FoodItem food;
    printf("\t\t\t\tID\tName\t\tPrice\n");
    while (fscanf(file, "%d %s %f", &food.id, food.name, &food.price) != EOF) {
        printf("\t\t\t\t%d\t%s\t\t%.2f\n", food.id, food.name, food.price);
    }

    fclose(file);
    sleep(5);
}

void placeOrder(OrderNode** orderMenu) {
    clearScreen();
    printf("\n\t\t\t\tPlace Your Order\n\n");

    FILE* file = fopen("order_menu.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    int id, quantity;
    printf("\t\t\t\tEnter item ID: ");
    scanf("%d", &id);
    printf("\t\t\t\tEnter quantity: ");
    scanf("%d", &quantity);

    FoodItem food;
    int found = 0;
    while (fscanf(file, "%d %s %f", &food.id, food.name, &food.price) != EOF) {
        if (food.id == id) {
            found = 1;
            OrderNode* newNode = (OrderNode*)malloc(sizeof(OrderNode));
            newNode->order.id = food.id;
            strcpy(newNode->order.name, food.name);
            newNode->order.price = food.price;
            newNode->order.quantity = quantity;
            newNode->next = NULL;

            if (*orderMenu == NULL) {
                *orderMenu = newNode;
            } else {
                OrderNode* temp = *orderMenu;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }

            printf("\n\t\t\t\tItem added to order menu successfully!\n");
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("\n\t\t\t\tItem not found!\n");
    }

    sleep(2);
}

void viewOrderedItems(OrderNode* orderMenu) {
    clearScreen();
    printf("\n\t\t\t\tView Your Ordered Items and Price\n\n");

    if (orderMenu == NULL) {
        printf("\n\t\t\t\tNo items in the order menu!\n");
        sleep(2);
        return;
    }

    OrderNode* temp = orderMenu;
    printf("\t\t\t\tID\tName\t\tPrice\t\tQuantity\n");
    while (temp != NULL) {
        printf("\t\t\t\t%d\t%s\t\t%.2f\t\t%d\n", temp->order.id, temp->order.name, temp->order.price, temp->order.quantity);
        temp = temp->next;
    }

    sleep(5);
}

void deleteOrderItem(OrderNode** orderMenu) {
    clearScreen();
    printf("\n\t\t\t\tDelete an Item from Order\n\n");

    if (*orderMenu == NULL) {
        printf("\n\t\t\t\tNo items in the order menu!\n");
        sleep(2);
        return;
    }

    int id;
    printf("\t\t\t\tEnter item ID to delete: ");
    scanf("%d", &id);

    OrderNode* temp = *orderMenu;
    OrderNode* prev = NULL;
    int found = 0;
    while (temp != NULL) {
        if (temp->order.id == id) {
            found = 1;
            if (prev == NULL) {
                *orderMenu = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (found) {
        printf("\n\t\t\t\tItem deleted from the order menu successfully!\n");
    } else {
        printf("\n\t\t\t\tItem not found!\n");
    }

    sleep(2);
}

void payBill(OrderNode** orderMenu, float* totalSales) {
    clearScreen();
    printf("\n\t\t\t\tPay Your Bill\n\n");

    if (*orderMenu == NULL) {
        printf("\n\t\t\t\tNo items in the order menu!\n");
        sleep(2);
        return;
    }

    float bill = 0.0;
    OrderNode* temp = *orderMenu;
    printf("\t\t\t\tID\tName\t\tPrice\t\tQuantity\tTotal\n");
    while (temp != NULL) {
        float total = temp->order.price * temp->order.quantity;
        printf("\t\t\t\t%d\t%s\t\t%.2f\t\t%d\t\t%.2f\n", temp->order.id, temp->order.name, temp->order.price, temp->order.quantity, total);
        bill += total;
        temp = temp->next;
    }

    printf("\n\t\t\t\tTotal Bill: $%.2f\n", bill);

    *totalSales += bill;

    // Saving the updated order menu and total sales
    saveBill(*orderMenu);
    while (temp != NULL) {
        OrderNode* next = temp->next;
        free(temp);
        temp = next;
    }
    *orderMenu = NULL;
    sleep(5);
}


void displayFoodMenu() {
    clearScreen();
    printf("\n\t\t\t\tFood Menu\n\n");

    FILE* file = fopen("order_menu.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    FoodItem food;
    printf("\t\t\t\tID\tName\t\tPrice\n");
    while (fscanf(file, "%d %s %f", &food.id, food.name, &food.price) != EOF) {
        printf("\t\t\t\t%d\t%s\t\t%.2f\n", food.id, food.name, food.price);
    }

    fclose(file);
    sleep(5);
}

void saveOrderMenu(OrderNode* orderMenu) {
    FILE* file = fopen("order_menu.txt", "w");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    OrderNode* temp = orderMenu;
    while (temp != NULL) {
        fprintf(file, "%d %s %.2f %d\n", temp->order.id, temp->order.name, temp->order.price, temp->order.quantity);
        temp = temp->next;
    }

    fclose(file);
}

void saveBill(OrderNode* orderMenu) {
    FILE* file = fopen("bills.txt", "a");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    OrderNode* temp = orderMenu;
    while (temp != NULL) {
        fprintf(file, "%d %s %.2f %d\n", temp->order.id, temp->order.name, temp->order.price, temp->order.quantity);
        temp = temp->next;
    }

    fclose(file);
}

void loadOrderMenu(OrderNode** orderMenu) {
    FILE* file = fopen("order_menu.txt", "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening file! Please try again.\n");
        sleep(2);
        return;
    }

    OrderNode* temp = *orderMenu;
    while (temp != NULL) {
        OrderNode* next = temp->next;
        free(temp);
        temp = next;
    }
    *orderMenu = NULL;

    OrderNode* newNode;
    while (!feof(file)) {
        newNode = (OrderNode*)malloc(sizeof(OrderNode));
        if (fscanf(file, "%d %s %f %d", &newNode->order.id, newNode->order.name, &newNode->order.price, &newNode->order.quantity) == 4) {
            newNode->next = NULL;

            if (*orderMenu == NULL) {
                *orderMenu = newNode;
            } else {
                temp = *orderMenu;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        } else {
            free(newNode);
        }
    }

    fclose(file);
}

void info()
{
    clearScreen();
    printf("\n\n\n\n\n");
    printf("\t\t\t\tRestaurant Management Systam\n\n\n");
    printf("\t\t\t\tDevelopers: \n\n");
    printf("\t\t\t\tMd. Rizuanul Haque Rakib - 221-15-5170\n\n");
    printf("\t\t\t\tNowshin Tabassum Rahman Nuha - 221-15-4795\n\n");
    printf("\t\t\t\tSagor Chandra Mojumdar - 221-15-4907\n\n");
    printf("\t\t\t\tSahabi Mukul - 221-15-4842\n\n");
    sleep(5);
}

void printLine() {
    printf("\n-----------------------------------------------------------------------------\n");
}
