#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "PreAssembly.h"

/*Main Function of the program - getting the names of the files from the command line*/
int main(int argc, const char** argv){
	int i;
	
	if (argc ==1){
		fprintf(stderr,"Error in main Function: There is not an input file\n");
	}
	else{
		for(i=1;i<argc;i++){
			fprintf(stderr,"Working on file %s.as,\n",argv[i]);
            PreAssemblyFunc(argv[i]);
		}
	}
    return 0;
}