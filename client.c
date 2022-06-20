#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

/* ./client mgureltol@umass.edu 27993 128.119.243.147 */



#define MAX_ARGS		10
#define NUM1_OP_NUM2_MSG_LEN	30

/*...............................................................................*/
void remove_EndOfLine(char *line)
{
    int i = 0;

    while (line[i] != '\n')
    {
        i++;
    }
    line[i] = '\0';
}
/*...............................................................................*/
void Process_Massage(char *massage_args[MAX_ARGS], char* massage)
{
    int i = 0;
    massage_args[i] = strtok(massage, " ");

    if (massage_args[0] == NULL)
    {
        printf("No Command...\n");
    }

    while (massage_args[i] != NULL)
    {
        i++;
        massage_args[i] = strtok(NULL, " ");
    }
}
/*...............................................................................*/

int Do_Math(int NUM_1,char OPeration ,int NUM_2)
{

	switch (OPeration)
	{
		case ('+'):
			return (NUM_1+NUM_2);
			break;

		case ('-'):
			return (NUM_1-NUM_2);
			break;
	
		case ('*'):
			return (NUM_1*NUM_2);
			break;

		case ('/'):
			return (NUM_1/NUM_2);
			break;
	}

}







int main(int argc, char* argv[])
{
	struct sockaddr_in Client_Struct;
	char msg[256] = "cs230 HELLO ";
	char newline[2] = {10,0};
	int Client_Socket;
	int Number_Of_Received_bytes;
	char *massage_args[MAX_ARGS];
	int NUM_1, NUM_2, Result;
	char Operation;
	


	Client_Socket = socket(AF_INET, SOCK_STREAM , 0);
	

	strcat(msg, argv[1]);
	strcat(msg, newline);
	Client_Struct.sin_family = AF_INET;
	Client_Struct.sin_addr.s_addr = inet_addr(argv[3]);
	Client_Struct.sin_port = htons(atoi(argv[2]));
	//printf("massage after concatinate with the email: \n --> %s \n", msg);
	//printf("........\n");	
	//printf("IP address : %s \n", argv[3]);
	//printf("Port number : %s \n", argv[2]);
	//printf("mail : %s \n", argv[1]);
	//printf("........\n");

	connect(Client_Socket , (struct sockaddr *) &Client_Struct , sizeof(Client_Struct));


	while(1)
	{	
		
		send(Client_Socket , msg , strlen(msg) , 0);
		//printf("msg send to the server : \n --> %s\n", msg);
		bzero((char*)&msg , sizeof(msg));
		
		Number_Of_Received_bytes = recv(Client_Socket , msg , sizeof(msg) , 0);

		if( (Number_Of_Received_bytes > 0) && ( Number_Of_Received_bytes < NUM1_OP_NUM2_MSG_LEN) ) 
		{
			//printf("msg reveived from the server : \n --> %d byts : %s \n",Number_Of_Received_bytes, msg);
			Process_Massage(massage_args , msg);
			//printf("NUM 1 : %s \n",massage_args[2]);
			//printf("OP : %s \n",massage_args[3]);
			//printf("NUM 2 : %s \n",massage_args[4]);
			NUM_1 = atoi(massage_args[2]);
			NUM_2 = atoi(massage_args[4]);
			Operation = massage_args[3][0];
			//printf("NUM 1 : %d \n",NUM_1);
			//printf("OP : %c \n",Operation);
			//printf("NUM 2 : %d \n",NUM_2);
			Result = Do_Math(NUM_1, Operation , NUM_2);
			//printf("Result : %d \n", Result);
			sprintf(msg, "cs230 %d\n", Result);
		}
		
		else if(Number_Of_Received_bytes > NUM1_OP_NUM2_MSG_LEN)
		{
			printf("\ncaprured flag : %s\n\n", msg);
			break;
		}

		else if( Number_Of_Received_bytes < 0)
		{
			printf("connection lost....\n");
			break;
		}
		
		
	
		//sleep(1);

	}

	close(Client_Socket);

}
	

