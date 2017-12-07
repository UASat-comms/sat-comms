#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "picosha2.h"
#include <sstream>
#include <cinttypes>

using namespace std;

std::string getSHA256Hash(const char *str) {
  std::stringstream s;
  s << str;
  return picosha2::hash256_hex_string(s.str());
}

std::string getSHA256Hash(std::string & str) {
  return picosha2::hash256_hex_string(str);
}


int main() {
  srand(time(NULL));
  cout << getSHA256Hash("test") << endl;  
  std::string s = "abc";
  cout << getSHA256Hash(s) << endl;  
  
  const char *str = getSHA256Hash(s).c_str();
  while(*str) {
    printf("%c", *str);
    ++str;
  }
  printf("%s\n", str);
}
