#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    char name[50];
    float price;
    int preparationTime;
    char availability[20];
} MenuItem;

typedef struct {
    char orderId[50];
    char foodName[50];
    float price;
    char orderTime[20];
    int preparationTime;
    char customer[20];
    char chef[20];
    int state;
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
    while (fscanf(file, "%s %s %f %s %d %s %s %d\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state) != EOF) {
        if (order.state == 0 && !isOrderApproved) { // İlk onaylanmamış sipariş
            approveOrder(&order); // Siparişi onayla
            isOrderApproved = 1;
        }
        // Siparişi geçici dosyaya yaz
        fprintf(tempFile, "%s %s %.2f %s %d %s %s %d\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state);
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
    while (fscanf(file, "%s %s %f %s %d %s %s %d\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state) != EOF) {
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
        while (fscanf(file, "%s %s %f %s %d %s %s %d\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state) != EOF) {
            if (strncmp(order.orderTime, date, 10) == 0) {
                fprintf(dailyFile, "%s %s %.2f %s %d %s %s %d\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state);
            } else {
                fprintf(tempFile, "%s %s %.2f %s %d %s %s %d\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state);
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
    while (fscanf(dailyFile, "%s %s %f %s %d %s %s %d\n", order.orderId, order.foodName, &order.price, order.orderTime, &order.preparationTime, order.customer, order.chef, &order.state) != EOF) {
        printf("%s | %s | %.2f | %s | %d | %s | %s | %d\n", order.orderId, order.foodName, order.price, order.orderTime, order.preparationTime, order.customer, order.chef, order.state);
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
    while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d\n", 
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
            while (fscanf(file, "%s %s %f %*s %*d %*s %*s %*d\n", 
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


#endif 
