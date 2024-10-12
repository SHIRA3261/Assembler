#include "global.h"
#include "first_pass.h"
#include "second_pass.h" 


/*this isnt really second pass on file - but it completes the coding and ceates ext and ent files*/
int second_pass(label_list* symbol_table, int labels_count, coded_label * coded_labels, char * file_name, int IC,code_word *** code_array, char** entry_list, int * entry_line, int entry_count)
{
	
	FILE * ext_file;
	char temp[LINE_LENGTH];
	char * ext_name;
	int j;
	
	strcpy(temp,file_name);
	ext_name = strcat(temp,".ext");
	/*updates lable table before second pass*/
	update_label_tabel(symbol_table,IC);
	
	/*run on coded labels, get the adress from label table and code the matching word*/
	for(j = 0; j < labels_count; j++)
	{
		char *label_name  = coded_labels[j].label_name;
		int code_index = coded_labels[j].label_ic -IC_VAL;
		int line_number = coded_labels[j].line_number;
		
		int label_index = find_label(label_name, symbol_table); 
		
		int type;
		int address = 0;
		
		label *temp = symbol_table->head;
    	int index = 0;
		
		/*label used in command does not exist in lable table*/
		if(label_index < 0)
		{
			print_error(line_number, "the label used in command does not exist",file_name);
			return 0;
		}
		
		/*get the type and address from label_table*/
	
		
    	
    	while (index < label_index)
    	{
        temp = temp->next;
        index++;
   		}
   		
   		address = temp->address;
		type = temp->type;
		

		/*if type = extern, call func to print to ext, adress in code remains zero, update ARE*/
		if(type == EXTERN)
		{ 
			(*code_array)[code_index]->label_address.addr = 0;
			(*code_array)[code_index]->label_address.ARE = 1;/*ext ARE*/
			ext_file = fopen(ext_name,"a");/*if ext has opened allready , the line will be apended*/
			fprintf(ext_file, "%s %d\n",label_name,(code_index+IC_VAL));
			fclose(ext_file);
		}
		
		/*else - update it in code array*/
		else
		{
			(*code_array)[code_index]->label_address.addr = address; 
			(*code_array)[code_index]->label_address.ARE = 2;/*label ARE*/
			
		}
		
		
	}/*end of for*/
	
	/*print entry file*/
	entry_file(symbol_table, file_name, entry_list, entry_line, entry_count);
	
	
	return 1;
			
}



/*HELPER FUNCTIONS:*/

void print_ob(char * file_name, code_word ** code_array, int * data_list, int IC, int DC)
{
	
	FILE * ob_file = get_file(file_name,".ob","w+");
	if(!ob_file)
	{
		printf("file opening failed\n");
		exit(0);
	}
	fprintf(ob_file, "    %d %d    \n", IC-IC_VAL, DC);
	print_commands_binary(ob_file, code_array, IC);
	print_data(ob_file, file_name, data_list, IC);
	fclose(ob_file);
	
}

/*prints commands to ob*/
void print_commands_binary(FILE * file, code_word **code_array, int ic)
{

int i;
int j = 0;
unsigned int * print;
for(i = IC_VAL; i < ic ; i++)
	{
	/* print the first 4 bytes of the union */
	print = (unsigned int*)(code_array[j]);
	fprintf(file,"%04d	%05o\n", i, (*print));
	j++;
	}

}

/*prints data to ob*/
void print_data(FILE * ob_file, char * file_name, int *item, int IC)
{
	int i = 0;
	int address;
	while(item[i] != END_ARR)
	{
		unsigned int u_num = (unsigned int) item[i];
		address = IC + i ;
		fprintf(ob_file, "%04d\t%05o\n", address, u_num & 077777);	
		i++;
	}
	
	return;
}		

/*updates lable table before second pass*/
void update_label_tabel(label_list *list, int IC)
{
	
	label * temp = list->head;
	
	while(temp!=NULL)
	{
	   if(temp->type == STR || temp->type == DATA)
	   {
	   		temp->address += IC; 
	   
	   }
	   
       temp = temp->next;
	}
	
return; 
}



/*create entry file*/
int entry_file(label_list *list, char * file_name, char** entry_list, int * entry_line, int entry_count)
{
	
	int i = 0;
	int status = 1;
	char name[LINE_LENGTH];
	int label_index;
	char ent_name[LINE_LENGTH];
	FILE * ent_file;
	
	strcpy(ent_name, file_name);
	strcat(ent_name, ".ent");
	
		/*run on entry list and get adreses for ent file*/
	for(i =0; i < entry_count; i++)
	{
		 int line_number = entry_line[i];
		 int type;
		int address = 0;		
		
    	int index = 0;
    	label *temp = list->head;
    	
		 strcpy(name,entry_list[i]);
		 label_index = find_label(name, list);
		 if (label_index < 0)
		 	{
			print_error(line_number, "the entry label is nod defined in file",file_name);
			status = 0;
			continue;
			}
			
		
 		/*get the adress from table*/
    	while (index < label_index)
    	{
        temp = temp->next;
        index++;
   		}
   		
   		address = temp->address;
		type = temp->type;
		
		if(type == EXTERN)
		{
			print_error(line_number, "the entry label is allready defined as extern",file_name);
			status = 0;
			continue;
		}
			
		/*write to entry file*/
		ent_file = fopen(ent_name,"a");
		fprintf(ent_file,"%s %d\n", name, address);
		fclose(ent_file);	
		
		
	}

	return status;
}	
	



