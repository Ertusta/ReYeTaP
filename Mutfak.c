#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

int main()
{
    int asciNumber;
    struct Asci asci[asciNumber];
    int currentHour, currentMinute;
    Order orders[MAX_ORDERS];
    int orderSize = 0;
    int earliestAsciIndex;
    char orderFilename[] = "Siparisler.txt";
    char birlestirilmisZaman[6];

    FILE *file = fopen("Asci.txt", "r");
    // ilk satýr asci sayisi
    fscanf(file, "%d", &asciNumber);
    for (int i = 0; i < asciNumber; i++)
    {
        fscanf(file, "%d %d", &asci[i].hour, &asci[i].minute); // Her bir asci elemanýna saat ve dakika al
    }
    fclose(file);

    // isi bitenleri kontrol et
    for (int i = 0; i < asciNumber; i++)
    {
        updateAsciTime(&asci[i]);
    }

    // siparisleri oku
    OrderList(orders, &orderSize, orderFilename);

    // sadece state 1 olan siparisleri al
    for (int i = 0; i < orderSize; i++)
    {
        if (orders[i].state == 1)
        {

            // isi en erken bitecek asciyi sec
            earliestAsciIndex = findEarliestAsci(asci, asciNumber);

            // saat dakika hazýrlamasý
            int time1 = orders[i].preparationTime + asci[earliestAsciIndex].minute;
            if (time1 >= 60)
            {
                asci[earliestAsciIndex].hour++;
                time1 -= 60;
                asci[earliestAsciIndex].minute = time1;
            }
            else
            {
                asci[earliestAsciIndex].minute = time1;
            }

            // saat ve dakika birleþtirilir
            sprintf(birlestirilmisZaman, "%02d:%02d", asci[earliestAsciIndex].hour, asci[earliestAsciIndex].minute);

            // tekrar atanmasin diye
            orders[i].state = 2;

            strcpy(orders[i].finalTime, birlestirilmisZaman);
            sprintf(orders[i].chef, "asci%d", earliestAsciIndex + 1);

            // atanan asci ve saatleri göster
            printf("%s:\n", orders[i].foodName);
            printf("asci%d: %s\n\n", earliestAsciIndex, birlestirilmisZaman);
        }
    }
    file = fopen(orderFilename, "w");
    if (file == NULL)
    {
        printf("Dosya açılamadı.\n");
    }

    for (int i = 0; i < orderSize; i++)
    {

        fprintf(file, "%s %s %.2f %s %d %s %s %d %s\n",
                orders[i].orderId,
                orders[i].foodName,
                orders[i].price,
                orders[i].orderTime,
                orders[i].preparationTime,
                orders[i].customer,
                orders[i].chef,
                orders[i].state,
                orders[i].finalTime);
    }

    yazdir(asci, asciNumber, "Asci.txt");
    printf("Veriler Asci.txt dosyasina yazildi.\n");

    return 0;
}
