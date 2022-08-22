
#ifndef FirstStepFunctions
#define FirstStepFunctions


extern struct Symbol_Table_List SymbolTable;
extern struct ExternLabels ExternLabelNode;

void append1(struct Symbol_Table_List** head_ref, char* new_name, int value, char* type);
void appendExternNode(struct ExternLabels** head_ref, char* new_name);
void OperandBinaryCodeRefisters(char* FirstOperand, char* SecondOperand);
int OperandBinaryCode(int DecFirstOperand, char* FirstOperand,int OpNumber,int* NumOfLine);
void FirstGroup(char* FirstOperand, char* SecondOperand,int OperationNumber, int* NumOfLine);
void SecondGroup(char* FirstOperand, char* null,int OperationNumber, int* NumOfLine);
void ThirdGroup(char* null, char* null2,int OperationNumber, int* NumOfLine);

#endif
