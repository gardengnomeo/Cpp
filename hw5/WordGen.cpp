#include <sstream>
#include <string>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <fstream>
#include "generate_functions.h"

//state everything that is being used in the code
using std::endl;
using std::cout;
using std::cerr;
using std::map;
using std::ifstream;
using std::string;
using std::stringstream;


int main(int argc, char * argv[]){

    // seed the RNG
    std::srand( ( unsigned int ) std::time( nullptr ) );

    // read command-line arguments
    // arg1: filename
    // arg2: max value of K
    // arg3: num characters to generate (or -m)
    if (4 != argc){
        cerr << "ERROR: Wrong number of command-line arguments (expected 3, received " << (argc-1) << ")" << endl;
        return -1;
    }

    // other error checking, rest of your main code to go here :)
    string filename = argv[1];

    //checks if file able to be opened
    ifstream inputFile(filename);
    if (!inputFile){
        cerr << "ERROR: Unable to open the file"<< endl;
        return -1;
    }

    //error check the k value
    int k;
    stringstream kcheck(argv[2]);
    kcheck >> k;
    if (kcheck.fail() || !kcheck.eof()){
        cerr << "ERROR: value of k must be a number" << endl;
        return -1;
    }
    if (k <= 0){
        cerr << "ERROR: value of k must be larger than 0" << endl;
        return -1;
    }

    
    //error check the n value
    bool print_m = false;
    int n;
    if (string(argv[3]) == "-m"){
        print_m = true;
    } else {
        stringstream ncheck(argv[3]);
        ncheck >> n;
        if (ncheck.fail() || !ncheck.eof()){
            cerr << "ERROR: value of n must be a number or -m" << endl;
            return -1;
        }
        if (n < k){
            cerr << "ERROR: value of n must be larger than or equal to k or be -m" << endl;
            return -1;
        }
    }

    //create map in map
    map<string, map<char, int>> kmap;
    
    //create map for k level and then print it out in JSON format
    if(print_m){
        BuildMap(inputFile, k, kmap);
        printJSON(kmap);
    } else {
        //iterate through the levels until k met
        for(int i = 1; i <= k; i++){
            map<string, map<char, int>> kmap;
            inputFile.clear();
            inputFile.seekg(0);
            string firstkchar = BuildMap(inputFile, i, kmap);
            cout << "Level " << i << ": ";
            generateText(kmap, n, firstkchar);
            if(i < k){
                cout << "~~~" <<endl;
            }
        }
    }
    return 0;
}
