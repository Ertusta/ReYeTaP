#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50

int main()
{
    MenuItem menu[MAX_MENU_ITEMS];        // Menü öğeleri için dizi
    int menuSize = 0;                     // Menüdeki öğe sayısı
    char filename[] = "Yemeklistesi.txt"; // Dosya adı
    loadMenu(menu, &menuSize, filename);
    char date[11];
    char secenek;
    char choice;
    char directory[] = "arsiv";
    int year, month;
    float dailyRevenue, monthlyRevenue, periodRevenue;
    char startDate[11], endDate[11];
    int yeniAsciSayisi;

    do
    {
        printf("Yemek Listesi Ayarlari icin '1'\n");
        printf("Yemek Onaylamak Icin icin '2'\n");
        printf("Arsiv Islemleri icin '3'\n");
        printf("Analiz Islemleri icin '4'\n");
        printf("Asci Sayisini Guncellemek icin '5'.\n");
        printf("Cikis icin 'q'\n");
        printf("Seciminiz: ");
        scanf(" %c", &choice);

        switch (choice)
        {
        case '1':
            printf("Yemek Listesi Ayarlari bolumu.\n");
            printf("Yemek eklemek icin 'e'\nyemek listesini gormek icin 'l'\nyemek bilgisi guncellemek icin 'g'\nyemek silmek icin 's' basiniz:\n");
            scanf(" %c", &secenek);
            if (secenek == 'e' || secenek == 'E')
            {
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
            }
            else if (secenek == 'l' || secenek == 'L')
            {
                printf("Yemek Listesi:\n");
                for (int i = 0; i < menuSize; i++)
                {
                    printf("%s: %.2f TL (%d dakika) - %s\n",
                           menu[i].name,
                           menu[i].price,
                           menu[i].preparationTime,
                           menu[i].availability);
                }
            }
            else if (secenek == 'g' || secenek == 'G')
            {
                updateMenuItem(menu, &menuSize, filename);
            }
            else if (secenek == 's' || secenek == 'S')
            {
                deleteMenuItem(menu, &menuSize, filename);
            }
            break;
        case '2':
            printf("Yemek Onaylama bolumu.\n");
            updateOrderStatus("Siparisler.txt");
            break;
        case '3':
            printf("Arsiv Islemleri bolumu.\n");
            printf("Gunluk rapor kaydetmek icin 'u'\nGunluk rapor goruntulemek icin 'a' giriniz:\n");
            scanf(" %c", &secenek);
            if (secenek == 'u' || secenek == 'U')
            {
                printf("Lutfen arsivlemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
                scanf("%10s", date);
                archiveDailyOrders("Siparisler.txt", date);
            }
            else if (secenek == 'a' || secenek == 'A')
            {
                printf("Lutfen goruntulemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
                scanf("%10s", date);
                viewDailyReport(date);
            }
            break;
        case '4':
            printf("Analiz Islemleri bolumu.\n");
            printf("Lutfen bir secim yapiniz:\n");
            printf("1 - Gunluk Kazanc\n");
            printf("2 - Aylik Kazanc\n");
            printf("3 - Iki Donem Arasindaki Kazanc\n");
            printf("4 - En Cok Tuketim.\n");
            printf("Seciminiz: ");
            scanf("%d", &choice);
            switch (choice)
            {
            case 1:
                // Kullanıcıdan tarih bilgisi isteniyor
                printf("Lutfen gunluk kazanci hesaplamak icin tarihi YYYY-MM-DD formatinda giriniz: ");
                scanf("%10s", date);
                // Günlük kazancı hesaplayıp ekrana yazdırıyoruz
                printf("Gunluk Kazanc: %.2f\n", calculateDailyRevenue(date, directory));
                break;
            case 2:
                // Kullanıcıdan yıl ve ay bilgisi isteniyor
                printf("Lutfen aylik kazanci hesaplamak icin yili ve ayi giriniz.\n");
                printf("Yil: ");
                scanf("%d", &year);
                printf("Ay: ");
                scanf("%d", &month);
                // Aylık kazancı hesaplayıp ekrana yazdırıyoruz
                printf("Aylik Kazanc: %.2f\n", calculateMonthlyRevenue(year, month, directory));
                break;
            case 3:
                // Kullanıcıdan dönem başlangıç ve bitiş tarihleri isteniyor
                printf("Lutfen donem kazancini hesaplamak icin baslangic tarihini YYYY-MM-DD formatinda giriniz: ");
                scanf("%10s", startDate);
                printf("Lutfen donem kazancini hesaplamak icin bitis tarihini YYYY-MM-DD formatinda giriniz: ");
                scanf("%10s", endDate);

                // Dönem kazancı hesaplanıyor ve sonuç ekrana yazdırılıyor
                periodRevenue = calculatePeriodRevenue(startDate, endDate, directory);
                if (periodRevenue >= 0)
                {
                    printf("Donem Kazanci: %.2f\n", periodRevenue);
                }
                break;
            case 4:
                findMostConsumedFood("arsiv");
                findMostProfitableDay("arsiv");
                findMostFrequentCustomer("arsiv");
                break;
            default:
                printf("Gecersiz secim yaptiniz.\n");
            }

        case '5':
            printf("Yeni asci sayisini giriniz: ");
            scanf("%d", &yeniAsciSayisi);

            // Fonksiyonu çağır
            if (updateChefCount("Asci.txt", yeniAsciSayisi) == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            printf("Asci sayisi başariyla guncellendi.\n");
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
