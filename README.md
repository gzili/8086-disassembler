# 8086 Disassembler
An x86 disassembler which supports the original Intel 8086/8088 instructions.
This disassembler was written in x86 Assembly language during the first semester of Software Engineering course in Vilnius University (VU).

## Features
* Supports every instruction from the original Intel 8086/8088 instruction set
* Small executable file size (less than 6KB)
* Fully-buffered file I/O
* Reads from and writes to files whose names are give as command-line arguments
* Prints helpful error messages for various different I/O errors

## Todos
 - [ ] Output the results in a more-readable DOS Debug format
 - [ ] Translate error messages from Lithuanian language (original) to English
 - [ ] Add comments in the code
 - [ ] Print a usage message when command-line arguments are supplied incorrectly
 
 ## Usage
 This program was written for Turbo Assembler (TASM) and uses TASM specific features, so it should be Assembled with TASM.
 
 The Assembled executable should be run with two positional arguments: input file name and output file name:
 `disasm <input_file_name> <output_file_name>`
 
 For example, to disassemble an executable `test.com` and output the disassembly result into `test.asm`, run:
 `disasm test.com test.asm`
 
 # Notes
 * This program was intended to be used with COM files, hence the starting 0x0100 offset.
 * The author Assembled this program with Turbo Assembler 3.1 and linked with Turbo Link 3.0. All tests were run in a DOSBOX 0.74-3 environment.
 
