#ifndef CommonFunctions
#define CommonFunctions



extern struct Symbol_Table_List SymbolTable;
extern struct ExternLabels ExternLabelNode;


int convertNegativeDec(int number);
void dataStatement(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep);
void stringStatement(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep);
void entryStatement(char* statement,char LastChar,struct Symbol_Table_List *node,void* obj,int* NumOfLine);
int externStatement(char* statement,char LastChar,struct ExternLabels *node,int* NumOfLine,struct Symbol_Table_List *);
void structStatement2(char* statement,char LastChar, int* NumOfLine,void* obj, int inSecondStep);
void CheckEntryInExternList(char* statement,char LastChar,struct ExternLabels *node, int* NumOfLine);
int checkOperand(char* Operand);
int checkIfAlreadyExist(char* label, struct Symbol_Table_List *node,struct ExternLabels *ExternNode);
int checkIfReserveWord(char label[]);
int checkLabel(char* str);
int CheckGuidanceSentence(char* str);
int CheckinstructionSentence(char* str);
int CheckinstructionSentenceStep2(char* str);
void compress_spaces(char *str);
int CheckComma(char *sentance);
long convert(int n);

char* replace_char(char* str, char find, char replace);




#endif