#include <stdio.h>
#include <stdbool.h>
#include "linked_list.h"

int main() {
    Node* head = create_node(0);

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);

    printf("List: ");
    print_list(head);

    printf("Searching for 20...\n");
    Node* found = find(head, 20);
    if (found)
        printf("Found node with value: %d\n", found->data);

    printf("Deleting 20...\n");
    delete_node(&head, 20);
    print_list(head);

    printf("\n");
    free_list(head);
    return 0;
}
