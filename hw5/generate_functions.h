#ifndef GENERATE_FUNCTIONS
#define GENERATE_FUNCTIONS

#include <string>
#include <map>

/* Builds a map for k-length substrings and their corresponding character frequencies from an input file */
std::string BuildMap(std::ifstream &inputFile, int k, std::map<std::string, std::map<char, int>> &kmap);

/* Prints the nested map structure in a JSON-like format */
void printJSON(const std::map<std::string, std::map<char, int>> &kmap);

/* Generates text of length `n` based on the character frequency map */
void generateText(const std::map<std::string, std::map<char, int>> &kmap, int n, std::string firstkchar);
    
#endif
