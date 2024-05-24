#include <stdio.h>
#include <ctype.h>

#define MAX_LENGTH 20

void readValidString(char *str, int maxLength) {
    int isValid = 1;
    char ch;
    int i = 0;

    printf("Enter a string (up to %d characters, A-Z, a-z, 0-9 only, no spaces): ", maxLength);

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (isalnum(ch)) {
            if (i < maxLength - 1) {
                str[i++] = ch;
            } else {
                printf("Max length reached. Ignoring extra characters.\n");
            }
        } else {
            isValid = 0;
            printf("Invalid character detected. Only A-Z, a-z, 0-9 are allowed. Please retry.\n");
            while ((ch = getchar()) != '\n' && ch != EOF); // Clear remaining characters in buffer
            break;
        }
    }

    str[i] = '\0';

    if (!isValid) {
        printf("Invalid characters detected. Please only enter A-Z, a-z, 0-9.\n");
    }
}

int main() {
    char inputString[MAX_LENGTH];

    readValidString(inputString, MAX_LENGTH);

    printf("Clean string: %s\n", inputString);

    return 0;
}