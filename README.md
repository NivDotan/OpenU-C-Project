# About
This project is an assignment in a C programming course. The purpose of this project is to build an assembler with macros for 16-instructions asssembly language, for an imaginary 10-bit CPU and 8 bits of memory size that encode in a special 32 base as an output. This project was done by @NivDotan

<span id=a1> <h2> Modules </h2> </span> 
1. main - main function definition, argv & argc processing, single/multi file processing.
2. PreAssembly - contain a function for getting the macro in the file and paste them in the output file.
3. PreAssemblyFunctions - contain functions for checking and getting the macros.
4. AssemblyFirstStep - contain a function for getting a single line and checking for errors.
5. FirstStepFunctions - contain functions for processing a line.
6. CommonFunctions - contain common functions for processing operands and instructions.
7. AssemblySecondStep - contain a function for getting a single line and contains methods for writing output files: *.ob, *.ext and *.ent.
8. SecondStepFunctions - contain functions for processing for processing operands and instructions and replacing them in a special base 32.


<hr/>
<span id=a1> <h2> Overview </h2> </span> 

The final project for my university programming lab in C course.

Receives an assembly file through the command line interface, and outputs an object file and if needed ext and ent files.

The assembly language used in here was created specifically for the assembler project and is not meant to be linked or run.

Made on ubuntu x64, tested successfully on ubuntu x32 and windows 10 x64.

<hr/>
<span id=a2> <h2> Building </h2> </span> 

Included in the project are source files and a makefile.

To build on Linux simply run the command "make" in the kernel while in the project directory.

And then type ./assembler file1 file2 file3 ... This will start the processing of each file.

<hr/>
<span id=a3> <h2> Running </h2> </span> 

Included in the project are sample assembly (.asm) files with expected output in a subfolder.

### Example:

#### ./assembler sample1.asm

After running *program_path*/Assembler    *sample_path*/sample1.asm,
You'll receive the following three files in the same directory as the sample1.asm file:

#### sample1.ob

#### sample1.ext

#### sample1.ent

<hr/>
<span id=a4> <h2> Assembly Language </h2> </span> 

Operands used in the language: 
* Registers ("Reg" in the param list), all of whom are presumed to have 10-bit width. There are 8 available registers (0 - 8), and are denoted with a '$', ie: "$1", "$5", etc.
* Immediate Integers ("Int_X" in the param list), the assumed size of the param is denoted in bits by the X in the type.
* Labels ("Lbl" in the param list), whom can be user defiened or external. label names must begin with a letter and contain only alpha-numerical characters. Labels can be no longer than 9 characters. Labels are unique and can't be redefined. Any label name definition must be followed immediatly by a colon, followed by any legal operation or directive and its parameters. for example: _label-name_**:** *command* *param-list*
*  Strings ("Str" in the param list), denoted by intial and terminating quotation marks, with free text contained betwen. 

Types of statements available in the language:
* Operations, which are encoded instructions of code given to the machine to execute at run-time. The operation can optionally begin with a label definition.
* Directives, which instruct the assembler what is must do in regards to the code in the assembly file, creating relevant memory and file assignments before run. The operation can optionally begin with a label definition.
* Both of the above statements share the syntax: [_label_:] *command-name* *param1*, *param2*, ...
* Comments, which can only be written as a whole line (not from the middle), and begin with the character ';'. Example: ;this is a comment
* All line statements may not exceed 80 characters.
