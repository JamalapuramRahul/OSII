/* Name : Rahul Jamalapuram
email: rahul.jamalapuram@okstate.edu
cwid: A20303740
*/
#include<stdio.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

void *messagesharing(void *);                   //function declaration for the thread function this will be called when the thread is created

char message[2000];                             // variable to hold the data    

char output[2000];                              //variable to hold the data

pthread_mutex_t mutex;                          //The mutex locks for accessing the critical section

int threadNumber=0;                             //variable to control number of threads

int main(){
  int server_socket;                            //initialising the server socket
  int client_socket;                            //initialising the client socket
  struct sockaddr_in server_address;            //this is the structure to hold the server information        
  pthread_t threads[3];                         //number of threads

  struct sockaddr_storage ServerInfo;           //this is to store the server data
  socklen_t address_size;                       //size of the address which is to be binded

  server_socket = socket(AF_INET, SOCK_STREAM, 0); //getting the server socket

  server_address.sin_family = AF_INET;             
  server_address.sin_port = htons(1557);            //port number to connect over the network
  server_address.sin_addr.s_addr = inet_addr("10.203.72.24");       //IP of the networn can be found out using ip route command , the IP after the src is the IP of the network

  memset(server_address.sin_zero, '\0', sizeof server_address.sin_zero);  //setting the memory for the address and assigning all the parity bits as blank spaces.

  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)); //binding the socket to the server to create a path for clients connection

  if(listen(server_socket,3)==0)                //after the binding listening to the port for any requests, accept no more than 3 requests
    printf("Listening to the port:\n");
  else
    perror("Could not listen:\n");

    while(1){
        
        address_size = sizeof ServerInfo;
        client_socket = accept(server_socket, (struct sockaddr *) &ServerInfo, &address_size);      //getiing the request from the client 
	printf("Connection recieved");
	
        if( pthread_create(&threads[threadNumber], NULL, messagesharing, &client_socket) != 0 ){    //creating the thread for each successful binding , inour case only 3
           printf("Failed to create thread\n");
           exit(0);
        }

	pthread_join(threads[threadNumber],NULL);
        threadNumber+=1;

	if(threadNumber>=3){
		threadNumber=0;
	}

	//printf("%s",output);
        
    }
  return 0;
}

//Thread method called for threads execution
void * messagesharing(void * client_socket){
  
  FILE *fp;                                         //creating a file pointer
  int socket = *((int *)client_socket);             //getting the socket from the argument
  recv(socket ,message , 2000 , 0);                 //recieving the data from the inialised socket

  pthread_mutex_lock(&mutex);                       //using lock for accessing the critical section and this is done to write the data to file and not to corrupt the writes
 	
  char c_message[2000];                             //creating a temporary variable for getting the messages or data from socket

  strcpy(c_message,message);
  strcat(output,c_message);
  
  fp=fopen("/home/rjamala/msg.txt","w+");           //opening a file for enabling the writes in W+ mode(write and read) change the path accordingly, if file isn't present this will create a new file
  fprintf(fp,"%s",output);                          //writing to the file
  fclose(fp);
  	
  printf("contents of the file: %s \n",output);
  fflush(stdout);
  
  pthread_mutex_unlock(&mutex);                     //releasing the lock so that other threads can access
  sleep(2);                                         //thread being sent to sleep mode as per the instructions
  
  send(socket,output,2000,0);                       //sending the data to the client
  close(socket);
  
  pthread_exit(NULL);                               //exit the thread
 }
