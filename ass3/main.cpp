#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <iostream>
#include "shelpers.hpp"

int main() {
    std::string line;
    while (std::getline(std::cin, line)) { // Loop to read user input
        if (line == "exit") {
            break; // Exit the program if "exit" is entered
        }
        std::vector<std::string> tokens = tokenize(line); // Tokenize the input line
        std::vector<Command> commands = getCommands(tokens); // Get commands based on tokens

        if (commands.empty()) continue; // Overlook empty command
        for (size_t i = 0; i < commands.size(); ++i) {
            Command cmd = commands[i];

            // Part4: cd command
            if (cmd.execName == "cd") {
                if (cmd.argv.size() == 1) {
                    const char* homeDir = getenv("HOME");
                    if (homeDir != nullptr) {
                        std::cout << "Changing directory to HOME: " << homeDir << std::endl;
                        if (chdir(homeDir) != 0) {
                            perror("cd to HOME failed");
                        }
                    } else {
                        std::cerr << "cd: HOME environment variable not set." << std::endl;
                    }
                }
                else if (cmd.argv.size() >= 2) {
                    if (chdir(cmd.argv[1]) != 0) {
                        perror("cd");
                    }
                } else {
                    std::cerr << "cd: missing operand" << std::endl;
                }
                continue;
            }

            // Set up pipe
            int pipefd[2];
            if (i < commands.size() - 1) {
                if (pipe(pipefd) == -1) {
                    perror("pipe error");
                    continue;
                }
                cmd.outputFd = pipefd[1];
                commands[i + 1].inputFd = pipefd[0];
            }

            // Redirection and command execution
            pid_t pid = fork();
            if (pid == 0) { // Child process
                // Input redirection
                if (cmd.inputFd != STDIN_FILENO) {
                    dup2(cmd.inputFd, STDIN_FILENO);
                    close(cmd.inputFd);
                }
                // Output redirection
                if (cmd.outputFd != STDOUT_FILENO) {
                    dup2(cmd.outputFd, STDOUT_FILENO);
                    close(cmd.outputFd);
                }

                // Change argv to fit the format of execvp
                std::vector<char *> argv;
                for (const auto &arg: cmd.argv) {
                    argv.push_back(const_cast<char *>(arg)); // Remove const to fit execvp
                }
                argv.push_back(nullptr); // Ensure argv ends with nullptr

                // Use execvp to execute the command
                execvp(cmd.execName.c_str(), argv.data());
                perror("execvp");
                exit(EXIT_FAILURE);
            } else if (pid > 0) { // Parent process
                int status;
                if (!cmd.background) {
                    waitpid(pid, &status, 0);
                } else {
                    //////////////////////////////////////    Background //////////////////////////
                    std::cout << "Started background process with PID: " << pid << std::endl;
                }

                // Close unused pipe
                if (i < commands.size() - 1) {
                    close(pipefd[1]); // Close the write end as the child process has already copied it
                }
                if (i > 0) {
                    close(commands[i - 1].inputFd); // Close the input end of the previous command
                }
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }

        // Part5: Environment variable setting
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string varName = line.substr(0, eqPos);
            std::string varValue = line.substr(eqPos + 1);
            if (setenv(varName.c_str(), varValue.c_str(), 1) == -1) {
                perror("setenv");
            }
        }
    }
    return 0;
}
