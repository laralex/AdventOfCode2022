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
   std::vector<int> buffer(10000);
   while(!input_file.eof()) {
      size_t move_n, buffer_i, from, to;
      input_file >> input_line >> move_n >> input_line >> from >> input_line >> to;
      --from;
      --to;
      move_n = std::min(move_n, stacks[from].size());
      std::cout << move_n << ' ' << from << ' ' << to << std::endl;
      buffer_i = move_n;
      while(buffer_i--) {
         buffer[buffer_i] = stacks[from].top();
         stacks[from].pop();
      }
      for(buffer_i = 0; buffer_i < move_n; ++buffer_i) {
         stacks[to].push(buffer[buffer_i]);
      }
   }
   for (auto& stack : stacks) {
      std::cout << stack.top();
   }
   return 0;
}