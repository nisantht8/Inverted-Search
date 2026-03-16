/*
Description:
    An Inverted Search is a technique that builds an index linking each unique word to the files and their occurrence counts where the word appears.
It helps in efficiently searching for words across multiple files, similar to how a search engine finds documents containing specific terms.*/

#include "invert.h"

int main(int argc, char *argv[])
{
    int size = 27;
    hash_t arr[size];
    create_HT(arr, size); // create empty hash table

    // check command-line arguments
    if (argc <= 1)
    {
        printf("Usage: %s <file1.txt> [file2.txt ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    Node *file_list = NULL; // linked list to store valid file names

    // check each given file
    for (int i = 1; i < argc; i++)
    {
        int len = strlen(argv[i]);

        // check .txt extension
        if (len <= 4 || strcmp(argv[i] + len - 4, ".txt") != 0)
        {
            printf("Error: '%s' is not a valid .txt file\n", argv[i]);
            continue;
        }

        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL)
        {
            printf("Error: '%s' not available.\n", argv[i]);
            continue;
        }

        // check if file is empty
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0)
        {
            printf("Error: '%s' is empty.\n", argv[i]);
            fclose(fp);
            continue;
        }
        fclose(fp);

        // add valid file to linked list
        insert_node(&file_list, argv[i]);
    }

    // if no valid files found
    if (file_list == NULL)
    {
        printf("\nNo valid files to store. Exiting...\n");
        return EXIT_FAILURE;
    }

    printf("\nValid files stored in linked list:\n");
    print_list(file_list);

    // check if backup.txt exists and has data
    FILE *check = fopen("backup.txt", "r");
    int backup_exists = 0;
    long file_size = 0;

    if (check)
    {
        fseek(check, 0, SEEK_END);
        file_size = ftell(check);

        if (file_size > 0)
            backup_exists = 1;   // backup has data
        else
            backup_exists = 2;   // backup file is empty

        fclose(check);
    }
    else
    {
        backup_exists = 0;       // backup does not exist
    }

    // show backup file status
    if (backup_exists)
        printf("\nExisting backup found. It will be merged after creating the new database.\n");
    else
        printf("\nNo existing backup found. Starting fresh.\n");

    int choice, flag_update = 0, flag_create = 0;

    // menu loop
    do
    {
        printf("\n::: MENU :::\n");
        printf("1. Create_Database\n");
        printf("2. Display_Database\n");
        printf("3. Search\n");
        printf("4. Save\n");
        printf("5. Update_Database\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                // create database only once
                if (!flag_create)
                {
                    flag_create = 1;
                    create_database(arr, size, file_list);

                    // if backup file has data, merge it
                    if (backup_exists == 1 && flag_update == 0)
                    {
                        printf("\nMerging existing backup into database...\n");
                        update_database(arr, size);
                        flag_update = 1;
                    }
                    else if (backup_exists == 2)
                    {
                        printf("\nError: Backup file is empty.\n"); 
                    }
                    else if (backup_exists == 0)
                    {
                        printf("\nError: No backup found. Created new database.\n");
                    }                
                }
                else
                {
                    printf("\nError: Database is already created.\n");
                }
                break;

            case 2:
                // display database
                display_database(arr, size);
                break;

            case 3:
                // search for a word
                search_database(arr);
                break;

            case 4:
                // save only if database is created
                if (flag_create == 1)
                {
                    backup_exists = 1;
                    save_database(arr, size);
                }
                else
                    printf("\nError: Create hashtable and save.\n");
                break;

            case 5:
                // allow update only once
                if (flag_update == 1)
                {
                    printf("\nError: Database already updated once.\n");
                    break;
                }

                if (backup_exists == 1 && flag_create == 0)
                {
                    flag_update = 1;
                    update_database(arr, size);
                }
                else if (backup_exists == 2)
                {
                    printf("\nError: Backup file is empty.\n"); 
                }
                else
                {
                    printf("\nError: Backup file does not exist.\n");
                }
                break;

            case 6:
                // exit
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return EXIT_SUCCESS;
}