#include <stdio.h>


#define BUF_SIZE 64

char *int2bin(u_int64_t a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 63; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

void printBIN(u_int64_t a, char * message){
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';
    int2bin(a, buffer, BUF_SIZE - 1);
    printf(" %s",message);
    printf(" a = %s \n\n", buffer);
}

u_int64_t init(u_int64_t seed){



    return seed;
}

u_int64_t update(u_int64_t state, FILE * fd){
    
    state = state ^ state;
    printBIN(state, "one");
    state = state | 1;
    printBIN(state, "two");
    state = state << 3;
    printBIN(state, "three");
    fwrite(&state, 1, 1, fd);
    
    return state;
}




int main() {
    FILE * fd;
    fd = fopen("output","w+");
    u_int64_t seed = 0xFFFFFFFFFFFFFFFFLL;
    //original seed printing
    printBIN(seed,"first");
    u_int64_t init_seed = init(seed);
    u_int64_t tmp = 0x0LL;
    for (long i = 0; i < 1; i++)
    {
        tmp = update(init_seed,fd);
        init_seed = tmp;
    }
    

    fclose(fd);

   return 0;
}