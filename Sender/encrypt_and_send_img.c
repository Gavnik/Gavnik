#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <mcrypt.h>
#include <math.h>

/*
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 */




/*

int encrypt(
    void* buffer,
    int buffer_len, // Because the plaintext could include null bytes
    char* IV, 
    char* key,
    int key_len 
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

int decrypt(
    void* buffer,
    int buffer_len,
    char* IV, 
    char* key,
    int key_len 
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

void display(char* ciphertext, int len){
  int v;
  for (v=0; v<len; v++){
    printf("%d ", ciphertext[v]);
  }
  printf("\n");
}

int main()
{
  MCRYPT td, td2;
  char* plaintext = "test text 123";
  char* IV = "AAAAAAAAAAAAAAAA";
  char* key = "0123456789abcdef";
  int keysize = 16; // 128 bits 
  char* buffer;
  int buffer_len = 16;

  buffer = calloc(1, buffer_len);
  strncpy(buffer, plaintext, buffer_len);

  printf("==C==\n");
  printf("plain:   %s\n", plaintext);
  encrypt(buffer, buffer_len, IV, key, keysize); 
  printf("cipher:  "); display(buffer , buffer_len);
  decrypt(buffer, buffer_len, IV, key, keysize);
  printf("decrypt: %s\n", buffer);
  
  return 0;
}
*********/



#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CBC 1

#include "aes.h"
#include "pkcs7_padding.c"

//static void test_encrypt_cbc(void);



// Convert text to hex.
/*
void string_to_hex(char *text, uint8_t *hex){
  
  int len = strlen(text); 
      
  for (int i = 0, j = 0; i < len; ++i, j += 2)
    sprintf(hex + j, "%02x", text[i] & 0xff);

  //printf("'%s' in hex is %s.\n", text, hex);
    
    }
*/

// Convert the hex back to a string.
void hex_to_string(uint8_t *hex, char *text, int len){  
  //int len = strlen(hex)/2;
  int num,length,i;
  char new_text[len*2];
  
  new_text[0]='\0';
  for (i = 0 ; i < len; ++i) {
    num = (int)hex[i] ;
    length = snprintf( NULL, 0, "%d", num );
    char* temp = malloc( length + 1 );
    snprintf( temp, length + 1, "%d", num );
    strcat(new_text, temp);
    free(temp);
  }
  for (i=0 ; i<len*2 ; i++){
      text[i]=new_text[i];
      }

}



static void test_encrypt_cbc(char *send_buffer,int size)
{
    //Initialization Vector
    uint8_t iv[]  = { 0x75, 0x52, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x21, 0x21};

    uint8_t i;                               
    //char* report = "my super secret thing that needs to remain that way!";
    char* key = "thisIstheKey";
    int dlen = size;
    int klen = strlen(key);
    struct AES_ctx ctx;
    
    
    // reset the iv !! important to work!
    AES_ctx_set_iv(&ctx,iv);
    
    printf("THE PLAIN TEXT STRING = ");
    for (i=0; i<dlen;i++){
        printf("%c", send_buffer[i]);
    }
    printf("\n");
    
   
    //Proper Length of report
    int dlenu = dlen;
    
    //Proper length of key
    int klenu = klen;
    if (klen % 16) {
        klenu += 16 - (klen % 16);
        printf("The original length of the KEY = %d and the length of the padded KEY = %d\n", klen, klenu);
    }
    
    // Make the uint8_t arrays
    uint8_t hexarray[256];
    uint8_t kexarray[klenu];
    
    // Initialize them with zeros
    memset( hexarray, 0, dlenu );
    memset( kexarray, 0, klenu );
    
    // Fill the uint8_t arrays
    for (int i=0;i<128;i++) {
        hexarray[i] = (uint8_t)send_buffer[i];
    }
    for (int i=0;i<klen;i++) {
        kexarray[i] = (uint8_t)key[i];
    }                           
    printf("The Unpadded STRING in hex is = ");
    for (i=0; i<128;i++){
        printf("%02x",hexarray[i]);
    } 
    printf("\n");
    
    int reportPad = pkcs7_padding_pad_buffer( hexarray, dlen, sizeof(hexarray), 16 );
    int keyPad = pkcs7_padding_pad_buffer( kexarray, klen, sizeof(kexarray), 16 );
    
    
    printf("The padded STRING in hex is = ");
    for (i=0; i<128;i++){
        printf("%02x",hexarray[i]);
    }
    
    printf("\n");
    
    printf("The padded key in hex is = ");
    for (i=0; i<klenu;i++){
        printf("%02x",kexarray[i]);
    }
    printf("\n");
        
    // In case you want to check if the padding is valid
    int valid = pkcs7_padding_valid( hexarray, dlen, sizeof(hexarray), 16 );
    int valid2 = pkcs7_padding_valid( kexarray, klen, sizeof(kexarray), 16 );
    printf("Is the pkcs7 padding valid  report = %d  |  key = %d\n", valid, valid2);
    
    //start the encryption
    
    AES_init_ctx_iv(&ctx, kexarray, iv);
    
    // encrypt
    AES_CBC_encrypt_buffer(&ctx, hexarray, 256);
    printf("the encrypted STRING = ");
    
    hex_to_string(send_buffer,hexarray,128);
    /*
    for (i=0; i<128;i++){
        //printf("%02x",hexarray[i]);
        send_buffer[i]=(char*)hexarray[i];
    }
    */
    printf("The SEND BUFFER is = ");
    for (i=0; i<128;i++){
        printf("%c",send_buffer[i]);
    } 
    printf("\n");
    
    printf("The padded key in hex is = ");
    for (i=0; i<klenu;i++){
        printf("%02x",kexarray[i]);
    }
    printf("\n");
    
    printf("\n");
   /*     
    // reset the iv !! important to work!
    AES_ctx_set_iv(&ctx,iv);
    
    // start decryption
    AES_CBC_decrypt_buffer(&ctx, hexarray, dlenu);
    
    size_t actualDataLength = pkcs7_padding_data_length( hexarray, dlenu, 16);
    printf("The actual data length (without the padding) = %ld\n", actualDataLength);
    
    printf("the decrypted STRING in hex = ");
    for (i=0; i<actualDataLength;i++){
        printf("%02x",hexarray[i]);
    }
    printf("\n");
    
    */
}











void send_image(int socket)
{
    FILE *picture;
    int size, read_size, stat, packet_index, i, rem_size;
    char send_buffer[256], pre_crypt_buffer[128], read_buffer[64];
    packet_index = 1;

    picture = fopen("image.jpg", "r");
    printf("Getting Picture Size\n");

    if(picture == NULL) {
         printf("Error Opening Image File"); }

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);
    printf("Total Picture size: %i\n",size);

    rem_size = size;
    
    //Send Picture Size
    printf("Sending Picture Size\n");
    write(socket, (void *)&size, sizeof(int));

    //Send Picture as Byte Array
    printf("Sending Picture as Byte Array\n");

    do { //Read while we get errors that are due to signals.
       stat=read(socket, &read_buffer , 65);
       printf("Bytes read: %i\n",stat);
    } while (stat < 0);

    printf("Received data in socket\n");
    printf("Socket data: %c\n", read_buffer);

    while(!feof(picture)) {
    //while(packet_index = 1){
       //Read from the file into our send buffer
       //bzero (pre_crypt_buffer,128);
       read_size = fread(pre_crypt_buffer, 1, sizeof(pre_crypt_buffer)-1, picture);
       
       //bzero(send_buffer,256);
       for (i=0; i<read_size+1;i++){
        send_buffer[i] = pre_crypt_buffer[i];
        }
       
       rem_size = rem_size - read_size ; // 127 equals read_size can be changed
       
       if (rem_size>128){
         test_encrypt_cbc(send_buffer,read_size); //encryption of block
        }else{
         test_encrypt_cbc(send_buffer,rem_size);   
            }
        
        
       //Send data through our socket
       do{
         stat = write(socket, send_buffer, read_size);
       }while (stat < 0);

       printf("Packet Number: %i\n",packet_index);
       printf("Packet Size Sent: %i\n",read_size);
       printf(" \n");
       printf(" \n");


       packet_index++;

       //Zero out our send buffer
       bzero(send_buffer, sizeof(send_buffer));
       bzero(pre_crypt_buffer, sizeof(pre_crypt_buffer));
      }
}















/*

int main(void)
{
    int exit=0;

#if defined(AES256)
    printf("\nTesting AES256\n\n");
#elif defined(AES192)
    printf("\nTesting AES192\n\n");
#elif defined(AES128)
    printf("\nTesting AES128\n\n");
#else
    printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    return 0;
#endif

    test_encrypt_cbc();

    return exit;
}

*/

int main(int argc , char *argv[])

{
#if defined(AES256)
    printf("\nTesting AES256\n\n");
#elif defined(AES192)
    printf("\nTesting AES192\n\n");
#elif defined(AES128)
    printf("\nTesting AES128\n\n");
#else
    printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    return 0;
#endif

    int socket_desc;
    struct sockaddr_in server;
    char *parray;


    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    memset(&server,0,sizeof(server));
    server.sin_addr.s_addr = inet_addr("192.168.1.24");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8080 );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
        printf("\n");
        close(socket_desc);
        puts("Connect Error");
        return 1;
    }

    puts("Connected\n");

    send_image(socket_desc);

    //close(socket_desc);

    return 0;
}





