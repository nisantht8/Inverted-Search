#include "invert.h"

// Function to search for a word in the database
int search_database(hash_t *table)
{
    char word[50];
    printf("Enter the word: ");
    scanf("%49s", word); // read word from user

    int index;
    // find hash index based on first character
    if (isalpha(word[0]))
    {
        char c = tolower(word[0]); // convert to lowercase for index
        index = c - 'a';
    }
    else
        index = 26; // for non-alphabetic characters

    main_node *mtemp = table[index].link; // point to first main node

    // traverse main nodes in the list
    while (mtemp)
    {
        // check for exact word match (case-sensitive)
        if (strcmp(mtemp->word, word) == 0)
        {
            printf("Search result: \"%s\" occurs in %d files.\n", mtemp->word, mtemp->filecount);

            // print all subnodes (file details)
            sub_node *stemp = mtemp->sublink;
            while (stemp)
            {
                printf("'%s' contains the word %d times.\n", stemp->filename, stemp->wordcount);
                stemp = stemp->link;
            }
            return 0; // word found, end function
        }
        mtemp = mtemp->mainlink; // move to next main node
    }

    // if word not found in table
    printf("Word '%s' not found in the database.\n", word);
    return 0;
}