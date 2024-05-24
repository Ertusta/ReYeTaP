#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <dirent.h>
#define MAX_PATH_LEN 256
#define MAX_FILE_LEN 256
#define MAX_LINE_LEN 1024
#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

typedef struct {
    char name[50];
    float price;
    int preparationTime;
    char availability[20];
} MenuItem;


// Asci yapisi tanimlamasi
struct Asci {
    int hour;
    int minute;
};

typedef struct {
    char orderId[50];
    char foodName[50];
    float price;
    char orderTime[20];
    int preparationTime;
    char customer[20];
    char chef[20];
    int state;
    char finalTime[20];
} Order;

// Menüyü dosyadan yükle
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

 //Siparişi Onaylama
void approveOrder(Order *order) {
    order->state = 1; // Siparişi onayla
}
void updateOrderStatus(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Gecici dosya acilamadi");
        exit(EXIT_FAILURE);
    }

    Order order;
    int isOrderApproved = 0;

    // Dosyadan siparişleri oku ve onaylanmamış olanları bul
    while (fscanf(file, "%s %s %f %s %d %s %s %d %s\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state, order.finalTime) != EOF) {
        if (order.state == 0 && !isOrderApproved) { // İlk onaylanmamış sipariş
            approveOrder(&order); // Siparişi onayla
            isOrderApproved = 1;
        }
        // Siparişi geçici dosyaya yaz
        fprintf(tempFile, "%s %s %.2f %s %d %s %s %d %s\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state, order.finalTime);
    }

    fclose(file);
    fclose(tempFile);

   // Eğer onaylanacak sipariş bulunamadıysa hata mesajı ver
    if (!isOrderApproved) {
        printf("Hata: Guncellenecek siparis bulunamadi.\n");
        remove("temp.txt"); // Geçici dosyayı sil
    } else {
        // Orijinal dosyayı geçici dosya ile değiştir
        remove(filename);
        rename("temp.txt", filename);
        printf("En ustteki onaylanmamis olan siparis onaylandi.\n");
    }

}


// Tarihe göre dosya adı oluştur
void createFileName(char *fileName, const char *date) {
    char folderPath[256] = "arsiv";
    sprintf(fileName, "%s/Siparis_%s.txt", folderPath, date);
}

void archiveDailyOrders(const char *filename, const char *date) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }

    char dailyFileName[64];
    createFileName(dailyFileName, date);

    int found = 0; // Tarih eşleşmesi kontrolü için değişken
    Order order;
    while (fscanf(file, "%s %s %f %s %d %s %s %d %s\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state, order.finalTime) != EOF) {
        if (strncmp(order.orderTime, date, 10) == 0) {
            found = 1; // Eşleşen tarih bulundu
            break;
        }
    }

    // Dosyayı baştan sona taradıktan sonra kapat
    fclose(file);

    if (!found) {
        // Eşleşen tarih bulunamadıysa hata mesajı ver
        fprintf(stderr, "Belirtilen tarihte siparis bulunamadi: %s\n", date);
    } else {
        // Eşleşen tarih bulunduysa, dosyaları oluştur ve işlemleri yap
        FILE *tempFile = fopen("temp.txt", "w");
        FILE *dailyFile = fopen(dailyFileName, "w");
        if (tempFile == NULL || dailyFile == NULL) {
            perror("Dosya olusturulamadi");
            exit(EXIT_FAILURE);
        }

        // Dosyayı tekrar aç
        file = fopen(filename, "r");
        if (file == NULL) {
            perror("Dosya acilamadi");
            exit(EXIT_FAILURE);
        }

        // Siparişleri tekrar oku ve dosyalara yaz
        while (fscanf(file, "%s %s %f %s %d %s %s %d %s\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state, order.finalTime) != EOF) {
            if (strncmp(order.orderTime, date, 10) == 0) {
                // State'i 3 olarak güncelle ve günlük dosyaya yaz
                fprintf(dailyFile, "%s %s %.2f %s %d %s %s 3 %s\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.finalTime);
            }else {
                fprintf(tempFile, "%s %s %.2f %s %d %s %s %d %s\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state, order.finalTime);
            }
        }

        // Dosyaları kapat
        fclose(file);
        fclose(dailyFile);
        fclose(tempFile);

        // Orijinal dosyayı geçici dosya ile değiştir
        remove(filename);
        rename("temp.txt", filename);
    }
}

// Kullanıcının istediği tarihe ait siparişleri görüntüle
void viewDailyReport(const char *date) {
    char dailyFileName[64];
    createFileName(dailyFileName, date);

    FILE *dailyFile = fopen(dailyFileName, "r");
    if (dailyFile == NULL) {
        printf("Hata: '%s' tarihine ait siparis raporu bulunamadi.\n", date);
        return;
    }

    Order order;
    printf("Tarih: %s\n", date);
    printf("Siparis ID | Yemek Adi | Fiyat | Siparis Zamani | Hazirlik Suresi | Musteri | sef | Durum\n");
    while (fscanf(dailyFile, "%s %s %f %s %d %s %s %d %s\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state, order.finalTime) != EOF) {
        printf("%s | %s | %.2f | %s | %d | %s | %s | %d | %s\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state, order.finalTime);
    }

    fclose(dailyFile);
    remove("temp.txt");
}



//Sipariş listesi
void OrderList(Order orders[], int *orderSize, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[100];
    *orderSize = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%s %s %f %s %d %s %s %d %s",
                   orders[*orderSize].orderId,
                   orders[*orderSize].foodName,
                   &orders[*orderSize].price,
                   orders[*orderSize].orderTime,
                   &orders[*orderSize].preparationTime,
                   orders[*orderSize].customer,
                   orders[*orderSize].chef,
                   &orders[*orderSize].state,
                   orders[*orderSize].finalTime) > 0) {
            (*orderSize)++;
        }
    }

    fclose(file);
}


// Dosya ismini tarih formatına dönüştürme
void extractDateFromFilename(char *filename, char *date) {
    // "Siparis_YYYY-MM-DD.txt" formatından tarihi çıkarır
    strncpy(date, filename + 8, 10);
    date[10] = '\0'; // Sonlandırıcı karakter eklenir
}

// Belirtilen tarihe ait siparişlerin toplam kazancını hesaplama
float calculateDailyRevenue(char *date, char *directory) {
    float totalRevenue = 0.0;
    char filename[256];
    sprintf(filename, "%s/Siparis_%s.txt", directory, date);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: '%s' tarihine ait dosya bulunamadi.\n", date);
        return -1;
    }

    Order order;
    while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d %*s\n",
                  order.orderId, order.foodName, &order.price) == 3) {
        totalRevenue += order.price;
    }

    fclose(file);
    return totalRevenue;
}

// Belirtilen aya ait siparişlerin toplam kazancını hesaplama
float calculateMonthlyRevenue(int year, int month, char *directory) {
    float totalRevenue = 0.0;
    char date[11];
    char filename[256];
    FILE *file;
    int fileFound = 0; // Dosya bulunup bulunmadığını kontrol etmek için değişken
    for (int day = 1; day <= 31; day++) {
        sprintf(date, "%04d-%02d-%02d", year, month, day);
        sprintf(filename, "%s/Siparis_%s.txt", directory, date);
        file = fopen(filename, "r");
        if (file) {
            fileFound = 1; // Dosya bulundu
            Order order;
            while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d %*s\n",
                          order.orderId, order.foodName, &order.price) == 3) {
                totalRevenue += order.price;
            }
            fclose(file);
        }
    }
    if (!fileFound) {
        printf("Hata: '%04d-%02d' ayina ait hicbir dosya bulunamadi.\n", year, month);
        return -1;
    }
    return totalRevenue;
}

// İki tarih arasındaki siparişlerin toplam kazancını hesaplama
float calculatePeriodRevenue(char *startDate, char *endDate, char *directory) {
    float totalRevenue = 0.0;
    // Tarihleri yıl, ay ve gün olarak parçalara ayırma
    int startYear, startMonth, startDay, endYear, endMonth, endDay;
    sscanf(startDate, "%4d-%2d-%2d", &startYear, &startMonth, &startDay);
    sscanf(endDate, "%4d-%2d-%2d", &endYear, &endMonth, &endDay);

    // Başlangıç ve bitiş tarihleri arasında döngü
    char currentDate[11];
    int fileFound = 0; // Dosya bulunup bulunmadığını kontrol etmek için değişken
    for (int year = startYear; year <= endYear; year++) {
        for (int month = (year == startYear ? startMonth : 1);
             month <= (year == endYear ? endMonth : 12); month++) {
            for (int day = (year == startYear && month == startMonth ? startDay : 1);
                 day <= (year == endYear && month == endMonth ? endDay : 31); day++) {
                sprintf(currentDate, "%04d-%02d-%02d", year, month, day);
                float dailyRevenue = calculateDailyRevenue(currentDate, directory);
                if (dailyRevenue >= 0) {
                    fileFound = 1; // En az bir dosya bulundu
                    totalRevenue += dailyRevenue;
                }
            }
        }
    }
    if (!fileFound) {
        printf("Hata: '%s' ile '%s' tarihleri arasinda hicbir dosya bulunamadi.\n", startDate, endDate);
        return -1;
    }
    return totalRevenue;
}


// En erken biten asci isini bulan fonksiyon
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



//suanki zamani alan fonksiyon
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

// Asçinin çalisma zamanini kontrol eden ve güncelleyen fonksiyon

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
//dosyadaki tum txtleri okur
void readTxtFiles(Order orders[], int *orderSize,const char *dirPath) {
    DIR *dir;
    struct dirent *ent;
    FILE *file;
    char filePath[MAX_PATH_LEN];
    char line[MAX_LINE_LEN];

    if ((dir = opendir(dirPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, ent->d_name);
                file = fopen(filePath, "r");
                if (file) {

                    while (fgets(line, sizeof(line), file)) {


                             if (sscanf(line, "%s %s %f %s %d %s %s %d %s",
                   orders[*orderSize].orderId,
                   orders[*orderSize].foodName,
                   &orders[*orderSize].price,
                   orders[*orderSize].orderTime,
                   &orders[*orderSize].preparationTime,
                   orders[*orderSize].customer,
                   orders[*orderSize].chef,
                   &orders[*orderSize].state,
                   orders[*orderSize].finalTime) > 0) {

            (*orderSize)++;
        }






                    }
                    fclose(file);
                    printf("\n");
                } else {
                    fprintf(stderr, "Error opening file: %s\n", filePath);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
}


// En çok tüketilen yemeği bulan fonksiyon
void findMostConsumedFood(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    FILE *file;
    Order order;
    char filename[256];

    // Yemek isimleri ve sayıları için sözlük yapısı
    struct FoodCount {
        char foodName[50];
        int count;
    } foodCounts[1000]; // Yeterli büyüklükte bir dizi tahsis edin
    int foodCountSize = 0;

    // Klasörü aç
    if ((dir = opendir(directory)) == NULL) {
        perror("opendir() error");
    } else {
        // Klasördeki her dosyayı oku
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".txt")) {
                // Dosya adını oluştur
                sprintf(filename, "%s/%s", directory, entry->d_name);
                file = fopen(filename, "r");
                if (file == NULL) {
                    perror("Dosya açılamadı");
                    continue;
                }

                // Dosyadaki her siparişi oku
                while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d %*s\n",
                              order.orderId, order.foodName, &order.price) == 3) {
                    // Yemek ismini ve sayısını güncelle
                    int found = 0;
                    for (int i = 0; i < foodCountSize; i++) {
                        if (strcmp(foodCounts[i].foodName, order.foodName) == 0) {
                            foodCounts[i].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(foodCounts[foodCountSize].foodName, order.foodName);
                        foodCounts[foodCountSize].count = 1;
                        foodCountSize++;
                    }
                }
                fclose(file);
            }
        }
        closedir(dir);
    }

    // En çok tüketilen yemeği bul
    int maxCount = 0;
    char mostConsumedFood[50];
    for (int i = 0; i < foodCountSize; i++) {
        if (foodCounts[i].count > maxCount) {
            maxCount = foodCounts[i].count;
            strcpy(mostConsumedFood, foodCounts[i].foodName);
        }
    }

    // En çok tüketilen yemeği ve sayısını yazdır
    printf("En cok tuketilen yemek: %s (%d adet)\n", mostConsumedFood, maxCount);
}



// En kazançlı günü bulan fonksiyon
void findMostProfitableDay(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    FILE *file;
    Order order;
    char filename[256];

    // Günler ve kazançları için sözlük yapısı
    struct DayProfit {
        char date[11];
        float profit;
    } dayProfits[1000]; // Yeterli büyüklükte bir dizi tahsis edin
    int dayProfitSize = 0;

    // Klasörü aç
    if ((dir = opendir(directory)) == NULL) {
        perror("opendir() error");
    } else {
        // Klasördeki her dosyayı oku
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".txt")) {
                // Dosya adını oluştur
                sprintf(filename, "%s/%s", directory, entry->d_name);
                file = fopen(filename, "r");
                if (file == NULL) {
                    perror("Dosya açılamadı");
                    continue;
                }

                // Dosya isminden tarih bilgisini al
                char date[11];
                strncpy(date, entry->d_name + 8, 10);
                date[10] = '\0'; // Sonlandırıcı karakter eklenir

                // Dosyadaki her siparişi oku
                while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d %*s\n",
                              order.orderId, order.foodName, &order.price) == 3) {
                    // Tarihi ve kazancı güncelle
                    int found = 0;
                    for (int i = 0; i < dayProfitSize; i++) {
                        if (strcmp(dayProfits[i].date, date) == 0) {
                            dayProfits[i].profit += order.price;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(dayProfits[dayProfitSize].date, date);
                        dayProfits[dayProfitSize].profit = order.price;
                        dayProfitSize++;
                    }
                }
                fclose(file);
            }
        }
        closedir(dir);
    }

    // En kazançlı günü bul
    float maxProfit = 0.0;
    char mostProfitableDay[11];
    for (int i = 0; i < dayProfitSize; i++) {
        if (dayProfits[i].profit > maxProfit) {
            maxProfit = dayProfits[i].profit;
            strcpy(mostProfitableDay, dayProfits[i].date);
        }
    }

    // En kazançlı günü ve kazancı yazdır
    printf("En kazancli gun: %s (Toplam kazanc: %.2f TL)\n", mostProfitableDay, maxProfit);
}

// En çok sipariş veren kullanıcıyı bulan fonksiyon
void findMostFrequentCustomer(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    FILE *file;
    Order order;
    char filename[256];

    // Müşteriler ve sipariş sayıları için sözlük yapısı
    struct CustomerOrderCount {
        char customerName[20];
        int orderCount;
    } customerOrderCounts[1000]; // Yeterli büyüklükte bir dizi tahsis edin
    int customerOrderCountSize = 0;

    // Klasörü aç
    if ((dir = opendir(directory)) == NULL) {
        perror("opendir() error");
    } else {
        // Klasördeki her dosyayı oku
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".txt")) {
                // Dosya adını oluştur
                sprintf(filename, "%s/%s", directory, entry->d_name);
                file = fopen(filename, "r");
                if (file == NULL) {
                    perror("Dosya açılamadı");
                    continue;
                }

                // Dosyadaki her siparişi oku
                while (fscanf(file, "%s %s %f %s %*d %s %*s %*d %*s\n",
                              order.orderId, order.foodName, &order.price, order.orderTime, order.customer) == 5) {
                    // Müşteri ismini ve sipariş sayısını güncelle
                    int found = 0;
                    for (int i = 0; i < customerOrderCountSize; i++) {
                        if (strcmp(customerOrderCounts[i].customerName, order.customer) == 0) {
                            customerOrderCounts[i].orderCount++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(customerOrderCounts[customerOrderCountSize].customerName, order.customer);
                        customerOrderCounts[customerOrderCountSize].orderCount = 1;
                        customerOrderCountSize++;
                    }
                }
                fclose(file);
            }
        }
        closedir(dir);
    }

    // En çok sipariş veren kullanıcıyı bul
    int maxOrderCount = 0;
    char mostFrequentCustomer[20];
    for (int i = 0; i < customerOrderCountSize; i++) {
        if (customerOrderCounts[i].orderCount > maxOrderCount) {
            maxOrderCount = customerOrderCounts[i].orderCount;
            strcpy(mostFrequentCustomer, customerOrderCounts[i].customerName);
        }
    }

    // En çok sipariş veren kullanıcıyı ve sipariş sayısını yazdır
    printf("En cok siparis veren kullanici: %s (%d siparis)\n", mostFrequentCustomer, maxOrderCount);
}

//id olusturma
#include <stdio.h>
#include <time.h>

char* getId() {
    static char id[50]; // id'yi statik bir char dizisi olarak tanımlayın

    time_t current_time;
    struct tm *local_time;
    current_time = time(NULL);
    local_time = localtime(&current_time);

    // Kod dizisini yerel olarak tanımlayın
    char kod[30];
    strftime(kod, sizeof(kod), "%Y%m%d%H%M%S", local_time);

    // id içine yazın
    sprintf(id, "Sip_%s", kod);

    return id;
}

//menu göster
void showMenu(MenuItem menu[], int *menuSize)
{
    printf("Yemek Seciniz:\n");
            for (int i = 0; i < *menuSize; i++)
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
}
//siparis olusturmak icin
void siparisOlustur(char id[20],char isim[25])
{

    MenuItem menu[MAX_MENU_ITEMS];
    int menuSize = 0;
    char name[50];
    float price;
    int preparationTime;
    char orderTime[20];
    char menuFilename[] = "yemeklistesi.txt";


    loadMenu(menu, &menuSize, menuFilename);


            int found=0;
            do {
                    scanf("%s", name);

                    for (int i = 0; i < menuSize; i++) {
                        if (strcmp(menu[i].name, name) == 0 && strcmp(menu[i].availability, "Evet") == 0) {
                            price = menu[i].price;
                            preparationTime = menu[i].preparationTime;

                            time_t current_time;
                            struct tm *local_time;
                            current_time = time(NULL);
                            local_time = localtime(&current_time);
                            strftime(orderTime, sizeof(orderTime), "%Y-%m-%d/%H:%M:%S", local_time);

                            char chef[10]="0";
                            int state=0;
                            char time[20]="00:00";

                            FILE *file = fopen("Siparisler.txt", "a");
                            fprintf(file, "%s %s %.2f %s %d %s %s %d %s\n",id,name,price,orderTime,preparationTime,isim,chef,state,time);
                            fclose(file);


                            found = 1;
                            break; // Break out of the for loop once a match is found
                            }
                        }

                    if (found==1) {

                    } else {
                    printf("bulunamadi tekrar dene.\n");
                            }
                    } while (!found);


}
void tumSiparisleriGoster(char isim[20])
{
    Order orders[MAX_ORDERS];
    int orderSize = 0;
    char orderFilename[] = "Siparisler.txt";

                OrderList(orders,&orderSize,orderFilename);



            int control=0;
            for (int i = 0; i <orderSize; i++)
            {

                if( orders[i].state==0&&strcmp(orders[i].customer,isim) == 0)
                {

                    if(control==0)
                    {
                        printf("Onaylanmayan siparisler:\n");
                        control=1;
                    }
                   printf("Siparis Id:%s Yemek ismi:%s Fiyat:%.2f Siparis Zamani:%s\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime);
                }
            }


            control=0;
            for (int i = 1; i <orderSize; i++)
            {
                if( orders[i].state==1&&strcmp(orders[i].customer,isim) == 0)
                {
                     if(control==0)
                    {
                        printf("\nOnaylanan siparisler:\n");
                        control=1;
                    }

                   printf("Siparis Id:%s Yemek ismi:%s Fiyat:%.2f Siparis Zamani:%s\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime);
                }
            }


            control=0;
            for (int i = 0; i <orderSize; i++)
            {





                char *token = strtok(orders[i].finalTime, ":");
                int hour = atoi(token);
                token = strtok(NULL, ":");
                int minute = atoi(token);

                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                int current_hour = tm->tm_hour;
                int current_minute = tm->tm_min;

                sprintf(orders[i].finalTime, "%d",hour * 60 + minute - current_hour * 60 - current_minute);

                if(hour<current_hour&&orders[i].state!=0)
                {

                    orders[i].state=3;

                }
                else if(hour==current_hour&&minute<current_minute&&orders[i].state!=0)
                {
                    orders[i].state=3;
                }



                if( orders[i].state==2&&strcmp(orders[i].customer,isim) == 0)
                {
                    if(control==0)
                    {
                        printf("\nyapilan siparisler:\n");
                        control=1;
                    }
                   printf("Siparis Id:%s Yemek ismi:%s Fiyat:%.2f Siparis Zamani:%s Kalan zaman:%s\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime,
                   orders[i].finalTime);
                }
            }


            readTxtFiles(orders,&orderSize,"arsiv");
            control=0;
            for (int i = 0; i <orderSize; i++)
            {

                char *token = strtok(orders[i].finalTime, ":");
                int hour = atoi(token);
                token = strtok(NULL, ":");
                int minute = atoi(token);

                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                int current_hour = tm->tm_hour;
                int current_minute = tm->tm_min;

                sprintf(orders[i].finalTime, "%d",hour * 60 + minute - current_hour * 60 - current_minute);



            if(hour<current_hour&&orders[i].state!=0)
                {

                    orders[i].state=3;

                }
                else if(hour==current_hour&&minute<current_minute&&orders[i].state!=0)
                {
                    orders[i].state=3;
                }

                if( orders[i].state==3&&strcmp(orders[i].customer,isim) == 0)
                {
                    if(control==0)
                    {
                        printf("Hazir siparisler:\n");
                        control=1;
                    }

                   printf("Siparis Id:%s Yemek ismi:%s Fiyat:%.2f Siparis Zamani:%s\n",
                   orders[i].orderId,
                   orders[i].foodName,
                   orders[i].price,
                   orders[i].orderTime);
                }

            }

        }

// Aşçı sayısını güncelleyen fonksiyon
int updateChefCount(const char *filename, int yeniAsciSayisi) {
    // Geçici bir dosya oluştur
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *file = fopen(filename, "r");
    if (tempFile == NULL || file == NULL) {
        perror("Dosya açılamadı veya oluşturulamadı");
        return EXIT_FAILURE;
    }

    // Yeni aşçı sayısını geçici dosyaya yaz
    fprintf(tempFile, "%d\n", yeniAsciSayisi);

    // İlk satırı atla
    fscanf(file, "%*d\n");

    // Geri kalan içeriği geçici dosyaya kopyala
    char buffer;
    while ((buffer = fgetc(file)) != EOF) {
        fputc(buffer, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    // Geçici dosyayı orijinal dosya ile değiştir
    remove(filename);
    rename("temp.txt", filename);

    return EXIT_SUCCESS;
}


        
#endif
