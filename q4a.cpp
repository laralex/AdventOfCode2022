#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <set>

struct Range {
   long long begin, end;

   Range& operator=(const Range& other) = default;
};

long long read_and_parse_int(std::istream& stream, char separator) {
   static std::string input;
   std::getline(stream, input, separator);
   return std::stoi(input);
}

int main() {
   std::ifstream input_file("data/q4.txt");
   long long fully_covered_count = 0;
   while(!input_file.eof()) {
      Range elf1, elf2;
      elf1.begin = read_and_parse_int(input_file, '-');
      elf1.end   = read_and_parse_int(input_file, ',');
      elf2.begin = read_and_parse_int(input_file, '-');
      elf2.end   = read_and_parse_int(input_file, '\n');
      if (elf2.begin < elf1.begin) {
         // swap to make sure elf1 atleast range precedes elf2 range
         Range tmp = elf2;
         elf2 = elf1;
         elf1 = tmp;
      }
      if (elf2.end <= elf1.end || elf2.begin == elf1.begin) {
         std::cout << "fully contained: " << elf1.begin << '-' << elf1.end << ' ' << elf2.begin << '-' << elf2.end << '\n';
         ++fully_covered_count;
      } else {
         std::cout << "not contained: " << elf1.begin << '-' << elf1.end << ' ' << elf2.begin << '-' << elf2.end << '\n';
      }
   }
   std::cout << fully_covered_count;
   return 0;
}