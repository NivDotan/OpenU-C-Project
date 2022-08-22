#ifndef AssemblyFirstStep
#define AssemblyFirstStep

struct Symbol_Table_List {
	char* name;
	int value;
	char* type;
	struct Symbol_Table_List *next;
};

struct ExternLabels {
	char* name;
	struct ExternLabels *next;
};

void Step1(char* filename);
extern int Error;
#endif