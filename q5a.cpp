#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <stack>

int main() {
   std::ifstream input_file("data/q5.txt");
   int n_stacks;
   input_file >> n_stacks;
   std::string input_line;
   std::vector<std::stack<char>> stacks(n_stacks);
   for (auto& stack : stacks) {
      input_file >> input_line;
      for (auto c : input_line) {
         stack.push(c);
      }
   }
   long long fully_covered_count = 0;
   while(!input_file.eof()) {
      size_t move_n, from, to;
      input_file >> input_line >> move_n >> input_line >> from >> input_line >> to;
      --from;
      --to;
      move_n = std::min(move_n, stacks[from].size());
      std::cout << move_n << ' ' << from << ' ' << to << std::endl;
      while(move_n--) {
         stacks[to].push(stacks[from].top());
         stacks[from].pop();
      }
   }
   for (auto& stack : stacks) {
      std::cout << stack.top();
   }
   return 0;
}