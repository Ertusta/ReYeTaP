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


void OrderList(Order orders[], int *orderSize, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Dosya açılamadı: %s\n", filename);
        return;
    }

    char line[100];
    *orderSize = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%s %s %f %s %d %s %s %d",
                   orders[*orderSize].orderId,
                   orders[*orderSize].foodName,
                   &orders[*orderSize].price,
                   orders[*orderSize].orderTime,
                   &orders[*orderSize].preparationTime,
                   orders[*orderSize].customer,
                   orders[*orderSize].chef,
                   &orders[*orderSize].state) == 8) {
            (*orderSize)++;
        }
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
        scanf(" %s",isim);


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




    do{

        printf("siparis vermek icin 'q' \nmevcut ve eski siparisleri gormek icin 'l' \ncikmak icin 't' tuslayiniz:");
        scanf(" %c",&istek);





    if(istek=='q')
    {





        loadMenu(menu, &menuSize, menuFilename);

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
    strftime(orderTime, sizeof(orderTime), "%Y-%m-%d-%H:%M:%S", local_time);


    char chef[10]="Sef1";
    int state=0;


    FILE *file = fopen("Siparisler.txt", "a");

    if (file == NULL)
    {
        printf("Dosya açma hatası!");

    }




    fprintf(file, "%s %s %.2f %s %d %s %s %d\n",id,name,price,orderTime,preparationTime,isim,chef,state);

    fclose(file);






    }

    if(istek=='l')
    {

    OrderList(orders,&orderSize,orderFilename);



             printf("Onaylanmayan siparisler:\n");

            for (int i = 0; i <orderSize; i++)
            {






                if( orders[i].state==0&&strcmp(orders[i].orderId,id) == 0)
                {



                   printf("%s %s %f %s %d %s %s %d\n",
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

                if( orders[i].state==1&&strcmp(orders[i].orderId,id) == 0)
                {



                   printf("%s %s %f %s %d %s %s %d\n",
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

                if( orders[i].state==2&&strcmp(orders[i].orderId,id) == 0)
                {



                   printf("%s %s %f %s %d %s %s %d\n",
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






}
