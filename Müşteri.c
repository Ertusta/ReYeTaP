#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include <dirent.h>

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
    char isim[25];
    char orderTime[20];
    char name[50];
    float price;
    int preparationTime;

        printf("isim giriniz:");
        scanf(" %s",isim);

    do{
        printf("siparis vermek icin '1' \nmevcut ve eski siparisleri gormek icin '2' \ncikmak icin '3' tuslayiniz:");
        scanf(" %c",&istek);

        loadMenu(menu, &menuSize, menuFilename);


    if(istek=='1')
    {


       char  *id=getId();


        do{
        showMenu(menu, &menuSize);
        siparisOlustur(id,isim);


    printf("yemek secmeye devam etmek icin '1' siparisi yollamak icin '2' tiklayiniz:");
    scanf(" %c",&komut);

        }while(komut!= '2' );
    }
    else if(istek=='2')
    {
        tumSiparisleriGoster(isim);
    }

    }while(istek!='3');
return 76;
}
