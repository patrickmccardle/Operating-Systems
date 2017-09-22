
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2017
// server.h header file
// Homework 1
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and
// then binds it to the specified port_number
// for incoming client connections
//
//
// Arguments:	port_number = port number the server
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;

    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
//
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {
	// printf("in accept client, 77\n");

	int exit_status = OK;

	int client_socket_fd = -1;

	socklen_t client_length;

	struct sockaddr_in client_address;

	char request[512];

	client_length = sizeof( client_address );

    client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );

	// -------------------------------------
	// TODO:
	// -------------------------------------
	// Modify code to fork a child process
	// -------------------------------------


	//Forking
	// printf("forking, 100\n");

	if (fork() == 0) {
		// printf("inside fork, 104\n");


		if ( client_socket_fd >= 0 ) {
			// printf("client socket fd >= 0, 104\n");


			bzero( request, 512 );

			read( client_socket_fd, request, 511 );

			if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );




			 char* entity_body = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr></table></body></html>";

		 	char response[512];

		 	char* reqDuplicate = strdup(request);
			// 	char* reqDuplicate2 = strdup(request);

			printf("if GET request, 118\n");
		 	if (strcmp(strtok(reqDuplicate," "), "GET") == 0){
				// printf("120\n" );
				// printf("line 128 Dup\n, %s\n", reqDuplicate);
				// printf("line 128 Dup2\n, %s\n", reqDuplicate2);
		 		int count = 0;
		 		char* stringValues = (strtok(NULL, " "));
				// printf("line 134\n%s\n", stringValues);

		 		char* newString = stringValues+= 2;
				// printf("line 137\n%s\n", newString);
		 		char* getTokens = strtok (newString, "&=");
				// printf("line 139\n%s\n", getTokens);
		 		char get_entity[] = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";
		 		while (getTokens != NULL){
		 			if (count % 2 == 0){
		 				strcat(get_entity, "<tr><td><b>");
						// printf("line 144\n%s\n", get_entity);
		 				strcat(get_entity,getTokens);
						// printf("line 146\n%s\n", get_entity);
		 				strcat(get_entity, "</b></td>");
						// printf("line 148\n%s\n", get_entity);
		 				getTokens = strtok(NULL,"&=");
						// printf("line 147\n%s\n", getTokens);

		 			}
		 			else {
		 				strcat(get_entity, "<td>");
		 			strcat(get_entity, getTokens);
		 			strcat(get_entity, "</td>");
		 			getTokens = strtok (NULL, "&=");
		 			strcat(get_entity, "</tr>");
		 			}
		 			count++;
		 		}
		 		strcat(get_entity, "</table></body></html>");
		 		sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( get_entity ), get_entity );

		 	}
			// printf("almost inside post");
			if (strcmp(strtok(reqDuplicate," "), "POST") == 0){
				// printf("inside post");
				char post_entity[] = "<html><body><body><h2>CSIS 340 (Advanced Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";
				char* postTokens = (strtok(request, "\n"));
				// printf("171 postTokens %s\n", postTokens);
				int boolean = 0;

				while (boolean == 0){
					postTokens = strtok(NULL, "\n");
					// printf("176 postTokens %s\n", postTokens);
					if (strlen(postTokens) == 1){
						postTokens = strtok(NULL, "\n");
						// printf("179 postTokens %s\n", postTokens);
						boolean = 1;
					}

				}
				postTokens = strtok(postTokens,"&=");

				int count = 0;
				printf("\npost tokens: 2 %s\n", postTokens);
				while(postTokens != NULL){
					if(count % 2 == 0){
						strcat(post_entity, "<tr><td><b>");
						// printf("190 post entity %s\n", post_entity);
						strcat(post_entity, postTokens);
						// printf("193 post entity %s\n", post_entity);
						strcat(post_entity, "</b></td>");
						// printf("195 post entity %s\n", post_entity);
						postTokens = strtok(NULL,"&=");

					}
					else{
						strcat(post_entity,"<td>");
						strcat(post_entity, postTokens);
						strcat(post_entity,"</td>");
						postTokens = (strtok(NULL, "&="));
						strcat(post_entity, "</tr>");
					}
					count++;
				}

				strcat(post_entity, "</table><body></HTML>");
				sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( post_entity ), post_entity );

			}




			if ( DEBUG ) printf( "%s\n", response );

	    	write( client_socket_fd, response, strlen( response ) );

			close( client_socket_fd );


		} else {

			exit_status = FAIL;

		}


	if ( DEBUG ) printf("Exit status = %d\n", exit_status );
	exit(0);
}

else {
	close (client_socket_fd);
}

	return exit_status;

} // end accept_client function
