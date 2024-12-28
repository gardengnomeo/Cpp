#include <sstream>
#include <string>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <fstream>
#include <iterator>


//state everything that is being used in the code
using std::endl;
using std::cout;
using std::map;
using std::ifstream;
using std::string;
using std::stringstream;
using std::next;
using std::rand;

/*
This funcitons creates the map that contains the frtequencies of the letters and what
comes next. It takes in an input file, the number of letters that are the keys, and 
a map that has a map inside it.
It outputs the map inside a map fully filled out with the correct frequencies.
It also returns the first k characters of the file to start the bootstrapping of the
texts for later on.
*/
string BuildMap(ifstream &inputFile, int k, map<string,map<char,int>> &kmap){
    //defien variables needed to sort through funciton
    string temp_string;
    string firstkchar;
    char next_char;

    //loop through input chracter by character
    while(inputFile.get(next_char)){
        
        //create k length string and save the first k
        if( temp_string.length() < static_cast<string::size_type>(k)){
            temp_string += next_char;
            firstkchar += next_char;
            continue;
        }

        //add one to kmap representation and erase the first character
        if(temp_string.length() == static_cast<string::size_type>(k)){
            kmap[temp_string][next_char]++;
            temp_string.erase(0,1);
            temp_string += next_char;
        }
    }

    //"post append" the first k characters to the end of the file so it loops
    for (size_t i = 0; i < firstkchar.length(); i++){
        next_char = firstkchar[i];

        //add one ot kmap representation and erase the first character
        if(temp_string.length() == static_cast<string::size_type>(k)){
            kmap[temp_string][next_char]++;
            temp_string.erase(0,1);
            temp_string += next_char;
        }
    }
    return firstkchar;
}

/*
This function prints out the information in the map within a map in JSON format.
It takes in the map with a map inside of it and it prints out the information in that
map in the JSON format.
*/
void printJSON(const map<string,map<char,int>> &kmap){
    //print opening bracket
    cout<< "{" <<endl;
    
    //iterate through the outer keys
    for(map<string, map<char, int>>::const_iterator it = kmap.begin();
    it != kmap.end(); it++){
        cout<< "    \"" << it->first << "\": {" << endl;

        //iterate through the inner map
        for(map<char,int>::const_iterator innerIt = it->second.begin();
        innerIt != it->second.end(); innerIt++){
            cout<< "        \"" << innerIt->first << "\": " << 
                innerIt->second;
            //checks if last element of not
            if(next(innerIt) != it->second.end()){
                cout << "," << endl;
            } else {
                cout << endl;
            }
        }

        //adds closing and checks if the last element or not
        cout << "   }";
        if(next(it) != kmap.end()){
            cout << "," << endl;
        } else {
            cout << endl;
        }
    }
    //add closing bracket
    cout << "}" << endl;
}

/*
This function prints out text for a certain k level.
It takes in the map with a map inside it that has already been filled with the frequencies,
the number of characters to print, and the first k characters of the input text.
*/
void generateText(const map<string,map<char,int>> &kmap, int n, string firstkchar){
    //define variables
    string generatedText;
    string currentKey = firstkchar;

    //loop through text until n
    for(int i = 0; i < n; i++){
        //finds the current string in the map
        map<string, map<char, int>>::const_iterator it = kmap.find(currentKey);

        //gets the frequencies for the current iterator/string and sets the weight to 0
        const map<char, int> &charMap = it->second;
        int totalFreq = 0;

        //calculate the total frequencies
        for(map<char,int>::const_iterator charIt = charMap.begin(); 
        charIt != charMap.end(); charIt++){
            totalFreq += charIt->second;
        }

        //get a random number between 0 and totalFreq - 1
        int randomVal = rand() % totalFreq;

        //select the next character based off the percentages
        char nextChar = '\0';
        for(map<char,int>::const_iterator charIt = charMap.begin();
        charIt!= charMap.end(); charIt++ ){
            randomVal -= charIt->second;
            if(randomVal < 0){
                nextChar = charIt->first;
                break;
            }
        }

        //correct strings for next iteration
        generatedText += nextChar;
        currentKey.erase(0,1);
        currentKey += nextChar;
    }
    cout << generatedText << endl;
}
