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

int checkIsCustomInput(char *input){
    if(input == "a" || input == "b" || input == "c" || input == "d" || input == "f" || input == "g"){
        return 1;
    }
    return 0;
}

//a command function which will search in a file lines and returns first string that is divided by space in each line
void a(char *input){
    //get the file name
    char *fileName = strtok(input, " ");
    //get the string to search
    char *stringToSearch = strtok(NULL, " ");
    //open the file
    FILE *file = fopen(fileName, "r");
    //read the file line by line
    char *line = NULL;
    size_t size = 0;
    while(getline(&line, &size, file) != -1){
        //get the first string in the line
        char *firstString = strtok(line, " ");
        //compare the first string with the string to search
        if(strcmp(firstString, stringToSearch) == 0){
            //if they are equal then print the line
            printf("%s", line);
            break;
        }
    }
    //close the file
    fclose(file);
}

int main(){
    //list of custom commands including =>  a,b,c,d,f,g in array
    char *commands[7] = {"a", "b", "c", "d", "f", "g"};
    //first getting user inputs and storing them in a string in a loop
    char *input = NULL;
    size_t size = 0;
    int status = 1;
    while(status){
        //get current directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("myshell>%s: ", cwd);
        getline(&input, &size, stdin);
        //print input
        printf("input =>>>> %s", input);
        //if input is not one of the custom commands then we need to fork and exec with system and let it be handled by linux terminal
        //check if input is one of the custom commands
        int flag = checkIsCustomInput(input);
       
        //if flag is 0 
        if(flag == 0){
            //fork and exec
            pid_t pid = fork();
            if(pid == 0){
                printf("%s =>>>> ", input);
                //child process
                //exec
                execl("/bin/sh", "sh", "-c", input, NULL);
            }else{
                //parent process
                //wait
                wait(NULL);
                continue;
            }
        }else{

        }
    }
}

