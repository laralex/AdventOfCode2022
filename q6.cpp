#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <stack>
#include <iomanip>

const static size_t MARKER_WIDTH_PART_1 = 4;
const static size_t MARKER_WIDTH_PART_2 = 14;

int main() {
   std::ifstream input_file("data/q6.txt");
   std::unordered_map<char, size_t> last_seen_idx;
   std::string signal;
   size_t current_satisfaction = 0;
   size_t answer_1 = 0, answer_2 = 0;
   input_file >> signal;

   size_t i;
   for(i = 0; i < signal.size(); ++i) {
      char ch = signal[i];
      auto it = last_seen_idx.find(ch);
      if (it == last_seen_idx.end() || (i - (it->second)) > current_satisfaction) {
         ++current_satisfaction;
      } else {
         current_satisfaction = (i - (it->second));
      }
      std::cout << ch << ' ' << std::setw(4) <<  i << ' ' << current_satisfaction << std::endl;
      if (answer_1 == 0 && current_satisfaction == MARKER_WIDTH_PART_1) {
         answer_1 = i + 1;
      }
      if (answer_2 == 0 && current_satisfaction == MARKER_WIDTH_PART_2) {
         answer_2 = i + 1;
         break;
      }
      last_seen_idx[ch] = i;
   }
   std::cout << "Answer for Part I : " << answer_1 << '\n';
   std::cout << "Answer for Part II: " << answer_2 << '\n';

   return 0;
}