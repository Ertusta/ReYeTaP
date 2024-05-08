#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100


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

// Yemek bilgisi güncelleme işlemi
void updateMenuItem(MenuItem menu[], int *menuSize, const char *filename) {
    char name[50];
    printf("Guncellemek istediginiz yemegin adini girin: ");
    scanf("%49s", name);

    int found = 0;
    for (int i = 0; i < *menuSize; i++) {
        if (strcmp(menu[i].name, name) == 0) {
            printf("Yeni fiyat: ");
            scanf("%f", &menu[i].price);
            printf("Yeni hazirlik suresi (dakika): ");
            scanf("%d", &menu[i].preparationTime);
            printf("Yeni durum (Evet/Hayir): ");
            scanf("%19s", menu[i].availability);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Yemek bulunamadi.\n");
        return;
    }

    saveMenu(menu, *menuSize, filename);
    printf("Yemek bilgisi guncellendi.\n");
}

   // Yemek silme işlemi
void deleteMenuItem(MenuItem menu[], int *menuSize, const char *filename) {
    char name[50];
    printf("Silmek istediginiz yemegin adini girin: ");
    scanf("%49s", name);

    int found = 0;
    for (int i = 0; i < *menuSize; i++) {
        if (strcmp(menu[i].name, name) == 0) {
            for (int j = i; j < *menuSize - 1; j++) {
                menu[j] = menu[j + 1];
            }
            (*menuSize)--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Yemek bulunamadi.\n");
        return;
    }

    saveMenu(menu, *menuSize, filename);
    printf("Yemek silindi.\n");
}






int main() {
    MenuItem menu[MAX_MENU_ITEMS];
    Order orders[MAX_ORDERS];
    int menuSize = 0, orderSize = 0;
    char menuFilename[] = "yemeklistesi.txt";
    char orderFilename[] = "Siparisler.txt";
    int counter=0;




    printf("Kullanici Id girmek icin 'q' \nyeni hesap icin 'l' tuslayiniz:");

    char istek;
    char id[25];
    scanf("%c",&istek);
    char isim[25];


    if(istek=='l')
    {


        printf("isim giriniz:");
        scanf("%s",isim);

        sprintf(id, "%s_%d", isim, counter);
        counter++;
    }
    else if(istek=='q')
    {
        printf("id giriniz:");
        scanf("%s",&id);
    }

    loadMenu(menu, &menuSize, menuFilename);

    printf("Yemek Seciniz:\n");
            for (int i = 0; i < menuSize; i++) {
                printf("%s: %.2f TL (%d dakika) - %s\n",
                       menu[i].name,
                       menu[i].price,
                       menu[i].preparationTime,
                       menu[i].availability);
            }



    char name[50];
    float price;
    char orderTime[20];
     int preparationTime;

    scanf("%49s", name);

    int found = 0;
    for (int i = 0; i < 2; i++)
        {
        if (strcmp(menu[i].name, name) == 0)
            {
                price=menu[i].price;
            preparationTime=menu[i].preparationTime;

            break;
            }
        }




     time_t current_time;
    struct tm *local_time;
    current_time = time(NULL);
    local_time = localtime(&current_time);
    strftime(orderTime, sizeof(orderTime), "%Y-%m-%d %H:%M:%S", local_time);


    char chef='a';

    printf("hello");
    FILE *file = fopen(orderFilename, "w"); // "w" modunda açarak mevcut verileri üzerine yaz



   strcpy(orders->orderId, id);
   strcpy(orders->foodName, name);

   strcpy(orders->orderTime, orderTime);
   strcpy(orders->preparationTime, preparationTime);
   strcpy(orders->customer, isim);
   strcpy(orders->chef, chef);

        fprintf(file, "%s %s %.2f %s %d %s %s\n",
                orders->orderId,
                orders->foodName,
                price,
                orders->orderTime,
                orders->preparationTime,
                orders->customer,
                orders->chef);

    fclose(file);


}
