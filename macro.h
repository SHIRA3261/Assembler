#ifndef MACRO_H
#define MACRO_H

/**
 * @file macro_api.h
 * @brief API documentation for the macro processing module.
 *
 * This file provides the documentation for the functions, data structures, 
 * and macros used in the macro processing module. This module processes 
 * macros in source files and generates corresponding output files.
 */

#include <stdio.h>


#define MAX_NAME_LEN 31

#define MAX_CONTENT 100



/* ============================ Data Structures ============================ */

/**
 * @brief Represents a macro in the linked list.
 *
 * This structure contains the name and content of a macro, as well as a pointer 
 * to the next macro in the linked list.
 */
typedef struct macro
{
    char name[MAX_NAME_LEN];        /**< The name of the macro. */
    char content[MAX_CONTENT];      /**< The content of the macro. */
    struct macro *next;             /**< Pointer to the next macro in the list. */
} macro;

/**
 * @brief Represents a list of macros.
 *
 * This structure contains pointers to the head and tail of the macro list,
 * as well as a counter for the number of macros in the list.
 */
typedef struct mcr_list
{
    macro *head;                    /**< Pointer to the first macro in the list. */
    macro *tail;                    /**< Pointer to the last macro in the list. */
    int counter;                    /**< Number of macros in the list. */
} mcr_list;

/* ============================ Function Declarations ============================ */

/**
 * @brief Main function for processing macros in the source file.
 *
 * This function reads the source file, identifies macro definitions, and 
 * processes them. It generates a target file with the macros expanded.
 *
 * @param source_file The source file containing macros to be processed.
 * @param file_name The name of the source file (without extension).
 * @param macro_list The list of macros found in the source file.
 * @return int 0 if no errors occurred, otherwise a non-zero value.
 */
int macro_func(FILE *source_file, char *file_name, mcr_list *macro_list);

/**
 * @brief Creates an empty list for storing macros.
 *
 * This function allocates memory for a new macro list and initializes it 
 * with default values.
 *
 * @return mcr_list* A pointer to the newly created macro list.
 */
mcr_list *create_macro_list();

/**
 * @brief Adds a macro to the list.
 *
 * This function adds a new macro with the specified name and content to the 
 * end of the macro list.
 *
 * @param macro_list The list of macros.
 * @param macro_name The name of the macro to be added.
 * @param macro_content The content of the macro to be added.
 */
void add_macro_to_list(mcr_list *macro_list, char *macro_name, char *macro_content);

/**
 * @brief Retrieves the first word from a line.
 *
 * This function extracts the first word from a line and updates the index 
 * to point to the position after the first word.
 *
 * @param line The line from which to extract the first word.
 * @param first_word The buffer to store the extracted word.
 * @param i A pointer to the index to be updated.
 */
void get_first_word(char *line, char *first_word, int *i);

/**
 * @brief Retrieves a word from a line and updates the index.
 *
 * This function extracts a word from a line starting at the given index 
 * and updates the index to point to the position after the extracted word.
 *
 * @param line The line from which to extract the word.
 * @param macro_name The buffer to store the extracted word.
 * @param i A pointer to the index to be updated.
 */
void get_word_i(char *line, char *macro_name, int *i);

/**
 * @brief Finds a macro in the list by its name.
 *
 * This function searches for a macro with the specified name in the macro 
 * list and returns its index if found.
 *
 * @param macro_name The name of the macro to find.
 * @param list The list of macros to search.
 * @return int The index of the macro if found, otherwise -1.
 */
int find_macro(char *macro_name, mcr_list *list);

/**
 * @brief Retrieves the content of a macro by its index.
 *
 * This function returns the content of the macro at the specified index 
 * in the macro list.
 *
 * @param i The index of the macro in the list.
 * @param macro_list The list of macros.
 * @return char* The content of the macro.
 */
char *get_content(int i, mcr_list *macro_list);

/**
 * @brief Frees all memory allocated for the macro list.
 *
 * This function frees all memory associated with the macros in the list 
 * and the list itself.
 *
 * @param macro_list The list of macros to be freed.
 */
void free_macro_list(mcr_list *macro_list);

/**
 * @brief Validates a macro name.
 *
 * This function checks if the provided macro name is valid according to 
 * the rules defined (e.g., not a reserved word, within length limits, etc.).
 *
 * @param macro_name The macro name to be validated.
 * @return int 1 if the name is valid, otherwise 0.
 */
int validate_macro_name(char *macro_name);

/**
 * @brief Prints an error related to macros.
 *
 * This function prints an error message with the line number and the file 
 * name where the error occurred.
 *
 * @param line The line number where the error occurred.
 * @param error The error message.
 * @param file_name The name of the source file.
 */
void print_error_macro(int line, char *error, char *file_name);

#endif /* MACRO_H */

