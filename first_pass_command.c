

#include "first_pass_command.h"

#include "macro.h"

/*func for coding command word: */

int code_command(char *first_word, char **current_line, int line_number, char *file_name, int *IC, code_word ***code_array, coded_label **coded_labels, int *labels_count)

{

    code_word *first_code_word;
    opcode op = get_opcode(first_word);

    /*get arguments:*/

    /*first commands group - 2 operands*/

    if (op == MOV || op == CMP || op == ADD || op == SUB || op == LEA)

    {

        int reg1_num = 0;/*optional register numbers. updated by "get_argument_type" if the type is register. */
        int reg2_num = 0;

        char *arg1_str = (char *)malloc(LABEL_LENGTH * sizeof(char));
        char *arg2_str = (char *)malloc(LABEL_LENGTH * sizeof(char));

        arg_type arg1_type;
        arg_type arg2_type;

        if (arg1_str == NULL || arg2_str == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return 0;
        }

        /*saves the 2 args in strings and check syntax*/

        if (!get_2_operands(current_line, &arg1_str, &arg2_str, line_number, file_name))
        {
            return 0;
        }

        /*get args type*/
        arg1_type = get_argument_type(arg1_str, &reg1_num, line_number, file_name);
        arg2_type = get_argument_type(arg2_str, &reg2_num, line_number, file_name);

        /*validate arguments */

        if ((arg1_type == ERROR_ARG) || (arg2_type == ERROR_ARG) || !validate_addrressing(op, arg1_type, arg2_type, line_number, file_name))
        {
            return 0;
        }

        /*coding the first word and the args by functions*/
        
        first_code_word = code_first_word(op, arg1_type, arg2_type, IC);
        add_word_to_arr(first_code_word, code_array, IC);

        /*for 2 registers - 1 code word*/
        if ((arg1_type == REGISTER || arg1_type == REG_ADDR) && (arg2_type == REGISTER || arg2_type == REG_ADDR))
        {
            code_word *arg1_word = code_argument(arg1_str, arg1_type, reg1_num, reg2_num, IC, coded_labels, labels_count, line_number);
            add_word_to_arr(arg1_word, code_array, IC);
        }

        else /*code word for each argument*/
        {
            code_word *arg1_word = code_argument(arg1_str, arg1_type, reg1_num, reg2_num, IC, coded_labels, labels_count, line_number);
            code_word *arg2_word;
            add_word_to_arr(arg1_word, code_array, IC);
            arg2_word = code_argument(arg2_str, arg2_type, reg1_num, reg2_num, IC, coded_labels, labels_count, line_number);
            add_word_to_arr(arg2_word, code_array, IC);
        }

        /*CHECK if IC > MAX_IC*/
        if (*IC > RAM_SIZE)
        {
            print_error(line_number, "too many comands: out of RAM memmory", file_name);
            return 0;
        }
        
        return 1;

    } /*end if: first commands group*/

    /*second commands group - 1 operand*/
    if (op == CLR || op == NOT || op == INC || op == DEC || op == JMP || op == BNE || op == RED || op == PRN || op == JSR)
    {
        
        int reg_num = 0;
        char *arg1_str; 
        arg_type arg1_type;
        code_word *arg1_word;
        
        if (is_empty_line(current_line))
        {
            print_error(line_number, "missing operand after command", file_name);
            return 0;
        }
        
        arg1_str = get_word(current_line);
		arg1_type = get_argument_type(arg1_str, &reg_num, line_number, file_name);
		
        if (!is_empty_line(current_line))
        {
            print_error(line_number, "extranous text after command, this command gets only 1 operand", file_name);
            return 0;
        }

        if ((arg1_type == ERROR_ARG) || !validate_addrressing(op, ERROR_ARG, arg1_type, line_number, file_name))
        {
            return 0;
        }

		/*code the command by functions*/
        first_code_word = code_first_word(op, ERROR_ARG, arg1_type, IC);
        add_word_to_arr(first_code_word, code_array, IC);

        /*source = 0 because there is no source rgister*/
        arg1_word = code_argument(arg1_str, arg1_type, 0, reg_num, IC, coded_labels, labels_count, line_number);
        add_word_to_arr(arg1_word, code_array, IC);

        /*CHECK if IC > MAX_IC*/
        if (*IC > RAM_SIZE)
        {
            print_error(line_number, "too many comands: out of RAM memmory", file_name);
            return 0;
        }

        return 1;
    }/*end if - second command group*/

    /*third commands group - no operands*/
    if (op == RTS || op == STOP)
    {
        if (!is_empty_line(current_line))
        {
            print_error(line_number, "extranous text after command, this command doesnt get operands", file_name);
            return 0;
        }

        first_code_word = code_first_word(op, ERROR_ARG, ERROR_ARG, IC); /*no operands*/
        add_word_to_arr(first_code_word, code_array, IC);

        /*CHECK if IC > MAX_IC*/
        if (*IC > RAM_SIZE)
        {
            print_error(line_number, "too many comands: out of RAM memmory", file_name);
            return 0;
        }

        return 1;
    }

    /*else - the opcode doesnt match any command*/
    print_error(line_number, "Undefined command name", file_name);
    return 0; /*error*/

} /*end func: code command*/


/*HELPER FUNCTIONS:*/


/*gets a word and returns its opcode, or error opcode if its not command*/
int get_opcode(char *first_word) 
{
	char * operations[NUM_OF_OPERATIONS] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
    
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(operations[i], first_word) == 0)
            return i;/*the operations in their opcode index*/
    }
    
    return ERROR_OP;/*no opcode found*/
}


/*gets argument type and updates register num if its register. print error if illegal argument*/
int get_argument_type(char *arg_str, int *reg_num, int line_number, char *file_name)
{
    if (arg_str[0] == '#')/*immediate number starts with #*/
    {

		char c = arg_str[1];
		int i = 1;
		int num; 
		
		while(c != '\0')
		{
			if(!(isdigit(c) || ((c == '-' || c == '+') && i == 1))) /*check there is number after #*/
			{	
				print_error(line_number, "invalid argument type - no immidiate number after \'#\'", file_name);
        		return ERROR_ARG;		
			}
			
			i++;
			c = arg_str[i];
		}
		num = atoi(arg_str + 1);

        if (((num <= BIGGEST_IMM) && (num >= SMALLEST_IMM)))/*num in range*/
        {
            return IMM;
        }

		/*num not in range*/
        print_error(line_number, "invalid argument type - immidiate number is not in range", file_name);
        return ERROR_ARG;
    }

    if (arg_str[0] == '*')/*register address*/
    {

        if (register_num(arg_str + 1) >= 0) /*check after * its register*/
        {
            (*reg_num) = register_num(arg_str + 1);
            return REG_ADDR;
        }
        /*no register after * */
        print_error(line_number, "invalid argument type - no register name after'*'", file_name);
        return ERROR_ARG;
    }

    if (register_num(arg_str) >= 0)/*regular register*/
    {
        (*reg_num) = register_num(arg_str);
        return REGISTER;
    }

    /*else we return label, will be checked in second pass*/
    return LABEL;

}


/* returns 0 and prints error if adressing doesnt matche the operation, else 0*/
int validate_addrressing(int op, int source_type, int target_type, int line_number, char *file_name)
{
    if (((op == MOV || op == ADD || op == SUB || op == CLR || op == NOT || op == INC || op == DEC || op == RED) && (target_type == IMM)) ||
        (op == LEA && (target_type == IMM || source_type != LABEL)) ||
        ((op == JMP || op == BNE || op == JSR) && (target_type == IMM || target_type == REGISTER)))
    {
        print_error(line_number, "invalid adressing - operands dont match operation", file_name);
        return 0; /* illegal adressing , the adressing methods above dont match the operations*/
    }
    
    else
        return 1; /*adressing ia legal*/
}


/*gets the opcode and the arguments' type and return the first code word*/
code_word *code_first_word(int op, int source_addr, int target_addr, int *IC)
{
   
    int source_adrress_mask = 1 << source_addr;
    int target_adrress_mask = 1 << target_addr;
    code_word *first_word = (code_word *)calloc(1, sizeof(code_word));
    
    (*IC)++;

    if (first_word == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    /*if there is no argument - mask will be 0*/

    if (source_addr == ERROR_ARG)
    {
        source_adrress_mask = 0;
    }

    if (target_addr == ERROR_ARG)
    {
        target_adrress_mask = 0;
    }
    
    /*save coding in bit fields*/

    first_word->first_code_word.opcode = op;

    first_word->first_code_word.source_op = source_adrress_mask;

    first_word->first_code_word.target_op = target_adrress_mask;

    first_word->first_code_word.ARE = ARE_100;

    return first_word;
}


/*gets argument and its type, and returns the coding word, if label saves info for second pass*/
code_word *code_argument(char *arg_str, int addr_type, int source_reg, int target_reg, int *IC, coded_label **coded_labels, int *labels_count, int line_number)

{

    code_word *word = (code_word *)calloc(1, sizeof(code_word));

	(*IC)++;

    if (word == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    switch (addr_type)
    {
    
    case IMM:/*code imm number*/
    {
        int imm_num = atoi(arg_str + 1);
        word->number.num = imm_num;
        word->number.ARE = ARE_100; 
        break;
    }

    case LABEL: /*this will be updated in second pass*/
    {
        word->label_address.addr = 0;
        word->label_address.ARE = 0;
        /*prepare for second pass*/
        save_coded_label(arg_str, *IC, coded_labels, labels_count, line_number);
    }

    break;

    /*code 1 or 2 registers and registers adress*/
    case REG_ADDR:
    case REGISTER:
        word->registers.source_addr = source_reg;
        word->registers.target_addr = target_reg;
        word->registers.ARE = ARE_100;
        break;
    }

    return word;
}


/*adds space in the array and inserts the new word in the last index */
void add_word_to_arr(code_word *new, code_word ***code_words_array, int *IC) /*CHECK the **   */
{
    if (*IC == IC_VAL + 1) /*the first cell in array, memmory allocated in main allready*/
    {
        /*add the new word in the last index.*/
        (*code_words_array)[((*IC) - IC_VAL) - 1] = new;
    }

    else /*add memmory*/
    {
        code_word **temp = (code_word **)realloc(*code_words_array, ((*IC) - IC_VAL) * sizeof(code_word *));
        if (!temp)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        *code_words_array = temp;

        /*add the new word in the last index.*/

        (*code_words_array)[((*IC) - IC_VAL) - 1] = new;
    }
}

/*gets a string and saves 2 operand with comma between*/
int get_2_operands(char **line, char **arg1_str, char **arg2_str, int line_number, char *file_name)

{

    int i = 0;
    int j = 0;
    char c = (*line)[i];
    
	if (is_empty_line(line))
    {
        print_error(line_number, "missing operands after command", file_name);
        return 0;
    }
    
    while (isspace(c) && (c != '\n')) /*IS EMPTY LINE SKIPS SPACES????*/
    {
        i++;
        c = (*line)[i];
    }

    /* saving the first word in the line on string*/
    while (!isspace(c) && (c != '\0') && (c != '\n') && (j < LABEL_LENGTH) && (c != ','))
    {
        (*arg1_str)[j] = c;
        j++;
        i++;
        c = (*line)[i];
    }

    (*arg1_str)[j] = '\0';

    /*update the current line pointer for get comma*/
    *line = (*line + i); 

    /*check comma:*/
    if (c == ',')
    {
        *line = (*line + 1); /*move pointer after comma*/
    }

    else if (!get_comma(line))
    {
        print_error(line_number, "missing comma between operands", file_name);
        return 0;
    }

    if (is_empty_line(line))
    {
        print_error(line_number, "missing second operand", file_name);
        return 0;
    }

    i = 0;
    j = 0;
    c = (*line)[i];

    while (isspace(c) && (c != '\n')) /*SKIP SPACEs*/
    {
        i++;
        c = (*line)[i];
    }

    /*save second operand*/
    while (!isspace(c) && (c != '\0') && (c != '\n') && (j < LABEL_LENGTH))
    {
        (*arg2_str)[j] = (*line)[i];
        j++;
        i++;
        c = (*line)[i];
    }

    (*arg2_str)[j] = '\0';

    /*update the current line pointer*/

    *line = (*line + i);

    if (!is_empty_line(line)) /*the rest is not empty*/
    {
        puts(*line);
        print_error(line_number, "extranous text after command, this command gets only 2 operands", file_name);
        return 0;
    }

    return 1;
}

/*saves names and position of labels used in commands*/
void save_coded_label(char *arg_str, int IC, coded_label **coded_labels, int *labels_count, int line_number)
{
    coded_label new;
    strcpy(new.label_name, arg_str);
    new.label_ic = IC - 1;
    new.line_number = line_number;

    if (*labels_count != 0)
    {
        coded_label *temp = realloc(*coded_labels, sizeof(coded_label) * (*labels_count + 1));
        if (temp == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        *coded_labels = temp;
    }

	/*save coded label struct in last index of arr*/
    (*coded_labels)[*labels_count] = new;
    (*labels_count)++;
}

