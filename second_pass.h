#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/**
 * @brief Updates the label table before the second pass by adjusting the addresses of labels of type DATA or CODE.
 *
 * @param list Pointer to the label list.
 * @param IC Instruction Counter (IC) value used to update the label addresses.
 */
void update_label_tabel(label_list *list, int IC);

/**
 * @brief Executes the second pass on the assembly code, updates the code array with label addresses, and generates the .ext and .ent files.
 *
 * @param symbol_table Pointer to the symbol table (label list).
 * @param labels_count Number of coded labels.
 * @param coded_labels Array of coded labels.
 * @param file_name Name of the original file.
 * @param IC Instruction Counter (IC) value.
 * @param code_array Pointer to the array of coded words.
 * @param entry_list List of labels defined as ENTRY.
 * @param entry_line Array containing the line numbers where ENTRY labels are defined.
 * @param entry_count Number of ENTRY labels.
 *
 * @return 1 if the pass was successful, 0 if errors were found.
 */
int second_pass(label_list* symbol_table, int labels_count, coded_label * coded_labels, char * file_name, int IC, code_word *** code_array, char** entry_list, int * entry_line, int entry_count);

/**
 * @brief Generates the .ob file and writes the binary and octal representation of the code and data.
 *
 * @param file_name Name of the original file.
 * @param code_array Array of coded words.
 * @param data_list List of data items.
 * @param IC Instruction Counter (IC) value.
 * @param DC Data Counter (DC) value.
 */
void print_ob(char * file_name, code_word ** code_array, int * data_list, int IC, int DC);

/**
 * @brief Writes the binary representation of the commands to the .ob file.
 *
 * @param file Pointer to the .ob file.
 * @param code_array Array of coded words.
 * @param IC Instruction Counter (IC) value.
 */
void print_commands_binary(FILE * file, code_word **code_array, int IC);

/**
 * @brief Writes the binary representation of the data to the .ob file.
 *
 * @param ob_file Pointer to the .ob file.
 * @param file_name Name of the original file.
 * @param item Array of data items.
 * @param IC Instruction Counter (IC) value.
 */
void print_data(FILE * ob_file, char * file_name, int *item, int IC);

/**
 * @brief Creates the .ent file and writes the labels defined as ENTRY along with their addresses.
 *
 * @param list Pointer to the label list.
 * @param file_name Name of the original file.
 * @param entry_list List of labels defined as ENTRY.
 * @param entry_line Array containing the line numbers where ENTRY labels are defined.
 * @param entry_count Number of ENTRY labels.
 *
 * @return 1 if the file was created successfully, 0 if errors were found.
 */
int entry_file(label_list *list, char * file_name, char** entry_list, int * entry_line, int entry_count);

#endif

