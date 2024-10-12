
#include "global.h"
#include "macro.h"
#include "first_pass_command.h"
#include "first_pass.h"


int first_pass(FILE *file, char *file_name, mcr_list *macro_list, code_word ***code_array, int *IC_P, int *DC, label_list *list, int **data_list, int * entry_count,char *** entry_list, int** entry_line, coded_label** coded_labels, int* labels_count)
{

    char *name = (char *)malloc(LINE_LENGTH * sizeof(char));
    char line[LINE_LENGTH + 2];
    char *current_line;
    char *label_name = (char *)malloc(LABEL_LENGTH * sizeof(char));
    char *first_word = (char *)malloc(LINE_LENGTH * sizeof(char));
    char *temp_label = (char *)malloc(LABEL_LENGTH * sizeof(char));

    
    int *operand_int;
    int is_label = 0;
    int type;
    int count = 0;
    int *line_counter = &count;
    int status = 1;/*no errors in beginning*/
    
    if (!name || !label_name || !first_word || !temp_label)
    {
        printf("memory allocation falied");
        exit(1);
    }
    
	
    while (!feof(file))/*loop through the lines*/
    {
        
        /* reached the end of the file*/
		if (fgetc(file) == EOF)
			break;
		fseek(file, -1, SEEK_CUR);	
        	
        /*reset flag of label for next iteration*/
        type = 0;
        is_label = 0;
		current_line = line;


        fgets(line, LINE_LENGTH + 2, file);
        (*line_counter)++;
         /*line length is checked in macro pass */
	
	
	if(line[0] == ';')/*its a comment line */
		continue;
	
	if(is_empty_line(&current_line))/*its empty line*/
		continue;
	       
	first_word = get_word(&current_line);
	

        temp_label = check_label(first_word);/*gets label name without validation*/

        if (temp_label != NULL)/*optional label*/
        {
            if (validate_label(temp_label, line_counter, file_name, list, macro_list)) 
            
            {
                /* turning on the label flag*/
                is_label = 1;
                skip_space(&current_line);

                /* copy name of label*/
                strcpy(label_name, temp_label);
                
                if(is_empty_line(&current_line))/*its empty line*/
                {
                	print_error(*line_counter, "empty label", file_name);
                	status = 0;
                	continue;
                }

                /* move to next word */
                first_word = get_word(&current_line);
	
            }
	    else/*label is invalid*/
		{
	        status = 0;
			continue;
		}

        }

	
        /*.string or .data*/

        if (strcmp(first_word, ".string") == 0 || strcmp(first_word, ".data") == 0)
        {
	    
         /*2 of these are the same type!!!!!!!!!!!*/  
	    if (strcmp(first_word, ".string") == 0)
                    {
                        type = STR;
                    }
            else /* .data */
                    {	
                        type = DATA;
                    }
	    
	    if (is_label)
            {   
                    insert_label_table(list, label_name, DC, type);  
            }

             /*.string */
             
            if (type == STR)
            {
                operand_int = get_char_opperand(&current_line, line_counter, file_name);
            }
            /*.data*/
           if (type == DATA)
            {
                operand_int = get_int_opperand(&current_line, line_counter, file_name);
            }

		   
	    if (operand_int == NULL)
		{
	        status = 0;
			continue;
		}
			
            insert_datas(data_list, operand_int, DC);
            
            
           } 
 

        /*.extern or .entry*/
        else if (strcmp(first_word, ".extern") == 0)
        {
        	int zero_addr = 0;
        	
            name = get_word(&current_line);
         
                
			if (find_label(name, list) >= 0)
              {
                  print_error(*line_counter, "extern label cant be defined", file_name);
                  status = 0;
                  continue;
              }
              
            if(!validate_label(name, line_counter, file_name, list, macro_list)) 
            {
            	status = 0;
            	continue;	
            } 
            
              
            insert_label_table(list, name, &zero_addr, EXTERN);
			
                
        }

           
       
        
        else if (strcmp(first_word, ".entry") == 0)
        {
        	name = get_word(&current_line);
        	add_entry(name, entry_list, entry_count, *line_counter, entry_line) ; 

        	/*save entry name for second pass */
        
        }
        
        

		else /*the line is command*/
        {

            if (is_label)
            {
                insert_label_table(list, label_name, IC_P, CODE); 
            }

       /*call fuction to code command*/
        if(code_command(first_word, &current_line, *line_counter, file_name, IC_P, code_array, coded_labels, labels_count) == 0)
        {
        	status = 0;	
        }

        }


	
	/*free(name);
	free(label_name); 
	free(first_word); 
	free(temp_label);*/
			      
    } /*end of while-end of file :) */

    return status; /*status of first pass sucsses*/

} /*end of first pass*/



/***************************helper functions ***************************/

void skip_space(char **current_line)
{

    int i = 0;
    char c = (*current_line)[i];

/*Skip spaces until a non-space character is found*/
    while (isspace(c))
    {
        i++;
        c = (*current_line)[i];
    }

    *current_line = (*current_line + i);

    return;
}


/*return index on end of label*/
char *check_label(char *word)
{
    char *temp = (char *)malloc(LINE_LENGTH * sizeof(char));
    
    if (temp == NULL)
    /*handle memory allocation failure*/
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }


/*Check if the word ends with a colon (':')*/
    if (word[strlen(word) - 1] == ':')
    {
        int i = 0, j = 0;
        while (word[i] != EOS && word[i] != ':' && i < LABEL_LENGTH - 1)
        {
            temp[j++] = word[i++];
        }
        temp[j] = EOS;
        strcpy(word, temp);

        return temp;
    }

    return NULL;
}

/*check label name is valid*/
int validate_label(char *first_word, int *line_counter, char *file_name, label_list *list, mcr_list *macro_list)
{


    char c = first_word[0];
    int i = 0;
    /*Ensure the label contains only letters or digits*/
    while(c != '\0')
    {	
    	if(!isalpha(c) && !isdigit(c))
    	{
    		print_error(*line_counter, "label name must contain letters or numbers only", file_name);
       		return 0;
    	}
    	i++;
    	c = first_word[i];
    }
    
    /*check the label starts with a letter*/
     if(!isalpha(first_word[0]))
    {
    	print_error(*line_counter, "Label name must start with a letter", file_name);
        return 0;
    
    }
    
   /*check the label is not a reserved word*/
    if(is_language_word(first_word))
    {
        print_error(*line_counter, "Label name is a language word", file_name);
        return 0;
    }
    
    
  /*check the label name is not too long*/
    if (strlen(first_word) > LABEL_LENGTH)
    {
        print_error(*line_counter, "Label name is too long", file_name);
        return 0;
    }

    if (strcmp(first_word, "") == 0)
    {
        print_error(*line_counter, "no label name", file_name);
        return 0;
    }

    /* check if it define on macro  */
    if (find_macro(first_word, macro_list) >= 0)
    {
        print_error(*line_counter, "Label name has been define as a macro", file_name);
        return 0;
    }

    /* check if it already define */
    if (find_label(first_word, list) >= 0)
    {
        print_error(*line_counter, "Label name has been already define", file_name);
        return 0;
    }

    return 1;
}


/*search for a word in label table*/
int find_label(char *label_name, label_list *list)
{
    label *temp = list->head;
    int index = 0;
    /*Traverse the label list to find the label*/
    while (temp != NULL)
    {
        if (strcmp(temp->name, label_name) == 0)
        {
            return index;
        }

        temp = temp->next;
        index++;
    }

    return INVALID;
}

/*creates empty list*/
label_list *create_label_list()
{
    label_list *list = (label_list *)malloc(sizeof(label_list));
    /*handle memory allocation failure*/
    if (!list)
    {
        printf("memory allocation falied");
        exit(1);
    }
/*Initialize the list fields*/
    list->head = NULL;
    list->tail = NULL;
    list->counter = 0;
    return list;
}


/*insert a new label to table*/
void insert_label_table(label_list *list, char *label_name, int *address_t, int type_t)
{

    label *new = (label *)malloc(sizeof(label));
        /*handle memory allocation failure*/
    if (!new)
    {
        printf("memory allocation failed");
        exit(1);
    }
    
   	 /*Copy label details to the new label*/
    strcpy(new->name, label_name);
    new->address = *address_t;
    new->type = type_t;
    new->next = NULL;
    list->counter = (list->counter) + 1;

    if (list->head == NULL) /*this is the first label in list*/
    {
        list->head = new;
        list->tail = new;
    }
    else /*add label to end of list*/
    {
        (list->tail)->next = new;
        (list->tail) = new;
    }
}

/*insert data to data image*/
void insert_datas(int **data_list, int *operand, int *DC)
{
   
    /*sizof item = *DC*/
    	int i = 0;
    	int k = (*DC);/*to override the END_ARR*/
    	int * temp;
    	
    	/*Insert operands into the data list*/
    	while(operand[i] != END_ARR)
		{		
			(*data_list)[k] = operand[i];
			i++;
			k++;
			temp = (int *)realloc((*data_list),((k+2) * sizeof(int)));
			
			if(!temp)
			{
				printf("memmory alloction failed\n");
			}
			
			(*data_list) = temp;
		}
		(*data_list)[k] = END_ARR;/*end arr flag*/

    (*DC)  = k; /*MYBYE K +1 */

    return;
}


/*save .string operands in int array*/
int * get_char_opperand(char **array, int *line_counter, char *file_name)
{

    char c;
    int i = 0;
    int j = 0;
    int *oper = (int *)malloc(sizeof(int));
    c = (*array)[i] ;
    
    if (!oper)
    {
        printf("memory allocation falied");
        exit(1);
    }
       
/*Check for a space before the operand*/
    if ( c!= ' ' )
    {
        print_error(*line_counter, "required space after .string", file_name);
        free(oper);
        return NULL;
    }
    
    skip_space(array);
    i = 0;
    c = (*array)[i] ;
    
    
    /*Check for an opening quotation mark*/
      if ( c!= '\"' )
    {
        
        print_error(*line_counter, "missing opening \" ", file_name);
        free(oper);
        return NULL;
    }
    
     i++;
     c = (*array)[i] ;

/*Extract characters between quotation marks*/
    while ( c != '\n' && c!='\"')       
    {

        oper[j] = (int) c;

        i++;
        j++;
        oper = (int *)realloc(oper, (i+1) * sizeof(int));                                
                                               
        c = (*array)[i] ;
    }


        if ( c!= '\"')
    {
    	 putchar(c);
        print_error(*line_counter, "missing closing \"  ", file_name);
        free(oper);
        return NULL;
    }
     
    *array = (*array + i+1) ;
    oper[j] = '\0' ; 
    oper = (int *)realloc(oper, (i+2) * sizeof(int)); 
    if (!oper)
    {
        printf("memory allocation falied");
        exit(1);
    }
    oper[j+1] = END_ARR ; 
                                                                      
    /*Check if the line is empty after the operand*/
   if(is_empty_line(array))
		 return oper;
   else
   {
  	   print_error(*line_counter, "extanous text after string definition", file_name);
       free(oper);
       return NULL;                                                                            
   } 
   
}


/*save .data operands in int array*/
int *get_int_opperand(char **input, int *line_counter, char *file_name)
{
    int *numbers = malloc(sizeof(int));
    int i_num = 0; /*integer number*/
    int k = 0;     /*numbers array pointer*/
    int comma;
    int *temp; /*for memory allocation*/
    
    int i = 0;
	char c = (*input)[i];
	
	if(!numbers)
	{
        printf("memory allocation falied");
        exit(1);
	}

    while (c != EOS && c != EOF && c != '\n') /*get single number*/
    {
        int j = 0;            /*s_num counter*/
        char s_num[] = "                                                  "; /*empty string for string num*/
        i = 0;            /*input pointer*/
        c = (*input)[i];

        /*skip white spaces*/
        while (isspace(c))
        {
            i++;
            c = (*input)[i];
        }

        while (isdigit(c) || (c =='-') || (c =='+')) /*get string number*/
        {
            s_num[j] = c;
            j++;
            i++;
            c = (*input)[i];
        }

        i_num = atoi(s_num); /*convert to int*/
        
        
         if (i_num < SMALLEST_DATA || i_num > BIGGEST_DATA)
        {
            print_error(*line_counter, "integer data out of range", file_name);
            free(numbers);
            return NULL;
        }
        
        numbers[k] = i_num;
        k++;
        temp = realloc(numbers, sizeof(int) * (k + 1)); /*increase numbers array size for next number*/
        
        if (temp == NULL)
        {
            print_error(*line_counter, "memmory allocation failed", file_name);
            free(numbers);
            return NULL;
        }

        numbers = temp;
       
        /*return the last char back to input, we havent checked it yet*/
        *input = *input + i;

        /*skip white spaces*/
        while (isspace(c))
        {
            i++;
            c = (*input)[i];
        }
        
	
	if(is_empty_line(input))/*end of nymbers list*/
	{
		numbers[k] = END_ARR;
		return numbers;
	}	
	     
	comma = get_comma(input);
	
        if (comma == 0)
        {
            print_error(*line_counter, "no comma between numbers", file_name);
            free(numbers);
            return NULL;
        }

    } /*end get single number while*/
    return numbers;
}


void add_entry(char * name,char *** entry_list, int * size, int line_counter, int ** entry_line)        
{

/*add entry name into the array
* add number of line into the array
*/
 (*entry_list)[*size] = name ;
 (*entry_line)[*size] = line_counter;
 
 (*size)++;


}




