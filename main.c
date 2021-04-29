#include "libs/csv_helper.h"
#include "libs/Basic_Types.h"

#define FILE_NAME "PhoneBook.csv"
#define DISPLAY   1
#define ADD       2
#define DELETE    3
#define EDIT      4
#define SEARCH    5
#define EXIT_CODE 0

int32_t main(void);
void show_main_menu(void);
void get_input(const char* value);
PersonStruct create_person(const char* value);
void exit_program(void);

int32_t main(void)
{
    // EXIT Flag to check if the user want to exit the program
    char_t exit_status = '0';

    // buffer string to store the input from user in each question
    char_t input_value[MAX_CHAR];

    // buffer int to store the input from user in each question
    int32_t input;

    // Create a DB from 4 persons
    create_csv_DB(FILE_NAME);

    // Display the main menu of the application
    show_main_menu();

    // Keep the program running until we press 'q'
    while (exit_status == '0')
    {
        printf("Input:");
        scanf("%d", &input);

        // Check each time what did the user choose from the menu
        switch (input)
        {
            case DISPLAY:
            {
                // Display the content of the csv file
                display_csv_file(FILE_NAME);
                show_main_menu();
                break;
            }

            case ADD:;
                {
                    // Create a new person from the user input
                    PersonStruct person;
                    person = create_person(input_value);

                    // Add the new person to the end of the csv file
                    add_row(FILE_NAME, person);

                    // remove added person from memory
                    free(person.name);
                    free(person.email);
                    free(person.phone);

                    show_main_menu();
                    break;
                }


            case DELETE:
            {
                // Get field to delete
                printf("Write name or phone you want to delete from the DB:");
                get_input(input_value);

                // Delete the entire row from the csv file
                edit_row(FILE_NAME, input_value, "0");
                show_main_menu();
                break;
            }


            case EDIT:;
                {
                    // Get field to edit
                    char* field_to_edit;
                    printf("Write name or phone you want to edit from the DB:");
                    get_input(input_value);
                    field_to_edit = strdup(input_value);

                    int32_t indx = search_element(FILE_NAME, field_to_edit);
                    if (indx == -1)
                    {
                        printf("Cannot edit/delete non-existing line\n");
                        show_main_menu();
                        break;
                    }

                    // If the person exists
                    char* row_value = NULL;
                    get_row(FILE_NAME, indx, &row_value);
//                    char* rest_row_value = strdup(row_value);

                    PersonStruct temp_person;
//                    temp_person.name = strdup(strtok(row_value, ","));
//                    temp_person.email = strdup(strtok(NULL, ","));
//                    temp_person.phone = strdup(strtok(NULL, ","));

                    temp_person.name = strdup(strtok_r(row_value, ",", &row_value));
//                    printf("Row after 1 strtok_r: %s\n", rest_row_value);
                    temp_person.email = strdup(strtok_r(row_value, ",", &row_value));
//                    printf("Row after 2 strtok_r: %s\n", rest_row_value);
                    temp_person.phone = strdup(strtok_r(row_value, ",", &row_value));

                    printf("Choose field to edit:\n"
                           "1. name\n"
                           "2. email\n"
                           "3. phone\n");

                    get_input(input_value);
                    char* choice = strdup(input_value);
                    char new_line[MAX_CHAR] = {};

                    // Edit the name only
                    if (strcmp(choice, "1") == 0)
                    {
                        printf("Enter the new name:");
                        get_input(input_value);
                        char* new_field = strdup(input_value);

                        sprintf(new_line, "%s,%s,%s", new_field, temp_person.email, temp_person.phone);

                        // Edit (Replace) the entire row from the csv file
                        edit_row(FILE_NAME, field_to_edit, new_line);
                        free(new_field);
                    }

                        // Edit the email only
                    else if (strcmp(choice, "2") == 0)
                    {
                        printf("Enter the new email:");
                        get_input(input_value);
                        char* new_field = strdup(input_value);

                        sprintf(new_line, "%s,%s,%s", temp_person.name, new_field, temp_person.phone);

                        edit_row(FILE_NAME, field_to_edit, new_line);
                        free(new_field);
                    }

                        // Edit the phone only
                    else if (strcmp(choice, "3") == 0)
                    {
                        printf("Enter the new phone:");
                        get_input(input_value);
                        char* new_field = strdup(input_value);

                        sprintf(new_line, "%s,%s,%s", temp_person.name, temp_person.email, new_field);

                        edit_row(FILE_NAME, field_to_edit, new_line);
                        free(new_field);
                    }
                    else
                    {
                        /* Do nothing */
                    }

                    // remove variables from memory
                    free(field_to_edit);
                    free(choice);
                    free(temp_person.name);
                    free(temp_person.email);
                    free(temp_person.phone);

                    show_main_menu();
                    break;
                }


            case SEARCH:;
                {
                    // Get field to search
                    char* field_to_search;
                    printf("Write name or phone you want to search in the DB:");
                    get_input(input_value);
                    field_to_search = strdup(input_value);

                    // Search for existing person in the csv file
                    search_element(FILE_NAME, field_to_search);
                    show_main_menu();
                    break;
                }


            case EXIT_CODE:
            {
                exit_status = '1';
                exit_program();
                break;
            }


            default:
            {
                printf("Invalid operation!! Choose again\n\n");
                break;
            }

        }
    }
    return 0;
}

void show_main_menu(void)
{
    /* Display the main menu options to the user
     */
    printf("\n***** Please choose your operation *****\n");
    printf("1. Display all contacts\n"
           "2. Add a new contact\n"
           "3. Delete an existing contact\n"
           "4. Edit an existing contact\n"
           "5. Search for a contact by name or phone\n"
           "!!! To Exit the Program: Press 'q' !!!\n\n");
}

void get_input(const char* value)
{
    /* get input data from the user
     */

    // [] is the scanset character
    // ^\n tells to take input until newline doesn't get encountered
    // %*c, it reads newline character and here used * indicates that this newline character is discarded
    scanf("%sn]%*c", value);
}

PersonStruct create_person(const char* value)
{
    /* Create a new person from input data from the user
     */

    PersonStruct person;

    // Get person's data from user
    printf("Enter New Contact Data:\n");
    printf("Name:");
    get_input(value);
    person.name = strdup(value);

    printf("Email:");
    get_input(value);
    person.email = strdup(value);

    printf("Phone:");
    get_input(value);
    person.phone = strdup(value);

    return person;
}

void exit_program(void)
{
    /* Exit the program
     */

    printf("You exited the program!.. see you later!\n");
    exit(1);
}
