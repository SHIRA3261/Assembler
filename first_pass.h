
#ifndef PASS1_H
#define PASS1_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "macro.h"

#define END_ARR 30000 /*illegall number to point the end of num data list*/
#define BIGGEST_DATA 16383
#define SMALLEST_DATA -16384 

/* STRUCTURES */

/**
 * @brief Represents a label in the assembly code.
 */
typedef struct label
{
    char name[LABEL_LENGTH];  /* The name of the label. */
    int address;              /* The address associated with the label. */
    int type;                 /* The type of the label (e.g., code, data). */
    struct label *next;       /* Pointer to the next label in the list. */
} label;


/*label linked list*/
typedef struct label_list
{
	label *head;  /* Pointer to the first label in the list. */
    label *tail;  /* Pointer to the last label in the list. */
    int counter;  /* The number of labels in the list. */

}label_list;


/*datas struct*/
typedef struct datas
{
	int * data_items; /*Array of data items. */
}datas;

/*END OF STRUCT*/



/* FUNCTION DECLARATIONS */

/**
 * @brief Performs the first pass of assembly code processing.
 *
 * @param file Pointer to the file being processed.
 * @param file_name The name of the file.
 * @param macro_list Pointer to the list of macros.
 * @param code_array Pointer to the array of encoded words.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param list Pointer to the label list.
 * @param data_list Pointer to the array of data items.
 * @param entry_count Pointer to the count of entries.
 * @param entry_list Pointer to the list of entry labels.
 * @param entry_line Pointer to the array of entry line numbers.
 * @param coded_labels Pointer to the array of coded labels.
 * @param labels_count Pointer to the count of coded labels.
 * @return int 1 if the first pass is successful, 0 otherwise.
 */

int first_pass(FILE *file, char *file_name, mcr_list *macro_list, code_word ***code_array, int *IC, int *DC, label_list *list, int **data_list, int *entry_count, char ***entry_list, int **entry_line, coded_label **coded_labels, int *labels_count);

/**
 * @brief Skips any leading whitespace characters in a line.
 *
 * @param current_line Pointer to the current position in the line.
 */
void skip_space(char **current_line);

/**
 * @brief Checks if a word is a valid label.
 *
 * @param word The word to check.
 * @return char* Pointer to the label if valid, NULL otherwise.
 */
char *check_label(char *word);

/**
 * @brief Validates a label and adds it to the label list if valid.
 *
 * @param first_word The label to validate.
 * @param line_counter Pointer to the current line counter.
 * @param file_name The name of the file being processed.
 * @param list Pointer to the label list.
 * @param macro_list Pointer to the list of macros.
 * @return int 1 if the label is valid, 0 otherwise.
 */
int validate_label(char *first_word, int *line_counter, char *file_name, label_list *list, mcr_list *macro_list);

/**
 * @brief Finds a label in the label list.
 *
 * @param label_name The name of the label to find.
 * @param list Pointer to the label list.
 * @return int The address of the label if found, -1 otherwise.
 */
int find_label(char *label_name, label_list *list);

/**
 * @brief Creates a new label list.
 *
 * @return label_list* Pointer to the newly created label list.
 */
label_list *create_label_list();

/**
 * @brief Inserts a label into the label list.
 *
 * @param list Pointer to the label list.
 * @param label_name The name of the label.
 * @param address_t Pointer to the address associated with the label.
 * @param type_t The type of the label.
 */
void insert_label_table(label_list *list, char *label_name, int *address_t, int type_t);

/**
 * @brief Extracts a character operand from a line.
 *
 * @param array Pointer to the line being processed.
 * @param line_counter Pointer to the current line counter.
 * @param file_name The name of the file being processed.
 * @return int* Pointer to the extracted operand.
 */
int *get_char_opperand(char **array, int *line_counter, char *file_name);

/**
 * @brief Extracts an integer operand from a line.
 *
 * @param input Pointer to the line being processed.
 * @param line_counter Pointer to the current line counter.
 * @param file_name The name of the file being processed.
 * @return int* Pointer to the extracted operand.
 */
int *get_int_opperand(char **input, int *line_counter, char *file_name);

/**
 * @brief Inserts data into the data list.
 *
 * @param item Pointer to the data list.
 * @param operand Pointer to the operand to insert.
 * @param DC Pointer to the data counter.
 */
void insert_datas(int **item, int *operand, int *DC);

/**
 * @brief Adds an entry to the entry list.
 *
 * @param name The name of the entry.
 * @param entry_list Pointer to the entry list.
 * @param size Pointer to the size of the entry list.
 * @param line_counter The current line counter.
 * @param entry_line Pointer to the entry line numbers array.
 */
void add_entry(char *name, char ***entry_list, int *size, int line_counter, int **entry_line);





/*END-FUNC*/
#endif 
