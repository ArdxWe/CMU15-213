#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

void printsigset(const sigset_t *pset)
{
    for (int i = 0; i < 64; i++)
    {
        if (sigismember(pset, i + 1))
            putchar('1');
        else
            putchar('0');
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}

void siginthandle(int sig) {
    printf("sigint handle call\n");
    sleep(1);
    return;
}

int main() {
    signal(SIGINT, siginthandle);

    sigset_t sigint, prev;
    sigemptyset(&sigint);
    sigaddset(&sigint, SIGINT);
    sigprocmask(SIG_BLOCK, &sigint, &prev);

    int count = 1;
    while(true) {
        sigset_t now, pending;

        printf("block   set:");
        fflush(stdout);
        sigprocmask(SIG_BLOCK, NULL, &now);
        printsigset(&now);
        sigpending(&pending);
        printf("pending set:");
        fflush(stdout);
        printsigset(&pending);
        sleep(1);
        count++;
        if (count == 5) {
            sigprocmask(SIG_SETMASK, &prev, NULL);
        }
        else if (count == 10) {
            break;
        }
    }
    return 0;
}