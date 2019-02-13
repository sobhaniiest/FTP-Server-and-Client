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

/*
ls
pwd
put
get
cd
mkdir
bye
*/

char str[SIZE];
FILE *fp;

void fun(int newcmd, int newdata)
{
	//printf("\n\n\n\n\n");
	printf("Connected to 127.0.0.10.\n");
	printf("Name : ");
	bzero(str, sizeof(str)); 
    fgets(str, sizeof(str), stdin);
	write(newcmd, str, sizeof(str)); 
	bzero(str, sizeof(str)); 
    read(newcmd, str, sizeof(str)); 
    if(!strcmp(str,"i"))
    {
    	printf("User Name Okay ;-), Enter Password...\nPassword : ");
    	bzero(str, sizeof(str)); 
	    fgets(str, sizeof(str), stdin);
		write(newcmd, str, sizeof(str)); 
		bzero(str, sizeof(str)); 
	    read(newcmd, str, sizeof(str));
	    if(!strcmp(str,"r"))
	    	printf("User logged in, Yooo!!! ;-)\n");
	    else
	    {
	    	printf("Incorrect Password :-(\n");
	    	exit(0);
	    }

    }
    else
    {
    	printf("Incorrect User ID :-(\n");
    	exit(0);
    }

    //////////////////////////////////////////////////////////
    int i,k,m,flag;
    char ch,file[SIZE],com[SIZE];
    
    while(1)
    {
    	printf("ftp>");
    	bzero(str, sizeof(str));
    	fgets(str, sizeof(str), stdin);
    	str[strlen(str)-1] = '\0';

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
 			//printf("%s\n",com);
 			if(!strcmp(com,"bye"))
 				exit(0);
 			else if(!strcmp(com,"pwd"))
 			{
 				write(newcmd, str, sizeof(str));
				while(1)
	 			{
	 				recv(newdata, &ch, sizeof(char),0);
	 				if(ch == EOF)
	 					break;
	 				printf("%c",ch);
	 			}
 			}
 			else if(!strcmp(com,"ls"))
 			{
 				write(newcmd, str, sizeof(str));
				while(1)
	 			{
	 				recv(newdata, &ch, sizeof(char),0);
	 				if(ch == EOF)
	 					break;
	 				printf("%c",ch);
	 			}
 			}
 			else if(!strcmp(com,"\0"))
 				continue;
 			else
 				printf("Command Not Found!!!\n");

 		}
 		else
 		{
 			//printf("%s %s\n",com,file);
 			if(!strcmp(com,"put"))
 			{ 
 				fp = fopen(file,"r");
 				if(fp == NULL)
 				{
 					printf("%s doesn't exist!!!\n",file);
 					continue;
 				}
 				else
 				{
 					write(newcmd, str, sizeof(str));
 					while(ch=fgetc(fp))
 					{
 						send(newdata, &ch, sizeof(char),0);
 						if(ch == EOF)
 							break;
 					}
 					fclose(fp);
 					printf("File Transfer Complete!!!\n");
 				}

 			}
 			else if(!strcmp(com,"get"))
 			{
 				write(newcmd, str, sizeof(str));
 				bzero(str, sizeof(str)); 
	    		read(newdata, str, sizeof(str));
	    		if(!strcmp(str,"n"))
	    		{
	    			printf("%s doesn't exist!!!\n",file);
 					continue;
	    		}
	    		else
	    		{
	    			fp = fopen(file,"w");
					while(1)
	 				{
	 					recv(newdata, &ch, sizeof(char),0);
	 					if(ch == EOF)
	 						break;
	 					fprintf(fp,"%c",ch);
	 				}
	 				fclose(fp);
	 				printf("File is received!!!\n");
	    		}

 			}
 			else if(!strcmp(com,"cd"))
 			{
 				write(newcmd, str, sizeof(str));
 				bzero(str, sizeof(str));
 				read(newdata, str, sizeof(str));
 				printf("%s\n",str);
 			}
 			else if(!strcmp(com,"mkdir"))
 			{
 				write(newcmd, str, sizeof(str));
 				bzero(str, sizeof(str));
 				read(newdata, str, sizeof(str));
 				printf("%s\n",str);
 			}
 			else
 				printf("Command Not Found!!!\n");
 		}
    }
}

/*
void fun(int newcmd, int newdata)
{
	char str[SIZE];

		while(1)
		{
			bzero(str, sizeof(str)); 
		    printf("Client : "); 
		    fgets(str, sizeof(str), stdin);
		    write(newcmd, str, sizeof(str)); 
		    bzero(str, sizeof(str)); 
		    read(newdata, str, sizeof(str)); 
		    printf("Server >> %s", str); 
		}
}
*/
int main()
{

		struct sockaddr_in ccmd, cdata;
		char addr[20] = "127.0.0.10";
		//cmd
		//scanf(" %s",addr);
		bzero((char *)&ccmd, sizeof(ccmd));
		ccmd.sin_family = AF_INET;
		//ccmd.sin_addr.s_addr = inet_addr("10.32.5.249");
		if(!inet_pton(AF_INET,addr,&(ccmd.sin_addr)))
		{
			puts("address conversion error");
			return 0;
		}
		ccmd.sin_port = htons(PCC);

		//data
		//scanf(" %s",addr);
		bzero((char *)&cdata, sizeof(cdata));
		cdata.sin_family = AF_INET;
		//cdata.sin_addr.s_addr = inet_addr("10.32.5.249");
		if(!inet_pton(AF_INET,addr,&(cdata.sin_addr)))
		{
			puts("address conversion error");
			return 0;
		}
		cdata.sin_port = htons(PCF);

		int cmd,data;

		//////////////////cmd///////////////////

		if((cmd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket for cmd");
			exit(0);
		}
		else
			printf("Socket is created for cmd.....\n");

		ccmd.sin_port = htons(PSC);
		
		if((connect(cmd, (struct sockaddr *)&ccmd, sizeof(ccmd))) < 0)
		{
			perror("Connect is nt working for cmd");
			exit(0);
		}
		else
			printf("cmd Connecting.....\n");

		/////////////data////////////////

		if((data = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket for data");
			exit(0);
		}
		else
			printf("Socket is created for data.....\n");

		cdata.sin_port = htons(PRAND);
		
		if((connect(data, (struct sockaddr *)&cdata, sizeof(cdata))) < 0)
		{
			perror("Connect is not working for data");
			exit(0);
		}
		else
			printf("data Connecting.....\n");

		////////////////////////////////////

		fun(cmd,data);

		return 0;
}