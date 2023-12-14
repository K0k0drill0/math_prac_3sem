#ifndef UNDO_H
#define UNDO_H

#include "Liver.h"

typedef struct Action {
    Liver* then;
    Liver* now;
    struct Action* prev;
    struct Action* next;
} Action;

typedef struct UndoDeque {
    int len;
    int actions_amount;
    struct Action* right;
    struct Action* left;
} UndoDeque;

int init_Action(Action** act);
int init_UndoDeque(UndoDeque* dq);

int push_left_UndoDeque(UndoDeque* dq, Action* new);
int push_right_UndoDeque(UndoDeque* dq, Action* new);

int pop_left_UndoDeque(UndoDeque* dq);
int pop_right_UndoDeque(UndoDeque* dq);

int add_action(UndoDeque* dq, Action* act);
int undo(UndoDeque* dq, Liver** data_base);

int is_empty_UndoDeque(const UndoDeque dq);

Action* get_left_UndoDeque(const UndoDeque dq);
Action* get_right_UndoDeque(const UndoDeque dq);

int print_Action(const Action* act);
int print_UndoDeque(const UndoDeque dq);

void free_action(Action* act);
int free_UndoDeque(UndoDeque* dq);

#endif