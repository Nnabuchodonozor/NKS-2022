#include <stdio.h>
#include <stdint.h>
#include <time.h>


#define NR 4
#define PRINTING 1
#define BUF_SIZE 16

uint16_t SBOX [16] = { 0x4, 0x3, 0x6, 0x2, 0xF, 0x5, 0x8, 0x9, 0xD, 0xE, 0xC, 0xB, 0xA, 0x1, 0x0, 0x7};

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

// transposition for: f 2 b 4 9 0 7 8 6 a 1 c e 3 d 5
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
     output ^= ((original) & 0x8800) << 1; // 11->12    .. 40919    
     output ^= ((original) & 0x1000) << 2; // 12->14    .. 57303
     output ^= ((original) & 0x2000) >> 10; // 13->3    .. 57311
     output ^= ((original) & 0x4000) >> 1; // 14->13    .. 65503
     output ^= ((original) & 0x8000) >> 10; // 15->5    .. 65535


    return output;
}

void substitution(uint16_t * original){
    uint16_t output = 0;
    output = (SBOX[(*original>>0)&0xf] << 0) ^ 
            (SBOX[(*original>>4)&0xf] << 4) ^
            (SBOX[(*original>>8)&0xf] << 8) ^
            (SBOX[(*original>>12)&0xf] << 12);
    *original = output;
}

uint16_t transform_key(uint64_t key, int index ){
    uint16_t output = 0;
    return 1;
}

void SPN(uint16_t original, uint64_t k, FILE * fd){
    uint16_t output = 0;
    uint16_t reduced_key = transform_key(k,0);
    output = original ^ reduced_key;
    printBIN(output,"after xor");
    for(int i = 1; i < NR; i++){
       substitution(&output);
       printBIN(output,"after sub");
       output=  transposition(output);
       printBIN(output,"after trans");    
       reduced_key = transform_key(k,i);
       output = output ^ reduced_key;
       printBIN(output,"after xor with 1");    

    }
    printBIN(output,"last");
}



int main(int argc, const char * argv[]) {
    //  uint16_t input = 0xffff;
    //  uint16_t output = transposition(input);   
    //  uint64_t k = 0;
    // //100 files for testing
    // for(int i = 0; i < 1; i++){
    // FILE * fd;
    // char fname[100];
    // sprintf(fname, "%04i", i);
    // // fd = fopen(fname,"wb");
    // // 8 keys for file to have >1MB
    // for(; k < 1; k++){
    //     // every single possible open text for 16b
    //     for(uint16_t x = 0; x < 0x0001; x++)
    //     {
    //         SPN(x,k,fd);
    //     }
    //   }
    // }
    uint16_t a = 0xffff;
    printBIN(a,"a");
    
    substitution(&a);

    printBIN(a,"");
    return 0;
}