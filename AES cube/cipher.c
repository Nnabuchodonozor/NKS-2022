#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define NR 4
#define PRINTING 1
#define BUF_SIZE 17

uint16_t SBOX [16] = { 0xF, 0x2, 0xB, 0x4, 0x9, 0x0, 0x7, 0x8, 0x6, 0xA, 0x1, 0xC, 0xE, 0x3, 0xD, 0x5};

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
//                                          *** SPN **8

void substitution(uint16_t * original){
    uint16_t output = 0;
    output = (SBOX[(*original>>0)&0xf] << 0) ^ 
            (SBOX[(*original>>4)&0xf] << 4) ^
            (SBOX[(*original>>8)&0xf] << 8) ^
            (SBOX[(*original>>12)&0xf] << 12);
    *original = output;
}

void SPN(uint16_t original, uint64_t k, FILE * fd){
    uint16_t output = 0;
    uint16_t reduced_key = transform_key(k,0);
    output = original ^ reduced_key;
    printBIN(output,"after xor");
    for(int i = 1; i < NR ; i++){
       substitution(&output);
       printBIN(output,"after sub");
       output=  transposition(output);
       printBIN(output,"after trans");    
       reduced_key = transform_key(k,i);
       output = output ^ reduced_key;
       printBIN(output,"after xor with 1");    

    }
    printBIN(output,"last");
    fwrite(&output, 2, 1, fd);
}

int main(int argc, const char * argv[]) {
    printf("hello world\n");
    return 0;
}