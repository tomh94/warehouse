#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Shelf {
    int index;
    int capacity;
};

struct Address {
    char street[40];
    char city[40];
    char state[40];
    char zip[40];
};

struct Supplier {
    int ICO;
    char name[40];
    struct Address address;
};

struct Item {
    char name[40];
    int supplierPrice;
    int sellPrice;
    int DPH;
    int quantity;
    int cubicCapacity;
    struct Supplier supplier;
};

void showMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Vypis vsech produktu\n");
    printf("2. Vyhledavani produktu podle nazvu\n");
    printf("3. Vyhledavani podle ceny\n");
    printf("4. Zobrazeni detailu produktu\n");
    printf("5. Odstraneni produktu\n");
    printf("6. Upravit/přidat produktu\n");
    printf("X. Konec\n");
    printf("Vyberte akci: ");
}

int main(void) {

    char choice;
    do {
        showMenu();
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                printf("\n--- Vypis vsech produktu ---\n");
                // TODO
                break;
            case '2':
                printf("\n--- Vyhledavani produktu ---\n");
                // TODO
                break;
            case '3':
                printf("\n--- Vyhledavani podle ceny ---\n");
                // TODO
                break;
            case '4':
                printf("\n--- Detail produktu ---\n");
                // TODO
                break;
            case '5':
                printf("\n--- Odstraneni produktu ---\n");
                // TODO
                break;
            case '6':
                printf("\n--- Uprava produktu ---\n");
                // TODO
                break;
            case 'X':
            case 'x':
                printf("\nUkoncuji aplikaci...\n");
                break;
            default:
                printf("\nNeplatna volba\n");
        }
    } while (choice != 'X' && choice != 'x');

    return 0;
}