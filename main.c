#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
    creating a linux shell
*/

int main(){
    //first getting user inputs and storing them in a string in a loop
    char *input = NULL;
    size_t size = 0;
    int status = 1;
    while(status){
        printf("myshell> ");
        getline(&input, &size, stdin);
        //printf("%s", input);
        //now we need to parse the input
        char *token = strtok(input, " ");
        char *args[100];
        int i = 0;
        while(token != NULL){
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;
        //now we need to check if the user wants to exit
        if(strcmp(args[0], "exit ") == 0){
            status = 0;
        }
    }

}