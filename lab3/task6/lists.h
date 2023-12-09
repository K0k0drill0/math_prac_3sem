#ifndef LISTS_H
#define LISTS_H

typedef struct Coords {
	int x, y;
} Coords;

typedef struct Stop {
	char* arrived;
	char* departed;
	char state;

	Coords coords;

	struct Stop* next;
} Stop;

typedef struct Bus {
	char* number;
	Stop* route;

	struct Bus* next;
} Bus;

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
	INVALID_STATE,
	INVALID_COORDS,

	NO_SUCH_BUS,
	BRUH
};

int cmp_time(const char* a, const char* b);

int init_bus(Bus** bus, char* bus_number, Stop* route);
int init_stop(Stop** st, char* arrived, char* departed,
 const char state, const Coords crd);

int init_and_append_stop(Bus** bus, char* bus_name, char* arrived,
 char* departed, const char state, const Coords crds);
int append_stop_in_bus_list(Bus** bus, char* bus_name,
 Stop* stop);
int append_stop_in_list(Stop** head, Stop* stop);

int print_stop(const Stop* st);
int print_stop_list(Stop* head);
int print_bus(const Bus* bus);
int print_bus_list(Bus* head) ;

void free_stop(Stop* st);
void free_stop_list(Stop* head);
void free_bus_list(Bus* bus);

/*____________________________finders______________________________________*/

int most_routes(Bus* data_base, char** ans);
int least_routes(Bus* data_base, char** ans);

int longest_path(Bus* data_base, char** ans);
int shortest_path(Bus* data_base, char** ans);

int longest_route(Bus* data_base, char** ans);
int shortest_route(Bus* data_base, char** ans);

int longest_stop(Bus* data_base, char** ans);
int shortest_stop(Bus* data_base, char** ans);

int longest_own_stop(Bus* data_base, char** ans);
int shortest_own_stop(Bus* data_base, char** ans);

#endif