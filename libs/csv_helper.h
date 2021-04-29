//
// Created by Refaey on 15-Apr-21.
//

#ifndef TASK3_PHONEBOOK_CSV_HELPER_H
#define TASK3_PHONEBOOK_CSV_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Basic_Types.h"

#define TEMP_FILE_NAME "temp_file.csv"
#define MAX_CHAR 1024


typedef struct {
    char* name;
    char* email;
    char* phone;
} PersonStruct;

void create_csv_DB(const char* file_name);
int32_t get_rows_length(const char* file_name);
void display_csv_file(const char* file_name);
void print_row(const char* file_name, int32_t line_num);
void add_row(const char* file_name, PersonStruct row);
void edit_row(const char* file_name, const char* word, const char* new_line);
int32_t search_element(const char* file_name, const char* element);
void get_row(const char* file_name, int32_t row_num, char** row_output);


#endif //TASK3_PHONEBOOK_CSV_HELPER_H
