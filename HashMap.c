#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Define a structure for key-value pairs
typedef struct KeyValue {
    char* key;
    char* value;
    struct KeyValue* next; // For handling collisions using chaining
} KeyValue;

// Define a hash table structure
typedef struct HashTable {
    KeyValue** table;
} HashTable;

// Function to create a hash table
HashTable* createHashTable() {
    HashTable* hashTable = malloc(sizeof(HashTable));
    hashTable->table = malloc(sizeof(KeyValue*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

// Hash function to calculate index for a given key
unsigned int hash(const char* key) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

// Function to insert a key-value pair
void insert(HashTable* hashTable, const char* key, const char* value) {
    unsigned int index = hash(key);
    KeyValue* newNode = malloc(sizeof(KeyValue));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}

// Function to search for a value by key
char* search(HashTable* hashTable, const char* key) {
    unsigned int index = hash(key);
    KeyValue* current = hashTable->table[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

// Function to delete a key-value pair
void delete(HashTable* hashTable, const char* key) {
    unsigned int index = hash(key);
    KeyValue* current = hashTable->table[index];
    KeyValue* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                hashTable->table[index] = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Function to free the hash table
void freeHashTable(HashTable* hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        KeyValue* current = hashTable->table[i];
        while (current) {
            KeyValue* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(hashTable->table);
    free(hashTable);
}

// Example usage
int main() {
    HashTable* map = createHashTable();

    insert(map, "name", "Emily");
    insert(map, "age", "25");
    insert(map, "city", "Seattle");

    printf("Name: %s\n", search(map, "name"));
    printf("Age: %s\n", search(map, "age"));
    printf("City: %s\n", search(map, "city"));

    delete(map, "age");
    printf("Age after deletion: %s\n", search(map, "age")); // Should print (null)

    freeHashTable(map);
    return 0;
}
