#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int d; char c; char *buf;
    if(argc != 2) {fprintf(stderr, "Utilizare: %s fisier\n", argv[1]); return 1;}
    if((d = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1){perror(argv[1]); return 1;}
    
    write(d, "01", 2 * sizeof(char));
    close(d);
    if((d = open(argv[1], O_RDWR)) == -1){perror(argv[1]); return 1;}
    
    if((buf = mmap(NULL, 2 * sizeof(char),
    PROT_READ | PROT_WRITE, MAP_SHARED, d, 0)) == MAP_FAILED) {
        perror("mmap"); close(d); return 1;
    }
    while(read(0, &c, sizeof(char)) == sizeof(char)) {
        while(buf[1] != '1'); /* asteptare ocupata */
        buf[0] = c; buf[1] = '0'; /* conteaza ordinea atribuirilor */
    }
    while(buf[1] != '1');
    buf[1] = '2';
    return 0;
}