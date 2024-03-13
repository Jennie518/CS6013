/*
 Author: Yijun
 Description:piping lab
 Date:2/5/2024
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

#define MESSAGE_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) { //2 arguements because the first one is program itself
        std::cerr << "Usage: " << argv[0] << " <message>\n";
        exit(1);
    }
    
    //must create pipe before fork
    //both parent and child can access them
    int pipe_fd[2];
    pid_t child_pid;
    char message[MESSAGE_SIZE];
    ssize_t bytes_read;

    int readFd = pipe_fd[0];
    int writeFd = pipe_fd[1];
    
    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0) {
        // Child process
        close(writeFd); // Close the write end of the pipe in the child

        // Read the message length from the pipe
        size_t message_length;
        if (read(readFd, &message_length, sizeof(message_length)) == -1) {
            perror("Child read failed");
            exit(1);
        }

        // Read the message from the pipe
        bytes_read = read(readFd, message, message_length);
        if (bytes_read == -1) {
            perror("Child read failed");
            exit(1);
        }

        std::cout << "Child received message: " << std::string(message, bytes_read) << std::endl;

        close(readFd); // Close the read end of the pipe in the child
        exit(0);
    } else {
        // Parent process
        close(readFd); // Close the read end of the pipe in the parent

        // Send the length of the message to the child
        size_t message_length = strlen(argv[1]) + 1;
        if (write(writeFd, &message_length, sizeof(message_length)) == -1) {
            perror("Parent write failed");
            exit(1);
        }

        // Send the message to the child
        if (write(writeFd, argv[1], message_length) == -1) {
            perror("Parent write failed");
            exit(1);
        }

        close(pipe_fd[1]); // Close the write end of the pipe in the parent

        std::cout << "Parent sent message to child: " << argv[1] << std::endl;

        // Wait for the child to exit
        if (wait(NULL) == -1) {
            perror("Wait failed");
            exit(1);
        }
    }

    return 0;
}

