#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>  
#include "String.h"
#include "Validations.h"

typedef struct {
    String city;
    String street;
    unsigned int house_num;
    String housing;
    unsigned int apartment_num;
    String post_ind;
} Address;

typedef struct {
    Address address;
    double weight;
    String mail_id;
    String creation_time;
    String prognose_for_delivery;
    String receiving_time;
} Mail;

typedef struct {
    Address address;
    Mail* mails_arr;
} Post;

void print_mails_arr(const Mail* mails_arr, const int arr_tmp_size);

void free_everything(unsigned int amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
    va_end(args);
}

void free_address(Address adr) {
    string_delete(&(adr.city));
    string_delete(&(adr.street));
    string_delete(&(adr.housing));
    string_delete(&(adr.post_ind));
}

void free_mail(Mail mail) {
    free_address(mail.address);
    string_delete(&(mail.creation_time));
    string_delete(&(mail.mail_id));
    string_delete(&(mail.receiving_time));
    string_delete(&(mail.prognose_for_delivery));
}

void free_mails_arr(Mail* mails_arr, int* size) {
    for (int i = 0; i < *size; i++) {
        free_mail(mails_arr[i]);
    }
    *size = 0;
    free(mails_arr);
}

int get_string(char** str) {
    int str_max_size = 16;
    int str_tmp_size = 0;
    *str = (char*)malloc(sizeof(char) * str_max_size);
    if (*str == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    while ((c = getc(stdin)) != '\n') {
        if (str_tmp_size + 1 == str_max_size) {
            str_max_size *= 2;
            char* tmp = (char*)realloc(*str, sizeof(char) * str_max_size);
            if (tmp == NULL) {
                free(*str);
                return MEMORY_ISSUES;
            }
            *str = tmp;
        }
        (*str)[str_tmp_size] = c;
        str_tmp_size++;
    }
    (*str)[str_tmp_size] = '\0';
    return ok;
}

int tm_to_string(struct tm *temp_time, char** time_field) {
    *time_field = (char*)malloc(sizeof(char) * 20);
    if (*time_field == NULL) {
        return MEMORY_ISSUES;
    }
    sprintf(*time_field, "%02d:%02d:%04d %02d:%02d:%02d",
            temp_time->tm_mday, temp_time->tm_mon + 1, temp_time->tm_year + 1900,
            temp_time->tm_hour, temp_time->tm_min, temp_time->tm_sec);
    return ok;
}

int compare_mails(const void* a, const void* b) {
    Mail* mail1 = (Mail*)a;
    Mail* mail2 = (Mail*)b;
    int indexCmp = string_compare(mail1->address.post_ind, mail2->address.post_ind);
    if (indexCmp == 0) {
        return string_compare(mail1->mail_id, mail2->mail_id);
    }
    return indexCmp;
}

int cmp_time(const String a, const String b) {
    for (int i = 0; i < 4; i++) {
        if (a.data[i+6] != b.data[i+6]) {
            return a.data[i+6] < b.data[i+6] ? -1 : 1;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (a.data[i+3] != b.data[i+3]) {
            return a.data[i+6] < b.data[i+6] ? -1 : 1;
        }
    }
    return string_compare(a, b);
}

int compare_mails_by_creation_time(const void* a, const void* b) {
    Mail* mail1 = (Mail*)a;
    Mail* mail2 = (Mail*)b;
    return cmp_time(mail1->receiving_time, mail2->receiving_time);
    //return string_compare(mail1->receiving_time, mail2->receiving_time);
}

int get_mail(Mail* mail) {
    char *city = NULL, *street = NULL, *house_num = NULL, *housing = NULL, *apartment_num = NULL, *address_post_ind = NULL, *weight = NULL;
    char *mail_id = NULL, *time_needed_for_delivery = NULL, *time_field = NULL;

    printf("Input info about mail:\n");
    printf("City of recipient: ");
    int st = get_string(&city);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (strlen(city) == 0) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Street of recipient: ");
    st = get_string(&street);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (strlen(street) == 0) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Number of house of recipient: ");
    st = get_string(&house_num);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_uint(house_num)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Housing of recipient: ");
    st = get_string(&housing);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }

    printf("Apartment number of recipient: ");
    st = get_string(&apartment_num);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_uint(apartment_num)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Post index of recipient: ");
    st = get_string(&address_post_ind);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_recipient_ind(address_post_ind)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Weight of mail: ");
    st = get_string(&weight);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_udouble(weight)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("Mail index: ");
    st = get_string(&mail_id);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_mail_id(mail_id)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    printf("What time is needed for delivery (in seconds)? ");
    st = get_string(&time_needed_for_delivery);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    if (!is_valid_uint(time_needed_for_delivery)) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return INVALID_INPUT;
    }

    time_t timer = time(NULL);
    st = tm_to_string(localtime(&timer), &time_field);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->address.city), city);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->address.street), street);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    mail->address.house_num = atoi(house_num);
    if (errno == ERANGE) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return OVERFLOWED;
    }
    
    st = string_create(&(mail->address.housing), housing);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    mail->address.apartment_num = atoi(apartment_num);
        if (errno == ERANGE) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return OVERFLOWED;
    }
    
    st = string_create(&(mail->address.post_ind), address_post_ind);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    mail->weight = atof(weight);
    
    st = string_create(&(mail->mail_id), mail_id);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->creation_time), time_field);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    free(time_field);
    timer += atoi(time_needed_for_delivery);
    if (errno == ERANGE) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return OVERFLOWED;
    }
    st = tm_to_string(localtime(&timer), &time_field);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        return st;
    }
    st = string_create(&(mail->prognose_for_delivery), time_field);
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->receiving_time), "Not yet");
    if (st != ok) {
        free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
        // free MAIL!!!!
        return st;
    }
    free_everything(10, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id, time_field, time_needed_for_delivery);
    return ok;
}

int append_mail(Mail** mails_arr, int* mails_tmp_amount, int* mails_max_amount) {
    Mail mail;
    int st = get_mail(&mail);
    if (st != ok) {
        return st;
    }

    if (*mails_tmp_amount == *mails_max_amount) {
        *mails_max_amount *= 2;
        Mail* tmp = (Mail*)realloc(*mails_arr, sizeof(Mail) * *mails_max_amount);
        if (!tmp) {
            free_mail(mail);
            return MEMORY_ISSUES;
        }
        *mails_arr = tmp;
    }

    (*mails_arr)[*mails_tmp_amount] = mail;
    (*mails_tmp_amount)++;

    //print_mails_arr(*mails_arr, *mails_tmp_amount);
    qsort(*mails_arr, *mails_tmp_amount, sizeof(Mail), compare_mails);

    return ok;
}

int find_mail_by_ind(const Mail* mails_arr, const int mails_tmp_amount, String mail_id) { // возвращает индекс в массиве
    for (int i = 0; i < mails_tmp_amount; i++) {
        if (string_equal(mails_arr[i].mail_id, mail_id)) {
            return i;
        }
    }
    return mails_tmp_amount;
}

int get_mail_id(String* mail_id) {
    printf("Input mail id: ");
    char* str = NULL;
    int st = get_string(&str);
    if (st != ok) {
        free(str);
        return st;
    }
    st = string_create(mail_id, str);
    if (st != ok) {
        free(str);
        return st;
    }
    free(str);
    return ok;
}

int adding_info_received(Mail* mails_arr, const int mails_tmp_amount) {
    String mail_id;
    mail_id.data = NULL;
    int st = get_mail_id(&mail_id);
    if (st != ok) {
        string_delete(&mail_id);
        return st;
    }

    int ind = find_mail_by_ind(mails_arr, mails_tmp_amount, mail_id);
    if (ind == mails_tmp_amount) {
        string_delete(&mail_id);
        return INVALID_INPUT;
    }
    string_delete(&(mails_arr[ind].receiving_time));
    time_t timer = time(NULL);
    char* time_field = NULL;
    st = tm_to_string(localtime(&timer), &time_field);
    if (st != ok) {
        string_delete(&mail_id);
        free(time_field);
        return st;
    }
    st = string_create(&(mails_arr[ind].receiving_time), time_field);
    if (st != ok) {
        string_delete(&mail_id);
        free(time_field);
        return st;
    }
    string_delete(&mail_id);
    free(time_field);
    return ok;
}

int delete_mail(Mail* mails_arr, int* mails_tmp_amount) {
    String mail_id;
    mail_id.data = NULL;
    int st = get_mail_id(&mail_id);
    if (st != ok) {
        string_delete(&mail_id);
        return st;
    }

    int ind = find_mail_by_ind(mails_arr, *mails_tmp_amount, mail_id);
    if (ind == *mails_tmp_amount) {
        string_delete(&mail_id);
        return INVALID_INPUT;
    }

    free_mail(mails_arr[ind]);

    for (int i = ind+1; i < *mails_tmp_amount; i++) {
        mails_arr[i-1] = mails_arr[i];
    }

    (*mails_tmp_amount)--;
    return ok;
}

int find_all_delivered(const Mail* mails_arr, const int mails_tmp_amount) {
    Mail* mails_delivered = (Mail*)malloc(sizeof(Mail) * mails_tmp_amount);
    int mails_delivered_size = 0;
    if (mails_delivered == NULL) {
        return MEMORY_ISSUES;
    }

    String not_yet;
    int st = string_create(&not_yet, "Not yet");
    if (st != ok) {
        free_mails_arr(mails_delivered, &mails_delivered_size);
        return st;
    }

    for (int i = 0; i < mails_tmp_amount; i++) {
        if (!string_equal(not_yet, mails_arr[i].receiving_time)) {
            memcpy(&(mails_delivered[mails_delivered_size]), &(mails_arr[i]), sizeof(Mail));
            //mails_delivered[mails_delivered_size] = mails_arr[i];
            mails_delivered_size++;
            // st = append_mail(&mails_delivered, &mails_delivered_size, &mails_delivered_size);
            // if (st != ok) {
            //     free_mails_arr(mails_delivered, &mails_delivered_size);
            // }
        }
    }
    qsort(mails_delivered, mails_delivered_size, sizeof(Mail), compare_mails_by_creation_time);

    print_mails_arr(mails_delivered, mails_delivered_size);
    free(mails_delivered);
    string_delete(&not_yet);
    //free_mails_arr(mails_delivered, &mails_delivered_size);
    return ok;
}

int find_all_expired(const Mail* mails_arr, const int mails_tmp_amount) {
    Mail* mails_expired = (Mail*)malloc(sizeof(Mail) * mails_tmp_amount);
    int mails_expired_size = 0;
    if (mails_expired == NULL) {
        return MEMORY_ISSUES;
    }

    String not_yet;
    int st = string_create(&not_yet, "Not yet");
    if (st != ok) {
        free_mails_arr(mails_expired, &mails_expired_size);
        return st;
    }

    char* time_field;
    time_t timer = time(NULL);
    struct tm *temp_time = localtime(&timer);
    st = tm_to_string(temp_time, &time_field);
    for (int i = 0; i < mails_tmp_amount; i++) {
        if (string_equal(not_yet, mails_arr[i].receiving_time) && strcmp(time_field, mails_arr[i].prognose_for_delivery.data)) {
            memcpy(&(mails_expired[mails_expired_size]), &(mails_arr[i]), sizeof(Mail));
            //mails_delivered[mails_delivered_size] = mails_arr[i];
            mails_expired_size++;
            // st = append_mail(&mails_delivered, &mails_delivered_size, &mails_delivered_size);
            // if (st != ok) {
            //     free_mails_arr(mails_delivered, &mails_delivered_size);
            // }
        }
    }
    qsort(mails_expired, mails_expired_size, sizeof(Mail), compare_mails_by_creation_time);

    print_mails_arr(mails_expired, mails_expired_size);
    free(time_field);
    free(mails_expired);
    string_delete(&not_yet);
    //free_mails_arr(mails_delivered, &mails_delivered_size);
    return ok;
}

void print_mail(const Mail* mail) {
    printf("Mail id: %s\n", mail->mail_id.data);
    printf("Address of recipient:\n");
    printf("\tCity: %s\n", mail->address.city.data);
    printf("\tStreet: %s\n", mail->address.street.data);
    printf("\tHouse number: %u\n", mail->address.house_num);
    printf("\tHousing: %s\n", mail->address.housing.data);
    printf("\tApartment number: %u\n", mail->address.apartment_num);
    printf("\tRecipient post index: %s\n", mail->address.post_ind.data);
    printf("Weight of mail: %.2f\n", mail->weight);
    printf("Creation time: %s\n", mail->creation_time.data);
    printf("Prognosed delivery time: %s\n", mail->prognose_for_delivery.data);
    printf("Receiving time: %s\n\n", mail->receiving_time.data);
}

void print_mails_arr(const Mail* mails_arr, const int arr_tmp_size) {
    if (arr_tmp_size == 0) {
        printf("The array is empty!\n");
        return;
    }
    for (int i = 0; i < arr_tmp_size; i++) {
        print_mail(&(mails_arr[i]));
    }
}

void print_menu() {
    printf("1. Add mail\n");
    printf("2. Delete mail\n");
    printf("3. Add recieving date for mail\n");
    printf("4. Find by mail id\n");
    printf("5. Find all delivered mails\n");
    printf("6. Find all expired mails.\n");
    printf("0. Exit.\n");
}

/*
Че должен уметь:
добавить mail в post
удалить mail из post
Поиск mail по идентификатору и информативный вывод о письме

mail должны быть посортированы!! (Сначала по индексу получателя,
потом по идентификатору посылки)

Поиск всех доставленных отправлений
Поиск всех отправлений, срок доставки которых на текущий момент
 времени истек.
(Информацию о доставленных\недоставленных отправлениях выводить 
в порядке времени создания по возрастанию.)

Варианты действия для пользователя:
1. Добавить посылку. При добавлении должно случайно выбираться
время, к которому предположительно будет доставлена посылка (10-30 секунды). DONE
НЕ СЛУЧАЙНО. Даем выбрать пользователю, время доставки в секундах.
2. Добавить информацию о том, что посылка была доставлена. 
Доступ к письму по ид письма. Время доставки - текущее DONE
3. Удаление письма. Доступ по ид письма. После удаления сдвинуть друг
к другу элементы массива
4. Поиск письма по идентификатору DONE
5. Поиск всех доставленных отправлений. Во время поиска смотрим по порядку письма.
Если оно доставлено, то смотрим время доставки. Ищем то, которое наиболее
близко к последнему найденному, но при этом больше (от меньшего к большему)
6. Поиск всех отправлений, срок доставки которых на текущий момент времени
истек. Если не доставлено, то смотрим спрогнозированное время доставки.
Если оно еще не наступило, то выводим (от меньшего к большему.)
*/

int main() {

    int mails_arr_max_size = 16;
    int mails_arr_tmp_size = 0;
    Mail* mails_arr = (Mail*)malloc(sizeof(Mail) * mails_arr_max_size);
    if (mails_arr == NULL) {
        print_error(MEMORY_ISSUES);
        return 1;
    }
    
    print_menu();

    char* q = NULL;
    int st = ok;
    while (1) {
        printf("Your action: ");
        free(q);
        st = get_string(&q);
        if (st != ok) {
            break;
        }
        if (strlen(q) != 1) {
            print_error(INVALID_INPUT);
            continue;
        }

        if (q[0] == '1') {
            st = append_mail(&mails_arr, &mails_arr_tmp_size, &mails_arr_max_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
            
        }
        else if (q[0] == '2') {
            st = delete_mail(mails_arr, &mails_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
        }
        else if (q[0] == '3') {
            st = adding_info_received(mails_arr, mails_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
        }
        else if (q[0] == '4') {
            String mail_id;
            st = get_mail_id(&mail_id);
            if (st != ok) {
                string_delete(&mail_id);
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
            int ind = find_mail_by_ind(mails_arr, mails_arr_tmp_size, mail_id);
            if (ind == mails_arr_tmp_size) {
                printf("There are no such element!\n");
            }
            else {
                print_mail(&(mails_arr[ind]));
            }
            string_delete(&mail_id);
        }
        else if (q[0] == '5') {
            st = find_all_delivered(mails_arr, mails_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
        }
        else if (q[0] == '6') {
            st = find_all_expired(mails_arr, mails_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            } 
        }
        else if (q[0] == '0') {
            break;
        }
        else {
            print_error(INVALID_INPUT);
        }
        //free(q);
        //q = NULL;
    }
    if (st != ok) {
        print_error(st);
        free_mails_arr(mails_arr, &mails_arr_tmp_size);
        free(q);
        return 1;
    }
    free_mails_arr(mails_arr, &mails_arr_tmp_size);
    free(q);
    return 0;
}