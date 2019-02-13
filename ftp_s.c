//Inro - 13/02/2019

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>	
#include <sys/types.h>
#include <sys/socket.h>						
#include <netinet/in.h>
#include <arpa/inet.h>

// Server PORT
#define PSC 1024
#define PSF 1025
#define PRAND 1026

// Client PORT
#define PCC 2024
#define PCF 2025
#define SIZE 100

char str[SIZE];
FILE *fp;
int cd_flag=0;

void fun(int newcmd, int newrand)
{
	//printf("\n\n\n\n\n");

	bzero(str, SIZE); 
	read(newcmd, str, sizeof(str));
	str[strlen(str)-1] = '\0';
	//printf("%s, %ld\n",str,strlen(str));

	if(!(strcmp(str,"inro")))
	{
		bzero(str, SIZE);
		strcpy(str,"i");
		write(newcmd, str, sizeof(str));
	}
	else
	{
		bzero(str, SIZE);
		strcpy(str,"n");
		write(newcmd, str, sizeof(str));
	}

	bzero(str, SIZE); 
	read(newcmd, str, sizeof(str));
	str[strlen(str)-1] = '\0';
	//printf("%s, %ld\n",str,strlen(str));

	if(!(strcmp(str,"joker")))
	{
		bzero(str, SIZE);
		strcpy(str,"r");
		write(newcmd, str, sizeof(str));
	}
	else
	{
		bzero(str, SIZE);
		strcpy(str,"o");
		write(newcmd, str, sizeof(str));
	}

	//////////////////////////////////////////
	int i,k,m,flag;
    char ch,file[SIZE],com[SIZE];

	//int s = send(newcmd, &PRAND, sizeof(int),0);

	while(1)
	{
		bzero(str, SIZE); 
		read(newcmd, str, sizeof(str));

		k = 0;
    	m = 0;
    	flag = 0;
    	for(i=0;i<strlen(str);i++)
    	{
    		if(flag == 1)
    			file[k++] = str[i];
    		if(str[i] == ' ')
    			flag = 1;
    		if(flag == 0)
    			com[m++] = str[i];
    	}
    	file[k++] = '\0';
    	com[m++] = '\0';

		if(flag == 0)
		{
			if(!strcmp(com,"ls"))
			{
				system("ls>temp.txt");
				fp = fopen("temp.txt","r");
				while(ch=fgetc(fp))
 				{
 					send(newrand, &ch, sizeof(char),0);
 					if(ch == EOF)
 						break;
 				}
 				fclose(fp);
			}
			else if(!strcmp(com,"pwd"))
			{
				system("pwd>temp.txt");
				fp = fopen("temp.txt","r");
				while(ch=fgetc(fp))
 				{
 					send(newrand, &ch, sizeof(char),0);
 					if(ch == EOF)
 						break;
 				}
 				fclose(fp);
			}
		}
		else
		{
			if(!strcmp(com,"get"))
			{
				fp = fopen(file,"r");
 				if(fp == NULL)
 				{
 					bzero(str, SIZE);
					strcpy(str,"n");
					write(newrand, str, sizeof(str));
 					continue;
 				}
 				else
 				{
 					while(ch=fgetc(fp))
 					{
 						send(newrand, &ch, sizeof(char),0);
 						if(ch == EOF)
 							break;
 					}
 					fclose(fp);
 				}
			}
			else if(!strcmp(com,"put"))
			{
				fp = fopen(file,"w");
				while(1)
 				{
 					recv(newrand, &ch, sizeof(char),0);
 					if(ch == EOF)
 						break;
 					fprintf(fp,"%c",ch);
 				}
 				fclose(fp);

			}
			else if(!strcmp(com,"cd"))
			{
				if(cd_flag==1)
				{
					
					strcpy(str,"Error");
		  			write(newrand, str, sizeof(str));
					continue;
				}
				
				chdir(file);
				system("pwd>temppwd.txt");
				char string[20];
				FILE* fp=fopen("temppwd.txt","r");
				fscanf(fp,"%s",string);
				int count=0;
				for(int i=0;i<strlen(string);i++)
				{
					if(string[i]=='/')
						count++;
				}

				if(count == 2)
					cd_flag=1;
				

				bzero(str, sizeof(str));
				strcpy(str,"Directory is changed");
		  		write(newrand, str, sizeof(str));

			}
			else if(!strcmp(com,"mkdir"))
			{
				system(str);
				bzero(str, sizeof(str));
				strcpy(str,"New Directory is created");
		  		write(newrand, str, sizeof(str));
			}
		}
	}
}

/*
void fun(int newcmd, int newrand)
{
	char str[SIZE];

		while(1)
		{ 
		    bzero(str, SIZE); 
		    read(newcmd, str, sizeof(str));  
			printf("Client >> %s", str); 
			printf("Server : "); 
			bzero(str, SIZE); 
			fgets(str, sizeof(str), stdin);
			write(newrand, str, sizeof(str));  
		}		
}
*/

int main()
{
		struct sockaddr_in scmd, sdata, srand;
		char addr[20] = "127.0.0.10";

		//cmd
		bzero((char *)&scmd, sizeof(scmd));
		scmd.sin_family = AF_INET;
		if(!inet_pton(AF_INET,addr,&(scmd.sin_addr)))
		{
			puts("address conversion error");
			return 0;
		}
		//scmd.sin_addr.s_addr = inet_addr("10.32.5.249");
		//scmd.sin_addr.s_addr = INADDR_ANY;
		scmd.sin_port = htons(PSC);
		
		//file
		bzero((char *)&sdata, sizeof(sdata));
		sdata.sin_family = AF_INET;
		if(!inet_pton(AF_INET,addr,&(sdata.sin_addr)))
		{
			puts("address conversion error");
			return 0;
		}

		//sdata.sin_addr.s_addr = inet_addr("10.32.5.249");//INADDR_ANY;
		sdata.sin_port = htons(PSF);

		//rand
		bzero((char *)&srand, sizeof(srand));
		srand.sin_family = AF_INET;
		if(!inet_pton(AF_INET,addr,&(srand.sin_addr)))
		{
			puts("address conversion error");
			return 0;
		}

		//srand.sin_addr.s_addr = inet_addr("10.32.5.249");//INADDR_ANY;
		srand.sin_port = htons(PRAND);

		int cmd,data,rand;
		int b;

		/////////////////cmd///////////////////

		if((cmd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket for cmd");
			exit(0);
		}
		else
			printf("Socket is created for cmd...\n");

		b = bind(cmd, (struct sockaddr *)&scmd, sizeof(scmd));
		if(b < 0)
		{
			perror("Bind is not working for cmd");
			exit(0);
		}
		else
			printf("Socket is binded for cmd...\n");

		///////////////////data////////////////////

		if((data = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket for data");
			exit(0);
		}
		else
			printf("Socket is created for data...\n");

		b = bind(data, (struct sockaddr *)&sdata, sizeof(sdata));
		
		if(b < 0)
		{
			perror("Bind is not working for data");
			exit(0);
		}
		else
			printf("Socket is binded for data...\n");

		/////////////////rand//////////////////////

		if((rand = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket for rand");
			exit(0);
		}
		else
			printf("Socket is created for rand.....\n");

		b = bind(rand, (struct sockaddr *)&srand, sizeof(srand));
		
		if(b < 0)
		{
			perror("Bind is not working for rand");
			exit(0);
		}
		else
			printf("Socket is binded for rand.....\n");

		///////////////////////////////////////////

		///////////////////////////////////////////

		listen(cmd, 3);
		listen(data, 3);
		listen(rand, 3);

		int lcmd = sizeof(scmd);
		int ldata = sizeof(sdata);
		int lrand = sizeof(srand);

		int ndata,ncmd,nrand,pid;
		
		scmd.sin_port = htons(PCC);
		srand.sin_port = htons(PCF);

		while(1)
		{
			if((ncmd = accept(cmd, (struct sockaddr *)&scmd, &lcmd)) < 0)
			{
				perror("Not accepting cmd");
				exit(0);
			}
			else
				printf("cmd accepting.....\n");

			nrand = accept(rand, (struct sockaddr *)&srand, &lrand);
				
			if(nrand < 0)
			{
				perror("Not accepting rand");
				exit(0);
			}
			else
				printf("rand accepting.....\n");

			pid = fork();

			if(pid == 0)
			{
				close(cmd);
				close(data);
				close(rand);

				fun(ncmd,nrand);

				exit(0);
			}
			else
			{
				close(ncmd);
				close(nrand);
			}
		}

		return 0;
}