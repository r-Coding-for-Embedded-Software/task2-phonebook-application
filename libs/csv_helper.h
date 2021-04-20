//
// Created by Refaey on 15-Apr-21.
//

#ifndef TASK3_PHONEBOOK_CSV_HELPER_H
#define TASK3_PHONEBOOK_CSV_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEMP_FILE_NAME "temp_file.csv"
#define MAX_CHAR 1024


typedef struct {
    char* name;
    char* email;
    char* phone;
} PersonStruct;

void create_csv_DB(char* file_name);
int get_rows_length(char* file_name);
void display_csv_file(char* file_name);
void print_row(char* file_name, int line_num);
void add_row(char* file_name, PersonStruct row);
void edit_row(char* file_name, char* word, char* new_line);
int search_element(char* file_name, char* element);
void get_row(char* file_name, int row_num, char** row_output);

// Unfinished Function
void read_csv(char* file_name, PersonStruct * db);
void get_column(char* file_name, int column_num, char* output[]);

char* get_field(char* row, int num);
void printArray(char** arr, int length);

#endif //TASK3_PHONEBOOK_CSV_HELPER_H
