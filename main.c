#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0

#define DEFAULT_MAX_COMMAND 100

char* read_command(char str[]);
char* parse_command(char str[]);
int main_loop();

int main() {
    printf("> Shell: ");

    main_loop();

    return EXIT_SUCCESS;
}

int main_loop() {
    while (1) {
        char* str = (char*)calloc(DEFAULT_MAX_COMMAND, sizeof(char));
        read_command(str);
        if (strcmp(str, "exit") == 0) {
            return 0;
        }
        printf("> Shell: ");
    }
}

char* read_command(char str[]) {
    char ch = 0;
    int i = 0;

    while ((ch = getchar()) != '\n') {
        // @TODO: handle when user goes over DEFUALT_MAX_COMMAND
        str[i++] = ch;
    }
    str[i] = '\0';

    return str;
}
