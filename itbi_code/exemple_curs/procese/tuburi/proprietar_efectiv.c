#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int count(uid_t targetUid, const char *path) 
{
    struct stat statBuf;
    DIR *dirPtr;
    struct dirent *dirEntry;
    int pipeFds[2];

    int countResult;
    int savedErrno = errno;
    int childCount;

    /* ---- stat the path ---- */
    if (stat(path, &statBuf) == -1) {
        countResult = -1;
        savedErrno = errno;
        goto err_return;
    }

    /* Count this entry if owned by target user */
    countResult = (statBuf.st_uid == targetUid);

    /* If it's not a directory, return the count for this node only */
    if (!S_ISDIR(statBuf.st_mode))
        goto err_return;

    /* Open directory */
    if ((dirPtr = opendir(path)) == NULL) {
        countResult = -1;
        savedErrno = errno;
        goto err_return;
    }

    /* Create pipe for collecting results from children */
    if (pipe(pipeFds) == -1) {
        countResult = -1;
        savedErrno = errno;
        goto err_close_dir;
    }

    /* ---- Process directory entries ---- */
    while ((dirEntry = readdir(dirPtr)) != NULL) {

        /* Skip . and .. */
        if (!strcmp(dirEntry->d_name, ".") ||
            !strcmp(dirEntry->d_name, ".."))
            continue;

        switch (fork()) {

        case -1:     /* fork failed */
            close(pipeFds[1]);
            countResult = -1;
            savedErrno = errno;
            goto err_wait_children;

        case 0: {    /* child process */

            /* Move into the directory */
            if ((countResult = chdir(path)) == -1)
                perror(path);
            else {
                /* Recurse on the child entry */
                countResult = count(targetUid, dirEntry->d_name);

                if (countResult == -1) {
                    savedErrno = errno;
                    fprintf(stderr, "%s/%s: %s\n",
                            path,
                            dirEntry->d_name,
                            strerror(savedErrno));
                }
            }

            /* Report result to parent */
            write(pipeFds[1], &countResult, sizeof(int));
            exit(2);
        }
        }
    }

    /* ---- Parent collects results ---- */
    close(pipeFds[1]);

    while (read(pipeFds[0], &childCount, sizeof(int)) == sizeof(int)) {
        if (childCount == -1) {
            /* Some child reported an error */
            countResult = -1;
            savedErrno = ESPIPE;  /* arbitrary but signals pipe error */
            goto err_wait_children;
        }
        countResult += childCount;
    }

err_wait_children:
    close(pipeFds[0]);
    while (wait(NULL) != -1);

err_close_dir:
    closedir(dirPtr);

err_return:
    errno = savedErrno;
    return countResult;
}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s director\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int total = count(geteuid(), argv[1]);
    if (total == -1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    printf("Ai %d fisiere\n", total);
    return EXIT_SUCCESS;
}