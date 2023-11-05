#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
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
    String receiving_time;
} Mail;

typedef struct {
    Address address;
    Mail* mails_arr;
} Post;

void free_everything(int amount, ...) {
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
}

void free_mails_arr(Mail* mails_arr, int* size) {
    for (int i = 0; i < *size; i++) {
        free_mail(mails_arr[i]);
    }
    *size = 0;
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

int get_mail(Mail* mail) {
    char *city = NULL, *street = NULL, *house_num = NULL, *housing = NULL, *apartment_num = NULL, *address_post_ind = NULL, *weight = NULL;
    char *mail_id = NULL;

    printf("Input info about mail:\n");
    printf("City of recipient: ");
    int st = get_string(&city);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (strlen(city) == 0) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Street of recipient: ");
    st = get_string(&street);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (strlen(street) == 0) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Number of house of recipient: ");
    st = get_string(&house_num);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (!is_valid_uint(house_num)) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Housing of recipient: ");
    st = get_string(&housing);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }

    printf("Apartment number of recipient: ");
    st = get_string(&apartment_num);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (!is_valid_uint(apartment_num)) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Post index of recipient: ");
    st = get_string(&address_post_ind);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (!is_valid_recipient_ind(address_post_ind)) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Weight of mail: ");
    st = get_string(&weight);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (!is_valid_udouble(weight)) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    printf("Mail index: ");
    st = get_string(&mail_id);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return st;
    }
    if (!is_valid_mail_id(mail_id)) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return INVALID_INPUT;
    }

    time_t timer = time(NULL);
    struct tm *temp_time = localtime(&timer);

    char* time_field = (char*)malloc(sizeof(char) * 20);
    if (time_field == NULL) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        return MEMORY_ISSUES;
    }
    sprintf(time_field, "%02d:%02d:%04d %02d:%02d:%02d\0",
            temp_time->tm_mday, temp_time->tm_mon + 1, temp_time->tm_year + 1900,
            temp_time->tm_hour, temp_time->tm_min, temp_time->tm_sec);

    st = string_create(&(mail->address.city), city);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->address.street), street);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    mail->address.house_num = atoi(house_num);
    
    st = string_create(&(mail->address.housing), housing);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    mail->address.apartment_num = atoi(apartment_num);
    
    st = string_create(&(mail->address.post_ind), address_post_ind);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    mail->weight = atof(weight);
    
    st = string_create(&(mail->mail_id), mail_id);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->creation_time), time_field);
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }

    st = string_create(&(mail->receiving_time), "Not yet");
    if (st != ok) {
        free_everything(8, city, street, house_num, housing, apartment_num, address_post_ind,
        weight, mail_id);
        // free MAIL!!!!
        return st;
    }
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
    printf("Receiving time: %s\n\n", mail->receiving_time.data);
}

void print_mails_arr(const Mail* mails_arr, const int arr_tmp_size) {
    for (int i = 0; i < arr_tmp_size; i++) {
        print_mail(&(mails_arr[i]));
    }
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

*/

int main() {
    int mails_arr_max_size = 16;
    int mails_arr_tmp_size = 0;
    Mail* mails_arr = (Mail*)malloc(sizeof(Mail) * mails_arr_max_size);
    if (mails_arr == NULL) {
        print_error(MEMORY_ISSUES);
        return 1;
    }
    int st = append_mail(&mails_arr, &mails_arr_tmp_size, &mails_arr_max_size);
    if (st != ok) {
        print_error(st);
        free_mails_arr(mails_arr, &mails_arr_tmp_size);
        return 1;
    }
    st = append_mail(&mails_arr, &mails_arr_tmp_size, &mails_arr_max_size);
    if (st != ok) {
        print_error(st);
        free_mails_arr(mails_arr, &mails_arr_tmp_size);
        return 1;
    }
    print_mails_arr(mails_arr, mails_arr_tmp_size);
    return 0;
}
