/* 
GCCIT - A simple program to compile and run your C program without 
the need to fluff about. Simply run gccit <my_program.c>

Author: Vincent Bavitz
Date Began: 29 November 2017

Future improvements:
	Allow the program to be run from any terminal directory using absolute and relative paths
	Add more options ontop of -s [sudo]
	Fix the issue of program not receiving stdin from commands like:
		gccit myprog < sometext.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#define MIN_ARGS 2
#define MAX_CMD_OPTS_LEN 80

#define TRUE 1
#define FALSE 0


void compile_prog(int argc, char* argv[], char* file_name);
void build_opts(int argc, char* argv[], char** cmd_options);
void run_prog(int argc, char* argv[], char* file_name);



int main(int argc, char* argv[]){
	int offset = 1;
	
	if (strcmp(argv[1], "-s") == 0){
		offset++;
	}
	
	int suffix_offset;
	char* file_name = argv[offset]; /* my_prog */
	
	/* remove ".c" */
	suffix_offset = strlen(argv[offset]);
	file_name[suffix_offset - 2] = '\0';
	
	
	if (argc < MIN_ARGS){
		/* no arguments given */
		printf("Please include at least one argument for gccIT\n");
		exit(EXIT_FAILURE);
	}
	
	
	printf("Please ensure that your terminal is open to the same directory "
		   "as your file.\n");
	
	
	compile_prog(argc, argv, file_name);
	run_prog(argc, argv, file_name);
	
	/* take argv[0] and run gcc on it
	then take all other arguments and store them
	
	with stores argumetns you can append them to the call to your program.
	
	Thus if your program requires inputs like:
		aes@rosa: ~: myprog -x 50 -J 100 "jaybird"
		Then you can run gccit myprog.c -x 50 -J 100 "jaybird"
		
		Which will first take gccit -ansi -Wall -o myprog myprog.c
		Then run ./myprog -x 50 -J 100 "jaybird"	
	*/
	

	
	/* optionally output info like no of lines in code, etc? */

	

	
	return 0;	
}

void compile_prog(int argc, char* argv[], char* file_name){
	char* command;
	char* space = " ";
	char* cmd_prefix = "gcc -ansi -Wall -o ";
	char* file_ext = ".c";	


	
	/* peice parts of command together */
	command = (char*)malloc(strlen(cmd_prefix) + 
						 strlen(file_name) + 
						 strlen(space) + 
						 strlen(file_name) + 
						 strlen(file_ext) + 
						 1);
	assert(command != NULL);
	
	
	command[0] = '\0';
	strcat(command, cmd_prefix);
	strcat(command, file_name);
	strcat(command, space);
	strcat(command, file_name);
	strcat(command, file_ext);
	

	system(command);
	
}

void build_opts(int argc, char* argv[], char** cmd_options){
	/*
	Options for running the program itself; eg: "-F 20 -x 12" 
	Places options string, prepended with space, into cmd_options	
	
	int* sudo should be the first option, as 
	gccit -s myprog.c [options]
	*/
	
	int i = 2;
	char* space = " ";
	int len_count = 0; /* length of program opts string < MAX_CMD_OPTS_LEN */

	*cmd_options = (char*)malloc(sizeof(char) * MAX_CMD_OPTS_LEN + 1);
	assert(*cmd_options != NULL);
	
	/* advance i if sudo option is given */
	if (strcmp(argv[1], "-s") == 0){
		i++;	
	}
	
	(*cmd_options)[0] = ' ';
	(*cmd_options)[1] = '\0';
	for (i = i; i < argc; i++){
		if (len_count >= MAX_CMD_OPTS_LEN){
			(*cmd_options)[MAX_CMD_OPTS_LEN - 1] = '\0';	
			break;
		}
		
		strcat((*cmd_options), argv[i]);
		strcat((*cmd_options), space);
		
		len_count += strlen(argv[i]);
	}
	
	printf("OPTIONS: %s\n", *cmd_options);
	
}

void run_prog(int argc, char* argv[], char* file_name){
	/* 
	Runs the program with specified options
	*/
	
	char* run_cmd_prefix = "./";
	char* command;
	char* cmd_options;
	
	/* optional to run your program with sudo */
	if (strcmp(argv[1], "-s") == 0){
		run_cmd_prefix = "sudo ./";
	}
	else{
		run_cmd_prefix = "./";
	}
	
	
	build_opts(argc, argv, &cmd_options);
	
	command = (char*)malloc(strlen(run_cmd_prefix) + 
						 strlen(file_name) + 
						 strlen(cmd_options) +
						 1);
	assert(command != NULL);
	
	strcat(command, run_cmd_prefix);
	strcat(command, file_name);
	
	/* if options given */
	if (argc > MIN_ARGS){
		strcat(command, cmd_options);
	}

	system(command);
		
}








