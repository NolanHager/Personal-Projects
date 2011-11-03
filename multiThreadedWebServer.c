//Name: Jeffrey Nolan Hager
//Project: Multi Threaded Web Server


//====== Include files =================================================
#include <stdio.h>          // for printf()
#include <stdlib.h>         // for exit()
#include <string.h>         // for strcpy() and strlen()
#include <fcntl.h>          // for file i/o constants
#include <sys/stat.h>       // for file i/o constants  
#include <signal.h>			//for the signal
#include <pthread.h>		//threads
#include <sched.h>			//
#include <semaphore.h>		//for the semaphore that works for threads

// FOR BSD UNIX/LINUX  ================================================= 
#include <sys/types.h>      //   
#include <netinet/in.h>     //   
#include <sys/socket.h>     // for socket system calls  
#include <arpa/inet.h>      // for socket system calls (bind)    
//end BSD UNIX/LINUX 

//====== HTTP response messages =========================================
#define OK_IMAGE    "HTTP/1.0 200 OK\nContent-Type:image/gif\n\n"
#define OK_TEXT     "HTTP/1.0 200 OK\nContent-Type:text/html\n\n"
#define NOTOK_404   "HTTP/1.0 404 Not Found\nContent-Type:text/html\n\n"
#define MESS_404    "<html><body><h1>FILE NOT FOUND</h1></body></html>"

//=======Define Global Variables==============================================
#define BUF_SIZE            1024     // buffer size in bytes
#define PORT_NUM            6025     // Port number for a Web server (TCP 5080)
#define PEND_CONNECTIONS     100     // pending connections to hold 
#define TRUE                   1
#define FALSE				   0

#define NTHREADS			  50     // Number of child threads         
#define NUM_LOOPS			  10     // Number of local loops          
#define SCHED_INTVL			   5     // thread scheduling interval   
#define IM_HIGH				  10

unsigned int			client_s;				// Client socket descriptor

//=======pThreads Variables==================================================
sem_t thread_sem[NTHREADS];  
int   nextThread;  
int	  currentThreads;	
int   can_run;
int   i_stopped[NTHREADS];  


//=======pThread Definition==================================================
void *my_thread(void * arg){
	char           in_buf[BUF_SIZE];           // Input buffer for GET resquest
	char           out_buf[BUF_SIZE];          // Output buffer for HTML response
	char           *file_name;                 // File name
	unsigned int   fh;                         // File handle (file descriptor)
	unsigned int   buf_len;                    // Buffer length for file reads
	unsigned int   retcode;                    // Return code
	int			   priority;				   // Used when testing for priority

	unsigned int   myClient_s;
	myClient_s =  *(unsigned int*) arg;

		//receive the first HTTP request (HTTP GET)
		retcode = recv(myClient_s, in_buf, BUF_SIZE, 0);

		//if receive error
		if (retcode < 0){   
			printf("recv error detected ...\n"); 
		}
		else{    
			strtok(in_buf, " ");
			file_name = strtok(NULL, " ");

			//High priority===========================================================//

			if(strcmp(&file_name[1], "test_00.jpg") == 0){
				priority = pthread_setschedprio(pthread_self(), IM_HIGH);
				if(priority){
					printf("\n this is really HIGH...priority \n");
				}
				else{
					printf("\n Priority no work right \n");
				}
			}

			//========================================================================//



			fh = open(&file_name[1], O_RDONLY, S_IREAD | S_IWRITE);
			if (fh == -1){
				printf("File %s not found - sending an HTTP 404 \n", &file_name[1]);
				strcpy(out_buf, NOTOK_404);
				send(myClient_s, out_buf, strlen(out_buf), 0);
				strcpy(out_buf, MESS_404);
				send(myClient_s, out_buf, strlen(out_buf), 0);
			}
			else{
				printf("File %s is being sent \n", &file_name[1]);
				if ((strstr(file_name, ".jpg") != NULL)||(strstr(file_name, ".gif") != NULL)) {
					strcpy(out_buf, OK_IMAGE); 
				}
				else{ 
					strcpy(out_buf, OK_TEXT); 
				}

				send(myClient_s, out_buf, strlen(out_buf), 0);

				buf_len = 1;
				int count;
				count = 0;
				while (buf_len > 0){
					buf_len = read(fh, out_buf, BUF_SIZE);
					if (buf_len > 0){ 
						send(myClient_s, out_buf, buf_len, 0); 
						count = count +1;
						if(count = 500){
							printf("%d bytes transferred..\n",buf_len);
							count = 0;
						}//end count if
					}//end buf_len >0
				}//end while()

				close(fh);			// close the file
				close(myClient_s);	// close the client connection
				pthread_exit(NULL);	//closing the thread

			}// end fh == -1 if
		}// end recieve if

	return arg; 

}//end pThread

//===== Main program ========================================================
int main(int argc, char *argv[])
{
	//local variables for socket connection 
	unsigned int			server_s;				// Server socket descriptor
	struct sockaddr_in		server_addr;			// Server Internet address
	struct sockaddr_in		client_addr;			// Client Internet address
	struct in_addr			client_ip_addr;			// Client IP address
	int						addr_len;				// Internet address length
	unsigned int			myChild;				//holds arguments for child
	pthread_attr_t			attr;
	pthread_t				threads;

	int						i;
	int						all_threads_stopped; 
	pthread_t				threadID;

	// Initialize thread Semaphoe and thread status
	for (i = 0; i < NTHREADS; i++){
		sem_init(&thread_sem[i], 0, 0);		// initialzie thread semapahore    
		i_stopped[i] = FALSE;				//initialize the status of each thread  
	}


	// create a new socket
	server_s = socket(AF_INET, SOCK_STREAM, 0);

	//====fill-in address information, and then bind it ==================
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));

	//=================Listen for connections and then accept=============
	listen(server_s, PEND_CONNECTIONS);


	//================Main Server Loop====================================
	pthread_attr_init(&attr);			// initialize the pthread attr 

	while(TRUE){
		printf("\n my server is ready ...\n");  

		// wait for the next client to arrive. This is the blocking method
		addr_len = sizeof(client_addr);
		client_s = accept(server_s, (struct sockaddr *)&client_addr, &addr_len);
		myChild = client_s;
  
		if (client_s == -1){
		  perror("ERROR - Unable to create socket \n");
		  continue;
		}
		else{
				//============Creating a child thread================================
				pthread_create (					// Create a child thread        
						   &threadID,		// Thread ID (system assigned)  
						   &attr,					// Default thread attributes    
						   my_thread,				// Thread routine               
						   &myChild);		// Arguments to be passed   

		}// end if-else statement client_s == FALSE
	}//server while loop
   
	 //To make sure this "main" returns an integer
	close (server_s);  // close the primary socket
	return (TRUE);        // return code from "main"
}