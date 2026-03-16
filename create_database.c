#include "invert.h"
#include <ctype.h>

// Create a new subnode
sub_node *create_subnode(char *filename)
{
    sub_node *new = malloc(sizeof(sub_node)); // allocate memory
    if (new == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    new->wordcount = 1; // set word count
    strncpy(new->filename, filename, sizeof(new->filename) - 1); // copy file name
    new->filename[sizeof(new->filename) - 1] = '\0'; // null terminate
    new->link = NULL; // set link to NULL
    return new;
}

// Create a new mainnode
main_node *create_mainnode(char *word, char *filename)
{
    main_node *new = malloc(sizeof(main_node)); // allocate memory
    if (new == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    strncpy(new->word, word, sizeof(new->word) - 1); // copy word
    new->word[sizeof(new->word) - 1] = '\0'; // null terminate
    new->filecount = 1; // set file count
    new->mainlink = NULL; // set link to NULL
    new->sublink = create_subnode(filename); // create subnode
    return new;
}

// Create the database
void create_database(hash_t *table, int size, Node *file_list)
{
    Node *fileptr = file_list; // point to file list

    while (fileptr)
    {
        FILE *fp = fopen(fileptr->f_name, "r"); // open file
        if (!fp)
        {
            printf("Error opening file: %s\n", fileptr->f_name);
            fileptr = fileptr->link;
            continue;
        }

        char word[50];
        while (fscanf(fp, "%49s", word) == 1) // read words
        {
            int index;
            if (isalpha(word[0])) // if starts with alphabet
            {
                char c = tolower(word[0]); // make lowercase
                index = c - 'a'; // find index
            }
            else
                index = 26; // non-alphabet words

            main_node *mtemp = table[index].link;
            main_node *mprev = NULL;
            int word_found = 0;

            while (mtemp) // search for word
            {
                if (strcmp(mtemp->word, word) == 0)
                {
                    word_found = 1;
                    sub_node *stemp = mtemp->sublink;
                    sub_node *sprev = NULL;
                    int file_found = 0;

                    while (stemp) // search for file
                    {
                        if (strcmp(stemp->filename, fileptr->f_name) == 0)
                        {
                            stemp->wordcount++; // increase count
                            file_found = 1;
                            break;
                        }
                        sprev = stemp;
                        stemp = stemp->link;
                    }

                    if (!file_found) // if file not found
                    {
                        sub_node *newsub = create_subnode(fileptr->f_name);
                        if (sprev)
                            sprev->link = newsub;
                        else
                            mtemp->sublink = newsub;
                        mtemp->filecount++; // increase file count
                    }
                    break;
                }
                mprev = mtemp;
                mtemp = mtemp->mainlink;
            }

            if (!word_found) // if word not found
            {
                main_node *newmain = create_mainnode(word, fileptr->f_name);
                if (mprev)
                    mprev->mainlink = newmain;
                else
                    table[index].link = newmain;
            }
        }

        fclose(fp); // close file
        printf("\nDatabase created/merged successfully for file '%s'.", fileptr->f_name);
        fileptr = fileptr->link; // move to next file
    }
    printf("\n");
}