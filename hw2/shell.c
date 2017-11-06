// ----------------------------------------------
// These are the only libraries that can be
// used. Under no circumstances can additional
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {

	if (strcmp(line, "") == 0) {
		p_cmd -> path = NULL;
		p_cmd -> argv = NULL;
		p_cmd -> argc = 0;
	}
	else {
		int count = 0;
		//white space to separate the commands from the arguments
		char* tokenizer = strtok(line, " ");
		//array of strings
		char** token = malloc(100 * sizeof(char*));
		while(tokenizer != NULL) {
			token[count] = tokenizer;
			count++;
			tokenizer = strtok(NULL, " ");
		}
		token[count + 1] = NULL;

		for(int i = 0; valid_builtin_commands[i] != NULL; i++) {

			if (strcmp(token[0], valid_builtin_commands[i]) == 0) {
				p_cmd -> path = token[0];
				p_cmd -> argv = token;
				p_cmd -> argc = count;
			}
		}

		if (find_fullpath(token[0], p_cmd) == TRUE) {
			p_cmd -> path = NULL;
			p_cmd -> argv = NULL;
			p_cmd -> argc = -1;
		}
		else {
			p_cmd -> argv = token;
			p_cmd -> argc = count;
		}

	} //close else


	// ----------------------------------------
	// TODO: you fully implement this function

} // end parse function


int execute( command_t* p_cmd ) {
	int status;
	char fullpath[1000];
	if(p_cmd -> argc > 0)
	{
		if(fork() == 0)
		{
			find_fullpath(fullpath, p_cmd);
			execv(fullpath, p_cmd->argv);
			exit(1);
		}
		wait(&status);
	}
	else if (p_cmd -> argc == -1)
	{
		perror("command is not found");
	}
	return wait(&status);


	// ----------------------------------------
	// TODO: you fully implement this function

	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed
	// to reflect the correct return
	// value described in the header file



} // end execute function


int find_fullpath( char* command_name, command_t* p_cmd ) {


	char* path = getenv( "PATH" );
	printf("PATH = %s\n", path);
	char* path_to_variable = strtok(path, ":");

	char* dir_or_file = (char*) malloc(sizeof(char) * strlen(path));
	p_cmd -> path = (char*) malloc(sizeof(char) * strlen(path));

	int boolean = 0;

	while(boolean == 0 && path_to_variable != NULL){
		struct stat buffer;
		int exists;
		strcpy(dir_or_file, path_to_variable);
		printf("(line 65ish) path_to_var = %s\n", dir_or_file);
		strcat(dir_or_file, "/");
		strcat(dir_or_file, command_name);
		exists = stat(dir_or_file, &buffer);
		if (exists == 0) {
			p_cmd -> path = strcpy(p_cmd -> path, dir_or_file);
			boolean = 1;
			return TRUE;
		}
		path_to_variable = strtok(NULL, ":");

	}
	if (boolean != 1) {
		p_cmd -> path = strcpy(p_cmd -> path, dir_or_file);
		return FALSE;
	}
	else {
		return FALSE;
	}


	// ----------------------------------------
	// TODO: you fully implement this function


	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed
	// to reflect the correct return
	// value described in the header file

} // end find_fullpath function


int is_builtin( command_t* p_cmd ) {

	int cnt = 0;

	while ( valid_builtin_commands[cnt] != NULL ) {

		if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

			return TRUE;

		}

		cnt++;

	}

	return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

	// only builtin command is cd

	if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

	struct stat buff;
	int status = ERROR;

	if ( p_cmd->argc == 1 ) {

		// -----------------------
		// cd with no arg
		// -----------------------
		// change working directory to that
		// specified in HOME environmental
		// variable

		status = chdir( getenv("HOME") );

	} else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


		// -----------------------
		// cd with one arg
		// -----------------------
		// only perform this operation if the requested
		// folder exists

		status = chdir( p_cmd->argv[1] );

	}

	return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

	int i=0;

	while ( p_cmd->argv[i] != NULL ) {
		free( p_cmd->argv[i] );
		i++;
	}

	free( p_cmd->argv );
	free( p_cmd->path );

} // end cleanup function


int equals( char* str1, const char* str2 ) {

	// First check length

	int len[] = {0,0};

	char* b_str1 = str1;
	const char* b_str2 = str2;

	while( (*str1) != '\0' ) {
		len[0]++;
		str1++;
	}

	while( (*str2) != '\0' ) {
		len[1]++;
		str2++;
	}

	if ( len[0] != len[1] ) {

		return FALSE;

	} else {

		while ( (*b_str1) != '\0' ) {

			// don't care about case (you did not have to perform
			// this operation in your solution

			if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

				return FALSE;

			}

			b_str1++;
			b_str2++;

		}

	}

	return TRUE;


} // end compare function definition
