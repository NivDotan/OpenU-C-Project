#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "FirstStepFunctions.h"
#include "SecondStepFunctions.h"
#include "CommonFunctions.h"
#include "AssemblyFirstStep.h"


/*Getting a int and converting it to 2's complement binary*/
int convertNegativeDec(int number){
	int tmp,i,FoundOne,j;
	long SwitchBinary;
	SwitchBinary = FoundOne = j = 0;
	i = 1;
	
	tmp = convert(abs(number));	
	
	if (tmp == 0){
		SwitchBinary = 1111111111;
	}
	else if(number == 511 || number == -512){
		if(number == 511){
			SwitchBinary = 0111111111;
		}
		else if(number == -512){
			SwitchBinary = 1000000000;
		}
	}
	else{
		while(tmp){
			if(FoundOne == 0){
				if(tmp %10 == 1){
					FoundOne = 1;
					if(i == 1){
						SwitchBinary = SwitchBinary + 1;
					}
					else{
						SwitchBinary = SwitchBinary + i;
					}
					j++;
					i = i *10;
				}
				else{
					j++;
					i = i *10;
				}
			}
			else{
				if(tmp %10 == 1){
					i = i *10;
					j++;
				}
				else{
					j++;
					SwitchBinary = SwitchBinary + i;
					i = i*10;
				}
			}
			tmp /=10;
		}
		
		switch(j){
			case 1:
				i = 1111111110;
				break;

			case 2:
				i = 1111111100;
				break;


			case 3:
				i = 1111111000;
				break;


			case 4:
				i = 1111110000;
				break;


			case 5:
				i = 1111100000;
				break;


			case 6:
				i = 1111000000;
				break;


			case 7:
				i = 1110000000;
				break;


			case 8:
				i = 1100000000;
				break;


			case 9:
				i = 1000000000;
				break;


		}
		SwitchBinary = SwitchBinary +i;
		}
		

		
	
	return SwitchBinary;
}

/*Function for data statement*/
void dataStatement(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep){
	int i,negativeNum,comma;
	char*appendTmp,*tmpArray,*tmpBuff;

	

	tmpArray = (char *)malloc(sizeof(char) * (1+strlen(statement)));
	if(tmpArray == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	negativeNum = comma =0;
	strcpy(tmpArray,statement);
	
	while ((appendTmp = strtok(NULL, " ")) != NULL){
		tmpBuff = (char*)realloc(tmpArray, strlen(tmpArray)+strlen(appendTmp)+1);
		if (tmpBuff == NULL){
			fprintf ( stderr, "realloc problem\n");
		}
		else{
			tmpArray = tmpBuff;
		}
		
		strcat(tmpArray,appendTmp);
		
	}
	
	LastChar = '\0';
	
	
	strcat(tmpArray,(char*)&LastChar);
	compress_spaces(tmpArray);
	
	for(i=0;i<strlen(tmpArray);i++){/*Handling commas for easier use with strtok*/
		if(tmpArray[i]==','){
			if (comma == 0){
				tmpArray[i] = ' '; 
				comma = 1;
			}
			else {
				Error = 1;
				fprintf(stderr,"Too much comma in data, line number %d \n",*NumOfLine);
				return;
			}
			
		}
		if ((isspace(tmpArray[i])) == 0){
			comma =0;
		}
	}
	if (comma == 1){
		Error = 1;
		fprintf(stderr,"Too much comma in the end of data, line number %d \n",*NumOfLine);
		return;
	}
	
	
	appendTmp = strtok(tmpArray, " ");
	/**tmp = (char *)malloc(sizeof(char) * strlen(appendTmp));
	if(tmp == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	tmp[j]='\0';**/
	
	for (i=0;i<strlen(appendTmp);i++){
		if (appendTmp[i] == '\n'){
			break;
		}
		else if ((appendTmp[i] == '-') && (i == 0)){
			negativeNum = 1;
		}
		else if ((appendTmp[i] == '+')&& (i == 0)){
			continue;
		}
		
		else if(isdigit(appendTmp[i])){
			continue;
		}

		else{
			if ((int)appendTmp[i] != 10){
				Error = 1;
				fprintf(stderr,"Error in data, line number %d \n",*NumOfLine);
				return;
				}
			}
	}
	if (atoi(appendTmp)>511 || atoi(appendTmp)< -512){
		Error = 1;
		fprintf(stderr,"Error in data, Too large number, line number %d \n",*NumOfLine);
		return;
	}
	*NumOfLine = *NumOfLine +1;
	if(inSecondStep){
		PrintToObjFileAddress(*NumOfLine,obj);
	}
	if (negativeNum){
		if(inSecondStep){
			PrintToObjFileCodeNegativeNum(convertNegativeDec(atoi(appendTmp)),obj);
		}
		}
	else{
		if(inSecondStep){
			PrintToObjFileCode(atoi(appendTmp),obj);
		}
		}

	/*free(tmp);*/
	

	while((appendTmp = strtok(NULL, " ")) != NULL){
		/**tmp = (char *)malloc(sizeof(char) * strlen(appendTmp));
		if(tmp == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}**/
		for (i=0;i<strlen(appendTmp);i++){
			if (appendTmp[i] == '\n'){
				break;
			}
			else if ((appendTmp[i] == '-') && (i == 0)){
				negativeNum = 1;
			}
			else if ((appendTmp[i] == '+')&& (i == 0)){
				continue;
			}
			else if(isdigit(appendTmp[i])){
				continue;
			}

			else{
				if (appendTmp[i] != '\n'){
					
					Error = 1;
					fprintf(stderr,"Error in data, line number %d \n",*NumOfLine);
					return;
				}
				}
		}
		if (atoi(appendTmp)>511 || atoi(appendTmp)< -512){
			Error = 1;
			fprintf(stderr,"Error in data, Too large number %d, line number %d \n", atoi(appendTmp),*NumOfLine);
			return;
		}
		*NumOfLine = *NumOfLine +1;
		if(inSecondStep){
			PrintToObjFileAddress((*NumOfLine) ,obj);
		}
		if (negativeNum){
			if(inSecondStep){
				PrintToObjFileCodeNegativeNum(convertNegativeDec(atoi(appendTmp)),obj);
			}
			}
		else{
			if(inSecondStep){
				PrintToObjFileCode(atoi(appendTmp),obj);
			}
			}
		negativeNum = 0;
		/*free(tmp);*/
		
	
	}
	free(tmpArray);
}

/*Function for string statement*/
void stringStatement(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep){
	int i;
	if((statement[0] == '"') && (statement[strlen(statement) - 1] == '"')){
		for(i=1;i<(strlen(statement)-1);i++){
			if (statement[i] == '"'){
				Error = 1;
				fprintf(stderr,"There is a error in .string, line number %d \n",*NumOfLine);
				return; 
			}
			
			*NumOfLine = *NumOfLine +1;
			if(inSecondStep){
				PrintToObjFileAddress((*NumOfLine),obj);
				PrintToObjFileCode((int)statement[i],obj);
			}
		}
		*NumOfLine = *NumOfLine +1;
		if(inSecondStep){
			PrintToObjFileAddress((*NumOfLine),obj);
			PrintToObjFileCode(0,obj);
		}
		
	}
	else{
		Error = 1;
		fprintf(stderr,"There is a error in .string, line number %d \n",*NumOfLine);
		return;
	}
}

/*Function for entry statement*/
void entryStatement(char* statement,char LastChar,struct Symbol_Table_List *node,void* obj,int* NumOfLine){
	int index,i,intConvertDec,FlagFoundLabel;
	char* tmpOperand;
	FlagFoundLabel = 0;
	
	if (strtok(NULL, " ") != NULL){
		Error = 1;
		fprintf(stderr,"There is a error in .Entry, Entry label: %s \n",statement);
		return;
	}
	if ((strchr(statement,'\n')) != NULL){
		index = (int)((strchr(statement,'\n'))-statement);
		tmpOperand = (char *)malloc(sizeof(char) * index);
		if(tmpOperand == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		for(i=0;i<index;i++){
			tmpOperand[i] = statement[i];
		}
		tmpOperand[index] = '\0';
		
		while (node != NULL)
		{	
			if (strcmp(node->name, tmpOperand) == 0){
				
				intConvertDec =  (int)convert(node->value) ;
				fprintf(obj,"%s\t", statement);
				
				PrintToObjFileCode(BinToDec(intConvertDec),obj);
				FlagFoundLabel = 1;
				break;
			}
			node = node->next;
		}
		if (FlagFoundLabel == 0){
			Error = 1;
			fprintf(stderr,"There is a error in .Entry, Entry label: %s \n",statement);
			return;
		}
		free(tmpOperand);
	}
	else{
		while (node != NULL)
			{	
				if (strcmp(node->name, statement) == 0){
					
					intConvertDec =  (int)convert(node->value) ;
					
					fprintf(obj,"%s\t", statement);
					PrintToObjFileCode(BinToDec(intConvertDec),obj);
					FlagFoundLabel = 1;
					break;
				}
				node = node->next;
			}
			if (FlagFoundLabel == 0){
				Error = 1;
				fprintf(stderr,"There is a error in .Entry, Entry label: %s \n",statement);
				return;
			}
	}	
}

/*Function for extern statement*/
int externStatement(char* statement,char LastChar,struct ExternLabels *node,int* NumOfLine,struct Symbol_Table_List *LabelNode){
	int index,i;
	char* appendTmp,*tmp;
	
	if (strtok(NULL, " ") != NULL){
		Error = 1;
		fprintf(stderr,"There is a error in .Extern, Extern label: %s \n",statement);
		return 0;
	}
	
	if ((strchr(statement,'\n')) != NULL || ((strchr(statement,'\r')) != NULL)){

		index = (int)((strchr(statement,'\n'))-statement);
		
		if((strchr(statement,'\r')) != NULL){
			index = (int)((strchr(statement,'\r'))-statement);
		}
		
		
		tmp = (char *)malloc(sizeof(char) * (1+index)); 
		if(tmp == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		
		for(i=0;i<index;i++){
			if (isspace(statement[i])){
				Error = 1;
				fprintf(stderr,"There is a error in .Extern, line number %d \n",*NumOfLine);
				return 0;
			}
			tmp[i] = statement[i];
		}
		tmp[index] = '\0';
		
		while (LabelNode!= NULL)
		{	
			if (strcmp(LabelNode->name, tmp) == 0){
				Error = 1;
				fprintf(stderr,"Declaring an already defined label as external\n");
				return 0;
			}
			LabelNode = LabelNode->next;
		}
		free(tmp);
		
		return 1;
		
	}		
	else if ((appendTmp = strtok(NULL, " ")) != NULL){
		Error = 1;
		fprintf(stderr,"There is a error in .Extern, line number %d \n",*NumOfLine);
		return 0;
	}
		
	else{
		
		
		while (LabelNode != NULL)
		{	
			
			if (strcmp(LabelNode->name, statement) == 0){
				Error = 1;
				fprintf(stderr,"Declaring an already defined label as external\n");
				return 0;
			}
			LabelNode = LabelNode->next;
		}
		
		return 1;
	}
		
	
}

/*Function for struct statement*/
void structStatement2(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep){
	
	int i,comma;
	char* appendTmp ,*tmp,*tmpBuff;
	
	tmp = (char *)malloc(sizeof(char) * (1+strlen(statement)));
	
	if(tmp == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	comma = 0;
	
	
	strcpy(tmp,statement);
	
	while ((appendTmp = strtok(NULL, " ")) != NULL){
		
		tmpBuff = (char*)realloc(tmp, strlen(tmp)+strlen(appendTmp)+1);
		if (tmpBuff == NULL){
			fprintf ( stderr, "realloc problem\n");
		}
		else{
			tmp = tmpBuff;
		}
		
		strcat(tmp,appendTmp);
		
	}
	
	LastChar = '\0';
	
	
	strcat(tmp,(char*)&LastChar);

	compress_spaces(tmp);

	if (inSecondStep == 0){
		
		for(i=0;i<strlen(tmp);i++){
			if(tmp[i]==','){
				if (comma == 0){
					tmp[i] =' '; 
					comma = 1;
				}
				else {
					Error = 1;
					fprintf(stderr,"Too much comma in the end of Struct, line number %d \n",*NumOfLine);
					return;
				}
			}
			if ((isspace(tmp[i])) == 0){
				comma =0;
			}
		}
		if (comma == 1){
			Error = 1;
			fprintf(stderr,"Too much comma in the end of Struct, line number %d \n",*NumOfLine);
			return;
		}
	}

	else{
		for(i=0;i<strlen(tmp);i++){
			if(tmp[i]==','){
				tmp[i] = ' ';
			}
		}
	}
	
	
	appendTmp = strtok(tmp, " ");
	
	if((appendTmp[0] == '"') && (appendTmp[strlen(appendTmp) - 1] == '"')){
		for(i=1;i<(strlen(appendTmp)-1);i++){
			
			*NumOfLine = *NumOfLine +1;
			if(inSecondStep){
				PrintToObjFileAddress((*NumOfLine),obj);
				PrintToObjFileCode((int)appendTmp[i],obj);
			}
		}
		*NumOfLine = *NumOfLine +1;
		if(inSecondStep){
			PrintToObjFileAddress((*NumOfLine),obj);
			PrintToObjFileCode(0,obj);
		}
	}
	else if((appendTmp[0] == ',') || (appendTmp[0] == '-') || (appendTmp[0] == '+') || isspace(appendTmp[0]) || isdigit(appendTmp[0])) {
		if (atoi(appendTmp)>511 || atoi(appendTmp)< -512){
			Error = 1;
			fprintf(stderr,"There is a error in .Struct, Too large number, line number %d \n",*NumOfLine);
			return;
		}
		*NumOfLine = *NumOfLine +1;
		if(inSecondStep){
			PrintToObjFileAddress((*NumOfLine),obj);
			getNumInStatements(appendTmp,obj,*NumOfLine);
		}
		
	}
	else{
		Error = 1;
		
		fprintf(stderr,"There is a error in .Struct, line number %s, %d \n",appendTmp,*NumOfLine);
	}
	
	while((appendTmp = strtok(NULL, " ")) != NULL){
		
		if((appendTmp[0] == '"') && (appendTmp[strlen(appendTmp) - 1] == '"' && strlen(appendTmp)>2)){
			for(i=1;i<(strlen(appendTmp)-1);i++){
				
				*NumOfLine = *NumOfLine +1;
				if(inSecondStep){
					PrintToObjFileAddress((*NumOfLine),obj);
					PrintToObjFileCode((int)appendTmp[i],obj);
				}
			}
			*NumOfLine = *NumOfLine +1;
			if(inSecondStep){
				PrintToObjFileAddress((*NumOfLine),obj);
				PrintToObjFileCode(0,obj);
			}
		}
		else if((appendTmp[0] == ',') || (appendTmp[0] == '-') || (appendTmp[0] == '+') || isspace(appendTmp[0]) || isdigit(appendTmp[0])) {
			if (atoi(appendTmp)>511 || atoi(appendTmp)< -512){
				Error = 1;
				fprintf(stderr,"There is a error in .Struct, Too large number, line number %d \n",*NumOfLine);
				return;
			}
			*NumOfLine = *NumOfLine +1;
			if(inSecondStep){
				PrintToObjFileAddress((*NumOfLine),obj);
				getNumInStatements(appendTmp,obj,*NumOfLine);
			}
		}
		else{
			Error = 1;
			
			fprintf(stderr,"There is a error in .Struct, line number %s, %d \n",appendTmp,*NumOfLine);
		}
	}
	free(tmp);
}


/*Checking if the entry is inside the extern list*/
void CheckEntryInExternList(char* statement,char LastChar,struct ExternLabels *node, int* NumOfLine){
	int index,i;
	char* tmpOperand;
	
	statement = strtok(NULL, " ");
	if ((strchr(statement,'\n')) != NULL){
		index = (int)((strchr(statement,'\n'))-statement);
		tmpOperand = (char *)malloc(sizeof(char) * index);
		if(tmpOperand == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		for(i=0;i<index;i++){
			tmpOperand[i] = statement[i];
		}
		
		tmpOperand[index] = '\0';
		
		while (node != NULL)
		{	
			if (strcmp(node->name, tmpOperand) == 0){
				Error = 1;
				fprintf(stderr,"There is a error in .Entry, line number %d \n",*NumOfLine);
				break;
			}
			node = node->next;
		}
		free(tmpOperand);
	}
	else{
		while (node != NULL)
			{	
				if (strcmp(node->name, statement) == 0){
					Error = 1;
					fprintf(stderr,"There is a error in .Entry, Entry label: %s \n",statement);
					break;
				}
				node = node->next;
			}
	}	
}

/*Checking if the operand is legal*/
int checkOperand(char* Operand){
	char FirstChar,Registers[2];
	int i,DecOperand,negative,positive,Len,Tmp;
	FirstChar = Operand[0];
	Registers[0] = 'r';
	Registers[1] = '0';
	positive = negative = 0;
	
	if((int)Operand[strlen(Operand)-1] == 10 || (int)Operand[strlen(Operand)-1] == 13){
		Len = strlen(Operand)-1;
	}
	else{
		Len = strlen(Operand);
	}
	if(((int)FirstChar == (int)('#')) != 0){
		for(i =1;i<Len;i++){
			if ((Operand[i] == '-') && (negative == 0) && (positive == 0)){
				negative=1;
			}
			else if ((Operand[i] == '+') && (negative == 0) && (positive == 0)){
				positive=1;
			}
			else if((Operand[i]- '0')<0 || (Operand[i]- '0')>9){ 
				return 6;
			}
		}
		if((Len == 1) || ((negative == 1 || positive == 1) &&(Len == 2))){
			return 6;
		}
		else if((Len == 2 && Operand[1]=='0') || (Len == 3 && (negative == 1 || positive == 1) && Operand[2]=='0')){
			return 0;
		}
		else{
			Tmp = atoi(Operand + 1);
			if(Tmp==0 || Tmp > 127 || Tmp < -128){
				return 6;
			}
		}
		DecOperand = 0;
		return DecOperand;
	}
	
	else if (strchr(Operand, '.') != NULL){
		DecOperand = 2;
		return DecOperand;
		
	}
	
	else if ((strlen(Operand) == 2) && (Operand[0] == Registers[0])){
		
		for(i=0;i<8;i++){
			if (Registers[1] == Operand[1]){
				DecOperand = 3;
				return DecOperand;
			}
			else if ((i == 7)&& (isdigit(Operand[1] - '0'))){ 
				DecOperand = 5;
				return 5;
			}
			else{
				Registers[0] = 'r';
				Registers[1] = i+'0';
			}
		}
		
	}/**
	if (checkIfReserveWord(Operand)){
		DecOperand = 1;
		return DecOperand;
		
	}**/
	if (checkLabel(Operand)){
		DecOperand = 1;
		return DecOperand;
		
	}

	return 4; 
}

/*Checking if a label is already exist in one of the lists*/
int checkIfAlreadyExist(char* label, struct Symbol_Table_List *node,struct ExternLabels *ExternNode){ 
	int index,i;
	char * tmpOperand;
	
	index = (int)((strchr(label,':'))-label);
	
	tmpOperand = (char *)malloc(sizeof(char) * index+1);
	if(tmpOperand == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	for(i=0;i<index;i++){
		tmpOperand[i] = label[i];
	}
	tmpOperand[index] = '\0';

	while (node != NULL)
  	{	
		  if (strcmp(node->name, tmpOperand) == 0){
			fprintf(stderr,"The label: %s already exist \n", tmpOperand);
			return 0;
		  }
		  	
     	node = node->next;
  	}
	
	while (ExternNode != NULL)
  	{	
		
		  if (strcmp(ExternNode->name, tmpOperand) == 0){
			fprintf(stderr,"The label: %s already exist as extern\n", tmpOperand);
			return 0;
		  }
		  	
     	ExternNode = ExternNode->next;
  	}
  	
	return 1;
}
 
 /*Checking if a label is a reserve word*/
int checkIfReserveWord(char label[]){
	int i;
	const char *statement[]={ 
	 		".data", 
	 		".string", 
	 		".struct"};
	const char *instruction[]={ 
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
	if (label[strlen(label)-1] == '\n')
		label[strlen(label)-1] = '\0';
	if (label[0]=='r'){
		if ( ((label[1] - '0')>=0) &&  ((label[1] - '0') <= 0) && (label[2]==':')){
			return 0;
		}
	}
	for(i= 0; i<3;i++){
		if ((strcmp(label, (statement[i])) == 0))
			return 0;
	}
	for(i= 0; i<16;i++){
		if ((strcmp(label, (instruction[i])) == 0))
			return 0;
	}
	return 1;

}

 /*Checking the label*/
int checkLabel(char* str){
	int i,j;
    char label[31];
	label[0] = '\0';
	
    j=0;
    for (i = 0; i <  strlen(str); i++){
		if ((isalpha(str[i]) || isdigit(str[i]) || (int)str[i] == 58 || (int)str[i] == 10 || (int)str[i] == 13)  && i < 31){
			label[j] = str[i];
			j++;
			label[j] = '\0';
		}
	}
	
	
	if ((strcmp(label, str) == 0) && (checkIfReserveWord(label))){ 
		return 1;
	}
	else{
		Error = 1;
		fprintf(stderr," Illegal Label name: %s \n",str);
		return 0;
	}
		
}

 /*Checking the Guidance sentance*/
int CheckGuidanceSentence(char* str){
	int i,j,lenStr, lenStatement;
	const char *statement[]={ 
	 		".data", 
	 		".string", 
	 		".struct",
			".extern",
			".entry"};
	
	lenStr = strlen(str);
	
	if(str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == '\r'){
		
		for(i=0; i<5; i++){
			lenStatement = strlen(statement[i]) + 1;
			if(lenStr == lenStatement){
				
				for(j=0; j<lenStatement-2; j++){
					if ((str[j] == (statement[i][j])) && (j == lenStr-3))
						return 6;
					else if (str[j] != (statement[i][j])){
						break;
					}
				}
			}
		}
	}
	else{
		
		for(i=0; i<5; i++){
			lenStatement = strlen(statement[i]);
			if(lenStr == lenStatement){
				for(j=0; j<lenStatement; j++){
					if ((str[j] == (statement[i][j])) && (j == lenStr-1))
						return i;
					else if (str[j] != (statement[i][j])){
						
						break;
					}
				}
			}
		}
	}
	
	
	Error = 1;
	fprintf(stderr,"Undefined statement name %s \n",str);
	return 5;
		
}

 /*Checking the instruction sentance*/	
int CheckinstructionSentence(char* str){	
	int i,j,lenStr, lenInstruction;
	const char *instruction[]={ 
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
	lenStr = strlen(str);
	
	if((int)str[strlen(str)-1] == 10 || (int)str[strlen(str)-1] == 13){
		lenStr = strlen(str)-1;
	}
	else{
		lenStr = strlen(str);
	}
	
	for(i=0; i<16; i++){
		lenInstruction = strlen(instruction[i]);
		
		if(lenStr == lenInstruction){
			for(j=0; j<lenInstruction; j++){
				/*printf(" lenStr:%c, lenInstruction:%c", str[j] , (instruction[i].name)[j]);*/
				if ((str[j] == (instruction[i][j])) && (j == lenStr-1))
					return i;
				else if (str[j] != (instruction[i][j])){
					break;
					
				}
			}
		}
		
	}
	Error = 1;
	
	fprintf(stderr," Undefined sinstruction name %s \n",str);
	return 16;
}

 /*Checking the instruction sentance in step2*/
int CheckinstructionSentenceStep2(char* str){	
	int i,j,lenStr, lenInstruction;
	
	const char *instruction[]={ 
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
	lenStr = strlen(str);
	for(i=0; i<16; i++){
		lenInstruction = strlen(instruction[i]);
		
		if(lenStr == lenInstruction){
			for(j=0; j<lenInstruction; j++){
				/*printf(" lenStr:%c, lenInstruction:%c", str[j] , (instruction[i].name)[j]);*/
				if ((str[j] == (instruction[i])[j]) && (j == lenStr-1))
					return i;
				else if (str[j] != (instruction[i])[j]){
					break;
					
				}
			}
		}
		
	}
	
	return 16;
}

 /*Decraseing the size of the spaces in a sentance*/
void compress_spaces(char *str)
{
    char *dst = str;

    for (; *str; ++str) {
        *dst++ = *str;

        if (isspace(*str)) {
            do ++str; 

            while (isspace(*str));

            --str;
        }
    }

    *dst = 0;
}

 /*Checking if the comma is in the right order*/
int CheckComma(char *sentance){
	int comma, space,word,i;
	comma = space = word = 0;
	/*1 - Error of no comma between operand*/
	/*2 - Error of too many comma*/
	/*0 - All is good*/
	for (i = 0; i<strlen(sentance); i++){
		if (isspace(sentance[i]) == 0 && sentance[i] !=','){
			if(comma == 0 && space ==1){
				return 1;
			}
			word = 1;
		}
		else if (isspace(sentance[i]) == 1){
			word = 0;
			space = 1;
		}
		else if(sentance[i] ==','){
			if (comma == 1){
				return 2;
			}
			space = 0;
			comma = 1;
			word = 0;
		}

	}
	return 0;
}

 /*Convert from int to binary*/
long convert(int n){
	long bin;
	int rem,i;
	bin = 0;
	i = 1;

	while(n!=0){
		rem = n%2;
		n /= 2;
		bin += (rem * i);
		i *= 10;
	}
	
	return bin;
}


