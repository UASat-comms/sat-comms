#include <iostream>
#include <fstream>
#include <sstream>
#include "picosha2.h"

using namespace std;

std::string getSHA256Hash(const char *str) {
  std::stringstream s;
  s << str;
  return picosha2::hash256_hex_string(s.str());
}

std::string getSHA256Hash(std::string str) {
  return picosha2::hash256_hex_string(str);
}


int main(int argc, const char **argv) {
    if(argc < 2) {
        cout << "Enter a filename!" << endl;
        return 1;
    }

    ifstream myfile(argv[1]);
    string mystr;
    char c;

    if(myfile.is_open()) {
        while(!myfile.eof()) {
            myfile.get(c);
            mystr.push_back(c);
        }
        myfile.close();
    } else {
        cout << "File not open!" << endl;
        return 2;
    }

    string temp = mystr;
    temp.pop_back(); // Must remove the last char, (EOF).
    cout << "String after pop_back: <" << temp << ">" << endl;
    cout << "Char count: <" << temp.length() << ">" << endl;
    cout << "Your SHA256 hash for " << argv[1] << " is: ";
    cout << getSHA256Hash(temp) << endl;

    return 0;
}
