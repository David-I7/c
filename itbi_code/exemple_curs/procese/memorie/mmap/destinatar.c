#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    int d; char *buf;
    if(argc != 2){fprintf(stderr, "Utilizare: %s fisier\n", argv[1]); return 1;}
    if((d = open(argv[1], O_RDWR)) == -1){perror(argv[1]); return 1;}
    
    if((buf = mmap(NULL, 2 * sizeof(char),
    PROT_READ | PROT_WRITE, MAP_SHARED, d, 0)) == MAP_FAILED) {
        perror("mmap"); close(d); return 1;
    }
    do{
        while(buf[1] != '0' && buf[1] != '2');
        if(buf[1] == '0') {write(1, buf, sizeof(char)); buf[1] = '1';}
        else break;
    }while(1);
    return 0;
}