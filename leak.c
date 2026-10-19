/*
 * leak.c - reads lines into a linked list. The buggy version never frees the
 * list; build with -DFIXED to free it. Run both under valgrind to compare.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node { char *text; struct Node *next; } Node;

static Node *push(Node *head, const char *s)
{
    Node *n = malloc(sizeof *n);
    n->text = strdup(s);
    n->next = head;
    return n;
}

int main(void)
{
    const char *words[] = { "fork", "exec", "wait", "pipe", "dup2" };
    Node *list = NULL;
    for (int i = 0; i < 5; i++) list = push(list, words[i]);

    int count = 0;
    for (Node *n = list; n; n = n->next) count++;
    printf("stored %d syscalls, last added: %s\n", count, list->text);

#ifdef FIXED
    while (list) {
        Node *next = list->next;
        free(list->text);
        free(list);
        list = next;
    }
#endif
    return 0;
}
