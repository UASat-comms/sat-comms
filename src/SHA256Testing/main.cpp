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
    stringstream mystr;
    string line;

    if(myfile.is_open()) {
        while(getline(myfile, line)) {
            cout << "line read: <" <<  line << ">" << endl;
            mystr << line << endl;
        }
        myfile.close();
    } else {
        cout << "File not open!" << endl;
        return 2;
    }

    string temp = mystr.str();
    cout << "String before pop_back: <" << temp << ">" << endl;
    cout << "Char count: <" << temp.length() << ">" << endl;
    cout << "Your SHA256 hash for " << argv[1] << " is: ";
    cout << getSHA256Hash(temp) << endl;
    
    temp.pop_back();
    cout << "String after pop_back: <" << temp << ">" << endl;
    cout << "Char count: <" << temp.length() << ">" << endl;
    cout << "Your SHA256 hash for " << argv[1] << " is: ";
    cout << getSHA256Hash(temp) << endl;

    return 0;
}
