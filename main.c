#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definování začátku

#define MAX_WAREHOUSE_CAPACITY 1000.0
#define INITIAL_CAPACITY 3

// struktura struktur

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
    long long EAN;
    int productionYear;
    char batch[10];
    char name[40];
    double supplierPrice;
    double sellPrice;
    int DPH;
    int quantity;
    double cubicCapacity;
    struct Supplier supplier;
};


// pomocné metody

double getTotalVolume(struct Item *items, int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += items[i].cubicCapacity * items[i].quantity;
    }
    return total;
}

int getTotalQuantity(struct Item *items, int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += items[i].quantity;
    }
    return total;
}

/** funkce na zvětšení paměti
 *
 * @param items
 * @param capacity
 * @return 1 když zvětší pamět, 0 když to nejde
 */
int growItems(struct Item **items, int *capacity) {
    int newCapacity = (*capacity) * 2;
    struct Item *tmp = realloc(*items, newCapacity * sizeof(struct Item));
    if (!tmp) {
        printf("CHYBA: Nedostatek pameti (realloc selhal).\n");
        return 0;
    }
    *items = tmp;
    *capacity = newCapacity;
    return 1;
}

// hlavní meníčko

void showMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Vypis vsech produktu\n");
    printf("2. Vyhledavani podle nazvu\n");
    printf("3. Vyhledavani podle ceny\n");
    printf("4. Detail produktu\n");
    printf("5. Odstraneni produktu\n");
    printf("6. Uprava produktu\n");
    printf("7. Pridani produktu\n");
    printf("8. Stav skladu\n");
    printf("9. Naskladneni\n");
    printf("X. Konec\n");
    printf("Vyberte akci: ");
}

// volba, vypíše všechny položky na skladě
void showAllItems(struct Item *items, int count) {
    if (count == 0) {
        printf("Sklad je prazdny.\n");
        return;
    }
    printf("%-5s %-30s %10s %10s\n", "ID", "Nazev", "Cena bez DPH", "Pocet ks");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-30s %9.2f Kc %7d ks\n",
               items[i].ID, items[i].name, items[i].sellPrice, items[i].quantity);
    }
}

// ukáže vyplněnost skladu
void showWarehouseStatus(struct Item *items, int count) {
    int totalQuantity = getTotalQuantity(items, count);
    double totalVolume = getTotalVolume(items, count);
    double available = MAX_WAREHOUSE_CAPACITY - totalVolume;
    double usage = (totalVolume * 100.0) / MAX_WAREHOUSE_CAPACITY;

    printf("\n=== Stav skladu ===\n");
    printf("Celkova kapacita: %.0f m3\n", MAX_WAREHOUSE_CAPACITY);
    printf("Obsazeno:         %.2f m3 (%d ks)\n", totalVolume, totalQuantity);
    printf("Volne misto:      %.2f m3\n", available);
    printf("Vyuziti:          %.1f%%\n", usage);

    if (usage > 90.0) {
        printf("UPOZORNENI: Sklad je temer plny!\n");
    }
}

// výpis detailu produktu
void printItemDetail(struct Item *item) {
    double priceWithDPH = item->sellPrice * (1.0 + item->DPH / 100.0);
    printf("\n=== Detail produktu ===\n");
    printf("ID:                  %d\n",      item->ID);
    printf("Nazev:               %s\n",      item->name);
    printf("EAN:                 %lld\n",    item->EAN);
    printf("Rok vyroby:          %d\n",      item->productionYear);
    printf("Sarze:               %s\n",      item->batch);
    printf("Nakupni cena:        %.2f Kc (bez DPH)\n", item->supplierPrice);
    printf("Prodejni cena:       %.2f Kc (bez DPH)\n", item->sellPrice);
    printf("Prodejni cena s DPH: %.2f Kc\n", priceWithDPH);
    printf("DPH:                 %d%%\n",    item->DPH);
    printf("Mnozstvi:            %d ks\n",   item->quantity);
    printf("Objem/ks:            %.4f m3\n", item->cubicCapacity);
    printf("--- Dodavatel ---\n");
    printf("ICO:    %d\n",  item->supplier.ICO);
    printf("Nazev:  %s\n",  item->supplier.name);
    printf("Adresa: %s, %s, %s, %s\n",
           item->supplier.address.street,
           item->supplier.address.city,
           item->supplier.address.zip,
           item->supplier.address.state);
}

// vyhledávání podle názvu
void searchItems(struct Item *items, int count) {
    char search[40];
    printf("Hledat nazev: ");
    scanf(" %39[^\n]", search);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(items[i].name, search)) {
            printf("ID: %d | %s | %.2f Kc (bez DPH) | %d ks\n",
                   items[i].ID, items[i].name, items[i].sellPrice, items[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("Zadny produkt nenalezen.\n");
}

// vyhledávání podle ceny
void searchItemsByPrice(struct Item *items, int count) {
    double minPrice, maxPrice;
    printf("Minimalni cena (bez DPH): ");
    scanf("%lf", &minPrice);
    printf("Maximalni cena (bez DPH): ");
    scanf("%lf", &maxPrice);

    if (minPrice > maxPrice) {
        printf("Chyba: minimalni cena je vyssi nez maximalni.\n");
        return;
    }

    int found = 0;
    printf("\nProdukty v rozmezi %.2f - %.2f Kc (bez DPH):\n", minPrice, maxPrice);
    for (int i = 0; i < count; i++) {
        if (items[i].sellPrice >= minPrice && items[i].sellPrice <= maxPrice) {
            double priceWithDPH = items[i].sellPrice * (1.0 + items[i].DPH / 100.0);
            printf("ID: %d | %s | bez DPH: %.2f Kc | s DPH: %.2f Kc | DPH: %d%%\n",
                   items[i].ID, items[i].name,
                   items[i].sellPrice, priceWithDPH, items[i].DPH);
            found = 1;
        }
    }
    if (!found) printf("Zadny produkt v tomto rozmezi.\n");
}
// funkce na výběr produktu k vypsání jeho detailu
void showItemDetail(struct Item *items, int count) {
    showAllItems(items, count);
    int searchID;
    printf("Zadejte ID produktu: ");
    scanf("%d", &searchID);

    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            printItemDetail(&items[i]);
            return;
        }
    }
    printf("Produkt s ID %d nebyl nalezen.\n", searchID);
}

// odstraní item
void removeItem(struct Item **items, int *count, int *capacity) {
    showAllItems(*items, *count);
    int searchID;
    printf("Zadejte ID produktu k odstraneni: ");
    scanf("%d", &searchID);

    for (int i = 0; i < *count; i++) {
        if ((*items)[i].ID == searchID) {
            printf("Odstranuji: %s (ID: %d)\n", (*items)[i].name, (*items)[i].ID);

            // posun prvku vlevo
            for (int j = i; j < *count - 1; j++) {
                (*items)[j] = (*items)[j + 1];
            }
            (*count)--;

            // aktualizuj ID
            for (int j = i; j < *count; j++) {
                (*items)[j].ID = j;
            }

            // zmensi pamet pokud je vyuzita mene nez ctvrtina (ale ne pod INITIAL_CAPACITY)
            if (*count > INITIAL_CAPACITY && *count < (*capacity) / 4) {
                int newCapacity = (*capacity) / 2;
                struct Item *tmp = realloc(*items, newCapacity * sizeof(struct Item));
                if (tmp) {
                    *items = tmp;
                    *capacity = newCapacity;
                }
            }

            printf("Produkt byl uspesne odstranen.\n");
            return;
        }
    }
    printf("Produkt s ID %d nebyl nalezen.\n", searchID);
}
// upraví item
void editItem(struct Item *items, int count) {
    showAllItems(items, count);
    int searchID;
    printf("Zadejte ID produktu k uprave: ");
    scanf("%d", &searchID);

    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            printItemDetail(&items[i]);

            printf("\n=== Co chcete upravit? ===\n");
            printf("1. Nazev\n2. EAN\n3. Rok vyroby\n4. Sarze\n");
            printf("5. Nakupni cena\n6. Prodejni cena\n7. DPH\n8. Objem\n9. Dodavatel\n0. Zrusit\n");
            printf("Vyberte moznost: ");

            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Novy nazev: ");
                    scanf(" %39[^\n]", items[i].name);
                    break;
                case 2:
                    printf("Nove EAN: ");
                    scanf("%lld", &items[i].EAN);
                    break;
                case 3:
                    printf("Novy rok vyroby: ");
                    scanf("%d", &items[i].productionYear);
                    break;
                case 4:
                    printf("Nova sarze: ");
                    scanf(" %9[^\n]", items[i].batch);
                    break;
                case 5:
                    printf("Nova nakupni cena (bez DPH): ");
                    scanf("%lf", &items[i].supplierPrice);
                    break;
                case 6:
                    printf("Nova prodejni cena (bez DPH): ");
                    scanf("%lf", &items[i].sellPrice);
                    break;
                case 7:
                    printf("Nove DPH (%%): ");
                    scanf("%d", &items[i].DPH);
                    break;
                case 8:
                    printf("Novy objem (m3): ");
                    scanf("%lf", &items[i].cubicCapacity);
                    break;
                case 9:
                    printf("Nove ICO: ");
                    scanf("%d", &items[i].supplier.ICO);
                    printf("Novy nazev dodavatele: ");
                    scanf(" %39[^\n]", items[i].supplier.name);
                    printf("Ulice: ");
                    scanf(" %39[^\n]", items[i].supplier.address.street);
                    printf("Mesto: ");
                    scanf(" %39[^\n]", items[i].supplier.address.city);
                    printf("PSC: ");
                    scanf(" %39[^\n]", items[i].supplier.address.zip);
                    printf("Stat: ");
                    scanf(" %39[^\n]", items[i].supplier.address.state);
                    break;
                case 0:
                    printf("Uprava zrusena.\n");
                    return;
                default:
                    printf("Neplatna volba.\n");
                    return;
            }
            printf("Produkt byl aktualizovan.\n");
            return;
        }
    }
    printf("Produkt s ID %d nebyl nalezen.\n", searchID);
}
// přidání nového produktu
void addItem(struct Item **items, int *count, int *capacity) {
    double totalVolume = getTotalVolume(*items, *count);
    double availableVolume = MAX_WAREHOUSE_CAPACITY - totalVolume;

    if (availableVolume <= 0.0) {
        printf("Sklad je plny! Nelze pridat novy produkt.\n");
        return;
    }
    // zkontroluje jestli je dostupná pamet a kdyžtak ji alokuje
    if (*count >= *capacity) {
        if (!growItems(items, capacity)) return;
    }

    printf("Volna kapacita skladu: %.2f m3\n", availableVolume);

    char copyChoice;
    printf("Chcete kopirovat existujici produkt? (a/n): ");
    scanf(" %c", &copyChoice);

    struct Item *item = &(*items)[*count];
    item->ID = *count;

    if (copyChoice == 'a' || copyChoice == 'A') {
        showAllItems(*items, *count);
        int copyID;
        printf("Zadejte ID produktu ke kopirovani: ");
        scanf("%d", &copyID);

        if (copyID < 0 || copyID >= *count) {
            printf("Neplatne ID.\n");
            return;
        }
        *item = (*items)[copyID];
        item->ID = *count;
        item->quantity = 0;

        printf("Nazev: ");    scanf(" %39[^\n]", item->name);
        printf("EAN: ");      scanf("%lld", &item->EAN);
        printf("Rok vyroby: "); scanf("%d", &item->productionYear);
        printf("Sarze: ");    scanf(" %9[^\n]", item->batch);
    } else {
        printf("Nazev: ");          scanf(" %39[^\n]", item->name);
        printf("EAN: ");            scanf("%lld", &item->EAN);
        printf("Rok vyroby: ");     scanf("%d", &item->productionYear);
        printf("Sarze: ");          scanf(" %9[^\n]", item->batch);
        printf("Nakupni cena (bez DPH): ");  scanf("%lf", &item->supplierPrice);
        printf("Prodejni cena (bez DPH): "); scanf("%lf", &item->sellPrice);
        printf("DPH (%%): ");       scanf("%d", &item->DPH);
        item->quantity = 0;
        printf("Objem/ks (m3): ");  scanf("%lf", &item->cubicCapacity);
        printf("ICO dodavatele: "); scanf("%d", &item->supplier.ICO);
        printf("Nazev dodavatele: "); scanf(" %39[^\n]", item->supplier.name);
        printf("Ulice: ");          scanf(" %39[^\n]", item->supplier.address.street);
        printf("Mesto: ");          scanf(" %39[^\n]", item->supplier.address.city);
        printf("PSC: ");            scanf(" %39[^\n]", item->supplier.address.zip);
        printf("Stat: ");           scanf(" %39[^\n]", item->supplier.address.state);
    }

    (*count)++;
    printf("Produkt pridan s ID %d.\n", item->ID);
}
// funkce na naskladnění
void stockIn(struct Item *items, int count) {
    showAllItems(items, count);
    int searchID;
    printf("Zadejte ID produktu k naskladneni: ");
    scanf("%d", &searchID);

    for (int i = 0; i < count; i++) {
        if (items[i].ID == searchID) {
            printf("\nProdukt: %s\n", items[i].name);
            printf("Aktualni mnozstvi: %d ks\n", items[i].quantity);
            printf("Objem jednoho kusu: %.4f m3\n", items[i].cubicCapacity);

            double totalVolume  = getTotalVolume(items, count);
            double availableVol = MAX_WAREHOUSE_CAPACITY - totalVolume;
            printf("Volna kapacita skladu: %.2f m3\n", availableVol);

            if (availableVol <= 0.0) {
                printf("Sklad je plny!\n");
                return;
            }

            int maxPieces = (int)(availableVol / items[i].cubicCapacity);
            printf("Maximalne lze naskladnit: %d ks\n", maxPieces);

            int addQuantity;
            printf("Kolik kusu chcete naskladnit: ");
            scanf("%d", &addQuantity);

            if (addQuantity < 0) {
                printf("Mnozstvi nemuze byt zaporne.\n");
            } else if (addQuantity > maxPieces) {
                printf("Nedostatek mista! Maximum: %d ks.\n", maxPieces);
            } else {
                items[i].quantity += addQuantity;
                printf("Naskladneno %d ks. Nove mnozstvi: %d ks.\n",
                       addQuantity, items[i].quantity);
            }
            return;
        }
    }
    printf("Produkt s ID %d nebyl nalezen.\n", searchID);
}

// spuštení skladu
int main(void) {
    int capacity = INITIAL_CAPACITY;
    int count    = 0;

    struct Item *items = malloc(capacity * sizeof(struct Item));
    if (!items) {
        printf("Chyba: malloc selhal.\n");
        return 1;
    }

    // vytvoření struktur

    items[0].ID = 0; items[0].EAN = 8594002464874LL;
        items[0].productionYear = 2025;
        strcpy(items[0].batch, "112/25");
        strcpy(items[0].name, "Solaris");
        items[0].supplierPrice = 35.54; // bez DPH
        items[0].sellPrice     = 148.76; // bez DPH (180 Kc / 1.21)
        items[0].DPH = 21; items[0].quantity = 120; items[0].cubicCapacity = 0.001;
        items[0].supplier.ICO = 12345678;
        strcpy(items[0].supplier.name, "Vino Hruska s.r.o.");
        strcpy(items[0].supplier.address.street, "Blatnicka 143");
        strcpy(items[0].supplier.address.city, "Blatnicka");
        strcpy(items[0].supplier.address.state, "Ceska republika");
        strcpy(items[0].supplier.address.zip, "696 71");

        items[1].ID = 1; items[1].EAN = 8594002464881LL;
        items[1].productionYear = 2024;
        strcpy(items[1].batch, "108/24");
        strcpy(items[1].name, "Merzling - keg 50l");
        items[1].supplierPrice = 1652.89; items[1].sellPrice = 6611.57;
        items[1].DPH = 21; items[1].quantity = 15; items[1].cubicCapacity = 0.06;
        items[1].supplier.ICO = 87654321;
        strcpy(items[1].supplier.name, "Vino Hruska s.r.o.");
        strcpy(items[1].supplier.address.street, "Blatnicka 143");
        strcpy(items[1].supplier.address.city, "Blatnicka");
        strcpy(items[1].supplier.address.state, "Ceska republika");
        strcpy(items[1].supplier.address.zip, "696 71");

        items[2].ID = 2; items[2].EAN = 8594002464898LL;
        items[2].productionYear = 2025;
        strcpy(items[2].batch, "113/25");
        strcpy(items[2].name, "Devin");
        items[2].supplierPrice = 28.10; items[2].sellPrice = 156.20;
        items[2].DPH = 21; items[2].quantity = 8; items[2].cubicCapacity = 0.001;
        items[2].supplier.ICO = 11223344;
        strcpy(items[2].supplier.name, "Vino Hruska s.r.o.");
        strcpy(items[2].supplier.address.street, "Blatnicka 143");
        strcpy(items[2].supplier.address.city, "Blatnicka");
        strcpy(items[2].supplier.address.state, "Ceska republika");
        strcpy(items[2].supplier.address.zip, "696 71");

    count = 3;

    char choice;
    do {
        showMenu();
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                showAllItems(items, count);
                break;
            case '2':
                searchItems(items, count);
                break;
            case '3':
                searchItemsByPrice(items, count);
                break;
            case '4':
                showItemDetail(items, count);
                break;
            case '5':
                removeItem(&items, &count, &capacity);
                break;
            case '6':
                editItem(items, count);
                break;
            case '7':
                addItem(&items, &count, &capacity);
                break;
            case '8':
                showWarehouseStatus(items, count);
                break;
            case '9':
                stockIn(items, count);
                break;
            case 'X':
            case 'x':
                printf("\nUkoncuji aplikaci...\n");
                break;
            default:
                printf("\nNeplatna volba.\n");
        }
    } while (choice != 'X' && choice != 'x');

    free(items);
    return 0;
}