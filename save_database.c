#include "invert.h"

// Save the database to a backup file
void save_database(hash_t *table, int size)
{
    FILE *fp = fopen("backup.txt", "w"); // open file for writing
    if (fp == NULL)
    {
        printf("Error: Unable to create backup file.\n");
        return;
    }

    // Loop through each index in the hash table
    for (int i = 0; i < size; i++)
    {
        if (table[i].link != NULL) // if index has data
        {
            main_node *mtemp = table[i].link; // point to first main node
            while (mtemp) // traverse main nodes
            {
                // write main node info (index, word, file count)
                fprintf(fp, "#%d ; %s ; %d ; ", i, mtemp->word, mtemp->filecount);

                sub_node *stemp = mtemp->sublink; // point to first subnode
                while (stemp) // traverse subnodes
                {
                    // write filename and word count
                    fprintf(fp, "[%s : %d]", stemp->filename, stemp->wordcount);
                    if (stemp->link != NULL) // add separator if more subnodes
                        fprintf(fp, " ; ");
                    stemp = stemp->link;
                }

                fprintf(fp, " ; #\n"); // mark end of record
                mtemp = mtemp->mainlink; // move to next main node
            }
        }
    }

    fclose(fp); // close file
    printf("\nDatabase successfully saved to 'backup.txt'.\n");
}