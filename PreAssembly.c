#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "PreAssemblyFunctions.h"
#include "AssemblyFirstStep.h"
/*Pre Assembly step - getting the file name and handling the copy to a new file .am*/

void PreAssemblyFunc(const char* filename){
	int GetContextMacro,error,i,tmpChar;
	char* Str,*fileCopy,*tmp,*tmpBuff,*fileTmp,*MainFileName;
	FILE *fp, *fr;
	struct node *head;
	struct node *MacroNames;

	MainFileName = (char *)malloc(sizeof(char) * (strlen(filename) + 1));
	if(MainFileName == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	strcpy(MainFileName,filename);

	fileCopy = (char *)malloc(sizeof(char) * (strlen(filename) + 3));
	if(fileCopy == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	strcpy(fileCopy,filename);

	fileTmp = (char *)malloc(sizeof(char) * (strlen(filename) + 3));
	if(fileTmp == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	strcpy(fileTmp,filename);
	
    error  = 0;

	tmpBuff = (char*)realloc(fileTmp, strlen(fileTmp)+2);
	if (tmpBuff == NULL){
		fprintf ( stderr, "realloc problem\n");
	}
	else{
		Str = tmpBuff;
	}
    
   
	strcat(fileTmp,".as");
	strcat(fileCopy,".am");
	fp = fopen(fileTmp, "r");
	if (fp == NULL)
    {
        fprintf(stderr,"Error: could not open file %s \n", fileTmp);
		return;
    }
    fr = fopen(fileCopy,"w");




	
    
	head = NULL;
	MacroNames = NULL;
	GetContextMacro = 0;
	i = 0;
	fprintf(fr,";file %s \n", fileTmp);
	
	while(1){	
		tmpChar = (fgetc(fp));/*Getting each char with no limit of the line length*/
		
		if (tmpChar == EOF){
			tmpChar = '\n';
			
			strcat(Str, (char*)&tmpChar);
			tmp = Str;
			
			if (strcmp(tmp, "endmacro\n")==0 && (GetContextMacro == 1)){/*If we got a endmacro word so we will append it to the linked list*/
				appendMacroParser(&head, tmp,0);
				GetContextMacro = 0;
			}

			else if (strncmp(tmp,"macro",5) == 0){/*If we got a macro word so we will start checking the macro*/
				
				if(CheckNameMacro(Str) && (MacroFinder(tmp, MacroNames) == 0)){
					
					appendMacroParser(&head, tmp,0);
					appendMacroParser(&MacroNames, tmp,1);
					GetContextMacro = 1;
				}
				else {
					error= 1;
					fprintf(stderr,"Error: Error in macro\n");
					break;
				}
					
			}
			
			else if (GetContextMacro){/*Appending the macro context*/
				appendMacroParser(&head, tmp,0);
			}
			else if (MacroFinder(Str, MacroNames)){
				MacroChanger(Str, head, fr);
				}
			
			else{ /*printing the rest of file into the .am file*/
				
				fprintf(fr,"%s", Str);
			}
			
			break;
		}

		else if (tmpChar == '\n') {/*Same comment from if char == EOF*/
				
				strcat(Str, (char*)&tmpChar);
				tmp = Str;
				if (strcmp(tmp, "endmacro\n")==0 && (GetContextMacro == 1)){
					
					appendMacroParser(&head, tmp,0);
					GetContextMacro = 0;
				
				}

				else if (strncmp(tmp,"macro",5) == 0){	
					if(CheckNameMacro(tmp) && (MacroFinder(tmp, MacroNames) == 0)){
						
						appendMacroParser(&head, tmp,0);
						appendMacroParser(&MacroNames, tmp,1);
						GetContextMacro = 1;
					
					}
					else {
						error= 1;
						fprintf(stderr,"Error: Error in macro\n");
					
						break;
					}
					
				}
				
				else if (GetContextMacro){
					appendMacroParser(&head, tmp,0);
				}
				else if (MacroFinder(Str, MacroNames)){
					
					MacroChanger(tmp, head, fr);
					
					}
				
				else{ 
					fprintf(fr,"%s", tmp);
					
				}



				free(Str);
				Str = (char*)malloc(1);
				if(Str == NULL){
					fprintf(stderr,"Problem with malloc\n");
					exit(1);
				}
				i = 0;
		}
		else{  
			
			if(i == 0){
				Str = (char*)malloc(2);
				memset(Str, 0, 2);/*Setting the char* to 0 because i use strcat*/
				if(Str == NULL){
					fprintf(stderr,"Problem with malloc\n");
					exit(1);
				}
			}
			if(i ==0  && ((int)tmpChar== 32 || (int)tmpChar== 9) ){
				tmpChar = '\0';
				strcat(Str, (char*)&tmpChar);
				continue;
			}
			else {/*Handle tab char as space*/
				if ((int)tmpChar == 9){
					strcat(Str, " ");
					tmpChar = '\0';
					strcat(Str, (char*)&tmpChar);
				}
				else{
					strcat(Str, (char*)&tmpChar);
					tmpChar = '\0';
					strcat(Str, (char*)&tmpChar);
					
				}
				
				tmpBuff = (char*)realloc(Str, strlen(Str)+2);
				if (tmpBuff == NULL){
					fprintf ( stderr, "realloc problem\n");
				}
				else{
					Str = tmpBuff;
				}
				
				i++;
			}
			
			
		}
		
		
   	}

   fclose(fp);
   fclose(fr);
   free(Str);
   if (!error){
		freeListNode(head);/*Free linked list*/
		freeListNode(MacroNames);/*Free linked list*/
		Step1(MainFileName);

		/*free(fileCopy);*/
		
		free(fileTmp);
		free(MainFileName);

   }
   
   

}
