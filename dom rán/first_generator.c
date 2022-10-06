#include <stdio.h>


u_int64_t init(u_int64_t seed){



    return seed;
}

u_int64_t update(u_int64_t state, FILE * fd){
    

    fwrite(&state, 1, 1, fd);
    
    return 0;
}


int main() {
    FILE * fd;
    fd = fopen("output","w+");
    u_int64_t seed = 0xFFFFFFFFFFFFFFFFLL;
    u_int64_t init_seed = init(seed);
    for (long i = 0; i < 1000000; i++)
    {
        init_seed = update(init_seed,fd);
    }
    

    fclose(fd);
   return 0;
}