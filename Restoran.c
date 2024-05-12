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
        printf("Yemek eklemek icin 'e'\nyemek listesini gormek icin 'l'\nyemek bilgisi guncellemek icin 'g'\nyemek silmek icin 's'\nYemek onaylamak icin 'o'\nGunluk rapor kaydetmek icin 'u'\nGunluk rapor goruntulemek icin 'a'\ncikis yapmak icin 'q' girin: ");
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
        }else if (choice == 'g' || choice == 'G') {
            updateMenuItem(menu, &menuSize, filename);
        }else if (choice == 's' || choice == 'S') {
            deleteMenuItem(menu, &menuSize, filename);
        }else if (choice == 'o' || choice == 'O') {
            updateOrderStatus("Siparisler.txt");
        }else if (choice == 'u' || choice == 'U'){
            printf("Lutfen arsivlemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
            scanf("%10s", date);
            archiveDailyOrders("Siparisler.txt", date);
        }else if (choice == 'a' || choice == 'A'){
            printf("Lutfen goruntulemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
            scanf("%10s", date);
            viewDailyReport(date);
        }
    } while (choice != 'q' && choice != 'Q');

    printf("Programdan cikis yapildi.\n");
    return 0;
}
