#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>
#include <setjmp.h>
jmp_buf jb;
void handler(int n) {longjmp(jb, 1);}
int main(int argc, char *argv[]) {
    long pagesize; char *buffer; char *p;
    if(signal(SIGSEGV, handler) == SIG_ERR) {
        perror("signal()"); return 1;
    }
    if((pagesize = sysconf(_SC_PAGE_SIZE)) == -1) {
        perror("sysconf()"); return 1;
    }
    if((buffer = memalign(pagesize, 4 * pagesize)) == NULL) {
        perror("memalign()"); return 1;
    }
    /* Protectia initiala este PROT_READ | PROT_WRITE */
    if (mprotect(buffer + pagesize * 2, pagesize, PROT_READ) == -1) {
        perror("mprotect()"); return 1;
    }
    printf("Dimensiunea paginii: %ld\nAdresa de start: %p\n", pagesize, buffer);
    if(! setjmp(jb)) for (p = buffer ; ; *p++ = 'x');
    printf("Primit SIGSEGV la adresa: %p\nDeplasamentul este: %ld\n", p, p - buffer);
    return 0;
}