#ifndef HASH_TAB
#define HASH_TAB

#define NO_DUP 0
#define HAS_DUP 1

struct hash_list {
	struct hash_list *next;
	void *key;
	void *value;
};

struct hash_tab {
	struct hash_list **table;
	int size;
	unsigned int (*hash) (void *, int);
	int (*compare) (void *, void *);
};

// Functions to manage a hash_table
//

// Create a hash table
struct hash_tab *create_hash_tab (
		int size, 
		unsigned int (*hash) (void *, int), 
		int (*compare) (void *, void *));

// Destroy a hash table
void destroy_hash_tab (struct hash_tab *hash_table);

// Look for the corresponding value of a key
// NULL is returned if the key-value pair does not exist
void *lookup (struct hash_tab *hash_table, void *key);

// Insert a key-value pair
// Returns the previous value of the key
void *insert (struct hash_tab *hash_table, void *key, void *value, int flag);

// Get the hash_list structure that maps a specific key to a value
// NULL is returned if the key-value pair does not exist
struct hash_list *get_map (struct hash_tab *hash_table, void *key);

// Functions that can be used depending on the type of the hash table
//

// Create a hash from a string
unsigned int string_hash (void *str, int size);

// Compare strings to check whether are equal
int string_compare (void *str1, void *str2);

#endif
