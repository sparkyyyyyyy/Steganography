#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <tuple> // Needed for std::tuple

std::tuple<unsigned char, unsigned char, unsigned char> splitByte(unsigned char byte);
unsigned char mergeBits(unsigned char first, unsigned char mid, unsigned char last);
std::string crypt(const std::string &input, const std::string &key);

void embed(const std::string &vesselPath, const std::string &outPath, const std::string &fileToEmbed, const std::string &passcode);
void extract(const std::string &embPath, const std::string &passcode);

#endif