#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>


void siginthandle(int sig) {
    printf("sigint handle call\n");
    unsigned int rs = sleep(5);
    printf("remain seconds: %d\n", rs);
    return;
}

void sigtstphandle(int sig) {
    printf("sigtstp handle call\n");
    sleep(1);
    return;
}

int main() {
    signal(SIGINT, siginthandle);
    signal(SIGTSTP, sigtstphandle);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTSTP);

    unsigned int rs = sleep(5);
    printf("main routine remain seconds: %d\n", rs);
    return 0;
}