#include <stdio.h>
#include <dirent.h>
#include <string.h>

typedef struct {
    char orderId[50];
    char foodName[50];
    float price;
    char orderTime[20];
    int preparationTime;
    char customer[20];
    char chef[20];
    int state;
    char finalTime;
} Order;

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


int main() {
    
    findMostConsumedFood("arsiv");
    findMostProfitableDay("arsiv");
    findMostFrequentCustomer("arsiv");
    
    return 0;
}
