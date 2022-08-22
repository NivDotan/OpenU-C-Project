#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "SecondStepFunctions.h"
#include "AssemblySecondStep.h"

/*Struct with the statement/instruction sentance name and thier functions for step 2*/
struct { 
	char *name;  
	void (*func)(char* FirstOperand, char* SecondOperand,int OperationNumber,int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode);  
	} instruction1[]={ 
	 		{"mov",FirstGroupSecondStep}, 
	 		{"cmp",FirstGroupSecondStep}, 
	 		{"add",FirstGroupSecondStep}, 
	 		{"sub",FirstGroupSecondStep}, 
			{"not",SecondGroupSecondStep},
			{"clr",SecondGroupSecondStep},
	 		{"lea",FirstGroupSecondStep}, 
	 		{"inc",SecondGroupSecondStep}, 
	 		{"dec",SecondGroupSecondStep}, 
	 		{"jmp",SecondGroupSecondStep}, 
	 		{"bne",SecondGroupSecondStep}, 
	 		{"get",SecondGroupSecondStep}, 
	 		{"prn",SecondGroupSecondStep}, 
	 		{"jsr",SecondGroupSecondStep}, 
	 		{"rts",ThirdGroupSecondStep}, 
	 		{"hlt",ThirdGroupSecondStep}, 
			{"not_valid",NULL} 
	}; 

struct { 
	char *name;  
	void (*func)(char* statement,char LastChar, int* NumOfLine,void *obj, int inSecondStep);  
	} statement1[]={ 
	 		{".data",dataStatement}, 
	 		{".string",stringStatement}, 
	 		{".struct",structStatement2}, 
			{"not_valid",NULL} 
	}; 

/*The second step in the assembly, we will review all the .am file and will call the functions from here*/
void Step2(char* filename, struct Symbol_Table_List *node,int lastRowInstruction,struct ExternLabels *ExternNode,int entryFlag, int MaxLines){
	int NumOfSentence,NumOfLine,i,HadComma,count,lastInstruction,ExternNodeFlag;
	char* tmp,*FirstOp,*SecondOp,*word, LastChar,*objCopy,*entCopy,*extCopy,*str,*str2;
	FILE *fp, *obj, *ent,*ext;
	
	
	HadComma = count = ExternNodeFlag = 0;
    NumOfLine = lastInstruction = 99;
	
	str = (char *)malloc(sizeof(char) * 81);
	if (str == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	objCopy = (char *)malloc(sizeof(char) * (strlen(filename) + 3));
	if (objCopy == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	entCopy = (char *)malloc(sizeof(char) * (strlen(filename) + 4));
	if (entCopy == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	extCopy = (char *)malloc(sizeof(char) * (strlen(filename) + 4));
	if (extCopy == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	strcpy(objCopy,filename);
	strcpy(entCopy,filename);
	strcpy(extCopy,filename);
	strcat(filename,".am");
	strcat(objCopy,".ob"); 
	strcat(entCopy,".ent");
	strcat(extCopy,".ext"); 
	
	
	fp = fopen(filename, "r");
	obj = fopen(objCopy,"w");
	if (entryFlag == 1)
		ent = fopen(entCopy,"w");
	if (ExternNode !=NULL){
		ext = fopen(extCopy,"w");
		ExternNodeFlag = 1;
	}
	else{
		ext = NULL;
	}
	
	
	if (fp == NULL)
    {
        fprintf(stderr,"Error: could not open file %s \n", filename);
        exit(1);
    }

	PrintToObjFileAddress((lastRowInstruction - 100),obj);
	PrintToObjFileCode((MaxLines- lastRowInstruction + 1),obj);
	
	while(fgets (str, 80, fp)!=NULL){ 
		compress_spaces(str);
		LastChar = str[strlen(str)-1];
		str2 = (char *)malloc(sizeof(char) * (strlen(str))+1);
		if (str2 == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		strcpy(str2,str);
		str[strlen(str)-1]='\0';

		if ((int)str[0] == 59){
			free(str2);
			continue;	/*Comment Sentance*/
			}
		
		else{							

			word = strtok(str, " ");
			
			if(word == NULL){
				free(str2);
				continue;	/*Empty Sentance*/
			}
			
			else{
				
			/* get the second word from the message, NULL must be
			* used to get tokens from the previous string now */

				if ((int)word[strlen(word)-1] == 58){

					if (checkLabel(word)){ 
						word = strtok(NULL, " ");
						
						if (CheckinstructionSentenceStep2(word)<16){
							NumOfSentence = CheckinstructionSentenceStep2(word);
							
							str2[strlen(str2)-1]='\0';
							tmp = (char *)malloc(sizeof(char) * (strlen(str2)+1));
							if (tmp == NULL){
								fprintf(stderr,"Problem with malloc\n");
								exit(1);
							}
							strcpy(tmp,str2);
							
							for(i=0;i<strlen(tmp);i++){
								if ((tmp[i]==',') && (HadComma == 1)){
									fprintf(stderr,"Error too many comma, line number %d \n",NumOfLine);
								}
								else if((HadComma==0) && (tmp[i]==' ')){
									HadComma = 0;
								}
								else if((tmp[i]==',') || (tmp[i]==' ')){
									tmp[i] = ' ';
									HadComma = 1;
								}
								
								else{
									HadComma = 0;
								}
							}
							
							FirstOp = strtok(tmp, " ");
							FirstOp = strtok(NULL, " ");
							
							if  (NumOfSentence < 14){
								if((NumOfSentence < 4) || (NumOfSentence == 6)){
									FirstOp = strtok(NULL, " ");
									SecondOp = strtok(NULL, " ");
									
									if (strtok(NULL, " ") != NULL){
										fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
									}
									(*(instruction1[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
								}
								else{
									FirstOp = strtok(NULL, " ");
									if (strtok(NULL, " ") != NULL){
										fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
									}
									(*(instruction1[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
								}
							}
							else{
								if (strtok(NULL, " ") != NULL){
									fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
								}
								(*(instruction1[NumOfSentence].func))(NULL,NULL,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
							}
							
							free(tmp);
							free(str2);
						}
						else{
							free(str2);
						}
						
					}
				}
				else if(strcmp(word, ".entry") == 0){
					
					word = strtok(NULL, " ");
					entryStatement(word,LastChar,node , ent,&NumOfLine);
					
					free(str2);
				}
				
				else if(strcmp(word, ".extern") == 0){
					
					free(str2);
					continue;
				}

				else if((int)word[0] == 46){
					continue;
				}

				else if (CheckinstructionSentence(word)<16){
					NumOfSentence = CheckinstructionSentence(word);
					if (NumOfSentence == 15 || NumOfSentence == 14){
						NumOfLine++;
						NumOfSentence = CheckinstructionSentence(word);
						(*(instruction1[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
						free(str2);
						
					}
					else {
						
						str2[strlen(str2)-1]='\0';
						tmp = (char *)malloc(sizeof(char) * (strlen(str2)+1));
						if (tmp == NULL){
							fprintf(stderr,"Problem with malloc\n");
							exit(1);
						}
						strcpy(tmp,str2);
						tmp[strcspn(tmp, "\n")] = 0;
						compress_spaces(tmp);
						for(i=0;i<strlen(tmp);i++){
							if ((tmp[i]==',') && (HadComma == 1)){
								fprintf(stderr,"Error too many comma, line number %d \n",NumOfLine);
								}
							else if((HadComma==0) && (tmp[i]==' ')){
									HadComma = 0;
								}
							else if((tmp[i]==',') || (tmp[i]==' ')){
								tmp[i] = ' ';
								HadComma = 1;
							}
							else{
								HadComma = 0;
							}
						}
						
						FirstOp = strtok(tmp, " ");
						
						
						if  (NumOfSentence < 14){
							if((NumOfSentence < 4) || (NumOfSentence == 6)){
								
								FirstOp = strtok(NULL, " ");
								SecondOp = strtok(NULL, " ");
								
								if (strtok(NULL, " ") != NULL){
									fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
								}
								
								(*(instruction1[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
								
								
							}
							else{
								FirstOp = strtok(NULL, " ");
								if (strtok(NULL, " ") != NULL){
									fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
								}
								(*(instruction1[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
							}
						}
						else{
							if (strtok(NULL, " ") != NULL){
								fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
							}
						
							(*(instruction1[NumOfSentence].func))(NULL,NULL,NumOfSentence,&NumOfLine,node,obj,ExternNodeFlag,ext,ExternNode);
							
						}
						free(tmp);
						free(str2);
						
					}

					

					
					
				}
				
			}
		}
	}
	NumOfLine--;
	fclose(fp);
	fp = fopen(filename, "r");
	
	
	while(fgets (str, 80, fp)!=NULL){ 
		
		LastChar = str[strlen(str)-1];
		str[strlen(str)-1]='\0';

		
		if ((int)str[0] == 59){
			continue;	/*Comment Sentance*/
		}
		
		else{							
			compress_spaces(str);

			word = strtok(str, " ");
			if(word == NULL){
				continue;	/*Empty Sentance*/
			}
			
			else{

			/* get the second word from the message, NULL must be
			* used to get tokens from the previous string now */

				if ((int)word[strlen(word)-1] == 58){

					if (checkLabel(word)){ 
						
						word = strtok(NULL, " ");
						if((int)word[0] == 46){
							
							NumOfSentence = CheckGuidanceSentence(word);
							if(NumOfSentence<5){
								word = strtok(NULL, " ");
								(*(statement1[NumOfSentence].func))(word,LastChar,&NumOfLine,obj,1);
	
							}
						}
					}
				}
				else if((int)word[0] == 46){
					NumOfSentence = CheckGuidanceSentence(word);
					word = strtok(NULL, " ");
					if(NumOfSentence<3){
						(*(statement1[NumOfSentence].func))(word,LastChar,&NumOfLine,obj,1);

					}
				}
				
			}
		}
	}
	
	fprintf(stderr,"Finish assembling %s,\n",filename);
	fclose(fp);
	fclose(obj);
	if (entryFlag == 1)
		fclose(ent);
	if (ExternNode !=NULL)
		fclose(ext);
	free(objCopy);
	free(entCopy);
	free(extCopy);
	freeListSymbol_Table_List(node);/*Free linked list*/
	freeListExternLabels(ExternNode);/*Free linked list*/

	return;
}

