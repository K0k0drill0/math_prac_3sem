#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* last_name;
    char* first_name;
    char* middle_name;
    int birth_day;
    int birth_month;
    int birth_year;
    char* gender;
    float income;
} Liver;

Liver* create_liver(char* last_name, char* first_name, char* middle_name,
                    int birth_day, int birth_month, int birth_year,
                    char* gender, float income) {
    Liver* liver = malloc(sizeof(Liver));
    liver->last_name = strdup(last_name);
    liver->first_name = strdup(first_name);
    liver->middle_name = middle_name ? strdup(middle_name) : NULL;
    liver->birth_day = birth_day;
    liver->birth_month = birth_month;
    liver->birth_year = birth_year;
    liver->gender = strdup(gender);
    liver->income = income;
    return liver;
}

void destroy_liver(Liver* liver) {
    free(liver->last_name);
    free(liver->first_name);
    free(liver->middle_name);
    free(liver->gender);
    free(liver);
}

char* liver_to_string(Liver* liver) {
    char* str;
    if (liver->middle_name) {
        asprintf(&str, "%s %s %s, %d.%d.%d, %s, %.2f",
                 liver->last_name, liver->first_name, liver->middle_name,
                 liver->birth_day, liver->birth_month, liver->birth_year,
                 liver->gender, liver->income);
    } else {
        asprintf(&str, "%s %s, %d.%d.%d, %s, %.2f",
                 liver->last_name, liver->first_name,
                 liver->birth_day, liver->birth_month, liver->birth_year,
                 liver->gender, liver->income);
    }
    return str;
}

bool liver_equal(Liver* liver1, Liver* liver2) {
    return strcmp(liver1->last_name, liver2->last_name) == 0 &&
           strcmp(liver1->first_name, liver2->first_name) == 0 &&
           (liver1->middle_name == NULL && liver2->middle_name == NULL ||
            liver1->middle_name != NULL && liver2->middle_name != NULL &&
            strcmp(liver1->middle_name, liver2->middle_name) == 0) &&
           liver1->birth_day == liver2->birth_day &&
           liver1->birth_month == liver2->birth_month &&
           liver1->birth_year == liver2->birth_year &&
           strcmp(liver1->gender, liver2->gender) == 0 &&
           liver1->income == liver2->income;
}

typedef struct {
    Liver** livers;
    int size;
    int capacity;
    char** history;
    int history_size;
    int history_capacity;
} Population;

Population* create_population() {
    Population* population = malloc(sizeof(Population));
    population->livers = malloc(10 * sizeof(Liver*));
    population->size = 0;
    population->capacity = 10;
    population->history = malloc(10 * sizeof(char*));
    population->history_size = 0;
    population->history_capacity = 10;
    return population;
}

void destroy_population(Population* population) {
    for (int i = 0; i < population->size; i++) {
        destroy_liver(population->livers[i]);
    }
    free(population->livers);
    for (int i = 0; i < population->history_size; i++) {
        free(population->history[i]);
    }
    free(population->history);
    free(population);
}

void population_read_file(Population* population, char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", file_path);
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* last_name = strtok(line, ",");
        char* first_name = strtok(NULL, ",");
        char* middle_name = strtok(NULL, ",");
        char* birth_date_str = strtok(NULL, ",");
        char* gender = strtok(NULL, ",");
        char* income_str = strtok(NULL, ",");
        int birth_day, birth_month, birth_year;
        sscanf(birth_date_str, "%d.%d.%d", &birth_day, &birth_month, &birth_year);
        float income = atof(income_str);
        Liver* liver = create_liver(last_name, first_name, middle_name,
                                    birth_day, birth_month, birth_year,
                                    gender, income);
        if (population->size == population->capacity) {
            population->capacity *= 2;
            population->livers = realloc(population->livers, population->capacity * sizeof(Liver*));
        }
        population->livers[population->size++] = liver;
    }
    fclose(file);
}

Liver** population_find_liver(Population* population, char* last_name, char* first_name,
                              char* middle_name, int birth_day, int birth_month,
                              int birth_year, char* gender, float income, int* result_size) {
    Liver** result = malloc(population->size * sizeof(Liver*));
    *result_size = 0;
    for (int i = 0; i < population->size; i++) {
        Liver* liver = population->livers[i];
        if ((!last_name || strcmp(liver->last_name, last_name) == 0) &&
            (!first_name || strcmp(liver->first_name, first_name) == 0) &&
            (!middle_name || liver->middle_name && strcmp(liver->middle_name, middle_name) == 0) &&
            (!birth_day || liver->birth_day == birth_day) &&
            (!birth_month || liver->birth_month == birth_month) &&
            (!birth_year || liver->birth_year == birth_year) &&
            (!gender || strcmp(liver->gender, gender) == 0) &&
            (!income || liver->income == income)) {
            result[(*result_size)++] = liver;
        }
    }
    return result;
}

void population_update_liver(Population* population, Liver* old_liver, Liver* new_liver) {
    for (int i = 0; i < population->size; i++) {
        if (population->livers[i] == old_liver) {
            population->livers[i] = new_liver;
            break;
        }
    }
    if (population->history_size == population->history_capacity) {
        population->history_capacity *= 2;
        population->history = realloc(population->history, population->history_capacity * sizeof(char*));
    }
    char* operation = "update";
    char* old_liver_str = liver_to_string(old_liver);
    char* new_liver_str = liver_to_string(new_liver);
    asprintf(&population->history[population->history_size++], "%s,%s,%s", operation, old_liver_str, new_liver_str);
    free(old_liver_str);
    free(new_liver_str);
}

void population_delete_liver(Population* population, Liver* liver) {
    for (int i = 0; i < population->size; i++) {
        if (population->livers[i] == liver) {
            for (int j = i; j < population->size - 1; j++) {
                population->livers[j] = population->livers[j + 1];
            }
            population->size--;
            break;
        }
    }
    if (population->history_size == population->history_capacity) {
        population->history_capacity *= 2;
        population->history = realloc(population->history, population->history_capacity * sizeof(char*));
    }
    char* operation = "delete";
    char* liver_str = liver_to_string(liver);
    asprintf(&population->history[population->history_size++], "%s,%s", operation, liver_str);
    free(liver_str);
}

void population_add_liver(Population* population, Liver* liver) {
    if (population->size == population->capacity) {
        population->capacity *= 2;
        population->livers = realloc(population->livers, population->capacity * sizeof(Liver*));
    }
    population->livers[population->size++] = liver;
    if (population->history_size == population->history_capacity) {
        population->history_capacity *= 2;
        population->history = realloc(population->history, population->history_capacity * sizeof(char*));
    }
    char* operation = "add";
    char* liver_str = liver_to_string(liver);
    asprintf(&population->history[population->history_size++], "%s,%s", operation, liver_str);
    free(liver_str);
}

void population_write_file(Population* population, char* file_path) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Failed to open file: %s\n", file_path);
        return;
    }
    for (int i = 0; i < population->size; i++) {
        Liver* liver = population->livers[i];
        char* liver_str = liver_to_string(liver);
        fprintf(file, "%s\n", liver_str);
        free(liver_str);
    }
    fclose(file);
}

void population_undo(Population* population) {
    int n = population->history_size;
    if (n >= 2) {
        for (int i = 0; i < n / 2; i++) {
            char* operation;
            char* old_liver_str;
            char* new_liver_str;
            sscanf(population->history[--population->history_size], "%m[^,],%m[^,],%m[^,]",
                   &operation, &old_liver_str, &new_liver_str);
            if (strcmp(operation, "add") == 0) {
                for (int j = 0; j < population->size; j++) {
                    if (strcmp(liver_to_string(population->livers[j]), old_liver_str) == 0) {
                        for (int k = j; k < population->size - 1; k++) {
                            population->livers[k] = population->livers[k + 1];
                        }
                        population->size--;
                        break;
                    }
                }
            } else if (strcmp(operation, "delete") == 0) {
                char* last_name = strtok(old_liver_str, " ");
                char* first_name = strtok(NULL, " ");
                char* middle_name = strtok(NULL, ",");
                char* birth_date_str = strtok(NULL, ",");
                char* gender = strtok(NULL, ",");
                char* income_str = strtok(NULL, ",");
                int birth_day, birth_month, birth_year;
                sscanf(birth_date_str, "%d.%d.%d", &birth_day, &birth_month, &birth_year);
                float income = atof(income_str);
                Liver* liver = create_liver(last_name, first_name, middle_name,
                                            birth_day, birth_month, birth_year,
                                            gender, income);
                if (population->size == population->capacity) {
                    population->capacity *= 2;
                    population->livers = realloc(population->livers, population->capacity * sizeof(Liver*));
                }
                population->livers[population->size++] = liver;
            } else if (strcmp(operation, "update") == 0) {
                char* old_last_name = strtok(old_liver_str, " ");
                char* old_first_name = strtok(NULL, " ");
                char* old_middle_name = strtok(NULL, ",");
                char* old_birth_date_str = strtok(NULL, ",");
                char* old_gender = strtok(NULL, ",");
                char* old_income_str = strtok(NULL, ",");
                int old_birth_day, old_birth_month, old_birth_year;
                sscanf(old_birth_date_str, "%d.%d.%d", &old_birth_day, &old_birth_month, &old_birth_year);
                float old_income = atof(old_income_str);
                Liver* old_liver = create_liver(old_last_name, old_first_name, old_middle_name,
                                                old_birth_day, old_birth_month, old_birth_year,
                                                old_gender, old_income);
                char* new_last_name = strtok(new_liver_str, " ");
                char* new_first_name = strtok(NULL, " ");
                char* new_middle_name = strtok(NULL, ",");
                char* new_birth_date_str = strtok(NULL, ",");
                char* new_gender = strtok(NULL, ",");
                char* new_income_str = strtok(NULL, ",");
                int new_birth_day, new_birth_month, new_birth_year;
                sscanf(new_birth_date_str, "%d.%d.%d", &new_birth_day, &new_birth_month, &new_birth_year);
                float new_income = atof(new_income_str);
                Liver* new_liver = create_liver(new_last_name, new_first_name, new_middle_name,
                                                new_birth_day, new_birth_month, new_birth_year,
                                                new_gender, new_income);
                for (int j = 0; j < population->size; j++) {
                    if (liver_equal(population->livers[j], new_liver)) {
                        population->livers[j] = old_liver;
                        break;
                    }
                }
                destroy_liver(new_liver);
            }
            free(operation);
            free(old_liver_str);
            free(new_liver_str);
        }
    }
}

int main() {
    Population* population = create_population();
    population_read_file(population, "data.txt");
    int result_size;
    Liver** result = population_find_liver(population, NULL, NULL, NULL, 0, 0, 0, NULL, 0.0, &result_size);
    for (int i = 0; i < result_size; i++) {
        char* liver_str = liver_to_string(result[i]);
        printf("%s\n", liver_str);
        free(liver_str);
    }
    for (int i = 0; i < result_size; i++) {
        destroy_liver(result[i]);
    }
    free(result);
    destroy_population(population);
    return 0;
}


