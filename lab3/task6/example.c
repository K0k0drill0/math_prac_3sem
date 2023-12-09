#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT,
    WRONG_AMOUNT_OF_ARGUMENTS
};

void print_error(int st) {
    switch (st)
    {
    case UNABLE_TO_OPEN_A_FILE:
        printf("Unable to open a file!\n");
        break;
    case INVALID_FILE:
        printf("Invalid file!\n");
        break;
    case INVALID_ARGUMENTS:
        printf("Invalid argument(s)!\n");
        break;
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input!\n");
        break;
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("Wrong amount of arguments!\n");
        break;
    default:
        break;
    }
}

typedef struct {
    double x, y;
} Coords;

typedef struct {
    char* number;
    char* arrivalTime;
    char* departureTime;
    char marker;
    Coords Coords;
} Stop;

typedef struct RouteNode {
    Stop stop;
    struct RouteNode* next;
} RouteNode;

typedef struct VehicleRoute {
    RouteNode route;
    struct VehicleRoute* next_route;
} VehicleRoute;

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int get_word_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        //free(*str_inp);
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                //free(str_inp);
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }

    if (ind == 0 && c == EOF) {
        return EOF;
    }
    
    (*str_inp)[ind] = '\0';
    return ok;
}

int read_stop_from_file(FILE* inp, Stop* stop) {
    char* num = NULL, *date1 = NULL, *time1 = NULL, *date2 = NULL,
     *time2 = NULL, *marker = NULL;

    int st;
    int st = get_word_from_file(inp, &num);
    if (get_word_from_file(inp, &num) || get_word_from_file(inp, &num) ||
    get_word_from_file(inp, &num) || get_word_from_file(inp, &num) || 
    get_word_from_file(inp, &num) || get_word_from_file(inp, &num)) {
        free_everything(6, num, date1, time1, date2, time2, marker);
        if (getc(inp) == EOF) {
            return EOF;
        }
        fseek(inp, -1, SEEK_CUR);
        return MEMORY_ISSUES;
    }

    stop->number = num;
    stop->arrivalTime = (char*)malloc(sizeof(char) * 20);
    if (stop->arrivalTime == NULL) {
        free_everything(6, num, date1, time1, date2, time2, marker);
        return MEMORY_ISSUES;
    }
    sprintf(stop->arrivalTime, "%s %s", date1, time1);

    stop->departureTime = (char*)malloc(sizeof(char) * 20);
    if (stop->departureTime == NULL) {
        free_everything(6, num, date1, time1, date2, time2, marker);
        return MEMORY_ISSUES;
    }
    sprintf(stop->departureTime, "%s %s", date2, time2);

    stop->marker = marker[0];

    free_everything(5, date1, time1, date2, time2, marker);
    return ok;
}

// Добавление остановки в пройденный маршрут
void addStopToRoute(VehicleRoute* vehicleRoute, Stop stop) {
    RouteNode* newNode = (RouteNode*)malloc(sizeof(RouteNode));
    newNode->stop = stop;
    newNode->next = NULL;

    if (vehicleRoute->route == NULL) {
        vehicleRoute->route = newNode;
    } else {
        RouteNode* current = vehicleRoute->route;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Поиск транспортного средства с наибольшим количеством маршрутов
void findVehicleWithMostRoutes(VehicleRoute* vehicleRoutes, int numRoutes) {
    int maxRoutes = 0;
    char maxVehicleNumber[20] = "";

    for (int i = 0; i < numRoutes; i++) {
        int numStops = 0;
        RouteNode* current = vehicleRoutes[i].route;
        while (current != NULL) {
            numStops++;
            current = current->next;
        }

        if (numStops > maxRoutes) {
            maxRoutes = numStops;
            strcpy(maxVehicleNumber, vehicleRoutes[i].location);
        }
    }

    printf("Транспортное средство с наибольшим количеством маршрутов: %s\n", maxVehicleNumber);
}

void free_everything(int amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
}

int main() {
    // Пути к файлам с данными остановочных пунктов
    char* filePaths[] = {"file1.txt", "file2.txt", "file3.txt"};

    // Ваш код для обработки данных с остановочных пунктов

    int numRoutes = sizeof(filePaths) / sizeof(filePaths[0]);
    VehicleRoute* vehicleRoutes = (VehicleRoute*)malloc(numRoutes * sizeof(VehicleRoute));

    // Инициализация данных о маршрутах транспортных средств
    for (int i = 0; i < numRoutes; i++) {
        strcpy(vehicleRoutes[i].location, filePaths[i]);
        vehicleRoutes[i].route = NULL;
    }

    // Взаимодействие с пользователем
    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Поиск транспортного средства с наибольшим количеством маршрутов\n");
        printf("2. Поиск транспортного средства, проехавшего больше всех маршрутов\n");
        printf("3. Поиск транспортного средства с самым длинным маршрутом\n");
        printf("4. Поиск транспортного средства с самым коротким маршрутом\n");
        printf("5. Поиск транспортного средства с самой длинной остановкой\n");
        printf("6. Поиск транспортного средства с самой короткой остановкой\n");
        printf("7. Поиск транспортного средства с самым большим временем простоя\n");
        printf("8. Поиск транспортного средства с самым меньшим временем простоя\n");
        printf("9. Выход\n");
        printf("Введите номер команды: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                findVehicleWithMostRoutes(vehicleRoutes, numRoutes);
                break;
            // ... Вызовите соответствующие функции в зависимости от выбора пользователя ...
            case 9:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Некорректный ввод. Попробуйте снова.\n");
                break;
        }

    } while (choice != 9);

    // Освобождение памяти
    for (int i = 0; i < numRoutes; i++) {
        RouteNode* current = vehicleRoutes[i].route;
        while (current != NULL) {
            RouteNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(vehicleRoutes);

    return 0;
}