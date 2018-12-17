#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h> 

void pipeline(char command[100],int n){

	// String Processing Logic
	pid_t pid;
	int status;
	int pipefd[2];
	int tempfd = 0;
	char *token;
	char *rest = command;
	int counter = 0;
	while((token = strtok_r(rest,"|",&rest))){
		char temp[100];
		int j = 0;
		while((*token)!='\0'){
			temp[j++] = *token;
			token++; 
		}
		temp[j] = '\0';
		int z = 0;
		char *arg[100];
		memset(arg,0,sizeof(arg));
		int k = 0;
		int y = 0;
		char temp1[100];
		while(z < strlen(temp)){
			while(temp[z]!=' '){
				temp1[y] = temp[z];
				y++;
				z++;
			}
			temp1[y] = '\0'; 
			arg[k] = strdup(temp1);
			k++;
			memset(temp1,0,sizeof(temp1));
			y=0;
			z++;
		}
		
		// PipeLine Logic
		
		
		status = pipe(pipefd);
		pid = fork();
		if(pid < 0){
			printf("Error Creating Pipe\n");
			exit(0);
		}else if(pid == 0){
			//child Process
			close(pipefd[0]);	//Closing Read End Of Pipe
			dup2(tempfd,0);		//Dup The Read File Discriptor Of Process With Pipe
			if(counter < n-1){
				dup2(pipefd[1],1);	//Dup The Write File Discriptor Of The Process With Pipe
			}
			int ret = 0;
			ret = execvp(arg[0],arg);
			if(ret == -1){
				printf("Execvp Error %d\n",errno);
				exit(0);
			}
		}else{
			//Parent Process
			wait(NULL);
			close(pipefd[1]);
			tempfd = pipefd[0];
			counter++;
		}
	}	
}

int main(int argc, char *argv[]){
	char command[100];
	memset(&command,'\0', sizeof(command));
	int n = 1;
	printf("Enter The Number Of Commands As Command Line Arguments And Commands Separated By Pipes And Space After Each Command \n");
	printf("Example : cat /etc/passwd |grep root |wc \n");
	scanf("%[^\n]s",command); 
	n = atoi(argv[1]);
	pipeline(command,n);
	return 0;
}
