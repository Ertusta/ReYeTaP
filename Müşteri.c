#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100


void loadMenu(MenuItem menu[], int *menuSize, const char *filename)
    {
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[100];
    *menuSize = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%s %f %d %s",
                   menu[*menuSize].name,
                   &menu[*menuSize].price,
                   &menu[*menuSize].preparationTime,
                   menu[*menuSize].availability) == 4)
            {
            (*menuSize)++;
            }
    }

    fclose(file);
    }


void OrderList(Order orders[], int *orderSize, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[100];
    *orderSize = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%s %s %f %s %d %s %s %d",
                   orders[*orderSize].orderId,
                   orders[*orderSize].foodName,
                   &orders[*orderSize].price,
                   orders[*orderSize].orderTime,
                   &orders[*orderSize].preparationTime,
                   orders[*orderSize].customer,
                   orders[*orderSize].chef,
                   &orders[*orderSize].state) == 8)
        {
            (*orderSize)++;
        }
    }

    // Dosyanın sonuna ulaşılıp ulaşılmadığını kontrol et
    if (!feof(file))
    {
        printf("Hata: Dosya sonuna ulasilamadi.\n");
    }

    fclose(file);
}



int main() {

    MenuItem menu[MAX_MENU_ITEMS];
    Order orders[MAX_ORDERS];
    int menuSize = 0, orderSize = 0;
    char menuFilename[] = "yemeklistesi.txt";
    char orderFilename[] = "Siparisler.txt";
    int satir_sayisi = 0;




    printf("Kullanici Id girmek icin 'q' \nyeni hesap icin 'l' tuslayiniz:");

    char istek;
    char id[25];
    scanf("%c",&istek);
    char isim[25];
    char orderTime[20];
    char kod[20];

    if(istek=='l')
    {


        printf("isim giriniz:");
        scanf("%s",isim);


        time_t current_time;
        struct tm *local_time;
        current_time = time(NULL);
        local_time = localtime(&current_time);

        strftime(kod, sizeof(kod), "%Y%m%d%H%M%S", local_time);

        sprintf(id, "%s_%s", isim, kod);


    }
    else if(istek=='q')
    {
        printf("id giriniz:");
        scanf("%s",&id);
    }

    loadMenu(menu, &menuSize, menuFilename);
    OrderList(orders, &orderSize, orderFilename);

    printf("%d",orderSize);


    printf("Yemek Seciniz:\n");

            for (int i = 0; i < menuSize; i++)
            {


                if (strcmp(menu[i].availability, "Evet") == 0)
                {
                    printf("%s: %.2f TL (%d dakika) - %s\n",
                       menu[i].name,
                       menu[i].price,
                       menu[i].preparationTime,
                       menu[i].availability);
                }

            }



    char name[50];
    float price;

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


    char chef[10]="Sef1";
    int state=0;


    FILE *file = fopen("Siparisler.txt", "w"); // "w" modunda açarak mevcut verileri üzerine yaz

    if (file == NULL)
    {
        printf("Dosya açma hatası!");
        return 1; // Programı sonlandır
    }




    fprintf(file, "%s %s %.2f %s %d %s %s %d\n",id,name,price,orderTime,preparationTime,isim,chef,state);

    fclose(file);


}
