#ifndef __CHECKSUM_H_
#define __CHECKSUM_H_

#include <string>
#include <sstream>
#include <fstream>

/* [NOTE]:
 * This checksum function was based on code found at:
 * https://stackoverflow.com/questions/3463976/c-file-checksum
 */
std::string checksum(std::ifstream &);
std::string checksum(const char *data, size_t dataSize);

#endif
