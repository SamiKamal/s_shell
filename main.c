#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// @TODO:
// - Ensure we check for errors

#define EXIT_SUCCESS 0

#define DEFAULT_MAX_COMMAND 100

char* read_command();
char* get_program_name(char str[]);
char** get_arguments(char str[]);
void cleanup_main_loop(char* command, char* name, char** args);
int main_loop();

int main() {
    printf("> Shell: ");

    main_loop();

    return EXIT_SUCCESS;
}

int main_loop() {
    while (1) {
        char* command = read_command();
        char* p_name = get_program_name(command);
        char** p_args = get_arguments(command);

        int status = 0;

        int pid = 0;
        if (strcmp(p_name, "exit") == 0) {
            cleanup_main_loop(command, p_name, p_args);
            exit(EXIT_SUCCESS);
        }

        if (strcmp(p_name, "cd") == 0) {
            chdir(p_args[1]);
        }
        else {
            pid = fork();
            if (pid < 0) {
                perror("fork fail");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                execvp(p_name, p_args);
            }
            int result = waitpid(pid, &status, 0);
            if (result < 0) {
                perror("waiting for child failed");
                exit(EXIT_FAILURE);
            }
        }

        printf("> Shell: ");

        cleanup_main_loop(command, p_name, p_args);
    }
}

char* read_command() {
    char* str = (char*)calloc(DEFAULT_MAX_COMMAND, sizeof(char));
    char ch = 0;
    int i = 0;

    while ((ch = getchar()) != '\n') {
        // @TODO: handle when user goes over DEFUALT_MAX_COMMAND
        str[i++] = ch;
    }
    str[i] = '\0';

    return str;
}

char* get_program_name(char str[]) {
    char* temp_str = (char*)calloc(strlen(str) + 1, sizeof(char));
    if (temp_str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(temp_str, str);
    char* ptr = strtok(temp_str, " ");

    return ptr;
}

char** get_arguments(char str[]) {
    char* temp_str = (char*)calloc((strlen(str) + 1), sizeof(char));
    if (temp_str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(temp_str, str);

    int arg_count = 1; // Start with 1 to account for the program name
    for (int i = 0; temp_str[i] != '\0'; i++) {
        if (temp_str[i] == ' ') {
            arg_count++;
        }
    }

    char** args = (char**)calloc((arg_count + 1), sizeof(char*)); // +1 for NULL pointer
    if (args == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    args[0] = strtok(temp_str, " "); // Extract the program name
    for (int i = 1; i < arg_count; i++) {
        args[i] = strtok(NULL, " "); // Extract subsequent arguments
    }
    args[arg_count] = NULL; // Null-terminate the array

    return args;
}

void cleanup_main_loop(char* command, char* name, char** args) {
    free(command);
    free(name);
    free(args[0]);
    free(args);
}
