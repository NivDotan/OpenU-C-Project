#ifndef AssemblySecondStep
#define AssemblySecondStep
#include "CommonFunctions.h"

extern struct Symbol_Table_List SymbolTable;
extern struct ExternLabels ExternLabelNode;
void Step2(char* filename, struct Symbol_Table_List *node,int lastRowInstruction,struct ExternLabels *ExternNode,int entryFlag, int MaxLines);
#endif