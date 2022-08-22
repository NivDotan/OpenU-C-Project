#ifndef SecondStepFunctions
#define SecondStepFunctions


extern struct Symbol_Table_List SymbolTable;
extern struct ExternLabels ExternLabelNode;


int OperandBinaryCode1(int DecFirstOperand, char* FirstOperand,int OpNumber,int* NumOfLine,struct Symbol_Table_List *node,void* obj,int entryFlag ,void *ext,struct ExternLabels *ExternNode);
void OperandBinaryCodeRefisters1(char* FirstOperand, char* SecondOperand, void* obj);

void PrintToObjFileAddress(int NumOfLine,void* obj);
void PrintToObjFileCode(int decCode,void* obj);
void PrintToObjFileCodeNegativeNum(int decCode,void* obj);

void freeListSymbol_Table_List(struct Symbol_Table_List *node);
void freeListExternLabels(struct ExternLabels *node);
void getNumInStatements(char* statement, void* obj, int NumOfLine);
int BinToDec(int num);
int ArrayBinForHltRts(int FourNum, int twoNumFirst, int twoNumSecond, int lastTwoNum);
void FirstGroupSecondStep(char* FirstOperand, char* SecondOperand,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode);
void SecondGroupSecondStep(char* FirstOperand, char* null,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode);
void ThirdGroupSecondStep(char* null, char* null2,int OperationNumber, int* NumOfLine,struct Symbol_Table_List *node,void *obj,int entryFlag,void *ext,struct ExternLabels *ExternNode);

#endif
