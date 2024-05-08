#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    char name[50];
    float price;
    int preparationTime;
    char availability[20];
} MenuItem;

typedef struct {
    char orderId[20];
    char foodName[50];
    float price;
    char orderTime[20];
    int preparationTime;
    char customer[20];
    char chef[20];
} Order;

#endif 
