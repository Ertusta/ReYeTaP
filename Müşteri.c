#include <stdio.h>

struct Veri {
    char id[50];
    char ad[50];
    float fiyat;
    float sure;
    char durum[50];
    char siparis[50];
    char hazirlanma[50];
    char kullanici[50];
    char asci[50];
};

int main() {
    struct Veri veri;

    // Dosyadan okuma i�lemi
    FILE *dosya = fopen("yemeklistesi.txt", "r"); // "r" ile dosyay� okuma modunda a�

    if (dosya == NULL) {
        printf("Dosya a�ma hatas�!");
        return 1;
    }

    printf("Yemek Listesi:\n");

    // Dosyadan veri oku ve ekrana yaz
    while (fscanf(dosya, "ID: %s\nAd: %s\nFiyat: %f\nS�re: %f\nDurum: %s\nSipari�: %s\nHaz�rlanma: %s\nKullan�c�: %s\nA���: %s\n--------------------------------\n",
                  veri.id, veri.ad, &veri.fiyat, &veri.sure, veri.durum, veri.siparis, veri.hazirlanma, veri.kullanici, veri.asci) != EOF) {
        printf("\nID: %s\n", veri.id);

    }

    // Dosyay� kapat
    fclose(dosya);

    return 0;
}
