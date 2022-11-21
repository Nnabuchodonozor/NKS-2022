#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include "logger.h"
#include <unistd.h> // notice this! you need it!

#define NR 2
#define PRINTING 1
#define BUF_SIZE 17

uint16_t SBOX [16] = { 0xF, 0x2, 0xB, 0x4, 0x9, 0x0, 0x7, 0x8, 0x6, 0xA, 0x1, 0xC, 0xE, 0x3, 0xD, 0x5};
uint16_t TSBOX [16] = {0x9, 0x4, 0xA, 0xB, 0xD, 0x1, 0x8, 0x5,0x6, 0x2, 0x0, 0x3, 0xC, 0xE, 0xF,0x7}; 
//                                      ***   utils functions ***
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void print2D(int tab[16][16]){  
    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            printf("%d ", tab[i][j]);
        }
        printf("\n");
        
    }
}

char *int2bin(u_int16_t a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 15; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

void printBIN(u_int16_t a , char * message){
    if (PRINTING==1)
    {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';
    int2bin(a, buffer, BUF_SIZE - 1);
    printf(" %s",message);
    printf(" %s \n", buffer);
    }
}
//                                          *** SPN **

void substitution(uint16_t * original){
    uint16_t output = 0;
    output = (SBOX[(*original>>0)&0xf] << 0) ^ 
            (SBOX[(*original>>4)&0xf] << 4) ^
            (SBOX[(*original>>8)&0xf] << 8) ^
            (SBOX[(*original>>12)&0xf] << 12);
    *original = output;
}

//mix column difusion b0 b1 b2 b3 b4 b5 b6 b7 .. c0 c1 c2 c3 c4 c5 c6 c7
// each bite 
uint16_t mix_col(uint16_t original){                
    uint16_t output = 0;  
    uint16_t b0 = (original & 0x8000) >> 15;
    uint16_t b1= (original & 0x4000) >> 14;
    uint16_t b2= (original & 0x2000) >> 13;
    uint16_t b3= (original & 0x1000) >> 12;
    uint16_t b4= (original & 0x0800) >> 11;
    uint16_t b5= (original & 0x0400) >> 10;
    uint16_t b6= (original & 0x0200) >> 9;
    uint16_t b7= (original & 0x0100) >> 8;
    uint16_t c0= (original & 0x0080) >> 7;
    uint16_t c1= (original & 0x0040) >> 6;
    uint16_t c2= (original & 0x0020) >> 5;
    uint16_t c3= (original & 0x0010) >> 4;
    uint16_t c4= (original & 0x0008) >> 3;
    uint16_t c5= (original & 0x0004) >> 2;
    uint16_t c6= (original & 0x0002) >> 1;
    uint16_t c7= (original & 0x0001);
    output ^= (b0 ^ b6)<<15;
    output ^= (b1 ^ b4 ^ b7)<<14;
    output ^= (b2 ^ b4 ^ b5)<<13;
    output ^= (b3 ^ b5)<<12;
    output ^= (b2 ^ b4)<<11;
    output ^= (b0 ^ b3 ^ b5)<<10;
    output ^= (b0 ^ b1 ^ b6)<<9;
    output ^= (b1 ^ b7)<<8;
    output ^= (c0 ^ c6)<<7;
    output ^= (c1 ^ c4 ^ c7)<<6;
    output ^= (c2 ^ c4 ^ c5)<<5;
    output ^= (c3 ^ c5)<<4;
    output ^= (c2 ^ c4)<<3;
    output ^= (c0 ^ c3 ^ c5)<<2;
    output ^= (c0 ^ c1 ^ c6)<<1;
    output ^= (c1 ^ c7);
    
    return output;
    }

uint16_t transposition(uint16_t original){                 // positioning 15 14 ... 1 0
    uint16_t output = 0;                                    //for original ffff debugging output is
     output ^= ((original) & 0x0001) << 15; // 0 -> 15   .. 32768 
     output ^= ((original) & 0x0002) << 1; // 1 -> 2     .. 32772
     output ^= ((original) & 0x0004) << 9; // 2 -> 11   .. 34820
     output ^= ((original) & 0x0008) << 1; // 3-> 4     .. 34836
     output ^= ((original) & 0x0010) << 5; // 4-> 9     .. 35348
     output ^= ((original) & 0x0020) >> 5; // 5 -> 0    .. 35349
     output ^= ((original) & 0x0040) << 1; // 6->7      .. 35477
     output ^= ((original) & 0x0080) << 1; // 7->8      .. 35733
     output ^= ((original) & 0x0100) >> 2; // 8->6      .. 35797
     output ^= ((original) & 0x0200) << 1; // 9->10     .. 36821
     output ^= ((original) & 0x0400) >> 9; // 10->1     .. 36823
     output ^= ((original) & 0x0800) << 1; // 11->12    .. 40919    
     output ^= ((original) & 0x1000) << 2; // 12->14    .. 57303
     output ^= ((original) & 0x2000) >> 10; // 13->3    .. 57311
     output ^= ((original) & 0x4000) >> 1; // 14->13    .. 65503
     output ^= ((original) & 0x8000) >> 10; // 15->5    .. 65535


    return output;
}


// SPN modified for 2 rounds no key transformation
uint16_t SPN(uint16_t original, uint64_t k){
    uint16_t output = 0;
    //printBIN(output,"after xor");
    output = original ^ k;
    substitution(&output);
    //printBIN(output,"after sub");
    output=  transposition(output);
    //printBIN(output,"after trans");    
    output = output ^ k;
    //printBIN(output,"after xor with 1");    
    substitution(&output);
    output = output ^ k;
    //printBIN(output,"last");
    return output;
}

void dif_profile(uint16_t key,FILE * fd) {
        int max=0;
        unsigned long zero_count=0;
        for(uint16_t a = 0x0001; a< 0xffff; a++){
        short table[0xffff] = {0} ;
        // uint16_t x1[16];
        // uint16_t y[16];
        // uint16_t y1[16];
        // uint16_t bigY[16];
        if(a%0x40==0){
         printf(" %d \n", (a/0x40));
        }
        uint16_t x1;
        uint16_t x2;
        // b = x
        for (uint16_t x = 0x0001; x < 0xffff; x++)
        {
            
            // x1[b]=a ^ b;
            // y[b]= SPN((b>>0)&0xf,key);
            // y1[b]=SPN((x1[b]>>0)&0xf,key);
            // bigY[b]=y[b] ^ y1[b];
            
            // 1. enc(x,k)
            // uint16_t b = SPN(x,key) ^ SPN(x+a,key);
            x1 = SPN(x,key);
            x2 = SPN(x+a,key);
            uint16_t b = x1 ^ x2;
            table[b-1]+=1;
        }
        for (uint16_t i = 0x0001; i < 0xffff; i++)
        {
            if(table[i-1]==0){
                zero_count+=1;
            }
            if(table[i-1]>max){
                max = table[i-1];
            }
            // printf(" %d",table[i-1]);
        }
        // fwrite(table, sizeof(short) , sizeof(table), fd);
        
    }
    // print2D(table);
    // fprintf(fd, "%s %d", "max diff was: \n", max);
    // fprintf(fd, "%s %d", "max number of zeros was: \n",zero_count);
    printf("max diff was: %d\n",max);
    printf("max number of zeros was: %lu\n",zero_count);
}

uint16_t generate_random_key(){
srand(time(NULL));   // Initialization, should only be called once.
return (rand() % 0xffff );
}


int main(int argc, const char * argv[]) {
    // char *p;
    FILE * pFile;
    uint16_t k = generate_random_key();
    // uint16_t k = 0x0000;
    // pFile =fopen("output.dat", "wb+");
    dif_profile(k,pFile);


    // fclose(pFile);
     
    // uint16_t x=strtol(argv[1],&p,10);

   
    return 0;
}

// while(1){
//     printf("prog\n");
//         sleep(5); // format is sleep(x); where x is # of seconds.
//     }