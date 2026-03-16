#include "invert.h"

/* Function to create and initialize the hash table */
void create_HT(hash_t *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i].index = i;    // Assign index for each hash entry
        arr[i].link = NULL;  // Initialize linked list to NULL
    }
}

/* Function to create a new file node */
Node *create_node(char *name)
{
    Node *newnode = (Node *)malloc(sizeof(Node)); // Allocate memory for new node
    if (newnode == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newnode->f_name = strdup(name);  // Duplicate the file name string
    newnode->link = NULL;            // Initialize link to NULL
    return newnode;
}

/* Function to insert a file node into the linked list */
void insert_node(Node **head, char *filename)
{
    FILE *fp = fopen("backup.txt", "r"); // Open backup file to check if filename exists
    int exists_in_backup = 0;            // Flag to check if file already backed up

    if (fp != NULL)
    {
        char line[512];
        while (fgets(line, sizeof(line), fp)) // Read each line in backup file
        {
            if (strstr(line, filename) != NULL) // Check if filename already present
            {
                exists_in_backup = 1;
                break;
            }
        }
        fclose(fp); // Close backup file
    }

    // If file already exists in backup, skip adding to list
    if (exists_in_backup)
    {
        printf("Info: '%s' already exists in backup. Skipping...\n", filename);
        return;
    }

    Node *temp = *head, *prev = NULL;

    // Check if file already exists in the linked list and remove it
    while (temp)
    {
        if (strcmp(temp->f_name, filename) == 0)
        {
            if (prev)
                prev->link = temp->link;
            else
                *head = temp->link;
            free(temp->f_name);
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->link;
    }

    // Create a new node for the given filename
    Node *newnode = create_node(filename);
    if (newnode == NULL)
        return;

    // Insert new node at the end of the linked list
    if (*head == NULL)
        *head = newnode;
    else
    {
        temp = *head;
        while (temp->link != NULL)
            temp = temp->link;
        temp->link = newnode;
    }
}

/* Function to print all filenames in the linked list */
void print_list(Node *head)
{
    Node *temp = head;
    int count = 1;
    while (temp != NULL)
    {
        printf("%d. %s\n", count, temp->f_name); // Display file index and name
        temp = temp->link;
        count++;
    }
}