#define EXIT_SUCCESS 1   // Macro for successful program termination
#define EXIT_FAILURE 0   // Macro for program failure

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for hash table
typedef struct hashtable
{
    int index;                 // Index value for hash table bucket
    struct mainnode *link;     // Pointer to main node list
} hash_t;

// Linked list structure to store file names
typedef struct files
{
    char *f_name;              // File name
    struct files *link;        // Pointer to next file node
} Node;

// Structure for subnode (stores filename and word count)
typedef struct subnode
{
    int wordcount;             // Number of times the word appears in file
    char filename[20];         // File name
    struct subnode *link;      // Pointer to next subnode
} sub_node;

// Structure for mainnode (stores word and link to subnodes)
typedef struct mainnode
{
    char word[30];             // Word stored in the database
    int filecount;             // Number of files containing the word
    struct mainnode *mainlink; // Pointer to next mainnode
    struct subnode *sublink;   // Pointer to list of subnodes (file info)
} main_node;

// Function declarations
void create_HT(hash_t *, int);                // Initialize hash table
void insert_node(Node **head, char *filename); // Insert a file into linked list
void print_list(Node *head);                  // Print list of files
void create_database(hash_t *table, int size, Node *file_list); // Build inverted index
void display_database(hash_t *table, int size); // Display the entire database
void save_database(hash_t *table, int size);   // Save database to a file
void update_database(hash_t *table, int size); // Update existing database
int search_database(hash_t *table);            // Search for a word in the database