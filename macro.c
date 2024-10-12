
#include "global.h"
#include "macro.h"

/*checks line length, goes to next line in file*/
#define CHECK_LINE_LENGTH \
if (strlen(line) > LINE_LENGTH)\
{\
	print_error_macro(line_number, "line is too long", file_name);\
	/* set the pointer to the next line, run to end of line in unknowen size */\
	do\
	{\
		c = fgetc(source_file);\
	} while (c != '\n' && c != EOF);\
	continue;\
	error =  1;\
}	


/* ================ pre assembler - macros ================*/


int macro_func(FILE * source_file, char * file_name,mcr_list * macro_list)
{

	char first_word[LINE_LENGTH] = "";
	char line[LINE_LENGTH + 2];
	char macro_name[MAX_NAME_LEN];
	char macro_content[RAM_SIZE] = "";
	char temp[LINE_LENGTH];
	int line_number = 0;
	int error = 0;
	char c;
	FILE * target_file;
	int index_in_list = 0;
	char *current_position;

	strcpy(temp, file_name);
	target_file = fopen(strcat(temp, ".am"), "w+");
	
	
	/* reading the source file */
	while(!feof(source_file))
	{
		
		int current_i = 0;
	
		/* reached the end of the file*/
		if (fgetc(source_file) == EOF)
			break;
		fseek(source_file, -1, SEEK_CUR);
		
		/* get the line */
		fgets(line, LINE_LENGTH + 2, source_file);
		line_number++;

		/* check if line length  > 80*/
		CHECK_LINE_LENGTH

		get_first_word(line, first_word, &current_i);/* i: the number of chars from macr to the macro name,including*/
		
		/* there is a macro */
		if(strcmp(first_word, "macr") == 0)
		{
			/*get macro name and update line index*/
			get_word_i(line, macro_name, &current_i);
			
			/*validate macro name*/
			if(!validate_macro_name(macro_name) || (find_macro(macro_name, macro_list) >= 0))
			{
				print_error_macro(line_number,"Invalid macro name", file_name);
				error = 1;
				continue;
			}
			
			/*check the rest of line after macro name is empty*/			
			current_position = (line + current_i);
			if(!is_empty_line(&current_position))
			{
				print_error_macro(line_number, "Extranous text after macro name", file_name);
				error = 1;
				continue;

			}

			/* get the next line */
			fgets(line, LINE_LENGTH + 2 , source_file);
			line_number++;

			/* check if line length  > 80*/
			CHECK_LINE_LENGTH

			/* saving macro content*/
			while(strcmp(first_word, "endmacr") != 0)
			{
				
				get_first_word(line, first_word, &current_i);
				
				if(strcmp(first_word,"endmacr") == 0)
				{
					
					/*check the rest of line after endmacr is empty */

					char *current_position = (line + current_i);
					if(!is_empty_line(&current_position))
					{
					print_error_macro(line_number, "Extranous text after end of macro", file_name);
					error = 1;
					}

					break;/*end of saving macro content*/
				}


				else/*inside macro*/
				{
					/*save line to macro content*/
					strcat(macro_content, line);
						
					/*get nex line*/
					fgets(line, LINE_LENGTH +2 , source_file);
					line_number++;
					/* check if line length  > 80*/
					CHECK_LINE_LENGTH
					
				}
					
			}/*end of while - saving macro content*/
			
			if(error)
			{
				continue;/*to next line*/
			}

			/* insert the macro into the list */
			add_macro_to_list(macro_list,macro_name,macro_content);
			
			/* reset macro variables*/
			strcpy(macro_content ,"");
			strcpy(macro_name ,"");
		
		}
		
		else/*no new macro definition*/
		{
			/* if the first word is a name of macro - replace in macro content*/
			index_in_list = find_macro(first_word, macro_list);
			if(index_in_list >= 0)
			{

				/*if the rest of line after macro name is not empty, dont copy macro content*/
				char *current_position = (line + current_i);
				
				if(!is_empty_line(&current_position))
				{
				print_error_macro(line_number,"extranous text after calling macro", file_name);
				continue;
				}

				/* get the macro content and copy to the targrt_file*/ 
				fputs(get_content(index_in_list,macro_list), target_file);
			}
			
			else/*copy the line to am file*/
			{
				fputs(line ,target_file);
			}
		}
	
	}/*end while on file*/

	
	fclose(target_file);
	fclose(source_file);
	return error;
}



/*==========================================helper functions==================================================*/

/*creates empty macro list*/
mcr_list * create_macro_list()
{
	mcr_list * macro_list = (mcr_list*) malloc(sizeof(mcr_list));
	if(!macro_list)
	{
		printf("memory allocation falied");
		exit(1);
	}
	macro_list->head = NULL;
	macro_list->tail = NULL;
	macro_list->counter = 0;
	return macro_list;
}

/*adds macro to list*/
void add_macro_to_list(mcr_list * macro_list, char * macro_name, char * macro_content)
{
	
	macro * new = (macro*) malloc(sizeof(macro));
	if(!new)
	{
		printf("memory allocation failed");
		exit(1);
	}

	strcpy(new->name, macro_name);
	strcpy(new->content, macro_content);
	new->next = NULL;
	macro_list-> counter = (macro_list-> counter)+1;

	if(macro_list->head == NULL)/*this is the first macro in list*/
	{
		macro_list->head = new;
		macro_list->tail = new;
	}
	else /*add macro to end of list*/
	{
		(macro_list->tail)->next = new;
		(macro_list->tail) = new;
	}

}


/*gets the first word from the line and updates i*/
void get_first_word(char * line, char * first_word, int * i)
{
		 int j = 0;
		(*i) = 0;
		
		while (isspace(line[(*i)]) && line[(*i)] != EOS)
		{
			(*i)++;
	    }
	    /* saving the first word in the line on string*/
		while (!isspace(line[(*i)]) && line[(*i)] != EOS && line[(*i)] != '\n') 
		{
			first_word[j++] = line[(*i)++];
		}
		
		first_word[j] = EOS;
}


/*gets a word from the line and updates i*/
void get_word_i(char * line, char * macro_name, int * i)/* i: the index of second word start*/
{
	int j = 0;
	while (isspace(line[(*i)]) && line[(*i)] != EOS)
			(*i)++;

	while (!isspace(line[(*i)]) && line[(*i)] != EOS)
			{
				macro_name[j] = line[(*i)];
				j++;		
				(*i)++;	
			}
	macro_name[j] = EOS;
}

/*finds macro in macro list*/
int find_macro(char * macro_name,mcr_list * list)
{
	macro * temp = list->head;
	int index = 0;
	while(temp!=NULL)
	{
		if(strcmp(temp->name,macro_name)==0)
		{
			return index;
		}

    temp = temp->next;
	index++;
	}

return INVALID;

}

/*returns given index macro content  */
char * get_content(int i, mcr_list * macro_list)
{
	macro * temp = macro_list->head;
	int j;

	for(j=0; j < i; j++)
	{
		temp = temp ->next;
	}

	return temp->content;
}

/*free each node in list*/
void free_macro_list(mcr_list * macro_list)
{
	macro* current =  macro_list -> head;
	while (current != NULL) 
	{
        /* point to the current node */
        macro *to_free = current;
        /* move to the next node*/
        current = current->next;
        /*free current node */
        free(to_free);
	}
          
}

/* validates macro name*/
int validate_macro_name(char * macro_name)
{
	/* LETTERS AND NUMBERS ONLY*/
	char c = macro_name[0];
    int i = 0;
    while(c != '\0')
    {
    	if(!isalpha(c) && !isdigit(c) && c != '_') /*macro name containes only this chars*/
    	{
       		return 0;
    	}
    	i++;
    	c = macro_name[i];
    }
  
   
	if(is_language_word(macro_name) || strlen(macro_name) > MAX_NAME_LEN || !isalpha(macro_name[0]))
		return 0;
		
	return 1;
}

/*prints error*/
void print_error_macro(int line, char * error, char * file_name)

{
	printf("in %s.as:%d: ERROR: %s\n", file_name, line, error);
}



