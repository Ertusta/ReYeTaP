
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

// Asci yap�s� tan�mlamas�
struct Asci {
    int hour;
    int minute;
};

// En erken biten asci i�ini bulan fonksiyon
int findEarliestAsci(struct Asci asci[], int asciNumber) {
    // En erken biti� saati ve asci i�inin indeksi
    int earliestFinishHour = asci[0].hour;
    int earliestFinishMinute = asci[0].minute;
    int earliestAsciIndex = 0;

    // Asci i�lerinin biti� saatlerini kontrol et
    for(int i = 1; i < asciNumber; i++) {
        if (asci[i].hour < earliestFinishHour || (asci[i].hour == earliestFinishHour && asci[i].minute < earliestFinishMinute)) {
            earliestFinishHour = asci[i].hour;
            earliestFinishMinute = asci[i].minute;
            earliestAsciIndex = i;
        }
    }

    return earliestAsciIndex;
}

// Belirli bir asci i�ini g�ncellemek i�in fonksiyon
void updateAsci(struct Asci asci[], int index, int newHour, int newMinute) {

    asci[index].hour = newHour;
    asci[index].minute = newMinute;
}
//suanki zaman� alan fonksiyon
void getCurrentTime(int *hour, int *minute) {
    time_t currentTime;
    struct tm *localTime;

    // Anl�k zaman� al
    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    // Saati ve dakikay� d�nd�r
    *hour = localTime->tm_hour;
    *minute = localTime->tm_min;
}

// A���n�n �al��ma zaman�n� kontrol eden ve g�ncelleyen fonksiyon
void updateAsciTime(struct Asci *asci) {
    int currentHour, currentMinute;

    // Anl�k saat ve dakikay� al
    getCurrentTime(&currentHour, &currentMinute);

    // E�er a���n�n �al��ma saati �u andan gerideyse, a���n�n zaman�n� g�ncelle
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
    //ilk sat�r asci sayisi
    fscanf(file, "%d", &asciNumber);
    for(int i = 0; i < asciNumber; i++)
    {
        fscanf(file, "%d %d", &asci[i].hour, &asci[i].minute); // Her bir asci eleman�na saat ve dakika al
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


            // saat dakika haz�rlamas�
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

                //saat ve dakika birle�tirilir
                sprintf(birlestirilmisZaman, "%02d:%02d",asci[earliestAsciIndex].hour,asci[earliestAsciIndex].minute);

                //tekrar atanmas�n diye
                orders[i].state=2;

                //atanan asci ve saatleri g�ster
                printf("%s:\n",orders[i].foodName);
                printf("asci%d: %s\n\n",earliestAsciIndex ,birlestirilmisZaman);
        }
    }




    //buraya da se�ilen siparise atanan asc� , saatin  state in dosyaya atanmas� kal�yor

    return 0;
}
