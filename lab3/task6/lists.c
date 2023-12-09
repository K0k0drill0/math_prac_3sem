#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

typedef long long ll;


int cmp_time(const char* a, const char* b) {
    for (int i = 0; i < 4; i++) {
        if (a[i+6] != b[i+6]) {
            return a[i+6] < b[i+6] ? -1 : 1;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (a[i+3] != b[i+3]) {
            return a[i+6] < b[i+6] ? -1 : 1;
        }
    }
    return strcmp(a, b);
}

int init_bus(Bus** bus, char* bus_number, Stop* route) {
	if (bus_number == NULL || route == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	*bus = (Bus*)malloc(sizeof(Bus));
	if (*bus == NULL) {
		return MEMORY_ISSUES;
	}

	(*bus)->number = bus_number;
	(*bus)->route = route;
	(*bus)->next = NULL;

	return ok;
}

int init_stop(Stop** st, char* arrived, char* departed,
 const char state, const Coords crd) {
	if (arrived == NULL || departed == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	*st = (Stop*)malloc(sizeof(Stop));
	if (*st == NULL) {
		return MEMORY_ISSUES;
	}

	(*st)->arrived = arrived;
	(*st)->departed = departed;
	(*st)->state = state;
	(*st)->coords = crd;
	(*st)->next = NULL;

	return ok;
}

int init_and_append_stop(Bus** bus, char* bus_name, char* arrived,
 char* departed, const char state, const Coords crds) {
	Stop* new_st = NULL;
	int st = init_stop(&new_st, arrived, departed, state, crds);
	if (st != ok) {
		free(bus_name);
		free(arrived);
		free(departed);
		return st;
	}

	st = append_stop_in_bus_list(bus, bus_name, new_st);
	if (st != ok) {
		free(bus_name);
		free(arrived);
		free(departed);
		free(new_st);
		return st;
	}
	return ok;
}

int append_stop_in_bus_list(Bus** bus, char* bus_name,
 Stop* stop) {
	int st = ok;
	Bus* prev = *bus;

	if (prev == NULL) {
		return init_bus(bus, bus_name, stop);
	}

	if (!strcmp(bus_name, prev->number)) {
		free(bus_name);
		return append_stop_in_list(&(prev->route), stop);
	}

	Bus* tmp = prev->next;

	while (tmp != NULL && strcmp(bus_name, tmp->number)) {
		prev = tmp;
		tmp = prev->next;
	}

	if (tmp == NULL) {
		Bus* new_node = NULL;
		st = init_bus(&new_node, bus_name, stop);
		if (st != ok) {
			return st;
		}
		prev->next = new_node;
	}
	else {
		free(bus_name);
		return append_stop_in_list(&(tmp->route), stop);
	}
	return ok;
}

int append_stop_in_list(Stop** head, Stop* stop) {
	if (stop == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	if (*head == NULL) {
		*head = stop;
		return ok;
	}

	Stop* prev = *head;

	if (cmp_time(prev->arrived, stop->arrived) == 1) {
		Stop* ex_h = (*head);
		*head = stop;
		(*head)->next = ex_h;
		return ok;
	}

	Stop* tmp = prev->next;

	while (tmp != NULL && cmp_time(tmp->arrived,  stop->arrived) != 1) {
		prev = tmp;
		tmp = tmp->next;
	}

	if (tmp == NULL) {
		tmp = stop;
		prev->next = tmp;
	}
	else {
		prev->next = stop;
		stop->next = tmp;
	}

	return ok;
}

// int find_bus_in_list_by_number(const Bus* head, const char* number, Bus** ans) {
// 	Bus* tmp = head;
// 	while (tmp != NULL && strcmp(tmp->number, number)) {
// 		tmp = tmp->next;
// 	}

// 	if (tmp == NULL) {
// 		*ans = NULL;
// 	}
// 	else {
// 		*ans = tmp;
// 	}

// 	return ok;
// }

int print_stop(const Stop* st) {
	if (st == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	printf("Arrival time: %s\n", st->arrived);
	printf("Departure time: %s\n", st->departed);
	printf("State: %c\n", st->state);
	printf("Coordinates: x = %d, y = %d\n\n", st->coords.x, st->coords.y);

	return ok;
}

int print_stop_list(Stop* head) {
	if (head == NULL) {
		printf("The list is empty!\n");
		return ok;
	}
	Stop* tmp = head;
	while (tmp != NULL) {
		int st = print_stop(tmp);
		if (st != ok) {
			return st;
		}
		tmp = tmp->next;
	}
	return ok;
}

int print_bus(const Bus* bus) {
	if (bus == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	printf("Number of bus: %s\n\n", bus->number);
	printf("Route of bus: \n");
	print_stop_list(bus->route);
	printf("\n");
	return ok;
}

int print_bus_list(Bus* head) {
	if (head == NULL) {
		printf("The list is empty!\n");
		return ok;
	}
	Bus* tmp = head;
	while (tmp != NULL) {
		int st = print_bus(tmp);
		if (st != ok) {
			return st;
		}
		tmp = tmp->next;
	}
	return ok;
}

void free_stop(Stop* st) {
	if (st == NULL) {
		return;
	}
	free(st->arrived);
	free(st->departed);
	free(st);
}

void free_stop_list(Stop* head) {
	Stop* prev = head;
	
	if (prev == NULL) {
		return;
	}
	free(prev->arrived);
	free(prev->departed);
	Stop* tmp = prev->next;
	free(prev);

	while (tmp != NULL) {
		prev = tmp;
		free(prev->arrived);
		free(prev->departed);
		tmp = tmp->next;
		free(prev);
	}
}

void free_bus_list(Bus* bus) {
	Bus* prev = bus;
	if (prev == NULL) {
		return;
	}

	Bus* tmp = prev->next;
	free(prev->number);
	free_stop_list(prev->route);
	free(prev);
	while (tmp != NULL) {
		prev = tmp;
		tmp = prev->next;
		free(prev->number);
		free_stop_list(prev->route);
		free(prev);
	}
}

/*____________________________finders______________________________________*/

/*
1. Количество маршрутов:
	Количиство финишей. Нет ни одного финиша - писька
2. Длина пути: 
	Расстояния между соседними остановками. Остановка одна у всех - писька
3. Длина маршрута:
	Каждый финиш обнуляем счетчик. Если нет финишей, то писька
4. Время остановки:
	Тут в любом случае что-то найдется.
5. Простой - время между финишем и стартом
*/

int most_routes(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int most_routes = 0;

	while (tmp != NULL) {
		Stop* route = tmp->route;
		int tmp_routes = 0;
		while (route != NULL) {
			if (route->state == 'F') {
				tmp_routes++;
			}
			route = route->next;
		}
		if (tmp_routes > most_routes) {
			most_routes = tmp_routes;
			bus_name = tmp->number;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int least_routes(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int least_routes = 1e9;

	while (tmp != NULL) {
		Stop* route = tmp->route;
		int tmp_routes = 0;
		while (route != NULL) {
			if (route->state == 'F') {
				tmp_routes++;
			}
			route = route->next;
		}
		if (tmp_routes < least_routes) {
			least_routes = tmp_routes;
			bus_name = tmp->number;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int distance(Coords a, Coords b) {
	return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

int longest_path(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int longest_path = 0;

	while (tmp != NULL) {
		Stop* prev_stop = tmp->route;
		Stop* tmp_stop = prev_stop->next;
		int tmp_path_lenth = 0;
		
		while (tmp_stop != NULL) {
			tmp_path_lenth += distance(prev_stop->coords, tmp_stop->coords);
			prev_stop = tmp_stop;
			tmp_stop = tmp_stop->next;
		}

		if (tmp_path_lenth > longest_path) {
			longest_path = tmp_path_lenth;
			bus_name = tmp->number;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int shortest_path(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int shortest_path = 1e9;

	while (tmp != NULL) {
		Stop* prev_stop = tmp->route;
		Stop* tmp_stop = prev_stop->next;
		int tmp_path_lenth = 0;
		
		while (tmp_stop != NULL) {
			tmp_path_lenth += distance(prev_stop->coords, tmp_stop->coords);
			prev_stop = tmp_stop;
			tmp_stop = tmp_stop->next;
		}

		if (tmp_path_lenth < shortest_path) {
			shortest_path = tmp_path_lenth;
			bus_name = tmp->number;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int longest_route(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int longest_route = 0;

	while (tmp != NULL) {
		Stop* prev_stop = tmp->route;
		Stop* tmp_stop = prev_stop->next;
		int tmp_route_lenth = 0;

		while (tmp_stop != NULL) {
			tmp_route_lenth += distance(prev_stop->coords, tmp_stop->coords);
			if (tmp_stop->state == 'F') {
				if (tmp_route_lenth > longest_route) {
					longest_route = tmp_route_lenth;
					bus_name = tmp->number;
				}
			}
			prev_stop = tmp_stop;
			tmp_stop = tmp_stop->next;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int shortest_route(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	int shortest_route = 1e9;

	while (tmp != NULL) {
		Stop* prev_stop = tmp->route;
		Stop* tmp_stop = prev_stop->next;
		int tmp_route_lenth = 0;

		while (tmp_stop != NULL) {
			tmp_route_lenth += distance(prev_stop->coords, tmp_stop->coords);
			if (tmp_stop->state == 'F') {
				if (tmp_route_lenth < shortest_route) {
					shortest_route = tmp_route_lenth;
					bus_name = tmp->number;
				}
			}
			prev_stop = tmp_stop;
			tmp_stop = tmp_stop->next;
		}

		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int parse_date(char* date_str, int* day, int* month, int* year, int* hours, int* minutes, int* seconds) {
    return sscanf(date_str, "%d.%d.%d %d:%d:%d", day, month, year, hours, minutes, seconds) == 6;
}

int days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
    }
    return days[month - 1];
}

int seconds_elapsed(char* start_date_str, char* end_date_str, ll* ans) {
    int start_day, start_month, start_year, start_hours, start_minutes, start_seconds;
    int end_day, end_month, end_year, end_hours, end_minutes, end_seconds;

    if (!parse_date(start_date_str, &start_day, &start_month, &start_year, &start_hours, &start_minutes, &start_seconds) ||
        !parse_date(end_date_str, &end_day, &end_month, &end_year, &end_hours, &end_minutes, &end_seconds)) {
        return BRUH; 
    }

    const int seconds_in_day = 24 * 60 * 60;
    const int seconds_in_hour = 60 * 60;
    const int seconds_in_minute = 60;

    int start_total_seconds = start_seconds + start_minutes * seconds_in_minute + start_hours * seconds_in_hour + start_day * seconds_in_day;
    int end_total_seconds = end_seconds + end_minutes * seconds_in_minute + end_hours * seconds_in_hour + end_day * seconds_in_day;

    if (start_total_seconds > end_total_seconds) {
        return BRUH; 
    }

    *ans = end_total_seconds - start_total_seconds;

    return ok; 
}

int longest_stop(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	ll longest_stop = 0;

	while (tmp != NULL) {
		Stop* tmp_stop = tmp->route;
		ll tmp_time = 0;
		while (tmp_stop != NULL) {
			seconds_elapsed(tmp_stop->arrived, tmp_stop->departed, &tmp_time);
			if (tmp_time > longest_stop) {
				bus_name = tmp->number;
				longest_stop = tmp_time;
			}
			tmp_stop = tmp_stop->next;
		}
		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int shortest_stop(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	ll shortest_stop = 1e18;

	while (tmp != NULL) {
		Stop* tmp_stop = tmp->route;
		ll tmp_time = 0;
		while (tmp_stop != NULL) {
			seconds_elapsed(tmp_stop->arrived, tmp_stop->departed, &tmp_time);
			if (tmp_time < shortest_stop) {
				bus_name = tmp->number;
				shortest_stop = tmp_time;
			}
			tmp_stop = tmp_stop->next;
		}
		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int longest_own_stop(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	ll longest_own_stop = 0;

	while (tmp != NULL) {
		Stop* prev = tmp->route;
		Stop* tmp_stop = prev->next;
		ll tmp_own_stop = 0;

		while (tmp_stop != NULL) {
			if (prev->state == 'F' && tmp_stop->state == 'S') {
				seconds_elapsed(prev->departed, tmp_stop->arrived, &tmp_own_stop);
				if (tmp_own_stop > longest_own_stop) {
					bus_name = tmp->number;
					longest_own_stop = tmp_own_stop;
				}
			}
			tmp_stop = tmp_stop->next;
		} 
		prev = tmp_stop;
		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}

int shortest_own_stop(Bus* data_base, char** ans) {
	if (data_base == NULL) {
		*ans = NULL;
		return NO_SUCH_BUS;
	}

	Bus* tmp = data_base;
	char* bus_name = tmp->number;
	ll shortest_own_stop = 1e18;

	while (tmp != NULL) {
		Stop* prev = tmp->route;
		Stop* tmp_stop = prev->next;
		ll tmp_own_stop = 0;

		while (tmp_stop != NULL) {
			if (prev->state == 'F' && tmp_stop->state == 'S') {
				seconds_elapsed(prev->departed, tmp_stop->arrived, &tmp_own_stop);
				if (tmp_own_stop < shortest_own_stop) {
					bus_name = tmp->number;
					shortest_own_stop = tmp_own_stop;
				}
			}
			prev = tmp_stop;
			tmp_stop = tmp_stop->next;
		} 
		tmp = tmp->next;
	}

	*ans = bus_name;
	return ok;
}