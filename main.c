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
    int ID;
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
void showAllItems(struct Item items[], int count) {
    printf("All items: \n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d. Name: %s\n", items[i].ID, items[i].name);
    }
}
int main(void) {
    struct Item items[3];

    // Product 1
    items[0].ID = 0;
    strcpy(items[0].name, "Notebook Dell XPS 15");
    items[0].supplierPrice = 25000;
    items[0].sellPrice = 32000;
    items[0].DPH = 21;
    items[0].quantity = 5;
    items[0].cubicCapacity = 50;
    items[0].supplier.ICO = 12345678;
    strcpy(items[0].supplier.name, "Tech Distributors s.r.o.");
    strcpy(items[0].supplier.address.street, "Vinohradska 123");
    strcpy(items[0].supplier.address.city, "Praha");
    strcpy(items[0].supplier.address.state, "Ceska republika");
    strcpy(items[0].supplier.address.zip, "110 00");

    // Product 2
    items[1].ID = 1;
    strcpy(items[1].name, "Logitech MX Master 3");
    items[1].supplierPrice = 1500;
    items[1].sellPrice = 2200;
    items[1].DPH = 21;
    items[1].quantity = 15;
    items[1].cubicCapacity = 5;
    items[1].supplier.ICO = 87654321;
    strcpy(items[1].supplier.name, "PC Components a.s.");
    strcpy(items[1].supplier.address.street, "Karlova 45");
    strcpy(items[1].supplier.address.city, "Brno");
    strcpy(items[1].supplier.address.state, "Ceska republika");
    strcpy(items[1].supplier.address.zip, "602 00");

    // Product 3
    items[2].ID = 2;
    strcpy(items[2].name, "Corsair K95 RGB");
    items[2].supplierPrice = 3500;
    items[2].sellPrice = 5000;
    items[2].DPH = 21;
    items[2].quantity = 8;
    items[2].cubicCapacity = 10;
    items[2].supplier.ICO = 11223344;
    strcpy(items[2].supplier.name, "Gaming Gear s.r.o.");
    strcpy(items[2].supplier.address.street, "Narodni 78");
    strcpy(items[2].supplier.address.city, "Ostrava");
    strcpy(items[2].supplier.address.state, "Ceska republika");
    strcpy(items[2].supplier.address.zip, "702 00");

    int itemCount = 3;

    char choice;
    do {
        showMenu();
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                printf("\n--- Vypis vsech produktu ---\n");
                showAllItems(items, itemCount);
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