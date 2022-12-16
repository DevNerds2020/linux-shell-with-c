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
    int input;

    while(!stop) {
        printf("Enter a number: ");
        scanf("%d", &input);
        printf("You entered: %d \n", input);
    }

    printf("exiting safely\n");
    system("pause");
    return 0;
}