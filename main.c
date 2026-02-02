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
    int EAN;
    int productionYear;
    char batch[10];
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

void searchItems(struct Item items[], int count) {
    char search[40];
    printf("Search items: \n");
    scanf("%39s", &search);

    printf("Searched items: \n");
    for (int i = 0; i < count; i++) {
        if (strstr(items[i].name, search)) {
            printf("ID: %d. Name: %s\n", items[i].ID, items[i].name);
        }
    }
}

int main(void) {
    struct Item items[1000];

    // Product 1
    items[0].ID = 0;
    items[0].EAN = 894662836;
    items[0].productionYear = 2025;
    strcpy(items[0].batch, "112/25");
    strcpy(items[0].name, "Solaris");
    items[0].supplierPrice = 43;
    items[0].sellPrice = 180;
    items[0].DPH = 21;
    items[0].quantity = 5;
    items[0].cubicCapacity = 8;
    items[0].supplier.ICO = 12345678;
    strcpy(items[0].supplier.name, "Víno Hruška s.r.o.");
    strcpy(items[0].supplier.address.street, "Blatnička 143");
    strcpy(items[0].supplier.address.city, "Blatnička");
    strcpy(items[0].supplier.address.state, "Česka republika");
    strcpy(items[0].supplier.address.zip, "696 71");

    // Product 2
    items[1].ID = 1;
    items[1].EAN = 894662834;
    items[1].productionYear = 2024;
    strcpy(items[1].batch, "108/24");
    strcpy(items[1].name, "Merzling - keg 50l");
    items[1].supplierPrice = 2000;
    items[1].sellPrice = 8000;
    items[1].DPH = 21;
    items[1].quantity = 15;
    items[1].cubicCapacity = 55;
    items[1].supplier.ICO = 87654321;
    strcpy(items[1].supplier.name, "Víno Hruška s.r.o.");
    strcpy(items[1].supplier.address.street, "Blatnička 143");
    strcpy(items[1].supplier.address.city, "Blatnička");
    strcpy(items[1].supplier.address.state, "Česka republika");
    strcpy(items[1].supplier.address.zip, "696 71");

    // Product 3
    items[2].ID = 2;
    items[2].EAN = 894662837;
    items[2].productionYear = 2025;
    strcpy(items[2].batch, "113/25");
    strcpy(items[2].name, "Děvín");
    items[2].supplierPrice = 3500;
    items[2].sellPrice = 5000;
    items[2].DPH = 21;
    items[2].quantity = 8;
    items[2].cubicCapacity = 8;
    items[2].supplier.ICO = 11223344;
    strcpy(items[2].supplier.name, "Víno Hruška s.r.o.");
    strcpy(items[2].supplier.address.street, "Blatnička 143");
    strcpy(items[2].supplier.address.city, "Blatnička");
    strcpy(items[2].supplier.address.state, "Česka republika");
    strcpy(items[2].supplier.address.zip, "696 71");

    int itemCount = 3;

    char choice;
    do {
        showMenu();
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("\n--- Vypis vsech produktu ---\n");
                showAllItems(items, itemCount);
                break;
            case '2':
                printf("\n--- Vyhledavani produktu ---\n");
                searchItems(items, itemCount);
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
