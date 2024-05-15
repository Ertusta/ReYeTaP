#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100


int main() {

    MenuItem menu[MAX_MENU_ITEMS];
    Order orders[MAX_ORDERS];
    int menuSize = 0, orderSize = 0;
    char menuFilename[] = "yemeklistesi.txt";
    char orderFilename[] = "Siparisler.txt";


    char istek;
    char komut;
    char id[50];
    char isim[25];
    char orderTime[20];
    char kod[20];

        printf("isim giriniz:");
        scanf(" %s",isim);


    do{
        printf("siparis vermek icin 'q' \nmevcut ve eski siparisleri gormek icin 'l' \ncikmak icin 't' tuslayiniz:");
        scanf(" %c",&istek);
        loadMenu(menu, &menuSize, menuFilename);


    if(istek=='q')
    {
        time_t current_time;
        struct tm *local_time;
        current_time = time(NULL);
        local_time = localtime(&current_time);
        strftime(kod, sizeof(kod), "%Y%m%d%H%M%S", local_time);
        sprintf(id, "Sip_%s", kod);
        loadMenu(menu, &menuSize, menuFilename);

        do{
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

    scanf(" %49s", name);

    int found = 0;
    for (int i = 0; i < menuSize; i++)
        {
        if (strcmp(menu[i].name, name) == 0&&strcmp(menu[i].availability, "Evet") == 0)
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
    strftime(orderTime, sizeof(orderTime), "%Y-%m-%d/%H:%M:%S", local_time);

    char chef[10]="0";
    int state=0;

    FILE *file = fopen("Siparisler.txt", "a");

    if (file == NULL)
    {
        printf("Dosya açma hatası!");

    }

    char time[20]="00:00";

    fprintf(file, "%s %s %.2f %s %d %s %s %d %s\n",id,name,price,orderTime,preparationTime,isim,chef,state,time);
    fclose(file);

    printf("yemek secmeye devam etmek icin l siparisi yollamak icin q tiklayiniz:");
    scanf(" %c",&komut);

        }while(komut!= 'q' );
    }
    else if(istek=='l')
    {
    OrderList(orders,&orderSize,orderFilename);
             printf("Onaylanmayan siparisler:\n");

            for (int i = 0; i <orderSize; i++)
            {

                if( orders[i].state==0&&strcmp(orders[i].customer,isim) == 0)
                {

                   printf("%s %s %.2f %s %d %s %s %d\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime,
                   orders[i].preparationTime,
                   orders[i].customer,
                   orders[i].chef,
                   orders[i].state);
                }
            }

            printf("Onaylanan siparisler:\n");

            for (int i = 1; i <orderSize; i++)
            {
                if( orders[i].state==1&&strcmp(orders[i].customer,isim) == 0)
                {
                   printf("%s %s %.2f %s %d %s %s %d\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime,
                   orders[i].preparationTime,
                   orders[i].customer,
                   orders[i].chef,
                   orders[i].state);
                }
            }

            printf("Eski siparisler:\n");

            for (int i = 0; i <orderSize; i++)
            {

                if( orders[i].state==2&&strcmp(orders[i].customer,isim) == 0)
                {
                   printf("%s %s %.2f %s %d %s %s %d\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime,
                   orders[i].preparationTime,
                   orders[i].customer,
                   orders[i].chef,
                   orders[i].state);
                }
            }
        }
    }while(istek!='t');
return 76;
}
