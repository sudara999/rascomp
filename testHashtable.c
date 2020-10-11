#include <stdio.h>
#include <stdlib.h>
#include "hash_tab.h"

int main()
{	
	// Create a string hash table
	struct hash_tab *hash_table = create_hash_tab (10, string_hash, string_compare);
	
	// Insert names to the hash table
	char sud[] = "sudara";
	char ran[] = "ranasinghe";
	if (insert(hash_table, sud, ran) == NULL)
		printf ("Inserted sudara:ranasinghe\n");
	
	if (insert(hash_table, "jiyeon", "Jung") == NULL)
		printf ("Inserted jiyeon:Jung\n");

	// Overwrite a value
	if (insert(hash_table, "jiyeon", "jung") == "Jung")
		printf ("Inserted jiyeon:jung\n");

	// Get the value for keys
	printf("Found sudara:%s\n", (char *)lookup(hash_table, sud));
	printf("Found jiyeon:%s\n", (char *)lookup(hash_table, "jiyeon"));
	
	// Check if a non-existing key exists
	if (lookup(hash_table, "thanh") == NULL)
		printf("Could not find thanh\n");	

	// Destroy the hash table
	destroy_hash_tab(hash_table);
	
	return 0;
}
