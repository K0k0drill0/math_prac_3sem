#ifndef LIVER_H
#define LIVER_H

typedef struct Liver {
    char* surname;
    char* name;
    char* last_name;
    char* birthday;
    char gender;
    double wage;

    struct Liver* next;
} Liver;

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT,
    WRONG_AMOUNT_OF_ARGUMENTS,
	INVALID_FUNCTION_ARGUMENT,
	OVERFLOWED,
	INVALID_DATE_OR_TIME,
    INVALID_DATA_IN_FILE,

    INVALID_DATA_ABOUT_USER,
    NO_SUCH_USER,
    NO_UNDO_ACTION
};

int init_liver(Liver** liver, char* name, char* surname, 
char* last_name, char* birthday, char gender, double wage);

int is_valid_liver(const Liver* liv);
int is_valid_liver_for_change(const Liver* liv);

int copy_liver(Liver* dest, Liver* source);
int insert_liver(Liver** head, Liver* new);

int find_liver(Liver* data_base, const Liver* needed, Liver** found);
int change_liver(Liver* liv_in_db, Liver* example);
int delete_liver(Liver** data_base, Liver* will_be_deleted);

int print_liver(FILE* stream, Liver* liv);
int print_liver_list(FILE* stream, Liver* head);

void free_liver_data(Liver* liver);
void free_liver_list(Liver* liver);

#endif