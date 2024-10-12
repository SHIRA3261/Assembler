
#include "global.h"
#include "macro.h"
#include "first_pass_command.h"
#include "first_pass.h"
#include "second_pass.h"

/*free all memmory macro*/
#define FREE_MEMORY \
	free_macro_list(macro_list);\
	free_code_array(code_array,IC-IC_VAL);\
	free_symbol_table(symbol_table);\
	free_entry_list(entry_list,entry_count);\
	free(entry_line);\
	free(data_list);\
	free(coded_labels);\

/*free each node in symbol list*/
void free_symbol_table(label_list * symbol_table);

/*runs on code array and free each code word*/
void free_code_array(code_word **code_array, int size);

/*runs on entry list and frees each string*/
void free_entry_list(char ** entry_list, int entry_count);





int main(int argc, char *argv[])
{
	int i;
	if (argc < 2) 
	{
        /* no input files*/
        fprintf(stderr, "Error: No input files\n");
        exit(1);
	}


    /* run on each argument file */
    for (i = 1; i < argc; i++) 
    {
	
	FILE *as_file;
	FILE *am_file;
	/*initialize data structures*/
	mcr_list * macro_list = create_macro_list();
	int IC = IC_VAL;
    int DC = 0;
	code_word ** code_array = (code_word **)malloc(sizeof(code_word *));
	label_list *symbol_table = create_label_list();
	int * data_list = (int *)malloc(sizeof(int));
	
	char ** entry_list = (char **)malloc(RAM_SIZE * sizeof(char *)) ;
	int entry_count = 0;
	int * entry_line = (int *)malloc(RAM_SIZE * sizeof(int));
	coded_label* coded_labels = (coded_label *)malloc(sizeof(coded_label));
	int labels_count = 0;
	
	data_list[0] = END_ARR;
	
	
	

     /* MACRO PROCESS */
	as_file = get_file(argv[i],".as","r+");
	
	 if(!as_file)
    {
        printf("%s: file opening failed\n", argv[i]);
        FREE_MEMORY
       continue;
    }

	if(macro_func(as_file,argv[i],macro_list) == 1)
	{
	remove(strcat(argv[i],".am")); 
	FREE_MEMORY
	continue;
	}
	
	
	am_file = get_file(argv[i],".am","r");
	
	 if(!am_file)
    {
        printf("%s: file opening failed\n", argv[i]);
        FREE_MEMORY
       continue;
    }
	
	/*FIRST PASS: SAVING LABELS, CODING DATA AND COMMANDS, SAVE DATA FOR SECOND PASS*/
	if(!first_pass(am_file, argv[i], macro_list, &code_array, &IC, &DC, symbol_table, &data_list, &entry_count, &entry_list, &entry_line, &coded_labels, &labels_count))
	{
	
	FREE_MEMORY
	fclose(am_file);	
	continue;
	}
	

	
	fclose(am_file);
	
	if( (IC + DC) > RAM_SIZE)
    {
        printf("in %s:ERROR:exception ram memory\n",argv[i]);  
        FREE_MEMORY
        continue;
    }


	
	/*SECOND PASS: COMPLETE THE CODING AND CREATE ENTRY AND EXTERN FILES*/
	if(!second_pass(symbol_table, labels_count, coded_labels, argv[i], IC, &code_array,entry_list, entry_line, entry_count))
	{
		FREE_MEMORY
		continue;
	
	}
	

	/*print ob file:*/
	print_ob(argv[i], code_array, data_list, IC, DC);
	
	/*print_label_tabel(symbol_table, argv[i]);*/
	
	/*free all dynamic memmory allocated*/
	FREE_MEMORY


    }/*end of files-loop*/

	return 0;
}/*end main*/



/*free memory functions*/

/*free each node in symbol list*/
void free_symbol_table(label_list * symbol_table)
{
	label* current =  symbol_table -> head;
	while (current != NULL) 
	{
        /* point to the current node */
        label* to_free = current;
        /* move to the next node*/
        current = current->next;
        /*free current node */
        free(to_free);
	}

}


/*runs on code array and free each code word*/
void free_code_array(code_word **code_array, int size)
{

int i;
for(i = 0; i < size; i++)
{
	free(code_array[i]);
}

free(code_array);

}

/*runs on entry list and frees each string*/
void free_entry_list(char ** entry_list, int entry_count)
{

int i;
for(i = 0; i < entry_count; i++)
{
	free(entry_list[i]);
}

free(entry_list);


}





