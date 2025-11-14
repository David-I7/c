#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

sigjmp_buf jumpBuffer;
// trebuie sa folosim sigjmp_buf pt ca altfel semnalul ar ramane blocat (semnalul este blocat cand intri in handler [default] si deblocat cand iesi pt a permite alte semnale n sa ruleze)
void sigpipeHandler(int sig) {
    signal(sig, sigpipeHandler);
    siglongjmp(jumpBuffer, 1);
}

int main() {
    char personalFifoName[11], peerFifoName[11], inputBuffer[PATH_MAX];
    char *messageTextPtr, readChar;
    uint8_t messagePacket[PIPE_BUF];
    int readFd, peerFd[10], peerCount, maxMessageLen, peerIndex;
    pid_t childPid;

    printf("Dati tubul propriu (max.10 car.): ");
    fgets(inputBuffer, PATH_MAX, stdin);
    sscanf(inputBuffer, "%10s", personalFifoName);
    fflush(stdin);

    if (mkfifo(personalFifoName, S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH) == -1) {
        perror(personalFifoName);
        return -1;
    }

    strcpy((char *)messagePacket, personalFifoName);
    strcat((char *)messagePacket, ": ");

    maxMessageLen =
        (PIPE_BUF - strlen((char *)messagePacket) * sizeof(char)) / sizeof(char);

    messageTextPtr =
        (char *)(messagePacket + strlen((char *)messagePacket));

    if (fork()) {
        readFd = open(personalFifoName, O_RDONLY);
    } else {
        readFd = open(personalFifoName, O_WRONLY);
        return 1;
    }

    open(personalFifoName, O_WRONLY);
    printf("Tubul propriu este: %s\n", realpath(personalFifoName, inputBuffer));

    printf("Dati tuburile adverse (max.10), unul pe linie, o linie goala la sfarsit:\n");

    for (peerCount = 0; peerCount < 10; ++peerCount) {
        fgets(inputBuffer, PATH_MAX, stdin);
        if (strcmp(inputBuffer, "\n") == 0) break;

        sscanf(inputBuffer, "%10s", peerFifoName);
        fflush(stdin);

        if ((peerFd[peerCount] = open(peerFifoName, O_WRONLY)) == -1) {
            perror(peerFifoName);
            --peerCount;
        }
    }

    printf("Incepe conversatia:\n");

    if ((childPid = fork())) {
        signal(SIGPIPE, sigpipeHandler);

        while (peerCount && fgets(messageTextPtr, maxMessageLen, stdin) != NULL) {
            fflush(stdin);

            if (messageTextPtr[strlen(messageTextPtr) - 1] != '\n')
                messageTextPtr[strlen(messageTextPtr) - 1] = '\n';

            for (peerIndex = 0; peerIndex < peerCount; ++peerIndex) {
                if (sigsetjmp(jumpBuffer, 1)) {
                    close(peerFd[peerIndex]);
                    --peerCount;
                    if (peerIndex < peerCount) {
                        // swapback array
                        peerFd[peerIndex] = peerFd[peerCount];
                        --peerIndex;
                    }
                } else {
                    write(peerFd[peerIndex], messagePacket,
                          strlen((char *)messagePacket));
                }
            }
        }

        kill(childPid, SIGKILL);
        while (wait(NULL) != -1);
        unlink(personalFifoName);

    } else {
        while (1) {
            read(readFd, &readChar, sizeof(char));
            write(1, &readChar, sizeof(char));
        }
    }

    return 0;
}