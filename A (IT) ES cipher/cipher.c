#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define NR 4
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
uint16_t transposition(uint16_t original){                
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
    output ^= (c0 ^ c6)<<6;
    output ^= (c0 ^ c6)<<5;
    output ^= (c0 ^ c6)<<4;
    output ^= (c0 ^ c6)<<3;
    output ^= (c0 ^ c6)<<2;
    output ^= (c0 ^ c6)<<1;
    output ^= (c0 ^ c6);
    
    return output;
    }

void SPN(uint16_t original, uint64_t k, FILE * fd){
    uint16_t output = 0;
    
    output = original ^ k;
    printBIN(output,"after xor");
    for(int i = 1; i < NR ; i++){
       substitution(&output);
       printBIN(output,"after sub");
       output=  transposition(output);
       printBIN(output,"after trans");    
       
       output = output ^ k;
       printBIN(output,"after xor with 1");    

    }
    printBIN(output,"last");
    fwrite(&output, 2, 1, fd);
}

void dif_table() {
    int table[16][16] = {0} ;
    for(uint16_t a = 0x0; a<= 0xf; a++){
        uint16_t x1[16];
        uint16_t y[16];
        uint16_t y1[16];
        uint16_t bigY[16];
        for (uint16_t b = 0x0; b <= 0xf; b++)
        {
            x1[b]=a ^ b;
            y[b]= SBOX[(b>>0)&0xf];
            y1[b]=SBOX[(x1[b]>>0)&0xf];
            bigY[b]=y[b] ^ y1[b];

        }
        for (uint16_t i = 0x0; i <= 0xf; i++)
        {
            table[a][bigY[i]]+=1; 
            
            // for (uint16_t j = 0; j <= 16; j++)
            // {
            //     if(bigY[j]==i){
            //         table[a][i] = table[a][i] +1; 
            //     }
            // }
            
        }                          
    }
    print2D(table);
}

int main(int argc, const char * argv[]) {
    printf("hello world\n");
    uint16_t input = 0xC916;
    printBIN( transposition(input) ," mc =");

    return 0;
}