#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define HASHSIZE 128

// Структура для представления элемента в цепочке
struct KeyValue {
    char *key;
    char *value;
    struct KeyValue *next;
};

// Структура для представления хеш-таблицы
struct HashTable {
    struct KeyValue *table[HASHSIZE];
};

// Хэш-функция для строкового ключа
unsigned int hash(char *key) {
    unsigned int hashval = 0;
    int base = 62;  // основание системы счисления

    while (*key) {
        hashval = (hashval * base) + (*key++);
    }

    return hashval % HASHSIZE;
}

// Вставка пары ключ-значение в хеш-таблицу
void insert(struct HashTable *ht, char *key, char *value) {
    unsigned int index = hash(key);

    struct KeyValue *newPair = (struct KeyValue *)malloc(sizeof(struct KeyValue));
    if (!newPair) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    newPair->key = strdup(key);
    newPair->value = strdup(value);
    newPair->next = NULL;

    if (ht->table[index] == NULL) {
        ht->table[index] = newPair;
    } else {
        struct KeyValue *current = ht->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPair;
    }
}

// Отладочная функция для вывода содержимого хеш-таблицы
void printHashTable(struct HashTable *ht) {
    for (int i = 0; i < HASHSIZE; ++i) {
        struct KeyValue *current = ht->table[i];
        while (current != NULL) {
            printf("(%s, %s) ", current->key, current->value);
            current = current->next;
        }
        if (ht->table[i] != NULL) {
            printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <ключ> <значение>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct HashTable ht;
    memset(&ht, 0, sizeof(struct HashTable));

    // Вставка ключ-значение в хеш-таблицу
    insert(&ht, argv[1], argv[2]);

    // Вывод содержимого хеш-таблицы (отладочная информация)
    printf("Хеш-таблица:\n");
    printHashTable(&ht);

    return 0;
}
