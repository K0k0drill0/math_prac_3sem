#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Liver.h"
#include "Undo.h"
#include "validators.h"

/*
Как будет работать анду?
Первые три команды выше будут заноситься в дек. Каждое четное добавление будет сопровождаться удалением 
элемента из начала. Вместе с самой командой будут добавляться копии старой и новой версии жителя.
*/

int init_Action(Action** act) {
    *act = (Action*)malloc(sizeof(Action));
    if (*act == NULL) {
        return MEMORY_ISSUES;
    }

    (*act)->then = NULL;
    (*act)->now = NULL;
    (*act)->prev = NULL;
    (*act)->next = NULL;

    return ok;
}

int init_UndoDeque(UndoDeque* dq) {
    if (dq == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    dq->left = NULL;
    dq->right = NULL;
    dq->len = 0;
    dq->actions_amount = 0;

    return ok;
}

int push_left_UndoDeque(UndoDeque* dq, Action* new) {
    if (dq == NULL || new == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (dq->len == 0) {
        dq->right = new;
        dq->left = new;
        dq->len++;
        return ok;
    }

    new->prev = NULL;
    new->next = dq->left;

    dq->left->prev = new;

    dq->left = new;
    dq->len++;
    return ok;
}

int push_right_UndoDeque(UndoDeque* dq, Action* new) {
    if (dq == NULL || new == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (dq->len == 0) {
        dq->right = new;
        dq->left = new;
        dq->len++;
        return ok;
    }

    new->next = NULL;
    new->prev = dq->right;

    dq->right->next = new;

    dq->right = new;
    dq->len++;
    return ok;
}

int pop_left_UndoDeque(UndoDeque* dq) {
    if (dq == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (dq->len == 0) {
        return ok;
    }

    if (dq->len == 1) {
        dq->len--;
        free_action(dq->left);
        dq->left = NULL;
        dq->right = NULL;
        return ok;
    }

    dq->len--;
    Action* will_be_deleted = dq->left;
    dq->left = dq->left->next;

    free_action(will_be_deleted);

    return ok;
}

int pop_right_UndoDeque(UndoDeque* dq) {
    if (dq == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (dq->len == 0) {
        return ok;
    }

    if (dq->len == 1) {
        dq->len--;
        free_action(dq->right);
        dq->left = NULL;
        dq->right = NULL;
        return ok;
    }

    dq->len--;
    Action* will_be_deleted = dq->right;
    dq->right = dq->right->prev;

    free_action(will_be_deleted);
    return ok;
}

int add_action(UndoDeque* dq, Action* act) {
    int st = ok;

    st = push_right_UndoDeque(dq, act);
    if (st != ok) {
        free_action(act);
        return st;
    }

    dq->actions_amount++;
    if (!(dq->actions_amount & 1)) {
        st = pop_left_UndoDeque(dq);
        if (st != ok) {
            return st;
        }
    }

    return ok;
}

int undo(UndoDeque* dq, Liver** data_base) {
    int st = ok;
    Action* last = get_right_UndoDeque(*dq);

    if (last == NULL) {
        return NO_UNDO_ACTION;
    }

    if (last->then == NULL) {
        Liver* found = NULL;
        st = find_liver(*data_base, last->now, &found);
        if (st != ok) {
            return st;
        }
        last->now = NULL;

        st = delete_liver(data_base, found);
        if (st != ok) {
            return st;
        }
    }
    else if (last->now == NULL) {
        Liver* new = (Liver*)malloc(sizeof(Liver));
        if (new == NULL) {
            return MEMORY_ISSUES;
        }

        st = copy_liver(new, last->then);
        if (st != ok) {
            free_liver_data(new);
            return st;
        }

        st = insert_liver(data_base, new);
        if (st != ok) {
            free_liver_data(new);
            return st;
        }
    }
    else {
        //
    }

    return pop_right_UndoDeque(dq);
}

int is_empty_UndoDeque(const UndoDeque dq) {
    return !(dq.len);
}

Action* get_left_UndoDeque(const UndoDeque dq) {
    return dq.left;
}

Action* get_right_UndoDeque(const UndoDeque dq) {
    return dq.right;
}

int print_Action(const Action* act) {
    if (act == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    printf("Then:\n");
    print_liver(stdout, act->then);
    printf("\nNow:\n");
    print_liver(stdout, act->now);
    printf("\n");
    return ok;
}

int print_UndoDeque(const UndoDeque dq) {
    printf("Length: %d\n", dq.len);
    Action* tmp = dq.left;

    int st = ok;
    while (tmp != NULL) {
        st = print_Action(tmp);
        if (st != ok) {
            return st;
        }
        tmp = tmp->next;
    }

    return ok;
}

void free_action(Action* act) {
    if (act == NULL) {
        return;
    }
    free_liver_data(act->then);
    free_liver_data(act->now);
    free(act);
}

int free_UndoDeque(UndoDeque* dq) {
    if (dq == NULL) {
        return ok;
    }
    int st = ok;
    while (!is_empty_UndoDeque(*dq)) {
        st = pop_left_UndoDeque(dq);
        if (st != ok) {
            return st;
        }
    }
    dq->actions_amount = 0;

    return ok;
}