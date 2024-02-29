#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXIT_SUCCESS 0

#define DEFAULT_MAX_COMMAND 100

char* read_command();
char* get_program_name(char str[]);
int main_loop();

int main() {
    printf("> Shell: ");

    main_loop();

    return EXIT_SUCCESS;
}

int main_loop() {
    while (1) {
        char* str = read_command();
        if (strcmp(str, "exit") == 0) {
            return 0;
        }
        printf("> Shell: ");
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

