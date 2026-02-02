#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Shelf {
    int index;
    float capacity;
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
    float supplierPrice;
    float sellPrice;
    int DPH;
    int quantity;
    float cubicCapacity;
    struct Supplier supplier;
};

void showMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Vypis vsech produktu\n");
    printf("2. Vyhledavani produktu podle nazvu\n");
    printf("3. Vyhledavani podle ceny\n");
    printf("4. Zobrazeni detailu produktu\n");
    printf("5. Odstraneni produktu\n");
    printf("6. Upravení produktu\n");
    printf("7. Přidání produktu\n");
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

void searchItemsBySellerPrice(struct Item items[], int count) {
    float minPrice, maxPrice;

    printf("Enter minimum price: ");
    scanf("%f", &minPrice);

    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    if (minPrice > maxPrice) {
        printf("špatný filtr\n");
        return;
    }
    printf("\nProdukty v tomto rozmezí %.2f - %.2f:\n", minPrice, maxPrice);
    for (int i = 0; i < count; i++) {
        if (items[i].sellPrice >= minPrice && items[i].sellPrice <= maxPrice) {
            printf("ID: %d, Name: %s, Price: %.2f\n", items[i].ID, items[i].name, items[i].sellPrice);
        }
    }
}

void printItemDetail(struct Item *item) {
    printf("\n=== Detail produktu ===\n");
    printf("ID: %d\n", item->ID);
    printf("Název: %s\n", item->name);
    printf("EAN: %d\n", item->EAN);
    printf("Rok výroby: %d\n", item->productionYear);
    printf("Šarže: %s\n", item->batch);
    printf("Nákupní cena: %.2f Kč\n", item->supplierPrice);
    printf("Prodejní cena: %.2f Kč\n", item->sellPrice);
    printf("DPH: %d%%\n", item->DPH);
    printf("Množství: %d ks\n", item->quantity);
    printf("Objem: %.2f m³\n", item->cubicCapacity);
    printf("\n--- Dodavatel ---\n");
    printf("IČO: %d\n", item->supplier.ICO);
    printf("Název: %s\n", item->supplier.name);
    printf("Adresa: %s, %s, %s %s\n",
           item->supplier.address.street,
           item->supplier.address.city,
           item->supplier.address.zip,
           item->supplier.address.state);
}

void showItemDetail(struct Item items[], int count) {
    int searchID;
    showAllItems(items,count);
    printf("Zadejte ID produktu: ");
    scanf("%d", &searchID);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            found = 1;
            printItemDetail(&items[i]);
            break;
        }
    }

    if (!found) {
        printf("Produkt s ID %d nebyl nalezen.\n", searchID);
    }
}

void removeItem(struct Item items[], int *count) {
    showAllItems(items, *count);
    int searchID;
    printf("Zadejte ID produktu k odstranění: ");
    scanf("%d", &searchID);

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (items[i].ID == searchID) {
            found = 1;
            printf("Odstraňuji produkt: %s (ID: %d)\n", items[i].name, items[i].ID);

            // Posun všechny následující prvky o jednu pozici doleva
            for (int j = i; j < *count - 1; j++) {
                items[j] = items[j + 1];
            }

            (*count)--;

            // Aktualizuj ID všech produktů od odstraněného místa
            for (int j = i; j < *count; j++) {
                items[j].ID = j;
            }

            printf("Produkt byl úspěšně odstraněn a ID byla aktualizována.\n");
            break;
        }
    }

    if (!found) {
        printf("Produkt s ID %d nebyl nalezen.\n", searchID);
    }
}

void editItem(struct Item items[], int count) {
    showAllItems(items, count);
    int searchID;
    printf("Zadejte ID produktu k úpravě: ");
    scanf("%d", &searchID);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            found = 1;
            
            // Zobraz detail produktu před úpravou
            printItemDetail(&items[i]);
            
            printf("\n=== Co chcete upravit? ===\n");
            printf("1. Název\n");
            printf("2. EAN\n");
            printf("3. Rok výroby\n");
            printf("4. Šarže\n");
            printf("5. Nákupní cena\n");
            printf("6. Prodejní cena\n");
            printf("7. DPH\n");
            printf("8. Množství\n");
            printf("9. Objem\n");
            printf("10. Dodavatel\n");
            printf("0. Zrušit\n");
            printf("Vyberte možnost: ");
            
            int choice;
            scanf("%d", &choice);
            
            switch (choice) {
                case 1:
                    printf("Nový název: ");
                    scanf(" %39[^\n]", items[i].name);
                    printf("Název byl aktualizován.\n");
                    break;
                case 2:
                    printf("Nové EAN: ");
                    scanf("%d", &items[i].EAN);
                    printf("EAN bylo aktualizováno.\n");
                    break;
                case 3:
                    printf("Nový rok výroby: ");
                    scanf("%d", &items[i].productionYear);
                    printf("Rok výroby byl aktualizován.\n");
                    break;
                case 4:
                    printf("Nová šarže: ");
                    scanf(" %9[^\n]", items[i].batch);
                    printf("Šarže byla aktualizována.\n");
                    break;
                case 5:
                    printf("Nová nákupní cena: ");
                    scanf("%f", &items[i].supplierPrice);
                    printf("Nákupní cena byla aktualizována.\n");
                    break;
                case 6:
                    printf("Nová prodejní cena: ");
                    scanf("%f", &items[i].sellPrice);
                    printf("Prodejní cena byla aktualizována.\n");
                    break;
                case 7:
                    printf("Nové DPH (%%): ");
                    scanf("%d", &items[i].DPH);
                    printf("DPH bylo aktualizováno.\n");
                    break;
                case 8:
                    printf("Nové množství: ");
                    scanf("%d", &items[i].quantity);
                    printf("Množství bylo aktualizováno.\n");
                    break;
                case 9:
                    printf("Nový objem: ");
                    scanf("%f", &items[i].cubicCapacity);
                    printf("Objem byl aktualizován.\n");
                    break;
                case 10:
                    printf("Nové IČO dodavatele: ");
                    scanf("%d", &items[i].supplier.ICO);
                    printf("Nový název dodavatele: ");
                    scanf(" %39[^\n]", items[i].supplier.name);
                    printf("Ulice: ");
                    scanf(" %39[^\n]", items[i].supplier.address.street);
                    printf("Město: ");
                    scanf(" %39[^\n]", items[i].supplier.address.city);
                    printf("PSČ: ");
                    scanf(" %39[^\n]", items[i].supplier.address.zip);
                    printf("Stát: ");
                    scanf(" %39[^\n]", items[i].supplier.address.state);
                    printf("Dodavatel byl aktualizován.\n");
                    break;
                case 0:
                    printf("Úprava zrušena.\n");
                    break;
                default:
                    printf("Neplatná volba.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Produkt s ID %d nebyl nalezen.\n", searchID);
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
    items[2].supplierPrice = 34;
    items[2].sellPrice = 189;
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
                searchItemsBySellerPrice(items, itemCount);
                break;
            case '4':
                printf("\n--- Detail produktu ---\n");
                showItemDetail(items, itemCount);
                break;
            case '5':
                printf("\n--- Odstraneni produktu ---\n");
                removeItem(items, &itemCount);
                break;
            case '6':
                printf("\n--- Upravit produkt ---\n");
                editItem(items, itemCount);
                break;
            case '7':
                printf("\n--- Nový produkt ---\n");
                // TODO
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
