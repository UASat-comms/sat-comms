#include "checksum.hpp"

/* [NOTE]:
 * This checksum function was found on:
 * https://stackoverflow.com/questions/3463976/c-file-checksum
 */
std::string checksum(std::ifstream &file) {
     unsigned char _sum = 0;
     char c;
     while (file.get(c)) {
          _sum ^= c;
     }
     std::stringstream streamer;
     for(int i = 0; i < 8; i++) {
          streamer << (int) ((_sum >> (7 - i)) & 0x0001);
     }
     return streamer.str();
}

std::string checksum(char *data, size_t dataSize) {
     unsigned char _sum = 0;
     for(int i = 0; i < dataSize; i++) {
          _sum ^= data[i];
     }
     std::stringstream streamer;
     for(int i = 0; i < 8; i++) {
          streamer << (int) ((_sum >> (7 - i)) & 0x0001);
     }
     return streamer.str();
}
