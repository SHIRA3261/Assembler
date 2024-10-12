#ifndef GLOBAL_H
#define GLOBAL_H

#define RAM_SIZE 4096
#define LINE_LENGTH 81
#define LABEL_LENGTH 31
#define NUM_OF_OPERATIONS 16
#define INVALID -1
#define IC_VAL 100
#define NUM_OF_LAN_WORDS 6
#define NUM_OF_OPERATIONS 16
#define BIGEST_NUM 32768
#define EOS		'\0'
#define EOL		'\n'


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




/*===============DATA STRUCTURES=======================*/


/*containes data about label used in commands, for 2nd pass*/
typedef struct coded_label
{
	char label_name[LABEL_LENGTH];
	int label_ic;
	int line_number;
	
}coded_label;



/*this is struct for coding words. each code word is one of the 4 types in the union*/
typedef union code_word
{
    struct
    {
	unsigned int ARE : 3;
	unsigned int target_op : 4;
	unsigned int source_op : 4;	
	unsigned int opcode : 4;
    } first_code_word;

    struct
    {
    unsigned int ARE : 3;
	unsigned int num : 12; /*in c90, the minimal size of int is 2 bytes = 16 bits*/
    } number;

    struct
    {
    unsigned int ARE : 3;
	unsigned int addr : 12;
    }label_address;

    struct
    {
	unsigned int ARE : 3;        
    unsigned int target_addr : 3;
	unsigned int source_addr : 3;
        
    } registers;
} code_word;


/* opcodes from 0-14, 15 is error  */
typedef enum opcode
{ MOV,CMP,ADD,SUB,LEA,CLR,NOT,INC,DEC,JMP,BNE,RED,PRN,JSR,RTS,STOP,ERROR_OP} 
opcode;

/*label types for table*/
typedef enum label_type
{CODE,STR,DATA,EXTERN}
label_type;


/*================general functions========================*/

FILE * get_file(char * file_name, char * ending,char * mode);
/**
 * @file global.h
 * @brief This file contains utility functions for file handling, error printing, string parsing, and language-specific checks.
 */  

/**
 * @brief Opens a file with the specified name, appending a given file extension.
 * 
 * @param file_name The base name of the file to be opened.
 * @param ending The file extension to be appended to the file name.
 * @param mode The mode in which the file should be opened (e.g., "r" for read, "w" for write).
 * 
 * @return FILE* A pointer to the opened file. Returns NULL if the file cannot be opened.
 */
FILE *get_file(char *file_name, char *ending, char *mode);

/**
 * @brief Prints an error message to the standard error output.
 * 
 * @param line The line number where the error occurred.
 * @param error The error message to be printed.
 * @param file_name The name of the file in which the error occurred.
 */
void print_error(int line, char *error, char *file_name);

/**
 * @brief Extracts the first word from a line of text.
 * 
 * @param line A pointer to the line from which to extract the word. The line pointer is updated to the position after the extracted word.
 * 
 * @return char* A pointer to the extracted word. The caller is responsible for freeing the memory. Returns NULL if memory allocation fails.
 */
char *get_word(char **line);

/**
 * @brief Checks if a line contains only white spaces.
 * 
 * @param current_line A pointer to the line to be checked.
 * 
 * @return int Returns 1 if the line contains only white spaces or is empty, otherwise returns 0.
 */
int is_empty_line(char **current_line);

/**
 * @brief Skips white spaces in a line and checks for a comma.
 * 
 * @param current_line A pointer to the line to be checked. The line pointer is updated to the position after the comma.
 * 
 * @return int Returns 1 if a comma is found, otherwise returns 0.
 */
int get_comma(char **current_line);

/**
 * @brief Determines if a string represents a valid register name.
 * 
 * @param str The string to be checked.
 * 
 * @return int Returns the register number if valid (0-7), otherwise returns -1.
 */
int register_num(char *str);

/**
 * @brief Checks if a given word is a reserved language word or operation.
 * 
 * @param str The word to be checked.
 * 
 * @return int Returns 1 if the word is a reserved language word, operation, or register, otherwise returns 0.
 */
int is_language_word(char *str);

#endif
