#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "llist.h"

/* TODO: Yeni bir dugum yaratip adresini donduren fonksiyonu yazin */
struct node* new_node(unsigned char data) {
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/* TODO: data parametresiyle verilen sayiyi listenin ilgili yerine 
 * yerlestiren fonksiyonu yazin. 
 * UYARI: Listedeki dugumlerin data degerleri kucukten buyuge 
 * sirali olmalidir! */
struct node* insert(struct node *list, unsigned char data) {
    struct node *newNode = new_node(data);

    if (list == NULL) {
        return newNode;
    }

    struct node *current = list;
    struct node *prev = NULL;
    while (current != NULL && current->data < data) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newNode->next = list;
        return newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
        return list;
    }
}

/* TODO: Parametre olarak aldigi listenin median/ortanca
 * degerini donduren fonksiyonu yazin */
unsigned char get_median(struct node *list) {
    int mid = 0, count = 0;
    for (struct node* i = list; i != NULL; i = i->next)
    {
        mid++;
    }
    mid /= 2;
    struct node* temp = list;
    while (count < mid)
    {
        count++;
        temp = temp->next;
    }
    return temp->data;
}

/* TODO: Parametre olarak aldigi listenin elemanlarini bastiran 
 * fonksiyonu yazin */
void print_list(struct node *list) {
    for (struct node* i = list; i != NULL; i = i->next)
    {
        printf("%hhu ", i->data);
    }
}

/* TODO: Parametre olarak aldigi liste (ve tum elemanlari) icin ayrilan 
 * yeri serbest birakan fonksiyonu yazin */
void free_list(struct node *list) {
  struct node *temp = list;
  while (temp != NULL) {
    struct node *next = temp->next;
    free(temp);
    temp = next;
  }
  list = NULL;
}
