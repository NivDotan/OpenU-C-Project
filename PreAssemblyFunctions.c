#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "PreAssemblyFunctions.h"

/*Append to a linked list the macro name or it context*/
void appendMacroParser(struct node** head_ref, char* new_data,int IsMacroName)
{
	
    struct node* new_node;
    struct node *last;  
	new_node = (struct node*) malloc(sizeof(struct node));
	last = *head_ref; 
	if (IsMacroName){
		new_node->data = (char *)malloc((strlen(new_data) + 1)-6);
		if (new_data == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		strncpy(new_node->data, &new_data[6],3);
	}
	else{
		new_node->data = (char *)malloc(strlen(new_data) + 1);
		if (new_data  == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		strcpy(new_node->data, new_data);
	}
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
    	
       *head_ref = new_node;
       return;
    }
 
    while (last->next != NULL)
        last = last->next;
    
    last->next = new_node;
    return;
}

/*Find if the macro already was defined*/
int MacroFinder(char* str,struct node *node){
	
	while (node != NULL){
		
		if(strncmp(str,node->data,(strlen(str)-1)) == 0){
			return 1;
		}
		else
			node = node->next;
	}
	return 0;
}

/*FInd the macro name and change it to the macro context*/
void MacroChanger(char* str,struct node *node,void *fr){
	int MacroFlag;
	MacroFlag = 0;
	while (node != NULL){
		if(strcmp((node->data), "endmacro\n")==0 && MacroFlag)
			break;
		else if (strncmp(str,&(node->data)[6],(strlen(str)-1)) == 0)
			MacroFlag = 1;
		else if(MacroFlag){
			fprintf(fr,"%s", node->data);
		}
		node = node->next;
	}
}

/*Check if the macro name is legal*/
int CheckNameMacro(char *sentance){
	int space,word,i,FirstWord,SecondWord,index;
	FirstWord = space = word = SecondWord = index = 0;

	for (i = 0; i<strlen(sentance); i++){
		if (FirstWord == 0){
			if (isspace(sentance[i]) == 0){
				FirstWord = 1;
			}
			else if (isspace(sentance[i]) == 1){
				space = 1;
			}
			index++;
		}
		else if (FirstWord == 1 && SecondWord == 1 && space ==1 && (isspace(sentance[i]) == 0)){
			fprintf(stderr,"There is too many word in the macro name");
			return 0;
		}
		else if (FirstWord == 1 && (isspace(sentance[i]) == 0) && space == 1){
			index++;
			SecondWord = 1;
			space = 0;
		}
		else if (FirstWord == 1 && (isspace(sentance[i]) == 1)){
			space = 1;
			index++;
			
		}
		else if (SecondWord == 1 && (isspace(sentance[i]) == 1)){
			space =1;
			
		}
		else{
			
		}
		
	}
	if (checkIfReserveWordInMacro(sentance, index))
		return 1;
	else 
		return 0;
}

/*check if the macro name is a reserve word*/
int checkIfReserveWordInMacro(char label[],int index){
	int i;
	char* instructionMacro[]={ 
	 		"mov", 
	 		"cmp", 
	 		"add", 
	 		"sub", 
			"not",
			"clr",
	 		"lea", 
	 		"inc", 
	 		"dec", 
	 		"jmp", 
	 		"bne", 
	 		"get", 
	 		"prn", 
	 		"jsr", 
	 		"rts", 
	 		"hlt" 
	}; 

	char* statementMacro[]={ 
	 		".data", 
	 		".string", 
	 		".struct", 
	 		".entry", 
	 		".extern"
	}; 

	if (label[strlen(label)-1] == '\n')
		label[strlen(label)-1] = '\0';
	if (label[index]=='r'){
		if (((label[index+1]- '0')>= 0) && ((label[index+1]- '0')<= 7)){
			return 0;
		}
	}
	for(i= 0; i<5;i++){
		if ((strstr(label, (statementMacro[i]))!=NULL))
			return 0;
	}
	for(i= 0; i<16;i++){
		if (strstr(label, (instructionMacro[i]))!=NULL)
			return 0;
	}
	return 1;

}

/*Free linked list*/
void freeListNode(struct node *node) {
	struct node * tmp;
	tmp = NULL;
	while(node) {
		tmp = node->next;
		free(node->data);
		free(node);
		node = tmp;
	}
}

