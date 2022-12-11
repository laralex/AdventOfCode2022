#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

struct Coords {
   int x, y;
};

bool operator<(const Coords& a, const Coords& b) {
   if (a.x == b.x) {
      return a.y < b.y;
   }
   return a.x < b.x;
}

void solve_for_rope_length(size_t rope_length, bool print_steps, bool print_interactive) {
   std::ifstream input_file("data/q9.txt");
   char action;
   int n_steps, change_x_sign, change_y_sign;
   std::vector<Coords> rope{rope_length, {0,0}};
   std::set<Coords> tail_visited_coords{};
   tail_visited_coords.emplace(*rope.rbegin());

   while (!input_file.eof()) {
      input_file >> action >> n_steps;
      change_x_sign = (action == 'L' ? -1 : 0) + (action == 'R' ? 1 : 0);
      change_y_sign = (action == 'D' ? -1 : 0) + (action == 'U' ? 1 : 0);

      for (int step = 0; step < n_steps; ++step) {
         auto parent_it = rope.begin();
         parent_it->x += change_x_sign;
         parent_it->y += change_y_sign;
         for (auto next_it = parent_it+1; next_it != rope.end(); ++next_it) {
            int gap_x = parent_it->x - next_it->x;
            int gap_y = parent_it->y - next_it->y;
            int change_x_sign = (gap_x == 0 ? 0: (gap_x > 0 ? 1 : -1));
            int change_y_sign = (gap_y == 0 ? 0: (gap_y > 0 ? 1 : -1));
            if (std::abs(gap_x) >= 2 || std::abs(gap_y) >= 2) {
               next_it->x += change_x_sign;
               next_it->y += change_y_sign;
            }
            ++parent_it;
         }
         tail_visited_coords.emplace(*rope.rbegin());
      }

      if (print_steps) {
         std::cout << action << ' ' << n_steps << ' ';
         for (const auto& segment : rope) {
            std::cout << '(' << segment.x << ',' << segment.y << ") ";
         }
         std::cout << "v=" << tail_visited_coords.size() << '\n';
         if (print_interactive) {
            std::cin.get();
         }
      }
   }

   std::cout << "Answer for rope of " << rope_length << ": " << tail_visited_coords.size() << '\n';
}

int main() {
   solve_for_rope_length(2, false, false);
   solve_for_rope_length(10, true, false);
   return 0;
}