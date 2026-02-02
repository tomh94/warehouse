#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WAREHOUSE_CAPACITY 1000

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
    double cubicCapacity;
    struct Supplier supplier;
};

float getTotalVolume(struct Item items[], int count) {
    float total = 0.0;
    for (int i = 0; i < count; i++) {
        total += items[i].cubicCapacity * items[i].quantity;
    }
    return total;
}

int getTotalQuantity(struct Item items[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += items[i].quantity;
    }
    return total;
}

void showMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Vypis vsech produktu\n");
    printf("2. Vyhledavani produktu podle nazvu\n");
    printf("3. Vyhledavani podle ceny\n");
    printf("4. Zobrazeni detailu produktu\n");
    printf("5. Odstraneni produktu\n");
    printf("6. Upravení produktu\n");
    printf("7. Přidání produktu\n");
    printf("8. Stav skladu\n");
    printf("9. Naskladnění\n");
    printf("X. Konec\n");
    printf("Vyberte akci: ");
}

void showAllItems(struct Item items[], int count) {
    printf("All items: \n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d. Name: %s\n", items[i].ID, items[i].name);
    }
}

void showWarehouseStatus(struct Item items[], int count) {
    int totalQuantity = getTotalQuantity(items, count);
    float totalVolume = getTotalVolume(items, count);
    float available = MAX_WAREHOUSE_CAPACITY - totalVolume;
    float usage = (totalVolume * 100.0) / MAX_WAREHOUSE_CAPACITY;

    printf("\n=== Stav skladu ===\n");
    printf("Celková kapacita: %d m³\n", MAX_WAREHOUSE_CAPACITY);
    printf("Obsazeno: %.2f m³ (%d ks)\n", totalVolume, totalQuantity);
    printf("Volné místo: %.2f m³\n", available);
    printf("Využití: %.1f%%\n", usage);

    if (usage > 90) {
        printf("Sklad je téměř plný\n");
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

            // move items by one to the left
            for (int j = i; j < *count - 1; j++) {
                items[j] = items[j + 1];
            }

            (*count)--;

            // actualize IDs
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

            printItemDetail(&items[i]);
            
            printf("\n=== Co chcete upravit? ===\n");
            printf("1. Název\n");
            printf("2. EAN\n");
            printf("3. Rok výroby\n");
            printf("4. Šarže\n");
            printf("5. Nákupní cena\n");
            printf("6. Prodejní cena\n");
            printf("7. DPH\n");
            printf("8. Objem\n");
            printf("9. Dodavatel\n");
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
                        printf("Nový objem: ");
                        scanf("%f", &items[i].cubicCapacity);
                        printf("Objem byl aktualizován.\n");
                        break;
                    case 9:
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

void addItem(struct Item items[], int *count) {
    int totalQuantity = getTotalQuantity(items, *count);
    int availableSpace = MAX_WAREHOUSE_CAPACITY - totalQuantity;
    
    if (availableSpace <= 0) {
        printf("Sklad je plný! Nelze přidat nový produkt.\n");
        return;
    }
    
    printf("Volná kapacita skladu: %d ks\n", availableSpace);
    
    char copyChoice;
    printf("Chcete kopírovat existující produkt? (a/n): ");
    scanf(" %c", &copyChoice);
    
    struct Item *item = &items[*count];
    item->ID = *count;
    
    if (copyChoice == 'a' || copyChoice == 'A') {
        int copyID;
        showAllItems(items, *count);
        printf("Zadejte ID produktu ke kopirovani: ");
        scanf("%d", &copyID);
        
        if (copyID >= 0 && copyID < *count) {
            // copy product
            *item = items[copyID];
            item->ID = *count;
            
            // rewrite specifications
            printf("Název: "); scanf(" %39[^\n]", item->name);
            printf("EAN: "); scanf("%d", &item->EAN);
            printf("Rok výroby: "); scanf("%d", &item->productionYear);
            item->quantity = 0;
            
            (*count)++;
            printf("Produkt byl zkopírován a upraven s ID %d.\n", item->ID);
        } else {
            printf("Neplatné ID.\n");
        }
    } else {
        // enter all specifications
        printf("Název: "); scanf(" %39[^\n]", item->name);
        printf("EAN: "); scanf("%d", &item->EAN);
        printf("Rok výroby: "); scanf("%d", &item->productionYear);
        printf("Šarže: "); scanf(" %9[^\n]", item->batch);
        printf("Nákupní cena: "); scanf("%f", &item->supplierPrice);
        printf("Prodejní cena: "); scanf("%f", &item->sellPrice);
        printf("DPH (%%): "); scanf("%d", &item->DPH);
        item->quantity = 0;
        printf("Objem: "); scanf("%f", &item->cubicCapacity);
        printf("IČO dodavatele: "); scanf("%d", &item->supplier.ICO);
        printf("Název dodavatele: "); scanf(" %39[^\n]", item->supplier.name);
        printf("Ulice: "); scanf(" %39[^\n]", item->supplier.address.street);
        printf("Město: "); scanf(" %39[^\n]", item->supplier.address.city);
        printf("PSČ: "); scanf(" %39[^\n]", item->supplier.address.zip);
        printf("Stát: "); scanf(" %39[^\n]", item->supplier.address.state);
        (*count)++;
        printf("Produkt byl přidán s ID %d.\n", item->ID);
    }
}

void stockIn(struct Item items[], int count) {
    showAllItems(items, count);
    int searchID;
    printf("Zadejte ID produktu k naskladnění: ");
    scanf("%d", &searchID);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            found = 1;
        
            printf("\nProdukt: %s\n", items[i].name);
            printf("Aktuální množství: %d ks\n", items[i].quantity);
            printf("Objem jednoho kusu: %.2f m³\n", items[i].cubicCapacity);
        
            float totalVolume = getTotalVolume(items, count);
            float availableSpace = MAX_WAREHOUSE_CAPACITY - totalVolume;
        
            printf("Volná kapacita skladu: %.2f m³\n", availableSpace);
        
            if (availableSpace <= 0) {
                printf("Sklad je plný! Nelze naskladnit.\n");
                break;
            }
        
            int maxPieces = (int)(availableSpace / items[i].cubicCapacity);
            printf("Maximálně lze naskladnit: %d ks\n", maxPieces);
        
            int addQuantity;
            printf("Kolik kusů chcete naskladnit: ");
            scanf("%d", &addQuantity);
        
            if (addQuantity < 0) {
                printf("Množství nemůže být záporné.\n");
            } else if (addQuantity > maxPieces) {
                printf("Nedostatek místa na skladě! Maximálně můžete naskladnit %d ks (%.2f m³).\n", 
                       maxPieces, maxPieces * items[i].cubicCapacity);
            } else {
                items[i].quantity += addQuantity;
                printf("Naskladněno %d ks (%.2f m³). Nové množství: %d ks\n", 
                       addQuantity, addQuantity * items[i].cubicCapacity, items[i].quantity);
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
    items[0].quantity = 120;
    items[0].cubicCapacity = 0.1;
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
    items[1].cubicCapacity = 1;
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
    items[2].cubicCapacity = 0.1;
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
                addItem(items, &itemCount);
                break;
            case '8':
                printf("\n--- Stav skladu ---\n");
                showWarehouseStatus(items, itemCount);
                break;
            case '9':
                printf("\n--- Naskladnění ---\n");
                stockIn(items, itemCount);
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
