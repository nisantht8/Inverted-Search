#include "invert.h"

// Update the database by reading data from backup.txt
void update_database(hash_t *table, int size)
{
    FILE *fp = fopen("backup.txt", "r"); // open backup file for reading
    if (fp == NULL)
    {
        printf("Error: Could not open backup.txt for reading.\n");
        return;
    }

    char line[512]; // buffer to store each line
    while (fgets(line, sizeof(line), fp)) // read line by line
    {
        int index = -1, filecount = 0;
        char word[50] = "";

        line[strcspn(line, "\n")] = '\0'; // remove newline

        // read index, word, and file count from line
        if (sscanf(line, " #%d ; %49[^;] ; %d ;", &index, word, &filecount) != 3)
            continue;
        if (index < 0 || index >= size)
            continue;

        main_node *mtemp = table[index].link;
        main_node *mprev = NULL;
        int word_found = 0;

        // search if word already exists
        while (mtemp)
        {
            if (strcmp(mtemp->word, word) == 0)
            {
                word_found = 1;
                break;
            }
            mprev = mtemp;
            mtemp = mtemp->mainlink;
        }

        // if word not found, create a new main node
        if (!word_found)
        {
            mtemp = malloc(sizeof(main_node));
            if (!mtemp)
            {
                printf("Memory allocation failed.\n");
                fclose(fp);
                return;
            }
            strcpy(mtemp->word, word);
            mtemp->filecount = 0;
            mtemp->sublink = NULL;
            mtemp->mainlink = NULL;

            if (mprev)
                mprev->mainlink = mtemp;
            else
                table[index].link = mtemp;
        }

        // move pointer through line to find subnode data
        char *ptr = line;
        while ((ptr = strchr(ptr, '[')) != NULL)
        {
            char filename[50];
            int wordcount = 0;

            // read filename and wordcount inside brackets
            if (sscanf(ptr, " [%49[^]:] : %d]", filename, &wordcount) == 2)
            {
                sub_node *stemp = mtemp->sublink;
                sub_node *sprev = NULL;
                int file_found = 0;

                // check if same file already exists
                while (stemp)
                {
                    if (strcmp(stemp->filename, filename) == 0)
                    {
                        stemp->wordcount += wordcount; // update count
                        file_found = 1;
                        break;
                    }
                    sprev = stemp;
                    stemp = stemp->link;
                }

                // if file not found, create new subnode
                if (!file_found)
                {
                    sub_node *new_sub = malloc(sizeof(sub_node));
                    strcpy(new_sub->filename, filename);
                    new_sub->wordcount = wordcount;
                    new_sub->link = NULL;

                    if (sprev)
                        sprev->link = new_sub;
                    else
                        mtemp->sublink = new_sub;
                    mtemp->filecount++;
                }
            }
            ptr++; // move pointer forward
        }
    }
    fclose(fp); // close backup file
    printf("\nDatabase successfully merged with 'backup.txt'.\n");
}