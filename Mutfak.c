
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

// Asci yapýsý tanýmlamasý
struct Asci {
    int hour;
    int minute;
};

// En erken biten asci iþini bulan fonksiyon
int findEarliestAsci(struct Asci asci[], int asciNumber) {
    // En erken bitiþ saati ve asci iþinin indeksi
    int earliestFinishHour = asci[0].hour;
    int earliestFinishMinute = asci[0].minute;
    int earliestAsciIndex = 0;

    // Asci iþlerinin bitiþ saatlerini kontrol et
    for(int i = 1; i < asciNumber; i++) {
        if (asci[i].hour < earliestFinishHour || (asci[i].hour == earliestFinishHour && asci[i].minute < earliestFinishMinute)) {
            earliestFinishHour = asci[i].hour;
            earliestFinishMinute = asci[i].minute;
            earliestAsciIndex = i;
        }
    }

    return earliestAsciIndex;
}

// Belirli bir asci iþini güncellemek için fonksiyon
void updateAsci(struct Asci asci[], int index, int newHour, int newMinute) {

    asci[index].hour = newHour;
    asci[index].minute = newMinute;
}
//suanki zamaný alan fonksiyon
void getCurrentTime(int *hour, int *minute) {
    time_t currentTime;
    struct tm *localTime;

    // Anlýk zamaný al
    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    // Saati ve dakikayý döndür
    *hour = localTime->tm_hour;
    *minute = localTime->tm_min;
}

// Aþçýnýn çalýþma zamanýný kontrol eden ve güncelleyen fonksiyon
void updateAsciTime(struct Asci *asci) {
    int currentHour, currentMinute;

    // Anlýk saat ve dakikayý al
    getCurrentTime(&currentHour, &currentMinute);

    // Eðer aþçýnýn çalýþma saati þu andan gerideyse, aþçýnýn zamanýný güncelle
    if (asci->hour < currentHour || (asci->hour == currentHour && asci->minute < currentMinute)) {
        asci->hour = currentHour;
        asci->minute = currentMinute;
    }
}

int main() {
    int asciNumber;
    struct Asci asci[asciNumber];
    int currentHour, currentMinute;
    Order orders[MAX_ORDERS];
    int orderSize = 0;
     int earliestAsciIndex;
    char orderFilename[] = "Siparisler.txt";
    char birlestirilmisZaman[6];

    FILE *file = fopen("Asci.txt", "r");
    //ilk satýr asci sayisi
    fscanf(file, "%d", &asciNumber);
    for(int i = 0; i < asciNumber; i++)
    {
        fscanf(file, "%d %d", &asci[i].hour, &asci[i].minute); // Her bir asci elemanýna saat ve dakika al
    }
    fclose(file);


    //isi bitenleri kontrol et
    for(int i = 0; i < asciNumber; i++)
    {
        updateAsciTime(&asci[i]);
    }

    //siparisleri oku
    OrderList(orders,&orderSize,orderFilename);


    //sadece state 1 olan siparisleri al
    for(int i = 0; i < orderSize; i++)
    {
        if(orders[i].state==1)
        {

            //isi en erken bitecek asciyi sec
             earliestAsciIndex = findEarliestAsci(asci, asciNumber);


            // saat dakika hazýrlamasý
             int time1 =orders[i].preparationTime +asci[earliestAsciIndex].minute;
             if(time1>=60)
                {
                    asci[earliestAsciIndex].hour++;
                    time1-=60;
                    asci[earliestAsciIndex].minute=time1;
                }
                else
                {
                    asci[earliestAsciIndex].minute=time1;
                }

                //saat ve dakika birleþtirilir
                sprintf(birlestirilmisZaman, "%02d:%02d",asci[earliestAsciIndex].hour,asci[earliestAsciIndex].minute);

                //tekrar atanmasýn diye
                orders[i].state=2;

                //atanan asci ve saatleri göster
                printf("%s:\n",orders[i].foodName);
                printf("asci%d: %s\n\n",earliestAsciIndex ,birlestirilmisZaman);
        }
    }




    //buraya da seçilen siparise atanan ascý , saatin  state in dosyaya atanmasý kalýyor

    return 0;
}
