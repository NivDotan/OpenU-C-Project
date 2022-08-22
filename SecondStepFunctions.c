#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "SecondStepFunctions.h"
#include "AssemblyFirstStep.h"
#include "CommonFunctions.h"
/*the Base 32 array*/
const char Base32[] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

/*Print the code and the address in base 32 to the ob file*/
int OperandBinaryCode1(int DecFirstOperand, char* FirstOperand,int OpNumber,int* NumOfLine,struct Symbol_Table_List *node,void* obj,int entryFlag ,void *ext,struct ExternLabels *ExternNode){
	int index,tmp,SwitchBinary,i,intConvertDec,inEntry,Printed;
	char* tmpOperand;
	SwitchBinary = inEntry = Printed = 0;
	i = 1;
	
	if (DecFirstOperand == 0){ 
		memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
		if (atoi(FirstOperand) < 0){
			memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
			tmp = convert(atoi(FirstOperand)-1);
			
			if (tmp == 0){
				SwitchBinary = 1111111111;
				PrintToObjFileAddress((*NumOfLine) + 1,obj);
				PrintToObjFileCode((BinToDec(SwitchBinary*100)),obj);
			}
			else{
				while (tmp){
					if((tmp%10) == 1){
						SwitchBinary = SwitchBinary +0;
						if ((tmp/10)==0){
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
			
			PrintToObjFileAddress((*NumOfLine) + 1,obj);
			PrintToObjFileCode((BinToDec(SwitchBinary*100)),obj);
			return 1;
			
		}
		else{
			PrintToObjFileAddress((*NumOfLine) + 1,obj);
			PrintToObjFileCode((BinToDec((int)convert(atoi(FirstOperand))*100)),obj);
			return 1;
			
		}
		
	}
	else if (DecFirstOperand == 1){
		if(entryFlag == 1){
			if ((strchr(FirstOperand,'\n')) != NULL){
				
				index = (int)((strchr(FirstOperand,'\n'))-FirstOperand);
				tmpOperand = (char *)malloc(sizeof(char) * index);
				if(tmpOperand == NULL){
					fprintf(stderr,"Problem with malloc\n");
					exit(1);
				}
				for(i=0;i<index;i++){
					tmpOperand[i] = FirstOperand[i];
					
				}
				tmpOperand[index] = '\0';
				
				while (ExternNode != NULL)
				{	
					if (strcmp(ExternNode->name, tmpOperand) == 0){
						fprintf(ext,"%s\t", tmpOperand);
						PrintToObjFileAddress((*NumOfLine) + 1,ext);
						fprintf(ext,"\n");

						PrintToObjFileAddress((*NumOfLine) + 1,obj);
						PrintToObjFileCode(1,obj);
						Printed = 1;
						inEntry = 1;
						break;
					}
					ExternNode = ExternNode->next;
				}
				free(tmpOperand);
			}
			else{
				
				while (ExternNode != NULL)
					{	
						if (strcmp(ExternNode->name, FirstOperand) == 0){
							
							PrintToObjFileAddress((*NumOfLine) + 1,obj);
							PrintToObjFileCode(1,obj);
							fprintf(ext,"%s\t", ExternNode->name);
							PrintToObjFileAddress((*NumOfLine) + 1,ext);
							fprintf(ext,"\n");
							Printed = 1;
							inEntry = 1;
							break;
						}
						ExternNode = ExternNode->next;
					}
					
			}	
			if (inEntry)
				return 1;
			
		}
		if(inEntry == 0){
			if ((strchr(FirstOperand,'\n')) != NULL){
				
				index = (int)((strchr(FirstOperand,'\n'))-FirstOperand);
				tmpOperand = (char *)malloc(sizeof(char) * index);
				if(tmpOperand == NULL){
					fprintf(stderr,"Problem with malloc\n");
					exit(1);
				}
				for(i=0;i<index;i++){
					tmpOperand[i] = FirstOperand[i];
					
				}
				tmpOperand[index] = '\0';
				
				while (node != NULL)
				{	
					if (strcmp(node->name, tmpOperand) == 0){
						PrintToObjFileAddress((*NumOfLine) + 1,obj);
						intConvertDec =  (int)convert(node->value)* 100 + atoi(node->type);
						Printed = 1;
						PrintToObjFileCode(BinToDec(intConvertDec),obj);
						return 1;
					}
					node = node->next;
				}
				free(tmpOperand);
			}
			else{
				while (node != NULL)
					{	
						if (strcmp(node->name, FirstOperand) == 0){
							PrintToObjFileAddress((*NumOfLine) + 1,obj);
							intConvertDec =  (int)convert(node->value)* 100 + atoi(node->type);
							Printed = 1;
							PrintToObjFileCode(BinToDec(intConvertDec),obj);
							return 1;
						}
						node = node->next;
					}
					
			}	
			/*return 1;*/
			
		}
		
		if (Printed == 0){/*Check if the label exist*/
			fprintf(stderr,"Error- Label: %s dont exist, line number %d \n",FirstOperand,*NumOfLine);
			return 0;
		}
	}
	else if (DecFirstOperand == 2){
		index = (int)((strchr(FirstOperand,'.'))-FirstOperand);
		tmpOperand = (char *)malloc(sizeof(char) * index);
		if(tmpOperand == NULL){
			fprintf(stderr,"Problem with malloc\n");
			exit(1);
		}
		for(i=0;i<index;i++){
			tmpOperand[i] = FirstOperand[i];
		}
		tmpOperand[index] = '\0';
		while (node != NULL)
			{	
				if (strcmp(node->name, tmpOperand) == 0){
					intConvertDec =  (int)convert(node->value)* 100 + atoi(node->type);
					PrintToObjFileAddress((*NumOfLine) + 1,obj);
					Printed = 1;
					PrintToObjFileCode(BinToDec(intConvertDec),obj);
					break;
				}
				node = node->next;
			}
		
		if (Printed == 0){/*Check if the label exist*/
			fprintf(stderr,"Error- Label: %s dont exist, line number %d \n",tmpOperand,*NumOfLine);
			return 0;
		}
		free(tmpOperand);
		memmove(FirstOperand,FirstOperand+(index+1),strlen(FirstOperand));
		PrintToObjFileAddress((*NumOfLine) + 2,obj);
		PrintToObjFileCode((BinToDec((int)convert(atoi(FirstOperand))*100)),obj);
		return 2;
		
	}
	else if (DecFirstOperand == 3){
		memmove(FirstOperand,FirstOperand+1,strlen(FirstOperand));
		if (OpNumber == 1){
			PrintToObjFileAddress((*NumOfLine) + 1,obj);
			PrintToObjFileCode((BinToDec((int)convert(atoi(FirstOperand))*1000000)),obj);
		}
		else{
			PrintToObjFileAddress((*NumOfLine) + 1,obj);
			PrintToObjFileCode((BinToDec((int)convert(atoi(FirstOperand))*100)),obj);
		}
		return 1;
		
	}
	return 0;
}

/*Print the code in base 32 of a 2 registers to the ob file*/
void OperandBinaryCodeRefisters1(char* FirstOperand, char* SecondOperand, void* obj){
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
	PrintToObjFileCode((BinToDec(bin)),obj);
}

/*Print the address in base 32 of a line to the ob file*/
void PrintToObjFileAddress(int NumOfLine,void* obj){
	int firstChar,secondChar;
	char tmpArray[2];
	firstChar = NumOfLine%32;
	secondChar = (NumOfLine - firstChar) / 32;
	tmpArray[0] = (char)Base32[secondChar];
	tmpArray[1] = (char)Base32[firstChar];
	fprintf(obj,"%s\t", tmpArray);
}

/*Print the code in base 32 of a line to the ob file*/
void PrintToObjFileCode(int decCode,void* obj){
	int firstChar,secondChar;
	char tmpArray[2];
	firstChar = decCode%32;
	secondChar = (decCode- firstChar) / 32;
	tmpArray[0] = (char)Base32[secondChar];
	tmpArray[1] = (char)Base32[firstChar];
	fprintf(obj,"%s\n", tmpArray);
}

/*Print the code in base 32 of a negative number to the ob file*/
void PrintToObjFileCodeNegativeNum(int decCode,void* obj){
	int firstChar,secondChar,i,sum;
	char tmpArray[2];
	sum = 0;
	for(i=0;i<10;i++){
		if(decCode%10 == 1)
			sum = sum + pow(2,i);
		decCode = decCode /10;
	}
	firstChar = sum%32;
	secondChar = (sum- firstChar) / 32;
	tmpArray[0] = (char)Base32[secondChar];
	tmpArray[1] = (char)Base32[firstChar];
	fprintf(obj,"%s\n", tmpArray);

	
}

/*Free label linked list*/
void freeListSymbol_Table_List(struct Symbol_Table_List *node) {
	struct Symbol_Table_List * tmp;
	tmp = NULL;
	while(node) {
		tmp = node->next;
		free(node->name);
		free(node->type);
		free(node);
		node = tmp;
	}
}

/*Free extern linked list*/
void freeListExternLabels(struct ExternLabels *node) {
	struct ExternLabels * tmp;
	tmp = NULL;
	while(node) {
		tmp = node->next;
		free(node->name);
		free(node);
		node = tmp;
	}
}

/*Getting the number in the data and struct sentaces*/
void getNumInStatements(char* statement, void* obj, int NumOfLine){
    int i,j,negativeNum;
	char* tmp;
	tmp = (char *)malloc(sizeof(char) * 6);
	if(tmp == NULL){
		fprintf(stderr,"Problem with malloc\n");
		exit(1);
	}
	j=0;
	negativeNum = 0;
	tmp[j]='\0';
	
	for(i=0;i<=(strlen(statement));i++){
		
		if(i ==(strlen(statement))){
			
			if (negativeNum){
					PrintToObjFileCodeNegativeNum(convertNegativeDec(atoi(tmp)),obj);
				}
			else{
					PrintToObjFileCode(atoi(tmp),obj);
				}
			free(tmp);
		}
		else if (statement[i] == '\n'){
				break;
		}
		else if (statement[i] == '-'){
			negativeNum = 1;
		}
		else if (statement[i] == '+'){
			continue;;
		}
		else if((isdigit(statement[i])!=0)){
			tmp[j]=statement[i];
			tmp[j+1]='\0';
			
			j++;
			
		}
		else{
			/*Error = 1;*/
			fprintf(stderr,"There is a error in .Struct, line number %d \n",NumOfLine);
			break;
		}
			
	}
	
}

/*Convert for binary to Decimal*/
int BinToDec(int num){
	int decimal_num, base , rem;  
	decimal_num = 0;
	base = 1;
	while ( num > 0)  
    {  
        rem = num % 10; 
        decimal_num = decimal_num + rem * base;  
        num = num / 10; 
        base = base * 2;  
    }  
	return decimal_num;
}

/*getting the number for the HLT or RTS sentances*/
int ArrayBinForHltRts(int FourNum, int twoNumFirst, int twoNumSecond, int lastTwoNum){
	int bin,Dec;
	bin = 0;
	bin =bin + FourNum * 1000000;
	bin += twoNumFirst * 10000;
	bin += twoNumSecond * 100;	
	bin += lastTwoNum * 10;
	Dec = BinToDec(bin);
	return Dec;
}

/*Print the code and address for the first group of sentances*/
void FirstGroupSecondStep(char* FirstOperand, char* SecondOperand,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode){
	int DecFirstOperand, DecSecondOperand,NumOfRowAddress;
	
	DecFirstOperand = checkOperand(FirstOperand);
	DecSecondOperand = checkOperand(SecondOperand);
	
	*NumOfLine = *NumOfLine + 1;
	PrintToObjFileAddress((*NumOfLine), obj);
	PrintToObjFileCode(ArrayBinForHltRts( (int)convert(OperationNumber),(int)convert(DecFirstOperand),(int)convert(DecSecondOperand),00),obj);
	
	if ((DecFirstOperand == 3)&&(DecSecondOperand == 3)){
		*NumOfLine = *NumOfLine +1;
		PrintToObjFileAddress((*NumOfLine), obj);
		OperandBinaryCodeRefisters1(FirstOperand,SecondOperand,obj);
	}
	else{
		NumOfRowAddress = OperandBinaryCode1(DecFirstOperand,FirstOperand,1,NumOfLine,node,obj,entryFlag,ext,ExternNode);
		*NumOfLine = *NumOfLine + NumOfRowAddress;
		NumOfRowAddress = OperandBinaryCode1(DecSecondOperand,SecondOperand,2,NumOfLine,node,obj,entryFlag,ext,ExternNode);
		*NumOfLine = *NumOfLine + NumOfRowAddress;
	}
	
	
}

/*Print the code and address for the second group of sentances*/
void SecondGroupSecondStep(char* FirstOperand, char* null,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode){
	
	int DecFirstOperand,NumOfRowAddress;
	DecFirstOperand = checkOperand(FirstOperand);
	*NumOfLine = *NumOfLine + 1;
	PrintToObjFileAddress((*NumOfLine), obj);
	PrintToObjFileCode(ArrayBinForHltRts((int)convert(OperationNumber),00,(int)convert(DecFirstOperand),00),obj);
	NumOfRowAddress = OperandBinaryCode1(DecFirstOperand,FirstOperand,2,NumOfLine,node,obj,entryFlag,ext,ExternNode);
	*NumOfLine = *NumOfLine + NumOfRowAddress;
	
	
}

/*Print the code and address for the third group of sentances*/
void ThirdGroupSecondStep(char* null, char* null2,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode){
	*NumOfLine = *NumOfLine + 1;
	PrintToObjFileAddress((*NumOfLine),obj);
	PrintToObjFileCode(ArrayBinForHltRts((int)convert(OperationNumber),00,00,00),obj);
	*NumOfLine = *NumOfLine + 1;
}
