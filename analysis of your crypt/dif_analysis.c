#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define NR 4
#define PRINTING 1
#define BUF_SIZE 16

uint16_t TestSBOX [16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7} ;

                    // 0     1    2    3    4     5   6    7    8    9   a     b    c    d    e    f
uint16_t SBOX [16] = { 0x4, 0x3, 0x6, 0x2, 0xF, 0x5, 0x8, 0x9, 0xD, 0xE, 0xC, 0xB, 0xA, 0x1, 0x0, 0x7};
uint16_t INV_SBOX [16] = {0xE, 0xD,0x3, 0x1, 0x0, 0x5, 0x2, 0xF, 0x6, 0x7, 0xC, 0xB, 0xA, 0x8, 0x9, 0x4  };
char *key_permutations [NR] = {};

// https://stackoverflow.com/questions/26839558/hex-char-to-int-conversion

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

//https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

void swap(char *x, char *y) 
{ 
    char temp; 
    temp = *x; 
    *x = *y; 
    *y = temp; 
} 

// permutation counter with off switch when desired permutations 

int permute(char *a, int l, int r, int *counter, int *final_length) 
{     
int i; 

if (l == r) {
    // printf("%s\n", a);
        char *x =malloc (sizeof (char) * 17);;
    memcpy(x, a, 68 );
    key_permutations[*counter] = x;

    if(*counter == *final_length)
    return 1;
    else{
    *counter = *counter+1;
    }
    
    } 
else
{ 
    for (i = l; i <= r; i++) 
    { 
        swap((a+l), (a+i)); 
        if( permute(a, l+1, r, counter, final_length) ==1){
            return 1;
        } 
        if(*counter == *final_length)
        return 1;
        swap((a+l), (a+i)); //backtrack 
    } 

} 

return 0;
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
     output ^= ((original) & 0x0800) << 1; // 11->12    .. 40919    
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

uint16_t transform_key(uint16_t original, int index ){          // transpose bits in key based on lexicographical permutation on original key
    uint16_t output = 0;                                    
     printBIN(output," -1 ");
     output ^= ((original) & 0x0001) << hex2int(key_permutations[index][0]);  
     printBIN(output," 0 ");
     output ^= (((original) & 0x0002) >> 1) << hex2int(key_permutations[index][1]); 
     printBIN(output," 1 ");
     output ^= (((original) & 0x0004) >> 2) << hex2int(key_permutations[index][2]); 
     printBIN(output," 2 ");
     output ^= (((original) & 0x0008) >> 3) << hex2int(key_permutations[index][3]); 
     printBIN(output," 3 ");
     output ^= (((original) & 0x0010) >> 4) << hex2int(key_permutations[index][4]); 
     printBIN(output," 4 ");
     output ^= (((original) & 0x0020) >> 5) << hex2int(key_permutations[index][5]); 
     printBIN(output," 5 ");
     output ^= (((original) & 0x0040) >> 6) << hex2int(key_permutations[index][6]); 
     printBIN(output," 6 ");
     output ^= (((original) & 0x0080) >> 7) << hex2int(key_permutations[index][7]); 
     printBIN(output," 7 ");
     output ^= (((original) & 0x0100) >> 8) << hex2int(key_permutations[index][8]); 
     printBIN(output," 8 ");
     output ^= (((original) & 0x0200) >> 9) << hex2int(key_permutations[index][9]); 
     printBIN(output," 9 ");
     output ^= (((original) & 0x0400) >> 10) << hex2int(key_permutations[index][10]); 
     printBIN(output," a ");
     output ^= (((original) & 0x0800) >> 11) << hex2int(key_permutations[index][11]);     
     printBIN(output," b ");
     output ^= (((original) & 0x1000) >> 12) << hex2int(key_permutations[index][12]); 
     printBIN(output," c ");
     output ^= (((original) & 0x2000) >> 13) << hex2int(key_permutations[index][13]);
     printBIN(output," d ");
     output ^= (((original) & 0x4000) >> 14) << hex2int(key_permutations[index][14]); 
     printBIN(output," e ");
     output ^= (((original) & 0x8000) >> 15) << hex2int(key_permutations[index][15]); 
    printBIN(output," f ");

    return output;
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
            y[b]= TestSBOX[(b>>0)&0xf];
            y1[b]=TestSBOX[(x1[b]>>0)&0xf];
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
    // double clk = -clock();
    // char str[] = "0123456789ABCDEF"; 
    // int counter=0;
    // int final_length = NR;
    // permute(str, 0, 15, &counter, &final_length);

    
    // //100 files for testing
    // uint16_t key = 0xabcd;
    // for(int i = 0; i < 100; i++){
    // printf(" %d \n",i);
    // FILE * fd;
    // char fname[100];
    // sprintf(fname, "%04i.bin", i);
    // fd = fopen(fname,"wb");
    // // 8 keys for file to have >1MB
    // for(int k = 0; k < 8; k++){
    //     // every single possible open text for 16b
    //     for(uint16_t x = 0x0000; x < 0xffff; x++)
    //     {
    //         SPN(x,key,fd);
    //         key = key + 1;
    //     }
    //   }
    // }
    // printf(" T = %0.6lf s\n", clk/CLOCKS_PER_SEC);
    dif_table();
    return 0;
}