#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node
{
    int data;
    struct Node* next;
}Node;

Node* create_node(int value);

void append(Node** head, int value);

void print_list(Node* head);

Node* find(Node* head, int value);

bool delete_node(Node** head, int value);

void free_list(Node* head);

#endif