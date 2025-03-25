#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2]; // File descriptors for the pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) { // First child (Program 1)
        close(fd[0]);  // Close read end of the pipe
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(fd[1]);  // Close the original write end of the pipe

        char *cmd = "./program1";
        char *args[] = {"program1", NULL};
        execvp(cmd, args);
        perror("execvp");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) { // Second child (Program 2)
        close(fd[1]);  // Close write end of the pipe
        dup2(fd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(fd[0]);  // Close the original read end of the pipe

        char *cmd = "./program2";
        char *args[] = {"program2", NULL};
        execvp(cmd, args);
        perror("execvp");
        exit(1);
    }

    // Parent process: Close both ends of the pipe
    close(fd[0]);
    close(fd[1]);

    // Wait for both children to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
