#include "stack/intstack.h"
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist/intlist.h"
#include "queue/intqueue.h"
#include "internal/memmngr.h"

typedef struct _intnode {
    int value;
    struct _intnode* next;
} *IntNode;

struct _intstack {
    IntNode top;
    size_t size;
};

static bool _intstack_not_exists(const IntStack stack) {
    return !stack;
}

bool intstack_is_empty(const IntStack stack) {
    return _intstack_not_exists(stack) || !stack->top;
}

static IntNode _intstack_create_node(int value) {
    IntNode new_node = (IntNode) malloc(sizeof (struct _intnode));
    if (!new_node) return NULL;

    *new_node = (struct _intnode) {.value = value, .next = NULL};
    return new_node;
}

static void _intstack_free(IntStack stack) {
    intstack_clear(stack);
    free(stack);
}

IntStack intstack_new(void) {
    IntStack new_stack = (IntStack) malloc(sizeof (struct _intstack));
    if (_intstack_not_exists(new_stack)) return NULL;

    if (!_memmngr_register(new_stack, (void (*)(void*)) _intstack_free)) {
        free(new_stack);
        return NULL;
    }

    *new_stack = (struct _intstack) {.top = NULL, .size = 0};
    return new_stack;
}

void intstack_clear(IntStack stack) {
    if (_intstack_not_exists(stack)) return;

    for (IntNode curr = stack->top, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }

    stack->top = NULL;
    stack->size = 0;
}

bool intstack_push(IntStack stack, int value) {
    if (_intstack_not_exists(stack)) return false;

    IntNode new_node = _intstack_create_node(value);
    if (!new_node) return false;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
    return true;
}

bool intstack_pop(IntStack stack, int* out) {
    if (intstack_is_empty(stack)) return false;

    IntNode top = stack->top;
    if (out) *out = top->value;

    stack->top = top->next;
    free(top);

    stack->size--;
    return true;
}

bool intstack_peek(const IntStack stack, int* out) {
    if (intstack_is_empty(stack) || !out) return false;
    *out = stack->top->value;
    return true;
}

size_t intstack_size(const IntStack stack) {
    return _intstack_not_exists(stack) ? 0 : stack->size;
}

IntList intstack_to_list(const IntStack stack) {
    if (_intstack_not_exists(stack)) return NULL;

    IntList new_list = intlist_new();
    if (!new_list) return NULL;

    for (IntNode curr = stack->top; curr; curr = curr->next) {
        if (!intlist_push(new_list, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

IntQueue intstack_to_queue(const IntStack stack) {
    if (_intstack_not_exists(stack)) return NULL;
    
    IntQueue new_queue = intqueue_new();
    if (!new_queue) return NULL;
    
    for (IntNode curr = stack->top; curr; curr = curr->next) {
        if (!intqueue_enqueue(new_queue, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }    
    return new_queue;
}