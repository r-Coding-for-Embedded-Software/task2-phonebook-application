//
// Created by Refaey on 15-Apr-21.
//

#include "../libs/csv_helper.h"

// Create Database with 4 persons
void create_csv_DB(char* file_name)
{
    char* names [] = {"Body", "Refaey", "Kudo", "Okiya"};
    char* emails [] = {"body123@gmail.com", "refaey@gmail.com", "null@gmail.com", "noob@gmail.com"};
    char* phones [] = {"12345", "11012", "161718", "99933"};
    int len = sizeof(names)/sizeof(char*);

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
    for (int i = 0; i < len; i++)
    {
        PersonStruct person = {names[i], emails[i], phones[i]};
        fprintf(file_handler,"%s,%s,%s\n", person.name, person.email, person.phone);
    }

    // Close the file
    fclose(file_handler);
}

int get_rows_length(char* file_name)
{
    FILE *file_handler;
    int lines_num = 0;
    char row[MAX_CHAR];
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

void display_csv_file(char* file_name)
{
    FILE *file_handler;
    char ch;
    file_handler = fopen(file_name,"r");
    ch = (char) fgetc(file_handler);
    printf("\nContent of \"%s\" file:\n", file_name);

    while(ch!=EOF)
    {
        printf("%c", ch);
        ch = (char) fgetc(file_handler);
    }
    printf("\n");
    fclose(file_handler);
}

void get_row(char* file_name, int row_num, char** row_output)
{
    int line_indx = 1;
    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR];

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

void print_row(char* file_name, int line_num)
{
    int counter = 1;

    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR];

    while (fgets(row, MAX_CHAR, file_handler))
    {
        if (counter == line_num)
        {
            printf("%s\n", row);
            fclose(file_handler);
            return;
        }
        counter++;
    }

    // Close the file
    fclose(file_handler);
}

void add_row(char* file_name, PersonStruct row)
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

void edit_row(char* file_name, char* word, char* new_line)
{
    int line_num = search_element(file_name, word);

    // If the word is not found
    if (line_num == -1)
    {
        printf("\"%s\" does not exist in the file\n", word);
        printf("Cannot edit/delete non-existing line\n");
        return;
    }

    int counter = 0;

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
            else if (new_line)
            {
                // write the line to the temp file
                fprintf(file_handler2, "%s\n", new_line);
                printf("Edited Successfully!\n");
                printf("%s\n", new_line);
            }
            // When deleting the line
            else if (!new_line)
            {
                printf("Line is Deleted Successfully:\n%s", str);
            }
        }

    }

    fclose(file_handler1);
    fclose(file_handler2);

    remove(file_name);  		        // remove the original file
    rename(TEMP_FILE_NAME, file_name); 	// rename the temporary file to original name
}

int search_element(char* file_name, char* word)
{
    int line_indx = 1;
    FILE* file_handler = fopen(file_name, "r");
    char row[MAX_CHAR];

    while (fgets(row, MAX_CHAR, file_handler))
    {
        char* ptr = strstr(row, word);

        // if the word is found
        if (ptr != NULL)
        {
            // Close the file
            printf("Found %s in line %i\n", word, line_indx);
            print_row(file_name, line_indx);
            fclose(file_handler);
            return line_indx;
        }
        line_indx++;
    }

    // Close the file
    fclose(file_handler);
    printf("\"%s\" does not exist in the file\n", word);
    return -1;
}

// Unfinished Function
void get_column(char* file_name, int column_num, char* output[])
{
    int row_num = 0;
    char* token;
    FILE* file_handler = fopen(file_name, "r");

    // Length of the line to be read in the csv
    char row[MAX_CHAR];

    while (fgets(row, MAX_CHAR, file_handler))
    {
        token = get_field(row, column_num);
        output[row_num] = token;
        row_num++;
    }

    // Close the file
    fclose(file_handler);
}

// Unfinished Function
void read_csv(char* file_name, PersonStruct* db)
{
    FILE* file_handler = fopen(file_name, "r");

    // Length of the line to be read in the csv
    char row[MAX_CHAR];

    // Token we get from tokenization of each line (Column value)
    char *token;

    // Loop over all the lines
    // fgets -> reads up to n characters from the stream
    while (fgets(row, MAX_CHAR, file_handler))
    {
        printf("Row in start: %s", row);

        // Split the row into strings
        token = strtok(row, ",");

        if (token != NULL)
        {
            db->name = token;
            printf("name = %s\n", db->name);
            token = strtok(NULL, ", ");

            db->email = token;
            printf("email = %s\n", db->email);
            token = strtok(NULL, ", ");

            db->phone = token;
            printf("phone = %s\n", db->phone);
            token = strtok(NULL, ", ");
            db++;
        }

//        while (token != NULL)
//        {
//            printf("Token: %s\n", token);
//            token = strtok(NULL, ", ");
//        }
    }

    // Close the file
    fclose(file_handler);
}

// Unused function till now
char* get_field(char* row, int num)
{
    char* tok;

    // Check if pointer tok is not nullptr
    // Then it checks if it points to memory where the value (*tok) isn't '\0'.
    for (tok = strtok(row, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
        {
            return tok;
        }
    }
    return NULL;
}


// Function to print array of strings
void printArray(char** arr, int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("Element %i = %s\n", i, arr[i]);
        fflush(stdout);
    }
}
