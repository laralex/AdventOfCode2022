#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

const size_t TOP_K = 3;

int main() {
   std::ifstream input_file("data/q1.txt");
   long long current_elf_sum = 0;
   std::string line;
   std::vector<long long> elves_sums;
   while(std::getline(input_file, line)) {
      if (line.size() == 0 || line[0] == '\n') {
         elves_sums.push_back(current_elf_sum);
         current_elf_sum = 0;
         continue;
      }
      std::size_t pos{};
      current_elf_sum += std::stoi(line, &pos);
   }
   std::sort(elves_sums.begin(), elves_sums.end(), std::greater<long long>{});
   long long top_k_sums = 0;
   for (auto it = elves_sums.begin(); it != elves_sums.begin() + TOP_K; ++it) {
      top_k_sums += *it;
   }
   std::cout << top_k_sums;
   return 0;
}