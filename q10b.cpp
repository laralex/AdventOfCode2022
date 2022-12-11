#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <queue>

const static size_t ADDX_CLOCKS = 2;
const static size_t NOOP_CLOCKS = 1;

struct Command {
   bool is_add;
   int add_arg;
};

constexpr static size_t SCREEN_WIDTH = 40, SCREEN_HEIGHT = 6;

int draw_at_clock(int current_clock, int current_x) {
   size_t pixel_x = (current_clock) % SCREEN_WIDTH;
   std::cout << ((pixel_x >= current_x - 1 && pixel_x <= current_x + 1) ? '#' : '.');
   return ++current_clock;
}

void solve(bool print_steps, bool print_interactive) {
   std::ifstream input_file("data/q10.txt");
   std::queue<Command> commands;
   std::string command;
   while (!input_file.eof()) {
      input_file >> command;
      if (command == "addx") {
         int arg;
         input_file >> arg;
         commands.push({true, arg});
      } else if (command == "noop") {
         commands.push({false, 0});
      }
   }

   size_t clock=0;
   int x = 1;
   size_t wait_completion = 0;

   std::cout << "\nAnswer Part 2:\n";
   while(!commands.empty()) {
      auto cmd = commands.front();
      // std::cout << cmd.is_add << ' ' << cmd.add_arg;
      if (!wait_completion) {
         wait_completion = cmd.is_add ? ADDX_CLOCKS : NOOP_CLOCKS;
         continue;
      }

      if (clock % SCREEN_WIDTH == 0) {
         std::cout << '\n';
      }
      clock = draw_at_clock(clock, x);
      --wait_completion;
      if (wait_completion == 0) {
         if (cmd.is_add) {
            x += cmd.add_arg;
         }
         commands.pop();
      }
   }

}

int main() {
   solve(true, false);
   return 0;
}