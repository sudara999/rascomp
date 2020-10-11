#include <stdlib.h>
#include <string.h>
#include "hash_tab.h"

// Functions to manage a hash_table
//

// Create a hash table
struct hash_tab *create_hash_tab (
		int size, 
		unsigned int (*hash) (void *, int), 
		int (*compare) (void *, void *))
{
	struct hash_tab *hash_table = malloc(sizeof(struct hash_tab));
	struct hash_list **table = malloc(size * sizeof(struct hash_list *));
	for (int i = 0; i < size; i++)
		table[i] = NULL;
	hash_table->table = table;
	hash_table->size = size;
	hash_table->hash = hash;
	hash_table->compare = compare;
	return hash_table;
}

// Destroy a hash table
void destroy_hash_tab (struct hash_tab *hash_table)
{
	for (int i = 0; i < hash_table->size; i++) {
		struct hash_list *next_hl;
		for ( struct hash_list *hl = hash_table->table[i]; hl != NULL; hl = next_hl) {
			next_hl = hl->next;
			free(hl);
		}
	}
	free(hash_table);
}

// Get the hash_list structure that maps a specific key to a value
// NULL is returned if the key-value pair does not exist
static struct hash_list *get_map (struct hash_tab *hash_table, void *key)
{	
	for (struct hash_list *hl = 
			hash_table->table[hash_table->hash(key, hash_table->size)];
			hl != NULL;
			hl = hl->next)
		if (hash_table->compare(key, hl->key) == 0)
			return hl;
	return NULL;	
}

// Look for the corresponding value of a key
// NULL is returned if the key-value pair does not exist
void *lookup (struct hash_tab *hash_table, void *key)
{
	struct hash_list *map = get_map(hash_table, key);
	return map != NULL ? map->value: NULL;
}


// Insert a key-value pair
// Returns the previous value of the key
void *insert (struct hash_tab *hash_table, void *key, void *value)
{
	struct hash_list *map;
	if ((map = get_map(hash_table, key)) != NULL){
		void *old_value = map->value;
		map->value = value;
		return old_value;
	}
	else {
		unsigned int hash_val = hash_table->hash(key, hash_table->size);
		map = malloc(sizeof(struct hash_list));
		map->key = key;
		map->value = value;
		map->next = hash_table->table[hash_val];
		hash_table->table[hash_val] = map;
		return NULL;
	}
}

// Create a hash from a string
unsigned int string_hash (void *str, int size)
{
	unsigned int hashval = 0;
	for (char *s = (char *)str; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % size;
}

// Compare strings to check whether are equal
int string_compare (void *str1, void *str2)
{
	return strcmp((char *)str1, (char *)str2);	
}

