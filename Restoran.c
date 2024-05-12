#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50



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

   
void approveOrder(Order *order) {
    order->state = 1; // Siparişi onayla 
}

//Siparişi Onaylama
void updateOrderStatus(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya açılamadı");
        exit(EXIT_FAILURE);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Geçici dosya açılamadı");
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
    }

}


// Tarihe göre dosya adı oluştur
void createFileName(char *fileName, const char *date) {
    sprintf(fileName, "Siparis_%s.txt", date);
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


int main() {
    MenuItem menu[MAX_MENU_ITEMS]; // Menü öğeleri için dizi
    int menuSize = 0; // Menüdeki öğe sayısı
    char filename[] = "Yemeklistesi.txt"; // Dosya adı
    loadMenu(menu, &menuSize, filename);
    char date[11];

    char choice;
    do {
        printf("Yemek eklemek icin 'e'\nyemek listesini gormek icin 'l'\nyemek bilgisi guncellemek icin 'g'\nyemek silmek icin 's'\nYemek onaylamak icin 'o'\nGunluk rapor kaydetmek icin 'u'\nGunluk rapor goruntulemek icin 'a'\ncikis yapmak icin 'q' girin: ");
        scanf(" %c", &choice);

        if (choice == 'e' || choice == 'E') {
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
        } else if (choice == 'l' || choice == 'L') {
            printf("Yemek Listesi:\n");
            for (int i = 0; i < menuSize; i++) {
                printf("%s: %.2f TL (%d dakika) - %s\n", 
                       menu[i].name, 
                       menu[i].price, 
                       menu[i].preparationTime, 
                       menu[i].availability);
            }
        }else if (choice == 'g' || choice == 'G') {
            updateMenuItem(menu, &menuSize, filename);
        }else if (choice == 's' || choice == 'S') {
            deleteMenuItem(menu, &menuSize, filename);
        }else if (choice == 'o' || choice == 'O') {
            updateOrderStatus("Siparisler.txt");
        }else if (choice == 'u' || choice == 'U'){
            printf("Lutfen arsivlemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
            scanf("%10s", date);
            archiveDailyOrders("Siparisler.txt", date);
        }else if (choice == 'a' || choice == 'A'){
            printf("Lutfen goruntulemek istediginiz tarihi YYYY-MM-DD formatinda giriniz: ");
            scanf("%10s", date);
            viewDailyReport(date);
        }
    } while (choice != 'q' && choice != 'Q');

    printf("Programdan cikis yapildi.\n");
    return 0;
}
