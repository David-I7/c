#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

char inputLine[256];
char cmd1[256], op[256], cmd2OrFile[256];
char *argvCmd[256];

int main() {
    while (1) {

        printf(">>");
        fgets(inputLine, sizeof(inputLine), stdin);

        // Clear tokens
        cmd1[0] = op[0] = cmd2OrFile[0] = 0;

        // Parse: command operator argument
        sscanf(inputLine, "%s %s %s", cmd1, op, cmd2OrFile);

        // exit command
        if (!strcmp(cmd1, "exit")) {
            return 0;
        }

        // ---------- Handle PIPE: cmd1 | cmd2 ----------
        else if (!strcmp(op, "|")) {

            int pipeFds[2];
            pipe(pipeFds);

            // First child → left side of pipe
            if (!fork()) {
                close(1);          // redirect stdout
                dup(pipeFds[1]);
                close(pipeFds[0]);
                close(pipeFds[1]);

                argvCmd[0] = cmd1;
                argvCmd[1] = NULL;
                execv(cmd1, argvCmd);
                perror(cmd1);
                return 1;
            }

            // Second child → right side of pipe
            else if (!fork()) {
                close(0);          // redirect stdin
                dup(pipeFds[0]);
                close(pipeFds[0]);
                close(pipeFds[1]);

                argvCmd[0] = cmd2OrFile;
                argvCmd[1] = NULL;
                execv(cmd2OrFile, argvCmd);
                perror(cmd2OrFile);
                return 1;
            }

            // Parent waits
            else {
                close(pipeFds[0]);
                close(pipeFds[1]);
                while (wait(NULL) != -1);
            }
        }

        // ---------- Handle regular command (maybe with > redirection) ----------
        else if (fork()) {
            wait(NULL);
        } else {

            // output redirection
            if (!strcmp(op, ">")) {
                int outFd;
                outFd = open(cmd2OrFile,
                             O_WRONLY | O_CREAT | O_TRUNC,
                             S_IRUSR | S_IWUSR);

                if (outFd == -1) {
                    perror(cmd2OrFile);
                    return 1;
                }

                close(1);
                dup(outFd);
                close(outFd);
            }

            argvCmd[0] = cmd1;
            argvCmd[1] = NULL;

            execv(cmd1, argvCmd);
            perror(cmd1);
            return 1;
        }
    }

    return 0;
}