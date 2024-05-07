#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"

#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

// Menü öğelerini tutacak yapı
typedef struct {
    char name[50];
    float price;
    int preparationTime;
    char availability[10];
} MenuItem;

// Siparişleri tutacak yapı
typedef struct {
    char orderId[20];
    char foodName[50];
    float price;
    char orderTime[20];
    char preparationTime[20];
    char customer[50];
    char chef[50];
} Order;

// Menüyü dosyadan yükleme fonksiyonu
void loadMenu(MenuItem menu[], int *menuSize, const char *filename) {
    // Kod buraya eklenecek
}

// Siparişleri dosyaya kaydetme fonksiyonu
void saveOrder(Order orders[], int orderSize, const char *filename) {
    // Kod buraya eklenecek
}

// Yeni sipariş ID'si üretme fonksiyonu
void generateOrderId(char *orderId) {
    // Kod buraya eklenecek
}

// Mevcut yemekleri gösterme fonksiyonu
void showAvailableMenuItems(MenuItem menu[], int menuSize) {
    // Kod buraya eklenecek
}

// Yeni sipariş alma fonksiyonu
void takeNewOrder(MenuItem menu[], int menuSize, Order orders[], int *orderSize) {
    // Kod buraya eklenecek
}

int main() {
    MenuItem menu[MAX_MENU_ITEMS];
    Order orders[MAX_ORDERS];
    int menuSize = 0, orderSize = 0;
    char menuFilename[] = "yemeklistesi.txt";
    char orderFilename[] = "siparişler.txt";

    // Menüyü yükle
    loadMenu(menu, &menuSize, menuFilename);

    // Kullanıcıya mevcut yemekleri göster
    showAvailableMenuItems(menu, menuSize);

    // Yeni sipariş al
    takeNewOrder(menu, menuSize, orders, &orderSize);

    // Siparişleri dosyaya kaydet
    saveOrder(orders, orderSize, orderFilename);

    return 0;
}
