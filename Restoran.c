#include <stdio.h>
#include <string.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50



// Menüyü dosyadan yükle
void loadMenu(MenuItem menu[], int *menuSize, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[100];
    *menuSize = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%s %f %d %s", 
                   menu[*menuSize].name, 
                   &menu[*menuSize].price, 
                   &menu[*menuSize].preparationTime, 
                   menu[*menuSize].availability) == 4) {
            (*menuSize)++;
        }
    }

    fclose(file);
}

// Menüyü dosyaya kaydet
void saveMenu(MenuItem menu[], int menuSize, const char *filename) {
    FILE *file = fopen(filename, "w"); // "w" modunda açarak mevcut verileri üzerine yaz
    if (!file) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }
    for (int i = 0; i < menuSize; i++) {
        fprintf(file, "%s %.2f %d %s\n", 
                menu[i].name, 
                menu[i].price, 
                menu[i].preparationTime, 
                menu[i].availability);
    }
    fclose(file);
}

int main() {
    MenuItem menu[MAX_MENU_ITEMS]; // Menü öğeleri için dizi
    int menuSize = 0; // Menüdeki öğe sayısı
    char filename[] = "Yemeklistesi.txt"; // Dosya adı
    loadMenu(menu, &menuSize, filename);

    char choice;
    do {
        printf("Yemek eklemek icin 'e', yemek listesini gormek icin 'l', cikis yapmak icin 'q' girin: ");
        scanf(" %c", &choice);

        if (choice == 'e' || choice == 'E') {
            printf("Eklemek istediginiz yemegi yazin: ");
            scanf("%49s", menu[menuSize].name);
            printf("Fiyat: ");
            scanf("%f", &menu[menuSize].price);
            printf("Hazirlik suresi (dakika): ");
            scanf("%d", &menu[menuSize].preparationTime);
            printf("Mevcut mu? (Evet/Hayir): ");
            scanf("%19s", menu[menuSize].availability);
            menuSize++;
            saveMenu(menu, menuSize, filename);
            printf("Yemek eklendi ve %s dosyasina kaydedildi.\n", filename);
        } else if (choice == 'l' || choice == 'L') {
            printf("Yemek Listesi:\n");
            for (int i = 0; i < menuSize; i++) {
                printf("%s: %.2f TL (%d dakika) - %s\n", 
                       menu[i].name, 
                       menu[i].price, 
                       menu[i].preparationTime, 
                       menu[i].availability);
            }
        }
    } while (choice != 'q' && choice != 'Q');

    printf("Programdan cikis yapildi.\n");
    return 0;
}
