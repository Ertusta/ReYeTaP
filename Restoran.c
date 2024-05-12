#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50


int main() {
    MenuItem menu[MAX_MENU_ITEMS]; // Menü öğeleri için dizi
    int menuSize = 0; // Menüdeki öğe sayısı
    char filename[] = "Yemeklistesi.txt"; // Dosya adı
    loadMenu(menu, &menuSize, filename);
    char date[11];

    char choice;
    do {
        printf("Yemek Listesi Ayarlari icin '1'\n");
        printf("Yemek Onaylamak Icin icin '2'\n");
        printf("Arsiv Islemleri icin '3'\n");
        printf("Analiz Islemleri icin '4'\n");
        printf("Cikis icin 'q'\n");
        printf("Seciminiz: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("Yemek Listesi Ayarlari bolumu.\n");
                foodListSettings();
                break;
            case '2':
                printf("Yemek Onaylamak Icin bolumu.\n");
                confirmOrder();
                break;
            case '3':
                printf("Arsiv Islemleri bolumu.\n");
                archiveOperations();
                break;
            case '4':
                printf("Analiz Islemleri bolumu.\n");
                analysisOperations();
                break;
            case 'q':
            case 'Q':
                printf("Cikis yapildi.\n");
                break;
            default:
                printf("Gecersiz secim. Lutfen tekrar deneyin.\n");
        }
    } while (choice != 'q' && choice != 'Q');

    return 0;
}
