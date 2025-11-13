#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc,char** argv){
    if(argc != 3){
        fprintf(stderr,"Mod utilizare: %s <fisier_sursa> <fisier_dest>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* source = fopen(argv[1],"r");
    if (source == NULL){
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    
    FILE* dest = fopen(argv[2],"r");
    if (dest == NULL){
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    
    struct stat st;
    if (stat(argv[1],&st)){
        perror("stat()");
        exit(EXIT_FAILURE);
    };
    long source_size = st.st_size;
    if (stat(argv[2],&st)){
        perror("stat()");
        exit(EXIT_FAILURE);
    };
    long dest_size = st.st_size;

    if (source_size > dest_size || source_size == 0){
        printf("Numar aparitii: 0\n");
        exit(EXIT_SUCCESS);
    }

    char* source_buf = malloc(sizeof(char) * source_size);
    dest_size = 4096;
    char dest_buf[dest_size];

    fread(source_buf,sizeof(char),source_size,source);

    int read_count=0;
    int offset_source=0;
    int frequency = 0;
    while((read_count = fread(dest_buf,sizeof(char),dest_size,dest)) !=0){
        for(int i = 0; i < read_count;i++){
            if (dest_buf[i] == source_buf[offset_source]) {
                ++offset_source;
            }else if (offset_source != 0){
                offset_source = 0;
                if (dest_buf[i] == source_buf[offset_source])++offset_source;
            };

            if(offset_source == source_size){
                ++frequency;
                offset_source = 0;
            }
        }
    }

    printf("Numar aparitii: %d\n",frequency);
    fclose(source);
    fclose(dest);
    free(source_buf);
    exit(EXIT_SUCCESS);
}
