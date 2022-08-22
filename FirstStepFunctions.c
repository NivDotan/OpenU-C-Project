#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "AssemblyFirstStep.h"
#include "FirstStepFunctions.h"
#include "CommonFunctions.h"
 

/*Append to a linked list labels names*/
void append1(struct Symbol_Table_List** head_ref, char* new_name, int value, char* type)
{
    struct Symbol_Table_List* new_node;
    struct Symbol_Table_List *last;  
	new_node = (struct Symbol_Table_List*) malloc(sizeof(struct Symbol_Table_List));
	last = *head_ref;  
    new_name[strlen(new_name)-1] = '\0';
    new_node->name = (char *)malloc(strlen(new_name) + 1);
	if(new_node == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
    strcpy(new_node->name, new_name);
    new_node->value = value;
	
	new_node->type = malloc(strlen(type) + 1);
	
    strcpy(new_node->type, type);
	
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

/*Append to a linked list extern names*/
void appendExternNode(struct ExternLabels** head_ref, char* new_name)
{
    struct ExternLabels* new_node;
    struct ExternLabels *last;  
	new_node = (struct ExternLabels*) malloc(sizeof(struct ExternLabels));
	last = *head_ref; 
	if ( new_name[strlen(new_name)-1] == '\n' || new_name[strlen(new_name)-1] == '\r'){
		new_name[strlen(new_name)-1] = '\0';
	}
    
    new_node->name = (char *)malloc(strlen(new_name) + 1);
	if(new_node == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
    strcpy(new_node->name, new_name);
	
	
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

/*Check how many rows does we need to add*/
int OperandBinaryCode(int DecFirstOperand, char* FirstOperand,int OpNumber,int* NumOfLine){
	int index,tmp,SwitchBinary,i;
	SwitchBinary = 0;
	i = 1;
	
	if (DecFirstOperand == 0){ /*Number Operation*/
		memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
		if (atoi(FirstOperand) < 0){
			memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
			tmp = convert(atoi(FirstOperand)-1);
			
			if (tmp == 0){
				SwitchBinary = 1111111111;
			}
			else{
				while (tmp){
					if((tmp%10) == 1){
						SwitchBinary = SwitchBinary +0;
						if((tmp/10)==0){
							while(i!=10000000){
								i = i*10;
								SwitchBinary = SwitchBinary + i;
							}
						}
					}
					else{
						SwitchBinary = SwitchBinary + i;
						if((tmp/10)==0){
							while(i!=10000000){
								i = i*10;
								SwitchBinary = SwitchBinary + i;
							}
						}
					}
					i = i*10;
					tmp /= 10;
				}
			}
			return 1;
		}
		else{
			return 1;
		}
		
	}
	else if (DecFirstOperand == 1){/*Label Operation*/
		return 1;
	}
	else if (DecFirstOperand == 2){/*Label and a place in the label (Example: S1.2) Operation*/
		index = (int)((strchr(FirstOperand,'.'))-FirstOperand);
		memmove(FirstOperand,FirstOperand+(index+1),strlen(FirstOperand));
		if (atoi(FirstOperand) == 0){
			Error = 1;
			fprintf(stderr,"Error in struct operand, line %d \n",*NumOfLine );
			return 0;
		}
		return 2;
		
	}
	else if (DecFirstOperand == 3){/*Resister Operation*/
		memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
		if (atoi(FirstOperand)>7){
			fprintf(stderr,"Error in register name %s, line %d \n",FirstOperand,*NumOfLine );
			return 0;
		}
		else{
			}
		return 1;
		
	}
	return 0;
}

/*Checks the first group of the sentances*/
void FirstGroup(char* FirstOperand, char* SecondOperand,int OperationNumber, int* NumOfLine){
	int DecFirstOperand, DecSecondOperand,NumOfRowAddress;
	
	DecFirstOperand = checkOperand(FirstOperand);/*Check if the operand is legal*/
	DecSecondOperand = checkOperand(SecondOperand);

	/*Handling errors*/
	if (DecFirstOperand == 5){
		fprintf(stderr,"There is a error in the register, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	else if (DecSecondOperand == 5){
		fprintf(stderr,"There is a error in register, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	else if (DecFirstOperand == 6){
		fprintf(stderr,"There is a error in the number operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	else if (DecSecondOperand == 6){
		fprintf(stderr,"There is a error in number operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}

	if ((OperationNumber == 6) && ((DecFirstOperand == 0 ) || (DecFirstOperand == 3 ))){
		fprintf(stderr,"There is a error in the type of the operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	else if ((OperationNumber != 1) && (DecSecondOperand == 0 )){
		fprintf(stderr,"There is a error in the type of the operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	

	*NumOfLine = *NumOfLine + 1;
	
	if ((DecFirstOperand == 3)&&(DecSecondOperand == 3)){
		OperandBinaryCodeRefisters(FirstOperand,SecondOperand);
		*NumOfLine = *NumOfLine +1;
		
	}
	else{
		NumOfRowAddress = OperandBinaryCode(DecFirstOperand,FirstOperand,1,NumOfLine);
		*NumOfLine = *NumOfLine + NumOfRowAddress;
		NumOfRowAddress = OperandBinaryCode(DecSecondOperand,SecondOperand,2,NumOfLine);
		*NumOfLine = *NumOfLine + NumOfRowAddress;
	}
	
	
}

/*Checks the second group of the sentances*/
void SecondGroup(char* FirstOperand, char* null,int OperationNumber, int* NumOfLine){
	int DecFirstOperand,NumOfRowAddress;

	DecFirstOperand = checkOperand(FirstOperand);
	if (DecFirstOperand == 5){
		fprintf(stderr,"There is a error in the register, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}
	else if (DecFirstOperand == 6){
		fprintf(stderr,"There is a error in the number operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}

	if ((OperationNumber != 12) && (DecFirstOperand == 0 )){
		fprintf(stderr,"There is a error in the type of the operand, line number %d \n",*NumOfLine);
		Error = 1;
		return;
	}


	*NumOfLine = *NumOfLine + 1;
	NumOfRowAddress = OperandBinaryCode(DecFirstOperand,FirstOperand,2,NumOfLine);
	*NumOfLine = *NumOfLine + NumOfRowAddress;
	
	
}

/*Checks the Third group of the sentances, adding a row number*/
void ThirdGroup(char* null, char* null2,int OperationNumber, int* NumOfLine){
	*NumOfLine = *NumOfLine + 1;
}

/*Getting two operande of registers and convert them to binary*/
void OperandBinaryCodeRefisters(char* FirstOperand, char* SecondOperand){
	int tmp,bin;
	memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
	memmove(SecondOperand,SecondOperand+1,strlen(SecondOperand));
	
	tmp = atoi(SecondOperand);
	bin = (int)convert(atoi(FirstOperand));
	
	if(tmp == 0)
		bin = bin * 10000;
	else if(tmp == 1)
		bin = bin * 10000 + 1;
	else if(tmp == 2)
		bin = bin * 10000 + 10;
	else if(tmp == 3)
		bin = bin * 10000 + 11;
	else if(tmp == 4)
		bin = bin * 10000 + 100;
	else if(tmp == 5)
		bin = bin * 10000 + 101;
	else if(tmp == 6)
		bin = bin * 10000 + 110;
	else if(tmp == 7)
		bin = bin * 10000 + 111;
	bin = bin *100;
}
