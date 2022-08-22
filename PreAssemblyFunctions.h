struct node {
   char* data;
   struct node *next;
};

void appendMacroParser(struct node** head_ref, char* new_data,int IsMacroName);
int MacroFinder(char* str,struct node *node);
void MacroChanger(char* str,struct node *node,void *fr);
int CheckNameMacro(char *sentance);
int checkIfReserveWordInMacro(char label[],int index);
void freeListNode(struct node *node);
