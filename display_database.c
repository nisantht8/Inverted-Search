#include "invert.h"

// Display the contents of the database
void display_database(hash_t *table, int size)
{
    // Print table header
    printf("\n-----------------------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-10s | %-15s | %-10s |\n",
           "Index", "Word", "Filecount", "Filename", "Wordcount");
    printf("-----------------------------------------------------------------------\n");

    // Loop through each index in hash table
    for (int i = 0; i < size; i++)
    {
        if (table[i].link != NULL) // if index has data
        {
            main_node *mtemp = table[i].link; // point to first main node
            while (mtemp) // traverse all main nodes
            {
                sub_node *stemp = mtemp->sublink; // point to first subnode
                int first = 1; // flag for first line of word

                while (stemp) // traverse all subnodes
                {
                    if (first)
                    {
                        // Print full info (index, word, counts)
                        printf("| %-5d | %-15s | %-10d | %-15s | %-10d |\n", i, mtemp->word, mtemp->filecount, stemp->filename, stemp->wordcount);
                        first = 0; // next lines will skip word/index
                    }
                    else
                    {
                        // Print only file info for remaining subnodes
                        printf("| %-5s | %-15s | %-10s | %-15s | %-10d |\n", "", "", "", stemp->filename, stemp->wordcount);
                    }

                    printf("-----------------------------------------------------------------------\n");
                    stemp = stemp->link; // move to next subnode
                }
                mtemp = mtemp->mainlink; // move to next main node
            }
        }
    }
}
