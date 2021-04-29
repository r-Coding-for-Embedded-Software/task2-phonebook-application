//
// Created by Refaey on 15-Apr-21.
//

#include "../libs/csv_helper.h"
#include "../libs/Basic_Types.h"

// Create Database with 4 persons
void create_csv_DB(const char* file_name)
{
    char* names [] = {"Body", "Refaey", "Kudo", "Okiya"};
    char* emails [] = {"body123@gmail.com", "refaey@gmail.com", "null@gmail.com", "noob@gmail.com"};
    char* phones [] = {"12345", "11012", "161718", "99933"};
    int32_t len = sizeof(names)/sizeof(char*);

    // Create a file pointer, which will be used to access and write to a file
    FILE *file_handler = fopen(file_name, "w");

    // Check if the file was opened successfully
    if (file_handler == NULL)
    {
        perror("Unable to open the file.");
        exit(1);
    }

    // Write a row of column headers first to the file.
    fprintf(file_handler,"name,email,phone\n");

    // Add persons array to DB
    int32_t i = 0;
    for (i = 0; i < len; i++)
    {
        PersonStruct person = {names[i], emails[i], phones[i]};
        fprintf(file_handler,"%s,%s,%s\n", person.name, person.email, person.phone);
    }

    // Close the file
    fclose(file_handler);
}

int32_t get_rows_length(const char* file_name)
{
    FILE *file_handler;
    int32_t lines_num = 0;
    char row[MAX_CHAR] = {};
    file_handler = fopen(file_name,"r");

    // Loop through all lines
    while (fgets(row, MAX_CHAR, file_handler))
    {
        lines_num++;
    }

    // Close the file
    fclose(file_handler);
    return lines_num-1;
}

void display_csv_file(const char* file_name)
{
    FILE *file_handler;
    char ch;
    file_handler = fopen(file_name,"r");
    ch = (char) fgetc(file_handler);
    printf("\nContent of \"%s\" file:\n", file_name);

    while(ch != (char) EOF)
    {
        printf("%c", ch);
        ch = (char) fgetc(file_handler);
    }
    printf("\n");
    fclose(file_handler);
}

void get_row(const char* file_name, int32_t row_num, char** row_output)
{
    int32_t line_indx = 1;
    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR] = {};

    while (fgets(row, MAX_CHAR, file_handler))
    {
        /* skip the line at given line number */
        if (line_indx == row_num)
        {
            printf("Found line %i\n", row_num);
            *row_output = strdup(row);
            break;
        }
        line_indx++;
    }

    // Close the file
    fclose(file_handler);
}

void print_row(const char* file_name, int32_t line_num)
{
    int32_t counter = 1;

    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR] = {};

    while (fgets(row, MAX_CHAR, file_handler))
    {
        if (counter == line_num)
        {
            printf("%s\n", row);
            fclose(file_handler);
            break;
        }
        counter++;
    }

    // Close the file
    fclose(file_handler);
}

void add_row(const char* file_name, PersonStruct row)
{
    FILE* file_handler = fopen(file_name, "a");
    if (file_handler != NULL)
    {
        fprintf(file_handler, "%s,%s,%s\n", row.name, row.email, row.phone);
        printf("Added new person successfully:\n"
               "%s,%s,%s\n", row.name, row.email, row.phone);
    }
    fclose(file_handler);
}

void edit_row(const char* file_name, const char* word, const char* new_line)
{
    int32_t line_num = search_element(file_name, word);

    /* If the word is find */
    if (line_num != -1)
    {

        int32_t counter = 0;

        FILE *file_handler1, *file_handler2;
        char str[MAX_CHAR];

        // Open the source file in read mode
        file_handler1 = fopen(file_name, "r");

        if (file_handler1 == NULL)
        {
            printf("File not found or unable to open the input file!!\n");
            exit(1);
        }

        // open the temporary file in write mode
        file_handler2 = fopen(TEMP_FILE_NAME, "w");
        if (file_handler2 == NULL)
        {
            printf("Unable to open a temporary file to write!!\n");
            fclose(file_handler1);
            exit(1);
        }

        // copy all contents to the temporary file except the specific line
        while (!feof(file_handler1))
        {
            strcpy(str, "\0");
            fgets(str, MAX_CHAR, file_handler1);

            if (!feof(file_handler1))
            {
                counter++;

                /* skip the line at given line number */
                if (counter != line_num)
                {
                    fprintf(file_handler2, "%s", str);
                }
                // If found the line
                else if (strcmp(new_line, "0") != 0)
                {
                    // write the line to the temp file
                    fprintf(file_handler2, "%s\n", new_line);
                    printf("Edited Successfully!\n");
                    printf("%s\n", new_line);
                }
                // When deleting the line
                else
                {
                    printf("Line is Deleted Successfully:\n%s", str);
                }
            }

        }

        fclose(file_handler1);
        fclose(file_handler2);

        remove(file_name);                  // remove the original file
        rename(TEMP_FILE_NAME, file_name);  // rename the temporary file to original name
    }

    else
    {
        // If the word is not found
        printf("\"%s\" does not exist in the file\n", word);
        printf("Cannot edit/delete non-existing line\n");
    }

}

int32_t search_element(const char* file_name, const char* word)
{
    int32_t line_indx = 1;
    int32_t found_line_indx = -1;
    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR] = {};

    while (fgets(row, MAX_CHAR, file_handler))
    {
        char* ptr = strstr(row, word);

        // if the word is found
        if (ptr != NULL)
        {
            // Close the file
            found_line_indx = line_indx;
            printf("Found %s in line %i\n", word, found_line_indx);
            print_row(file_name, found_line_indx);
            fclose(file_handler);
            break;
        }
        line_indx++;
    }

    // If the row is not found
    if (found_line_indx == -1)
    {
        // Close the file
        fclose(file_handler);
        printf("\"%s\" does not exist in the file\n", word);
    }

    return found_line_indx;
}
