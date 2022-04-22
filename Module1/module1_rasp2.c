
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

// Generates and prints 'count' random
// numbers in range [lower, upper].

//0 is False and 1 is True

int isPrime(int n){
    int i;
    i=5;
    //corner cases
    if (n <= 1){
        return 0;
    }
    if (n <= 3){
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0){
        return 0;
    }
    while((i^2) <= n){
        if (n % i == 0 || n % (i + 2) == 0){
            return 0;
        }
        i = i + 6;
    }

    return 1;


}

void findRandomPrime(int lower, int upper,long double *count)
{
    int prime; 
    prime = 0;
    
    do {
        
        int num = rand() % (upper - lower + 1) + lower;
        //printf("%d ", num);
        prime = isPrime(num);
        *count = num;
        
        
    }while (prime == 0);
}


// Utility function to store prime
// factors of a number
int findPrimefactors(int *s, int n) {
 	int i , k , rt;
    
 	//s = (int*)malloc(sizeof(int));
 	k = 0;
    // devide n by 2 while n is even
    while (n % 2 == 0) {
        s[k]=2;
        n = n >> 1; // n/2 and round down
 	}
 	if (s[0]==2){ //in order to not have recurring "if" statements in the "while" statement
 		k=1;
 	}
    // n must be odd at this point. So we can 
    // skip one element (Note i = i +2)
    rt = (int)sqrt(n);
    for (i=3; i< rt; i=i+2){
         
        // While i divides n, print i and divide n
        while (n % i == 0) {            //could use if instead of while to not capture dublicates
 			s = (int*)realloc(s, sizeof(int)*(k+1));
            s[k] = i;
            k++;
            n = n / i;
        }
    }     
    // This condition is to handle the case
    // when n is a prime number greater than 2
    if (n > 2) {
        s[k] = n;
        k++;
    } 
 	k = k-1;
    printf("k is %d \n", k);
 	return k; // the number of elements stored in s[]

}
// Function to find smallest primitive
// root of n

int findPrimitive(int n, int *s) {
    int S_elements, r, i, flag, phi;
    
    long int small;
    long double mega;
 	/******************/ //Probably unnecessary
    // Check if n is prime or not
    if (isPrime(n) == 0){
        return -1;
    }
    
    /*****************/
 
    // Find value of Euler Totient function
    // of n. Since n is a prime number, the
    // value of Euler Totient function is n-1
    // as there are n-1 relatively prime numbers.
    phi = n - 1;
    
    // Find prime factors of phi and store in a set
    S_elements = findPrimefactors(s, phi); //number of elements in s
    
    //printf("S[0]%d S[1]%d S[2]%d S[3]%d S[4]%d \n",s[0],s[1],s[2],s[3],s[4]);
    printf("The number of S_elements is %d \n" , S_elements);
    printf("Phi is equal to %d \n" , phi);
    printf("The value of n is %d \n" , n);
 
    // Check for every number from 2 to phi
    //We can go further up from r=2 if we want to get other prime root modulo more often. Also we could generate several and 
    // pick random but it is expensive 
    // A good and cheap way to add more randomness could be
    /*  srand(time(NULL));   
        r = (rand() % 14)+2; 
     */   
    //in order to get random int numbers in the space [2,15]  
        
        
    
    for (r=2; r<=phi ; r++){
 
        // Iterate through all prime factors of phi.
        // and check if we found a power with value 1
        
        
        
        
        flag = 0;
        for (i=0; i<=S_elements; i++){
 
            // Check if r^((phi)/primefactors)
            // mod n is 1 or not
            small = phi/s[i];

            printf("For r equal to %d Small is %li \n" , r, small);
            /*****************************************/
            mega=1.0;
            while (small!=0)
            {
                mega = mega * r;  /* value = value*base; */
                //printf("now mega is : %Lf", mega);
                if (mega >= n){
                    mega = (fmod(mega,n)); //when mega gets equal or passes the value of n we need to mod it and multiply it again with the remaining powers
                    }                      //the final mod execution will be equal to [(mega^small) mod n]. The reason of not calculating mega^small is how big the number gets
                --small;                    //and as a result in some cases it is not accurate and leads to wrong calculations of primitive root modulo.
            }
            printf("[(mega^small) mod n] = %Lf \n", mega);
        
            
            /****************************************/
            //printf("mega%llu \n" , mega);
            //printf("mega is : %Lf \n" , mega);
            //if ((mega % n) == 1){ 
            //printf("fmod() has a result %Lf\n", (long double)(fmod(mega,n)));
            //printf("for r = %d we have remainder %Lf \n", r, ((long double)(remainder(mega,n))));
            if (mega == 1){ 
                
                flag = 1;
                break;
            }
        }
                // If there was no power with value 1.
        //printf("for r = %d we have remainder %Lf \n", r, (remainder(mega, (long double)n)));
        if (flag == 0){
            return r;
        }
    
 	}
    // If no primitive root found
    return -1;
}

/***
void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}
****/


// this function calculates A= (g^a) mod p    which is used in Diffie Hellman Key exchange
void power_mod(long double *Aa, long double g, long double a, long double p){
    
    *Aa=1;
    /*
    if (*Aa >= p){
        *Aa = (long double)fmod((*Aa), p);
        --a;
        } 
        */
    while (a != 0)
    {
        *Aa = (*Aa) * g; /* value = value*base; */
        //printf("now mega is : %Lf", mega);
        if (*Aa >= p){
            *Aa = (long double)fmod((*Aa), p); //when mega gets equal or passes the value of n we need to mod it and multiply it again with the remaining powers
            }                      //the final mod execution will be equal to [(mega^small) mod n]. The reason of not calculating mega^small is how big the number gets
        --a;                 //and as a result in some cases it is not accurate and leads to wrong calculations of primitive root modulo.
                            
    }
    
    
    }


void send_primitives_and_public_over_socket(int sockfd, long double number) { 
	char msg[MAX];
    
    bzero(msg, sizeof(msg));
    while (strlen(msg) == 0){
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
} 


void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


void receive_num(int sockfd , long double *receive){
	long double int_to_receive = 0;
	char msg_rec[]="message received!";
	
	while (1) {
		
		

		read(sockfd, &int_to_receive, sizeof(int_to_receive));
        if (int_to_receive!=0){
		//int_to_receive = ntohl(int_to_receive);
		
		//write(clientfd, &int_to_send, sizeof(int_to_send));
		printf("Received from client: %Lf \n", int_to_receive);
	
		write(sockfd, msg_rec , sizeof(msg_rec));
	
		
		
			*receive =  int_to_receive;
			break;
		}
		
		
		
		
		close(sockfd);
	}

}

































// Driver code
int main(int argc, char **argv)
{
    int sockfd, port, int_shared_key; 
	struct sockaddr_in servaddr; 
    int lower = 1000000, upper = 100000000;
    //I need to make them double because in some huge numbers they get negative and the result of shared key is incorrect 
    long double rcv_public_key, shared_key, prim_mod, secret_key, my_public_key, Prime=1;
    int *s;
    
    //open socket
    if (argc < 3) {
		printf("Some of the command line arguments missing");
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(errno); 
	}	
	printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
	port = atoi(argv[2]);
	servaddr.sin_port = htons(port); 

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(errno);
	} 
	printf("connected to the server..\n");
    
    
    /*                                       */
    
    
    s = (int*)malloc(sizeof(int)); //the s array will hold the prime factors which will be used in computation of primitive modulo
    // Use current time as
    // seed for random generator
    srand(time(0));

    //find a random prime between lowe and upper bountry
    findRandomPrime(lower, upper, &Prime);
    printf ("The random prime number generated is: %Lf \n", Prime);

    //find the primitive root modulo of the previously generated prime number
    prim_mod = findPrimitive(Prime,s);
    printf("Smallest primitive root of %Lf\n is %Lf \n", Prime, prim_mod);
    
    //generate random secret key
    srand(time(NULL));   
    secret_key = rand();
    printf("secret_key is : %Lf\n", secret_key);
    //generate public key
    power_mod(&my_public_key,prim_mod,secret_key,Prime);
    printf("public key is : %Lf\n", my_public_key);
    
    send_primitives_and_public_over_socket(sockfd,prim_mod);
    delay(0.1);
    send_primitives_and_public_over_socket(sockfd,Prime);
    delay(0.1);
    send_primitives_and_public_over_socket(sockfd,my_public_key);


    receive_num(sockfd,&rcv_public_key);

    printf("received public key is: %Lf\n", rcv_public_key);

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

