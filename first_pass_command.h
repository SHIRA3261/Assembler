#ifndef FIRST_PASS_COMMAND_H
#define FIRST_PASS_COMMAND_H

#include "global.h"

#define MAX_IC 4096
#define BIGGEST_IMM 2047
#define SMALLEST_IMM -2048
#define ARE_100 4


/*enum for addressing type*/
typedef enum { IMM, LABEL, REG_ADDR, REGISTER, ERROR_ARG } arg_type;



/**
 * @brief Encodes a command from a given line of input, saves the encoded data in provided structures.
 *
 * @param first_word The first word of the command to be encoded.
 * @param current_line Pointer to the current line being processed.
 * @param line_number The number of the current line for error reporting.
 * @param file_name The name of the file being processed.
 * @param IC Pointer to the instruction counter.
 * @param code_array Pointer to the array where the encoded words are stored.
 * @param coded_labels Pointer to the array of coded labels.
 * @param labels_count Pointer to the count of coded labels.
 * @return int 1 if encoding succeeded, 0 otherwise.
 */
int code_command(char *first_word, char **current_line, int line_number, char *file_name, int *IC, code_word ***code_array, coded_label **coded_labels, int *labels_count);


/**
 * @brief Retrieves the opcode associated with a given command word.
 *
 * @param first_word The command word whose opcode is to be retrieved.
 * @return int The opcode corresponding to the command, or ERROR_OP if not a valid command.
 */
int get_opcode(char *first_word);


/**
 * @brief Identifies the type of argument provided and updates the register number if the argument is a register.
 *
 * @param arg_str The argument string to be analyzed.
 * @param reg_num Pointer to the register number to be updated if the argument is a register.
 * @param line_number The current line number for error reporting.
 * @param file_name The name of the file being processed.
 * @return int The type of the argument, or ERROR_ARG if the argument is invalid.
 */
int get_argument_type(char *arg_str, int *reg_num, int line_number, char *file_name);


/**
 * @brief Validates if the addressing modes used in the command are appropriate for the operation.
 *
 * @param op The opcode of the command.
 * @param source_type The addressing type of the source operand.
 * @param target_type The addressing type of the target operand.
 * @param line_number The current line number for error reporting.
 * @param file_name The name of the file being processed.
 * @return int 1 if the addressing modes are valid, 0 otherwise.
 */
int validate_addrressing(int op, int source_type, int target_type, int line_number, char *file_name);


/**
 * @brief Encodes the first word of a command, given the opcode and the addressing types of its operands.
 *
 * @param op The opcode of the command.
 * @param source_addr The addressing type of the source operand.
 * @param target_addr The addressing type of the target operand.
 * @param IC Pointer to the instruction counter.
 * @return code_word* A pointer to the encoded first word.
 */
code_word *code_first_word(int op, int source_addr, int target_addr, int *IC);


/**
 * @brief Encodes an argument and its type into a code word.
 *
 * @param arg_str The argument string to be encoded.
 * @param addr_type The type of addressing for the argument.
 * @param source_reg The register number for the source operand.
 * @param target_reg The register number for the target operand.
 * @param IC Pointer to the instruction counter.
 * @param coded_labels Pointer to the array of coded labels.
 * @param labels_count Pointer to the count of coded labels.
 * @param line_number The current line number for error reporting.
 * @return code_word* A pointer to the encoded argument word.
 */
code_word *code_argument(char *arg_str, int addr_type, int source_reg, int target_reg, int *IC, coded_label **coded_labels, int *labels_count, int line_number);


/**
 * @brief Adds a new encoded word to the code array, resizing the array if necessary.
 *
 * @param new The new code word to be added.
 * @param code_words_array Pointer to the array of code words.
 * @param IC Pointer to the instruction counter.
 */
void add_word_to_arr(code_word *new, code_word ***code_words_array, int *IC);


/**
 * @brief Saves the name and position of a label used in a command for later processing.
 *
 * @param arg_str The label name to be saved.
 * @param IC The current instruction counter value.
 * @param coded_labels Pointer to the array of coded labels.
 * @param labels_count Pointer to the count of coded labels.
 * @param line_number The current line number for error reporting.
 */
void save_coded_label(char *arg_str, int IC, coded_label **coded_labels, int *labels_count, int line_number);


/**
 * @brief Extracts two operands from a line of input, validating their syntax.
 *
 * @param line Pointer to the line being processed.
 * @param arg1_str Pointer to the string where the first operand will be saved.
 * @param arg2_str Pointer to the string where the second operand will be saved.
 * @param line_counter The current line number for error reporting.
 * @param file_name The name of the file being processed.
 * @return int 1 if the operands are successfully extracted, 0 otherwise.
 */
int get_2_operands(char **line, char **arg1_str, char **arg2_str, int line_counter, char *file_name);

#endif /* FIRST_PASS_COMMAND_H */




























