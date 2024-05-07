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

    // Dosyadan okuma iþlemi
    FILE *dosya = fopen("yemeklistesi.txt", "r"); // "r" ile dosyayý okuma modunda aç

    if (dosya == NULL) {
        printf("Dosya açma hatasý!");
        return 1;
    }

    printf("Yemek Listesi:\n");

    // Dosyadan veri oku ve ekrana yaz
    while (fscanf(dosya, "ID: %s\nAd: %s\nFiyat: %f\nSüre: %f\nDurum: %s\nSipariþ: %s\nHazýrlanma: %s\nKullanýcý: %s\nAþçý: %s\n--------------------------------\n",
                  veri.id, veri.ad, &veri.fiyat, &veri.sure, veri.durum, veri.siparis, veri.hazirlanma, veri.kullanici, veri.asci) != EOF) {
        printf("\nID: %s\n", veri.id);

    }

    // Dosyayý kapat
    fclose(dosya);

    return 0;
}
