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

volatile sig_atomic_t signalFlag;

int checkIsCustomInput(char *input){
    //if we have a single string and the string is a custom command then return -1
    if(strchr(input, ' ') == NULL){
        if(strcmp(input, "a") == 0 || strcmp(input, "b") == 0 || strcmp(input, "c") == 0 || strcmp(input, "d") == 0 || strcmp(input, "f") == 0 || strcmp(input, "g") == 0){
            return -1;
        }
    }
    //get the first string in the input
   char delimiter[] = " ";
   char *firstWord, *context;
   firstWord = strtok_r(input, delimiter, &context);
//    printf("%s ", firstWord);
    //check if the first string is a custom command
    if(strcmp(firstWord, "a") == 0 || strcmp(firstWord, "b") == 0 || strcmp(firstWord, "c") == 0 || strcmp(firstWord, "d") == 0 || strcmp(firstWord, "f") == 0 || strcmp(firstWord, "g") == 0){
        //TODO check if we have | in the input for piping and dividing the input for example a file.txt | b | c
        return 1;
    }
    return 0;
}

//get file name 
char *getFileName(char *input){
    //get the last string in the input
    while(strchr(input, ' ') != NULL){
        input = strchr(input, ' ') + 1;
    }
    //delete space from the end of the string
    input[strlen(input) - 1] = '\0';
    //get the file name
    char *fileName = input;
    return fileName;
}

//a command function which will search in a file lines and returns first word that is divided by space in each line
void aCommand(char *input){
    //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    //read the file line by line
    char *line = NULL;
    size_t size = 0;
    while(getline(&line, &size, file) != -1){
        //get the first string in the line
        char *firstString = strtok(line, " ");
        //if the first string is not null
        if(firstString != NULL){
            //if the first string is equal to the word that we are searching for
            //print the first string
            printf("result of command a => %s \n", firstString);
            break;
        }
    }
    //close the file
    fclose(file);
}

//g command function which will show first 10 lines of a file
void gCommand(char *input){
   //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    int count = 0;
    while(getline(&line, &size, file) != -1){
        //print the line
        printf("%s", line);
        count++;
        if(count == 10){
            break;
        }
    }
    //close the file
    fclose(file);
}

//f command function which will show number of lines in a file
void fCommand(char *input){
    //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    int count = 0;
    while(getline(&line, &size, file) != -1){
        count++;
    }
    //print the number of lines
    printf("%d lines \n", count);
    //close the file
    fclose(file);
}

//d command function which shows lines that aren't comment # in a file
void dCommand(char *input){
    //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    while(getline(&line, &size, file) != -1){
        //check if the line is a comment or haves a comment any where in it
        if(strchr(line, '#') == NULL){
            //if it is not a comment then print the line
            printf("%s", line);
        }
        //if it has a comment in it then print the line before the comment
        else{
            //get the index of the comment
            int index = strchr(line, '#') - line;
            //print the line before the comment
            for(int i = 0; i < index; i++){
                printf("%c", line[i]);
            }
            printf(" \n");
        }
        // if(line[0] != '#'){
        //     //if it is not a comment then print the line
        //     printf("%s", line);
        // }
    }
    //close the file
    fclose(file);
}
//b command function which will print the most frequent word in a file
void bCommand(char *input){
    //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    //create a hash table to store the words and their frequencies
    char *words[100];
    int frequencies[100];
    int count = 0;
    while(getline(&line, &size, file) != -1){
        //get the first string in the line
        char *firstString = strtok(line, " ");
        //check if the word is already in the hash table
        int flag = 0;
        for(int i = 0; i < count; i++){
            if(strcmp(words[i], firstString) == 0){
                //if it is in the hash table then increase its frequency
                frequencies[i]++;
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            //if it is not in the hash table then add it to the hash table
            words[count] = firstString;
            frequencies[count] = 1;
            count++;
        }
    }
    //find the most frequent word
    int max = 0;
    int index = 0;
    for(int i = 0; i < count; i++){
        if(frequencies[i] > max){
            max = frequencies[i];
            index = i;
        }
    }
    //print the most frequent word
    printf("%s\n", words[index]);
    //close the file
    fclose(file);
}

//c command function which will delete all the spaces in a file and print all the words in a file without spaces
void cCommand(char *input){
    //get the file name from getFileName function
    char *fileName = getFileName(input);
    //open the file
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    while(getline(&line, &size, file) != -1){
        //get the first string in the line
        char *firstString = strtok(line, " ");
        //if the first string is not null
        if(firstString != NULL){
            //print the first string
            printf("%s", firstString);
        }
        //get the rest of the strings in the line
        while(firstString != NULL){
            firstString = strtok(NULL, " ");
            if(firstString != NULL){
                //print the rest of the strings
                printf("%s", firstString);
            }
        }
    }
    //close the file
    fclose(file);
}

//ctrl + c handler
void ctrlCHandler(int signum){
    // printf("here is ctrl + c");
    signalFlag = 1;
    system(" ");   
    return;
}

//arrow up handler
void arrowUpHandler(int signum){
    printf("here is arrow up");
    signalFlag = 2;
    return;
}

//arrow down handler
void arrowDownHandler(int signum){
    printf("here is arrow down");
    signalFlag = 3;
    return;
}

int main(int argc, char **argv){
    //array for storing the commands history
    char *history[1000];
    int historyCount = 0;
    //list of custom commands including =>  a,b,c,d,f,g in array
    char *commands[7] = {"a", "b", "c", "d", "f", "g"};
    //first getting user inputs and storing them in a string in a loop
    char *input = NULL;
    size_t size = 0;
    // int status = 1;

    //listen to ctrl + c signal if it is pressed then run loop again
    signal(SIGINT, ctrlCHandler);

    //listen to arrow up signal if it is pressed then print the last command
    signal(SIGTSTP, arrowUpHandler);
    //listen to arrow down signal if it is pressed then print the next command
    signal(SIGQUIT, arrowDownHandler);

    while(!signalFlag){
        //get current directory
        //history command default is ""
        // char *historyCommand = "";
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("myshell> %s: ", cwd);
        getline(&input, &size, stdin);
        //if input is exit then exit the program
        if(strcmp(input, "e") == 0){
            return 0;
        }
        // printf("input =>>>> %s", input);
        //copy input for not changing value
        char inputCopy[100];
        strcpy(inputCopy, input);
        //check if input is one of the custom commands
        int flag = checkIsCustomInput(inputCopy);
        // printf("flag =>>>> %d ", flag);
        if (flag == -1){
            //print please enter file name
            printf("please enter file name \n");
            continue;
        }else if(flag == 0){
            //fork and exec
            pid_t pid = fork();
            if(pid == 0){
                // printf("linux command =>>>> %s ", input);
                //child process
                //exec all of command with system
                system(input);
                //add the command to history
                history[historyCount] = input;
                historyCount++;
                //we can use execvp to run linux commands
                //execvp(input, NULL);
            }else{
                //parent process
                //wait for child process to finish waitpid
                waitpid(pid, NULL, 0);
                continue;
            }
        }else{
           //create a process for running the command
            pid_t pid = fork();
            if(pid == 0){
                char inputCopy[100];
                strcpy(inputCopy, input);
                char *firstString = strtok(inputCopy, " ");
                //check which command is it 
                if(strcmp(firstString, "a") == 0){
                    //run a command
                    // printf("%s =>>>", input);
                    aCommand(input);
                    //add the command to history
                    history[historyCount] = input;
                    historyCount++;
                }else if(strcmp(firstString, "b") == 0){
                    //run b command
                    bCommand(input);
                    //add the command to history
                    history[historyCount] = input;
                    historyCount++;
                }else if(strcmp(firstString, "c") == 0){
                    //run c command
                    cCommand(input);
                    history[historyCount] = input;
                    historyCount++;
                    // printf("not implemented yet");
                }else if(strcmp(firstString, "d") == 0){
                    //run d command
                    dCommand(input);
                    //add the command to history
                    history[historyCount] = input;
                    historyCount++;
                }else if(strcmp(firstString, "f") == 0){
                    //run f command
                    fCommand(input);
                    //add the command to history
                    history[historyCount] = input;
                    historyCount++;
                }else if(strcmp(firstString, "g") == 0){
                    //run g command
                    gCommand(input);
                    //add the command to history
                    history[historyCount] = input;
                    historyCount++;
                }
            }else{
                //parent process
                //wait for child process to finish waitpid
                waitpid(pid, NULL, 0);
                continue;
            }
        }
    }
    // printf("here out of while");
    //refresh the code with system()
    system("clear");
}

