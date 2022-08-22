#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "AssemblySecondStep.h"
#include "FirstStepFunctions.h"
#include "AssemblyFirstStep.h"
#include "CommonFunctions.h"

/*Extern int that act as a flag for if we get an error in any functions*/
int Error = 0;

/*Struct with the statement/instruction sentance name and thier functions*/
struct { 
	char *name;  
	void (*func)(char* statement,char LastChar, int* NumOfLine,void *obj, int inSecondStep);  
	} statement[]={ 
	 		{".data",dataStatement}, 
	 		{".string",stringStatement}, 
	 		{".struct",structStatement2}, 
			{"not_valid",NULL} 
	}; 
	
struct { 
	char *name;  
	void (*func)(char* FirstOperand, char* SecondOperand,int OperationNumber,int* NumOfLine);  
	} instruction[]={ 
	 		{"mov",FirstGroup}, 
	 		{"cmp",FirstGroup}, 
	 		{"add",FirstGroup}, 
	 		{"sub",FirstGroup}, 
			{"not",SecondGroup},
			{"clr",SecondGroup},
	 		{"lea",FirstGroup}, 
	 		{"inc",SecondGroup}, 
	 		{"dec",SecondGroup}, 
	 		{"jmp",SecondGroup}, 
	 		{"bne",SecondGroup}, 
	 		{"get",SecondGroup}, 
	 		{"prn",SecondGroup}, 
	 		{"jsr",SecondGroup}, 
	 		{"rts",ThirdGroup}, 
	 		{"hlt",ThirdGroup}, 
			{"not_valid",NULL} 
	}; 

/*The first step in the assembly, we will review all the .am file and will call the functions from here*/
void Step1(char* filename){
	int ErrorInSentance,NumOfSentence,NumOfLine,i,HadComma,count,lastInstruction,lastRowInstruction,entryFlag;
	char* tmp,*FirstOp,*SecondOp,*word, *LabelName,LastChar,*fileTmp, *str,*str2;
	
	struct Symbol_Table_List *SymbolTable;
	struct ExternLabels *ExternLabelNode;
	
	FILE *fp;
	
	fileTmp = (char *)malloc(sizeof(char) * (strlen(filename) + 3));
	if(fileTmp == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	strcpy(fileTmp,filename);
	str = (char *)malloc(sizeof(char) * 83);
	if(str == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	HadComma = count = entryFlag = ErrorInSentance = 0;
	
    NumOfLine = lastInstruction = 99;
    
	strcat(filename,".am");
	fp = fopen(filename, "r");

	if (fp == NULL)
    {
        
		fprintf(stderr,"Error: could not open file %s \n", filename);
        exit(1);
    }	
	
	
	SymbolTable = NULL;
	ExternLabelNode = NULL;
	while(fgets (str, 82, fp)!=NULL){ 
		
		
		LastChar = str[strlen(str)-1];
		
		
		if (strlen(str) > 81){
			Error = 1;
			fprintf(stderr,"Error: Sentance too long (will print the rest of the line in the next line as error) \n");
			continue;
		}
		else{
			str2 = (char *)malloc(sizeof(char) * ((strlen(str)+1)));
			if(str2 == NULL){
				fprintf(stderr,"Problem with malloc\n");
				exit(1);
			}

			
			
			strcpy(str2,str);
			
			
			if ((int)str[0] == 59){
				free(str2);
				continue;	/*Comment Sentance*/
			}
			
			else{							
				compress_spaces(str);

				word = strtok(str, " ");
				
				if(word == NULL || (int)str[0] == 13 || (int)str[0] == 10){
					
					free(str2);
					continue;	/*Empty Sentance*/
				}
				
				else{
					
				/* get the second word from the message, NULL must be
				* used to get tokens from the previous string now */

					/*Label Sentance*/
					if ((int)word[strlen(word)-1] == 58 || ((int)str[strlen(word)-1] == 13 &&(int)word[strlen(word)-2] == 58) || ((int)str[strlen(word)-1] == 10&&(int)word[strlen(word)-2] == 58)){
						
						if (checkLabel(word) && checkIfAlreadyExist(word,SymbolTable,ExternLabelNode)){ 
							
							word = strtok(NULL, " ");
							
							if(word == NULL){
								free(str2);
								fprintf(stderr,"Error: Empty label,\n");
								Error = 1;
								continue;	/*Empty Label*/
							}
							
							else if((int)word[0] == 46){
								NumOfSentence = CheckGuidanceSentence(word);
								
								if(NumOfSentence>2){
									free(str2);
									Error = 1;
									fprintf(stderr,"Label in the start on Entry or Extern, Line Number: %d \n",NumOfLine);
									continue;
									
								}
								else if (NumOfSentence<3){
									free(str2);
									continue;
								}
								
							}
							else if (CheckinstructionSentence(word)<16){
								NumOfSentence = CheckinstructionSentence(word);
								
								LabelName = strtok(str, " ");
								
								append1(&SymbolTable, LabelName,NumOfLine+1,"00");
								str2[strlen(str2)-1]='\0';
								tmp = (char *)malloc(sizeof(char) * (strlen(str2)+1));
								if(tmp == NULL){
									fprintf(stderr,"Problem with malloc\n");
									exit(1);
								}
								strcpy(tmp,str2);

								for(i=0;i<strlen(tmp);i++){
									if ((tmp[i]==',') && (HadComma == 1)){
										Error = 1;
										fprintf(stderr,"Error too many comma, line number %d \n",NumOfLine);
										ErrorInSentance = 1;
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
								
								if  ((NumOfSentence < 14) && (ErrorInSentance == 0)){
									if((NumOfSentence < 4) || (NumOfSentence == 6)){
										FirstOp = strtok(NULL, " ");
										SecondOp = strtok(NULL, " ");
										
										if (strtok(NULL, " ") != NULL){
											Error = 1;
											fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
										}
										else
											(*(instruction[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine);
									}
									else{
										FirstOp = strtok(NULL, " ");
										
										if (strtok(NULL, " ") != NULL){
											Error = 1;
											
											fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
										}
										else
											(*(instruction[NumOfSentence].func))(FirstOp,FirstOp,NumOfSentence,&NumOfLine);
									}
								}
								else{
									
									if (strtok(NULL, " ") != NULL){
										Error = 1;
										fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
									}
									else
										(*(instruction[NumOfSentence].func))(NULL,NULL,NumOfSentence,&NumOfLine);
								}
								lastRowInstruction = NumOfLine;
								
								free(tmp);
								free(str2);
							}
							
							
						}
						else{
							free(str2);
							fprintf(stderr,"Error in line number %d \n",NumOfLine);
							Error = 1;
						}
					}
					/*entry Sentance*/
					else if(strcmp(word, ".entry") == 0){
						entryFlag = 1;
						free(str2);
					}
					/*extern Sentance*/
					else if(strcmp(word, ".extern") == 0){
						
						word = strtok(NULL, " ");
						
						if(externStatement(word,LastChar,ExternLabelNode,&NumOfLine,SymbolTable))
							appendExternNode(&ExternLabelNode, word);

						free(str2);
						
					}
					/*Statement sentance*/
					else if((int)word[0] == 46){
						NumOfSentence = CheckGuidanceSentence(word);
						
						if (NumOfSentence ==6){
							fprintf(stderr,"Empty statement, Line Number: %d \n",NumOfLine);
							Error = 1;
						}
						else if(NumOfSentence ==5){
							fprintf(stderr,"Undefined statement name, Line Number: %d \n",NumOfLine);
							Error = 1;
						}
						else{
							if((word = strtok(NULL, " ")) != NULL){
								
								(*(statement[NumOfSentence].func))(word,LastChar,&NumOfLine,NULL,0);
							}
							else{
								fprintf(stderr,"Empty statement, Line Number: %d \n",NumOfLine);
								Error = 1;
							}

						}
						free(str2);
						
					}
					/*instruction sentance*/
					else if (CheckinstructionSentence(word)<16){
						NumOfSentence = CheckinstructionSentence(word);
						
						
						tmp = (char *)malloc(sizeof(char) * (strlen(str2)+1));
						if(tmp == NULL){
								fprintf(stderr,"Problem with malloc\n");
								exit(1);
						}
						
						strcpy(tmp,str2);
						
						compress_spaces(tmp);
						
						for(i=0;i<strlen(tmp);i++){
							if ((tmp[i]==',') && (HadComma == 1)){
								Error = 1;
								fprintf(stderr,"Error too many comma, line number %d \n",NumOfLine);
								ErrorInSentance = 1;
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
						
						
						if ((NumOfSentence < 14) && (ErrorInSentance == 0)){
							if((NumOfSentence < 4) || (NumOfSentence == 6)){
								FirstOp = strtok(NULL, " ");
								SecondOp = strtok(NULL, " ");
								if (strtok(NULL, " ") != NULL){
									Error = 1;
									fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
								}
								else
									(*(instruction[NumOfSentence].func))(FirstOp,SecondOp,NumOfSentence,&NumOfLine);
							}
							else{
								FirstOp = strtok(NULL, " ");
								if (strtok(NULL, " ") != NULL){
									Error = 1;
									fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
								}
								
								else
									(*(instruction[NumOfSentence].func))(FirstOp,FirstOp,NumOfSentence,&NumOfLine);
							}
						}
						else{
							if (strtok(NULL, " ") != NULL){
								Error = 1;
								fprintf(stderr,"Error: There is Too much word in the sentance, line number %d \n",NumOfLine);
							}
							else
								(*(instruction[NumOfSentence].func))(NULL,NULL,NumOfSentence,&NumOfLine);
						}
						lastRowInstruction = NumOfLine;
							
						free(tmp);
						free(str2);
						
					}
					else{
						
						Error = 1;
						fprintf(stderr,"Error Undefined Operation %s, line number: %d \n",word, NumOfLine);
						free(str2);
					}
				}
			}
		}
		
		
		
	}
	
	
	fclose(fp);
	fp = fopen(filename, "r");
	NumOfLine--;
	
	if(Error == 0){
		while(fgets (str, 80, fp)!=NULL){ 
			
			LastChar = str[strlen(str)-1];
			str[strlen(str)-1]='\0';
			
			if (strlen(str) > 80){
				continue;
			}

			
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
					/*Label sentance*/
					if ((int)word[strlen(word)-1] == 58){

						if (checkLabel(word)){ 
							
							word = strtok(NULL, " ");
							
							if((int)word[0] == 46){
								
								NumOfSentence = CheckGuidanceSentence(word);
								if(NumOfSentence<3){
									word = strtok(NULL, " ");
									
									append1(&SymbolTable, LabelName,NumOfLine+1,"10");
									(*(statement[NumOfSentence].func))(word,LastChar,&NumOfLine,NULL,0);
									LabelName = strtok(str, " ");
								
									
										
								}
								else if (NumOfSentence == 3){
									Error = 1;
									
									fprintf(stderr,"Error: There is a label and then entry, line number %d \n",NumOfLine);
								}
								else if (NumOfSentence == 4){
									Error = 1;
									
									fprintf(stderr,"Error: There is a label and then extern, line number %d \n",NumOfLine);
								}
							}
						}
					}
					

					else if(strcmp(word, ".entry") == 0){
						CheckEntryInExternList(word,LastChar,ExternLabelNode,&NumOfLine);
					}
					
					
					
				}
			}
			
		}
	}

	
	
	
	fclose(fp);
	if (!Error){
		Step2(fileTmp, SymbolTable,lastRowInstruction,ExternLabelNode,entryFlag,NumOfLine);
		free(fileTmp);
	}
	
	
	return;
}
