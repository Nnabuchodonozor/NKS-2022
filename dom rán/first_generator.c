#include <stdio.h>


#define BUF_SIZE 64
#define PRINTING 0

char *int2bin(u_int64_t a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 63; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

void printBIN(u_int64_t a, char * message){
    if (PRINTING==1)
    {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';
    int2bin(a, buffer, BUF_SIZE - 1);
    printf(" %s",message);
    printf(" a = %s \n\n", buffer);
    }
}

u_int64_t init(u_int64_t seed){



    return seed;
}

u_int64_t update(u_int64_t state, FILE * fd,long i){
    
// largest 64b prime = 18446744073709551557     .. will i need it?  idfc

    long modulo = i % 63;
    u_int64_t tmp = 1 ^ state;
    printBIN(state, "0");
    state >>= (modulo % 7);
    printBIN(state, "1");
    state = state | tmp;
    printBIN(state, "2");
    tmp = ~tmp; 
    printBIN(state, "3");
    tmp <<= 1;
    printBIN(state, "4");
    state  = state ^ tmp;
    printBIN(state, "5");
    state >>= 1;
    printBIN(state, "6");
    tmp = tmp & state;
    printBIN(state, "7");
    state <<= 3;
    printBIN(state, "8");
    state = state | tmp;
    printBIN(state, "9");
    tmp &= tmp;
    printBIN(state, "a");
    state =state | state; 
    printBIN(state, "b");
    state |=state;
    printBIN(state, "c");
    tmp >>= 3;
    printBIN(state, "d");
    state <<= 15;
    printBIN(state, "e");
    state = state ^ tmp;
    printBIN(state, "f");
    fwrite(&state, 1, 1, fd);
    return state;
}




int main() {
    FILE * fd;
    fd = fopen("output","wb");
    u_int64_t seed = 0xFFFFFFFFFFFFFFFFLL;
    //original seed printing
    printBIN(seed,"first");
    u_int64_t init_seed = init(seed);
    u_int64_t tmp = 0x0LL;
    for (long i = 0; i < 1000000; i++)
    {
        tmp = update(init_seed,fd,i);
        init_seed = tmp;
     
        if ((i % 100000 )== 0)
        {
           
       char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';
    int2bin(init_seed, buffer, BUF_SIZE - 1);
    printf(" last = %s \n\n", buffer);

        }
        

    }
    
    fclose(fd);

   return 0;
}