#ifndef _LLIST_H_
#define _LLIST_H_

/* Kullanacaginiz dugumler icin veri yapisi */
struct node {
    unsigned char data;
    struct node *next;
};

/* Fonksiyon basliklari */
struct node* new_node(unsigned char data);
struct node* insert(struct node *list, unsigned char data);
void print_list(struct node *list);
void free_list(struct node *list);
unsigned char get_median(struct node *list);

#endif
