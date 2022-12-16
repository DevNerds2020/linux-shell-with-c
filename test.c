#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}

int main(int argc, char **argv) {

    signal(SIGINT, inthand);
    char str[20];

    while(!stop) {
        printf("Enter a string: ");
        fgets(str, 20, stdin);
        printf("%s", str);
    }

    printf("exiting safely\n");
    system("pause");
    return 0;
}