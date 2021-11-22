#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

char emptystr[2000];


int word_count(char string[])
{
  //We start with first word unless we have a empty string then we have no words
  int count;
  if(*string!='\0'){
    count=1;
}
  else{
    count=0;
    return 0;
  }
  //while we dont reach the end of the string
  while(*string!='\0'){
    //if we detect a whitespace
    if(*string==' '){
      //get previous character
      string--;
      // If previous character is not a space we increase the count
      // Otherwise we dont since we already counted a word
      if(*string!=' '){
        count++;
      }
      //return pointer to current character
      string++;
    }
    // set pointer to next character
    string++;
  }
  return count;
}


int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	
	char client_message[2000];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 9888 );
	
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	int run = 1, constat = 0, count=0;
	while (run==1){
		
		count++;
		if(count > 0)
			run = 0; //just for dev purposes so I don t have to unbind port every time
	
		//Listen
		listen(socket_desc , 3);
	
		//Accept and incoming connection
		puts("Waiting for incoming connections...");
		c = sizeof(struct sockaddr_in);
	
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			perror("accept failed");
			return 1;
		}
		puts("Connection accepted");
		constat = 1;
		
		char sstatm[] = "[ss]";
		char sstat[] = "$hell";
		char ilgcmd[] = " illegal command\0";
		char hlpcmd[] = "help";
		char hlpmsg[] = "there is no help here also you re in hell";
	
		while(constat == 1){
		
			strcpy(client_message,emptystr);
			
			//Receive a message from client
			//while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
			if((read_size = recv(client_sock , client_message , 2000 , 0)) > 0)
			{
				client_message[read_size] = '\0';
				
				//printf("%s",sstatm);
				//printf(" len=%d\n",strlen(sstatm));
				//printf("%s",client_message);
				//printf(" len=%d\n",strlen(client_message));
				
				if(strcmp(client_message,sstatm)==0){
				//if(client_message[0] == '[' && client_message[3]== ']'){
					write(client_sock , sstat , strlen(sstat));
					//printf("sstatm=%s,len=%d,client_message=%s,len=%s\n",sstatm,strlen(sstatm),client_message,strlen(client_message));
				}
				else if(strcmp(client_message,hlpcmd)==0){
				
					write(client_sock, hlpmsg, strlen(hlpmsg));
				
				}
				else{
					
					int wc = word_count(client_message);
					printf("word count = %d",wc);
					
					
					
					//Send the message back to client
					strcat(client_message,ilgcmd);
					write(client_sock , client_message , read_size+16);
				}
			}
	
			if(read_size == 0)
			{
				puts("Client disconnected");
				fflush(stdout);
				constat = 0;
			}
			else if(read_size == -1)
			{
				perror("recv failed");
				constat = 0;
			}
		}
		
	}
	return 0;
}
