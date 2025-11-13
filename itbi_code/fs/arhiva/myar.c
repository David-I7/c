#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <utime.h>
#include <sys/types.h>

char dir_separator = '/';
#ifdef _WIN32
    dir_separator = '\\';
#endif
char* mesaj = "Mod utilizare: %s <ar|unar> <arhiva> <src|dest>\n";
char* default_arc_name = "archive.ar"; 
char archive_path[FILENAME_MAX]= {'\0'};
char real_path[FILENAME_MAX]={"\0"};

char* basename(char* path){
    int len = strlen(path);
    int i = len-1;
    while(i >= 0 && path[i] != dir_separator){
        --i;
    }
    return &path[i + 1];
}

int write_file(FILE* dest,const char *filePath){
    FILE* f = fopen(filePath,"r");
    if(filePath == NULL){return -1;}
    char buf[4096];
    int read_cnt = 0;
    while ((read_cnt = fread(buf,sizeof(char),4096,f)) > 0)
    {
        fwrite(buf,sizeof(char),read_cnt,dest);
    }
    
    fclose(f);
    return 0;
}
off_t min(off_t a,off_t b){
    if (a < b) return a;
    else return b;
}
void write_file2(FILE* dest,FILE* src,off_t size){
    off_t buf_size = 4096;
    if (buf_size > size) buf_size = size;
    char buf[buf_size];
    int read_cnt = 0;
    while (size > 0 && (read_cnt = fread(buf,sizeof(char),min(buf_size,size),src)) > 0)
    {
        fwrite(buf,sizeof(char),read_cnt,dest);
        size -= read_cnt;
    }
}

void concat_path(char* dest,const char* path1,const char* path2){
    if (path1 == NULL && path2 == NULL){
        strcpy(dest,"");
        return;
    }else if(path1 == NULL){
        strcpy(dest,path2);
        return;
    }else if (path2 == NULL){
        strcpy(dest,path1);
        return;
    }
    int len1 = strlen(path1);

    int append_separator = 0;
    if (len1 >= 1 && (path1[len1 -1] != dir_separator)){
        append_separator = 1;
    }
    strcpy(dest,path1);
    if (append_separator)
        strcat(dest,&dir_separator);
    strcat(dest,path2);
}
void concat_path2(char* dest,char* path){
    int len = strlen(dest);
    if (len >= 1 && (dest[len -1] != dir_separator)){
        strcat(&dest[len],&dir_separator);
    }
    strcat(&dest[len],path);
}
int archive_file(FILE* archive, char* file_path, char* archive_path){
    struct stat st;
    if (stat(file_path,&st)){return -1;}

    char* bname = basename(file_path);
    concat_path2(archive_path,bname);
    fwrite(archive_path,sizeof(char),strlen(archive_path) +1,archive);
    fwrite(&st.st_mode,sizeof(mode_t),1,archive);
    fwrite(&st.st_atime,sizeof(time_t),1,archive);
    fwrite(&st.st_mtime,sizeof(time_t),1,archive);
    fwrite(&st.st_size,sizeof(off_t),1,archive);

    write_file(archive,file_path);
    bname = basename(archive_path);
    --bname;
    bname[0] = '\0';
    return 0;
}

int archive_dir(FILE* archive, char* file_path, char* archive_path){
    struct stat st;
    if (stat(file_path,&st)){return -1;}
    
    DIR* dir = opendir(file_path);
    if (dir == NULL){return -1;}

    char* bname = basename(file_path);
    concat_path2(archive_path,bname);
    fwrite(archive_path,sizeof(char),strlen(archive_path) +1,archive);
    fwrite(&st.st_mode,sizeof(mode_t),1,archive);
    fwrite(&st.st_atime,sizeof(time_t),1,archive);
    fwrite(&st.st_mtime,sizeof(time_t),1,archive);
   
    
    struct dirent* dirent;
    while((dirent = readdir(dir)) !=NULL){
        if (strcmp(dirent->d_name,".") == 0 || strcmp(dirent->d_name,"..") == 0) continue;

        concat_path2(file_path,dirent->d_name);
        
        if (stat(file_path,&st) == 0){
            if (S_ISDIR(st.st_mode)){
                archive_dir(archive,file_path,archive_path);
            }else{
                archive_file(archive,file_path,archive_path);
            }
        }else{
            closedir(dir);
            return -1;
        };
        bname = basename(file_path);
        --bname;
        bname[0] = '\0';
    }

    closedir(dir);
    bname = basename(archive_path);
    --bname;
    bname[0] = '\0';
    return 0;
}

int unarchive_dir(FILE* archive,char *archive_rootdir){
    char path[256] = {0},c,*p = &archive_rootdir[strlen(archive_rootdir)];
    int i = 0;
    mode_t mode =0;
    off_t size=0;
    struct utimbuf utim;
    while (!feof(archive)){
        while((fread(&c,sizeof(char),1,archive) && c != '\0')){
            path[i++] = c;
        }
        if (i == 0) break; 
        path[i] = '\0';
        fread(&mode,sizeof(mode_t),1,archive);
        concat_path2(archive_rootdir,path);
        
        if (S_ISDIR(mode)){
            if (mkdir(archive_rootdir, mode & 0xFFF)){
                perror(archive_rootdir);
                exit(EXIT_FAILURE);
            };
            fread(&utim.actime,sizeof(time_t),1,archive);
            fread(&utim.modtime,sizeof(time_t),1,archive);
            utime(archive_rootdir,&utim);
        }else{
            FILE* f;
            if ((f = fopen(archive_rootdir,"wb")) == NULL){
                perror(archive_rootdir);
                exit(EXIT_FAILURE);
            }
            chmod(archive_rootdir,mode & 0xFFF);
            fread(&utim.actime,sizeof(time_t),1,archive);
            fread(&utim.modtime,sizeof(time_t),1,archive);
            fread(&size,sizeof(off_t),1,archive);
            write_file2(f,archive,size);
            fclose(f);
            utime(archive_rootdir,&utim);
        }
        p[0] = 0;
        path[0] = '\0';
        i=0;
    }
    return 0;
}

int unarchive_file(FILE* archive,char *archive_rootdir){
    char path[256] = {0},c,*p = &archive_rootdir[strlen(archive_rootdir)];
    int i = 0;
    mode_t mode =0;
    off_t size=0;
    struct utimbuf utim;
     while (!feof(archive)){
        while((fread(&c,sizeof(char),1,archive) && c != '\0')){
            path[i++] = c;
        }
        if (i > 0){
            
            return -1;
        }; 
        path[i] = '\0';
        fread(&mode,sizeof(mode_t),1,archive);
        concat_path2(archive_rootdir,path);
        
        if (S_ISDIR(mode)){
            return -1;
        }else{
            FILE* f;
            if ((f = fopen(archive_rootdir,"wb")) == NULL){
                perror(archive_rootdir);
                exit(EXIT_FAILURE);
            }
            chmod(archive_rootdir,mode & 0xFFF);
            fread(&utim.actime,sizeof(time_t),1,archive);
            fread(&utim.modtime,sizeof(time_t),1,archive);
            fread(&size,sizeof(off_t),1,archive);
            write_file2(f,archive,size);
            fclose(f);
            utime(archive_rootdir,&utim);
        }
        p[0] = 0;
        path[0] = '\0';
        i=0;
    }
    return 0;
}

int main(int argc, char**argv){
    if(argc != 4){
        fprintf(stderr,mesaj);
        exit(EXIT_FAILURE);
    }

    FILE* ar;
    struct stat st;
    
    if (strcmp(argv[1],"ar") == 0){  
        // archive 
        if(stat(argv[2],&st) == 0){        
            if (S_ISREG(st.st_mode)){
            ar = fopen(argv[2],"w");
            }else{
                // director
                char path[strlen(argv[2]) + strlen(default_arc_name) +2];
                concat_path(path,argv[2],default_arc_name);
                ar = fopen(path,"w");
            }
        }else{
            ar = fopen(argv[2],"w");
            if (ar == NULL) {perror(argv[2]); exit(EXIT_FAILURE);};
        };

        // src
        if(stat(argv[3],&st)){
            perror("stat()");
            exit(EXIT_FAILURE);
        };
        strcat(real_path,argv[3]);
        if (S_ISREG(st.st_mode)){
            archive_file(ar,real_path,archive_path);
        }else{
            archive_dir(ar,real_path,archive_path);
        }

    }else if(strcmp(argv[1],"unar") == 0){
        // archive
        if(stat(argv[2],&st)){
            perror("stat()");
            exit(EXIT_FAILURE);
        };

        if (S_ISREG(st.st_mode)){
            ar = fopen(argv[2],"r");
        }else{
            fprintf(stderr,"%s este un director",argv[2]);
            exit(EXIT_FAILURE);
        }

        // dest
        strcat(real_path,argv[3]);
        if(stat(argv[3],&st) == 0){
            // 
            if (S_ISREG(st.st_mode)){
                unarchive_file(ar,real_path);
            }else{
                unarchive_dir(ar,real_path);
            }
        }else{
            // create file or dir
        };
    }else{
        fprintf(stderr,mesaj);
        exit(EXIT_FAILURE);
    }

    fclose(ar);
    exit(EXIT_SUCCESS);
}