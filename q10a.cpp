#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

const static size_t ADDX_CLOCKS = 2;
const static size_t NOOP_CLOCKS = 1;

void solve(bool print_steps, bool print_interactive) {
   std::ifstream input_file("data/q10.txt");
   int x = 1, addx_arg = 0;
   std::string command;
   int signal_strength_sum = 0;
   size_t clock=0, sum_begin_at_clock = 20, sum_every_clocks = 40;
   int sum_last_clock = sum_begin_at_clock - sum_every_clocks;
   while (!input_file.eof()) {
      input_file >> command;
      if (command == "addx") {
         input_file >> addx_arg;
         clock += ADDX_CLOCKS;
         if (clock >= sum_last_clock + sum_every_clocks) {
            sum_last_clock += sum_every_clocks;
            signal_strength_sum += sum_last_clock * x;
            std::cout << "(add=" << sum_last_clock * x << ", sum=" << signal_strength_sum << ") ";
         }
         x += addx_arg;
      } else if (command == "noop") {
         clock += NOOP_CLOCKS;
      }
      if (print_steps) {
         std::cout << '(' <<clock << ' ' << command << ", x=" << x << ") ";
         if (print_interactive) {
            std::cin.get();
         }
      }
   }

   std::cout << "\nAnswer Part 1: " << signal_strength_sum << '\n';
}

int main() {
   solve(true, false);
   return 0;
}