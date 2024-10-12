#include "global.h"



/*general functions*/

/*gets a file with wanted ending*/
FILE * get_file(char * file_name, char * ending,char * mode)
{
	char temp[LINE_LENGTH];
	FILE * file;
	strcpy(temp, file_name);
	file = fopen(strcat(temp,ending), mode);
	
	return file;
}


/*print error message*/
void print_error(int line, char * error, char * file_name)
{
	fprintf(stderr,"in %s.am:%d: ERROR: %s\n", file_name, line, error);
}


/*gets a word from line*/
char *get_word(char **line)
{

    int i = 0, j = 0;
    char * word = (char *)malloc(LABEL_LENGTH * sizeof(char));
    if (word == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    while (isspace((*line)[i]) && (*line)[i] != '\n')/*skip spaces*/
    {
        i++;
    }
    
    /* saving the first word in the line on string*/
    while ((!isspace((*line)[i])) && ((*line)[i] != '\0') && j < LABEL_LENGTH)
    {
    word[j] = (*line)[i];
	j++;
	i++;
    }
	
    word[j] = '\0';
    
    /*update the current line pointer*/
    *line = (*line + i);
    return word;
}


/* returns 1 if the given string is white spaces, else 0*/
int is_empty_line(char **current_line)
{
    int i = 0;
    char c = (*current_line)[i];
    /*skip white spaces*/
    while (isspace(c))
    {
        i++;
        c = (*current_line)[i];
    }
    if (c == '\0' || c == EOF)
        return 1;
    else /*extra input*/
        return 0;
}

/*skips spaces, returns 1 if comma found, else 0*/
int get_comma(char **current_line)
{
    int i = 0;
    char c = (*current_line)[i];

    while (isspace(c) && c!= '\n')
    {
        i++;
        c = (*current_line)[i];
    }
    
    /*update the pointer to current place, char after the comma*/
    *current_line = (*current_line + (i + 1));
    if (c == ',')
    {   
        return 1;
    }
    else
    {
        return 0;
    }
}


/*returns register number, -1 if the string isnt register*/
int register_num(char *str)
{
    if (str[0] == 'r' && isdigit(str[1]) && str[2] == '\0')
    {
        int reg_num = str[1] - '0';  /*Convert character to integer*/
        if (reg_num >= 0 && reg_num <= 7)
        {
            return reg_num;
        }
    }
    return INVALID;
}



int is_language_word(char * str)/*check if the given word is in language*/
{
	char * lan_words[NUM_OF_LAN_WORDS] = {"macr", "endmacr", "data", "string", "entry", "extern"};
	char * operations[NUM_OF_OPERATIONS] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	
	int i;
	/* name of instruction */
	for(i = 0; i < NUM_OF_LAN_WORDS; i++)
	{
		if(strcmp(lan_words[i], str) == 0)
			return 1;
	}
	/*operation*/
	for(i = 0; i < NUM_OF_OPERATIONS; i++)
	{
		if(strcmp(operations[i], str) == 0)
			return 1;
	}
	/* name of register */
	if(register_num(str) >= 0)
		return 1;
	return 0;
}


