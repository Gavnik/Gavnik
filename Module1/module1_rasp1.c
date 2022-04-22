// C program for generating a
// random number in a given range.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/socket.h>
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#define MAXBUF		1024
#define QUEUE_NO 5


// this function calculates A= (g^a) mod p    which is used in Diffie Hellman Key exchange
void power_mod(long double *Aa, long double g, long double a, long double p){
    
    long double apotel=1;
    /*
    if (a >= p){
        *Aa = (long double)fmod((*Aa), p);
        --a;
        } 
        */
    while (a != 0)
    {
        apotel = apotel * g;  /* value = value*base; */
        //printf("now mega is : %Lf", mega);
        if (apotel >= p){
            apotel = (long double)fmod(apotel, p); //when mega gets equal or passes the value of n we need to mod it and multiply it again with the remaining powers
            }                      //the final mod execution will be equal to [(mega^small) mod n]. The reason of not calculating mega^small is how big the number gets
        --a;                 //and as a result in some cases it is not accurate and leads to wrong calculations of primitive root modulo.
    }
    
    *Aa = apotel;
    
    }
/*

void send_public_over_socket(int sockfd, int public) { 
	
	write(sockfd, &public, sizeof(public));

	//int_to_receive = ntohl(int_to_receive);
	printf("Public Key sent to RaspbPi1 : %d\n", public);
} 
*/

void receive_num(int clientfd ,int *i,long double *receive){
	long double int_to_receive = 0;
	char msg_rec[]="message received!";
	
	
	while (1) {
		
		

		read(clientfd, &int_to_receive, sizeof(int_to_receive));

		//int_to_receive = ntohl(int_to_receive);
		
		//write(clientfd, &int_to_send, sizeof(int_to_send));
		printf("Received from client: %Lf \n", int_to_receive);
	
		write(clientfd, msg_rec , sizeof(msg_rec));
	
		
		if (int_to_receive!=0){
			
			receive[*i] =  int_to_receive;

			break;
		}
		
		
		
		
		close(clientfd);
	}

}

void send_public_over_socket(int sockfd, long double number) { 
	char msg[MAX];
    
	write(sockfd, &number, sizeof(number)); 
    
    printf("Number sent to RaspbPi1 : %Lf\n", number);
	//write(sockfd, &primitive_root, sizeof(primitive_root));
	//write(sockfd, &public, sizeof(public));
    
	//int_to_receive = ntohl(int_to_receive);
	 
    read(sockfd, msg, sizeof(msg)); 
    printf("From Server : %s \n", msg); 
	//printf("Primitive root sent to RaspbPi1 : %d\n", primitive_root);
	//printf("Public Key sent to RaspbPi1 : %d\n", public);
	
	
} 

















int main(int argc, char **argv){
	int sockfd, clientfd, port, i, int_shared_key;  
	struct sockaddr_in s_addr, c_addr;
	char buffer[MAXBUF];
	//I need to make them double because in some huge numbers they get negative and the result of shared key is incorrect 
	long double  prim_mod, Prime, rcv_public_key, shared_key, secret_key, my_public_key, receive[3];//receive[0]->prime , receive[1]->primitive root , receive[2]->public key
	socklen_t socklen = (socklen_t)sizeof(struct sockaddr_in);

	if (argc < 2) {
		printf("Some of the command line arguments missing");
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(errno);
	}
        printf("Socket successfully created..\n");

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	port = atoi(argv[1]);
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&s_addr, sizeof(s_addr)) != 0){
		printf("socket creation failed...\n");
		exit(errno);
	}

	if (listen(sockfd, QUEUE_NO) != 0) {
		printf("socket listen failed...\n");
		exit(errno);
	}
	
	clientfd = accept(sockfd, (struct sockaddr*)&c_addr, &socklen);
	printf("%s:%d connected\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
	
	//receive: receive[0]->primitive mod receive[1]->primitive  receive[2]->public key 
	for (i=0;i<3;i++){
		
		receive_num(clientfd,&i,receive);
		
	}
	printf("receive[0] = %Lf receive[1] = %Lf receive[2] = %Lf \n",receive[0] ,receive[1], receive[2]);
	
	prim_mod = receive[0];
	Prime = receive[1];
	rcv_public_key = receive[2]; 
	
	//generate random secret key
    srand(time(NULL));   
    secret_key = rand();
    printf("secret_key is : %Lf\n", secret_key);
    //create my public key
    power_mod(&my_public_key,prim_mod,secret_key,Prime);
    printf("public key is : %Lf\n", my_public_key);
    
    send_public_over_socket(clientfd,my_public_key);
	
	//generate shared key
	power_mod(&shared_key,rcv_public_key,secret_key,Prime);
    printf("Our shared key is : %Lf !!!!\n", shared_key);
	
	if (shared_key>0){
		int_shared_key = (int)shared_key;
	}else{
		int_shared_key = (int)(shared_key*(-1));	
			}
	
	printf("Shared key in integer form is: %d\n", int_shared_key);
	
	close(sockfd);
	return 0;
}










