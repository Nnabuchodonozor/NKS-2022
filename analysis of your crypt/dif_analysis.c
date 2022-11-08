#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define NR 4
#define PRINTING 1
#define BUF_SIZE 17

static struct { float count; uint16_t key; } tuple;
// uint16_t TestSBOX [16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7} ;
//f2b490786a1ce3d5
                    // 0     1    2    3    4     5   6    7    8    9   a     b    c    d    e    f
uint16_t SBOX [16] = { 0xF, 0x2, 0xB, 0x4, 0x9, 0x0, 0x7, 0x8, 0x6, 0xA, 0x1, 0xC, 0xE, 0x3, 0xD, 0x5};
uint16_t INV_SBOX [16] = {0x5, 0xA,0x1, 0xD, 0x3, 0xF, 0x8, 0x6, 0x7, 0x4, 0x9, 0x2, 0xB, 0xE, 0xC, 0x0  };

// uint16_t SBOX [16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
// uint16_t INV_SBOX [16] = {0xE, 0x3,0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5  };


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

// transposition changed to inverse
uint16_t transposition(uint16_t original){                 // positioning 15 14 ... 1 0
    uint16_t output = 0;                                    //for original ffff debugging output is
     output ^= ((original) & 0x0001); // 0 -> 15   .. 32768 
     output ^= ((original) & 0x0002) << 3; // 1 -> 2     .. 32772
     output ^= ((original) & 0x0004) << 6; // 2 -> 11   .. 34820
     output ^= ((original) & 0x0008) << 9; // 3-> 4     .. 34836
     output ^= ((original) & 0x0010) >> 3; // 4-> 9     .. 35348
     output ^= ((original) & 0x0020); // 5 -> 0    .. 35349
     output ^= ((original) & 0x0040) << 3; // 6->7      .. 35477
     output ^= ((original) & 0x0080) << 6; // 7->8      .. 35733
     output ^= ((original) & 0x0100) >> 6; // 8->6      .. 35797
     output ^= ((original) & 0x0200) >> 3; // 9->10     .. 36821
     output ^= ((original) & 0x0400); // 10->1     .. 36823
     output ^= ((original) & 0x0800) << 3; // 11->12    .. 40919    
     output ^= ((original) & 0x1000) >> 9; // 12->14    .. 57303
     output ^= ((original) & 0x2000) >> 6; // 13->3    .. 57311
     output ^= ((original) & 0x4000) >> 3; // 14->13    .. 65503
     output ^= ((original) & 0x8000); // 15->5    .. 65535


    return output;
}

// substitution changed to inverse
void substitution(uint16_t * original){
    uint16_t output = 0;
    output = (INV_SBOX[(*original>>0)&0xf] << 0) ^ 
            (INV_SBOX[(*original>>4)&0xf] << 4) ^
            (INV_SBOX[(*original>>8)&0xf] << 8) ^
            (INV_SBOX[(*original>>12)&0xf] << 12);
    *original = output;
}

void inv_sbox(uint16_t * original){
    uint16_t output = 0;
    output = (INV_SBOX[(*original)&0xf]);
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

void dif_solve(int count[16][16][16][16],uint16_t y, uint16_t y1){

// this is done for when i have first subkey

uint16_t first_subkey = 0xdc07;
y = y ^ first_subkey;
y1 = y1 ^ first_subkey;
substitution(&y);
substitution(&y1);


// this is done for when i have second subkey

uint16_t second_subkey = 0x5509;
y = y ^ second_subkey;
y1 = y1 ^ second_subkey;
y=  transposition(y);
y1=  transposition(y1);
substitution(&y);
substitution(&y1);



// // this is done for when i have third subkey
// uint16_t third_subkey = 0xdc07;
// y = y ^ third_subkey;
// y1 = y1 ^ third_subkey;
// substitution(&y);
// substitution(&y1);
// y=  transposition(y);
// y1=  transposition(y1);



// this is done for when i have last subkey


for (uint16_t a = 0x0; a <= 0xf; a++)
{
    for (uint16_t b = 0x0; b <= 0xf; b++)
    {   
        for (uint16_t c = 0x0; c <= 0xf; c++)
        {
            for (uint16_t d = 0x0; d <= 0xf; d++)
            {
                uint16_t v1 = (a & 0xf) ^ ((y>>12)&0xf);
                uint16_t v2 = (b & 0xf) ^ ((y>>8)&0xf);
                uint16_t v3 = (c & 0xf) ^ ((y>>4)&0xf);
                uint16_t v4 = (d & 0xf) ^ ((y>>0)&0xf);    
                inv_sbox(&v1);
                inv_sbox(&v2);
                inv_sbox(&v3);
                inv_sbox(&v4);
                uint16_t V1 = (a & 0xf) ^ ((y1>>12)&0xf);
                uint16_t V2 = (b & 0xf) ^ ((y1>>8)&0xf);
                uint16_t V3 = (c & 0xf) ^ ((y1>>4)&0xf);
                uint16_t V4 = (d & 0xf) ^ ((y1>>0)&0xf);    
                inv_sbox(&V1);
                inv_sbox(&V2);
                inv_sbox(&V3);
                inv_sbox(&V4);
                uint16_t u1 = (V1 & 0xf) ^ (v1 & 0xf); 
                uint16_t u2 = (V2 & 0xf) ^ (v2 & 0xf);
                uint16_t u3 = (V3 & 0xf) ^ (v3 & 0xf);
                uint16_t u4 = (V4 & 0xf) ^ (v4 & 0xf); 
                if(((u1 & 0xf ) == 0xF)&&((u2 & 0xf) == 0xF)&&((u3 & 0xf ) == 0x0)&&((u4 & 0xF ) == 0xF)){
                // if(((u2 & 0xf) == 0x6)&&((u4 & 0xF ) == 0x6)){
                    count[a][b][c][d] += 1;
                }

            }
        }
    }
}
return;

}

int main(int argc, const char * argv[]) {
    
    // dif_table();
    // dif_solve();


    FILE * fd;
    fd = fopen("92202.f2b490786a1ce3d5.dat","r");
    int count[16][16][16][16] = {0};
    uint16_t Inputs [65535];
    char buffer[255];
    for(int i = 0; i < 65535; i++){
        fgets(buffer, 255, fd);
        Inputs[i] = strtol(buffer, NULL, 16);
    }
    uint16_t input_difference = 0xFF0F;
        // uint16_t input_difference = 0xFFFF;

    float correct_counter = 0.0f;
    for (uint16_t i = 0; i < 65535; i++)
    {
        for (uint16_t j = 0; j < 65535; j++)
        {
        if((i ^ j) == input_difference){
            correct_counter += 1.0f;
            dif_solve(count,Inputs[i],Inputs[j]);
        }
        }
    }
    
    float max = -1.0f;
    uint16_t subkey;
    for (uint16_t a = 0x0; a <= 0xf; a++)
    {
        for (uint16_t b = 0x0; b <= 0xf; b++)
        {   
            for (uint16_t c = 0x0; c <= 0xf; c++)
                {
                for (uint16_t d = 0x0; d <= 0xf; d++)
                {
                    if(count[a][b][c][d]>max){
                        max = count[a][b][c][d];
                        subkey = ((a & 0xf) << 12) ^
                                ((b & 0xf) << 8) ^
                                ((c & 0xf) << 4) ^
                                ((d & 0xf) << 0);
                    }
                }
            }
        }
    }
    double percetange = max/correct_counter;
    printBIN(subkey, "subkey from last round");
    printf("from all possible pairs (6500^2)/2 , were acceptable: %.0f",correct_counter);
    printf(" probability %.20f \n", percetange);
       



    return 0;
}