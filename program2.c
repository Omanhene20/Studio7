#include <stdio.h>

int main() {
    char input[256];

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF (CTRL-D)
        }
        printf("Program 2 got: %s", input);
    }

    return 0;
}
