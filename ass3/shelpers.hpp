#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

// Define Command struct to store information required for command execution
struct Command {
    std::string execName; // Command name
    std::vector<const char*> argv; // Command arguments, including the command itself and the terminating nullptr
    int inputFd = STDIN_FILENO; // Input file descriptor, defaulting to standard input
    int outputFd = STDOUT_FILENO; // Output file descriptor, defaulting to standard output
    bool background = false; // Whether to execute the command in the background

    Command() : inputFd(STDIN_FILENO), outputFd(STDOUT_FILENO), background(false) {}
};

// Tokenize the command line string and return a vector containing all the words
std::vector<std::string> tokenize(const std::string& s);

// Construct a list of Command objects based on the provided tokens
std::vector<Command> getCommands(const std::vector<std::string>& tokens);

// Overload the output operator for printing Command objects
std::ostream& operator<<(std::ostream& outs, const Command& c);

// Split a string containing special symbols
bool splitOnSymbol(std::vector<std::string>& words, int i, char c);
