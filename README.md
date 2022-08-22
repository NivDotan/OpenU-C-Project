# About
This project is an assignment in a C programming course. The purpose of this project is to build an assembler with macros for 16-instructions asssembly language, for an imaginary 10-bit CPU and 8 bits of memory size that encode in a special 32 base as an output. This project was done by @NivDotan

# Modules
1. main - main function definition, argv & argc processing, single/multi file processing.
2. PreAssembly - contain a function for getting the macro in the file and paste them in the output file.
3. PreAssemblyFunctions - contain functions for checking and getting the macros.
4. AssemblyFirstStep - contain a function for getting a single line and checking for errors.
5. FirstStepFunctions - contain functions for processing a line.
6. CommonFunctions - contain common functions for processing operands and instructions.
7. AssemblySecondStep - contain a function for getting a single line and contains methods for writing output files: *.ob, *.ext and *.ent.
8. SecondStepFunctions - contain functions for processing for processing operands and instructions and replacing them in a special base 32.

# Use
./assembler file1 file2 file3 ... Will start the processing of each file.
