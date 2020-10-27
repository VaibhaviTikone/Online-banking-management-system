#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include "tools.h"


int main(){
	struct sockaddr_in server;
	int sd,msgLength;
	char buff[50];
	char result;

	//connection establishment
	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	//server.sin_addr.s_addr=inet_addr("172.16.81.54"); //other machine
	//server.sin_addr.s_addr=INADDR_ANY; //same machine
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); //same machine
	server.sin_port=htons(8080);
	connect(sd,(struct sockaddr *)&server,sizeof(server));
	while(1){
	printf("\e[1;1H\e[2J");
	printf("\n\n\n\t\t\t*************************************\n");
	printf("\t\t\t***   WELCOME TO BANKING SYSTEM   ***\n");
	printf("\t\t\t*************************************\n\n\n");
	printf("1. Sign up\n2. Sign in\n3. Exit\n\nchoice:");
	//printf("*************************************\n");
	int choice;
	scanf("%d",&choice);

	switch(choice)
	{
		case 1:
			create_mode();
			break;
		case 2:
			chooseOption(sd);
			showMenu(sd);
			break;
		case 3:
			write(1,"\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n",sizeof("\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n"));
			exit(0);
			break;
		default:
			printf("Please enter valid choice.\n");
			break;
	}
}
	close(sd);
	return 0;
}
