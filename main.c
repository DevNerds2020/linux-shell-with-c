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
volatile sig_atomic_t status = 1;

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
        fprintf(stderr, "File not found \n");
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
            // break;
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
        fprintf(stderr, "File not found \n");
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
        fprintf(stderr, "File not found \n");
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
        fprintf(stderr, "File not found \n");
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
        fprintf(stderr, "File not found \n");
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
        fprintf(stderr, "File not found \n");
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

//e command function which will end the program
void eCommand(){
    status = 0;
    signalFlag = 1;
    exit(0);
}

//ctrl + c handler
void ctrlCHandler(int signum){
    signalFlag = 1;
}

//write the input to history.txt file
void writeHistory(char *input){
    //open the file
    FILE *file = fopen("history.txt", "a");
    if(file == NULL){
        fprintf(stderr, "File not found \n");
        return;
    }
    //write the input to the file
    fprintf(file, "%s", input);
    //close the file
    fclose(file);
}

//read the history from a file and print it
void printHistory(){
    //open the file
    FILE *file = fopen("history.txt", "r");
    if(file == NULL){
        fprintf(stderr, "File not found \n");
        return;
    }
    char *line = NULL;
    size_t size = 0;
    //read the file line by line and print it
    while(getline(&line, &size, file) != -1){
        printf("%s", line);
    }
    printf("\n");
    //close the file
    fclose(file);
}

//get the target line from history.txt file
char *getTargetLine(int targetLine){
    //open the file
    FILE *file = fopen("history.txt", "r");
    if(file == NULL){
        fprintf(stderr, "File not found \n");
        return NULL;
    }
    char *line = NULL;
    size_t size = 0;
    //get the target line number
    int count = 0;
    //read the file line by line and get the target line
    while(getline(&line, &size, file) != -1){
        count++;
        if(count == targetLine){
            //close the file
            fclose(file);
            return line;
        }
    }
    //close the file
    fclose(file);
    return NULL;
}

//get history.txt line count
int getLineCount(){
    //open the file
    FILE *file = fopen("history.txt", "r");
    if(file == NULL){
        fprintf(stderr, "File not found \n");
        return 0;
    }
    char *line = NULL;
    size_t size = 0;
    int count = 0;
    //read the file line by line and get the line count
    while(getline(&line, &size, file) != -1){
        count++;
    }
    //close the file
    fclose(file);
    return count;
}

//a function which will check if the custom command is a pipe command or not
int isPipeCommand(char *input){
    //check if the input contains a pipe
    if(strchr(input, '|') != NULL){
        return 1;
    }
    return 0;
}


void aCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    char *command = "bash a.sh ";
    //copy the into a new string
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
} 

void bCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    //run bscript.sh script
    char *command = "bash b.sh ";
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
}

void cCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    //run bscript.sh script
    char *command = "bash c.sh ";
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
}

void dCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    //run bscript.sh script
    char *command = "bash d.sh ";
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
}

void fCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    //run bscript.sh script
    char *command = "bash f.sh ";
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
}

void gCommandLinuxLike(char *input){
    char *fileName = getFileName(input);
    //run bscript.sh script
    char *command = "bash g.sh ";
    char *newString = malloc(strlen(command) + strlen(fileName) + 1);
    //add the command and the file name to the new string
    strcpy(newString, command);
    strcat(newString, fileName);
    system(newString);
    return;
}


void runCommand(char *input){
    char inputCopy[100];
    strcpy(inputCopy, input);
    char *firstString = strtok(inputCopy, " ");
    //check which command is it 
    if(strcmp(firstString, "a") == 0){
        //run a command
        // printf("%s =>>>", input);
        aCommand(input);
    }else if(strcmp(firstString, "b") == 0){
        //run b command
        bCommand(input);
    }else if(strcmp(firstString, "c") == 0){
        //run c command
        cCommand(input);
    }else if(strcmp(firstString, "d") == 0){
        //run d command
        dCommand(input);
    }else if(strcmp(firstString, "f") == 0){
        //run f command
        fCommand(input);
    }else if(strcmp(firstString, "g") == 0){
        //run g command
        gCommand(input);
    }
    return;
}

void runCommandWithLinuxHelp(char *input){
    char inputCopy[100];
    strcpy(inputCopy, input);
    char *firstString = strtok(inputCopy, " ");
    //check which command is it 
    if(strcmp(firstString, "a") == 0){
        //run a command
        // printf("%s =>>>", input);
        aCommandLinuxLike(input);
    }else if(strcmp(firstString, "b") == 0){
        //run b command
        bCommandLinuxLike(input);
    }else if(strcmp(firstString, "c") == 0){
        //run c command
        cCommandLinuxLike(input);
    }else if(strcmp(firstString, "d") == 0){
        //run d command
        dCommandLinuxLike(input);
    }else if(strcmp(firstString, "f") == 0){
        //run f command
        fCommandLinuxLike(input);
    }else if(strcmp(firstString, "g") == 0){
        //run g command
        gCommandLinuxLike(input);
    }
    return;
}

void runPipeCommand(char *input){
//divide the input it can be more than 2 commands
    char *commands[100];
    char *token = strtok(input, "|");
    int i = 0;
    while(token != NULL){
        commands[i] = token;
        token = strtok(NULL, "|");
        i++;
    }
    //run the commands
    char fullCommand[200] = "bash ";
    for(int j = 0; j < i; j++){
        if (j == 0){
            char command = commands[0][0];
            char commandWithSh[5] = {command, '.', 's', 'h'};
            strcat(fullCommand, commandWithSh);
            char *fileName = commands[0] + 2;
            strcat(fullCommand, " ");
            strcat(fullCommand, fileName);
        }else {
            char targetCommand[200] = "| bash ";
            char command = commands[j][1];
            char commandWithSh[5] = {command, '.', 's', 'h'};
            strcat(targetCommand, commandWithSh);
            strcat(fullCommand, targetCommand);
        }
    }
    system(fullCommand);
    exit(0);
    return;
}

int shellCore(){
    signalFlag = 0;
    //list of custom commands including =>  a,b,c,d,f,g in array
    char *commands[7] = {"a", "b", "c", "d", "f", "g"};
    //first getting user inputs and storing them in a string in a loop
    char *input = NULL;
    // char input[1000];
    size_t size = 0;
    // int status = 1;

    //listen to ctrl + c signal if it is pressed then run loop again
    signal(SIGINT, ctrlCHandler);
    int historyLine = 0;
    

    while(!signalFlag){
        //get current directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("myshell> %s: ", cwd);
        
        //get the input if it is not null then it is from the history and it has a default value else it is from the user
        getline(&input, &size, stdin);
      
        //get arrow up and down inputs for changing the history
        if(strcmp(input, "\033[A\n") == 0){
            //arrow up
            //get the line count
            int lineCount = getLineCount();
            //get the target line
            char *targetLine = getTargetLine(lineCount - historyLine);
            if(targetLine != NULL){
                //print the target line
                // printf("%s", targetLine);
                //copy the target line to input
                strcpy(input, targetLine);
                //increase the history line
                historyLine++;
            }
            continue;
        }else if(strcmp(input, "\033[B\n") == 0){
            //arrow down
            //get the line count
            int lineCount = getLineCount();
            //get the target line
            char *targetLine = getTargetLine(lineCount - historyLine);
            if(targetLine != NULL){
                //print the target line
                // printf("%s", targetLine);
                //copy the target line to input
                strcpy(input, targetLine);
                //decrease the history line
                historyLine--;
            }
            continue;
        }
        //if input is e then end the program
        if(strcmp(input, "e\n") == 0){
            eCommand();
            break;
        }
        //if input is h then print the history of commands
        if(strcmp(input, "h\n") == 0){
            printHistory();
            continue;
        }
        char inputCopy[100];
        strcpy(inputCopy, input);
        //check if input is one of the custom commands
        int flag = checkIsCustomInput(inputCopy);
        // printf("flag =>>>> %d ", flag);
        if (flag == -1){
            //print please enter file name stderror
            fprintf(stderr, "Please enter file name \n");
            continue;
        }else if(flag == 0){
            //fork and exec
            pid_t pid = fork();
            if(pid == 0){
                // printf("linux command =>>>> %s ", input);
                //child process
                //exec all of command with system
                system(input);
                //we can use execvp to run linux commands
                //execvp(input, NULL);
                writeHistory(input);
                exit(0);
            }else{
                //parent process
                //wait for child process to finish waitpid
                waitpid(pid, NULL, 0);
                historyLine = 0;
                continue;
            }
        }else{
           //create a process for running the command
            pid_t pid = fork();
            if(pid == 0){
                //if the command is a pipe command
                if(isPipeCommand(input) == 1){
                    runPipeCommand(input);
                }
                // runCommand(input);
                runCommandWithLinuxHelp(input);
                writeHistory(input);
                exit(0);
            }else{
                //parent process
                //wait for child process to finish waitpid
                waitpid(pid, NULL, 0);
                historyLine = 0;
                continue;
            }
        }
    }
}

int main(int argc, char **argv){
    while(status){
        status = shellCore();
    }
}

