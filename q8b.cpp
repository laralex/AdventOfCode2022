#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <stack>

// A hash function used to hash a pair of any kind
struct hash_pair {
   template <class T1, class T2>
   size_t operator()(const std::pair<T1, T2>& p) const
   {
      auto hash1 = std::hash<T1>{}(p.first);
      auto hash2 = std::hash<T2>{}(p.second);

      if (hash1 != hash2) { return hash1 ^ hash2; }
      return hash1;
   }
};

using Coords = std::pair<size_t,size_t>;
using CoordsSet = std::unordered_set<Coords, hash_pair>;

bool put_if_new(Coords element, CoordsSet& counted_already) {
   if (counted_already.find(element) == counted_already.end()) {
      counted_already.emplace(std::move(element));
      return true;
   }
   return false;
}

int main() {
   std::ifstream input_file("data/q8.txt");
   std::string input_line;
   std::vector<std::vector<char>> grid;

   while (!input_file.eof()) {
      input_file >> input_line;
      grid.push_back(std::vector<char>(input_line.size()));
      for (int col_idx = 0; col_idx < input_line.size(); ++col_idx) {
         grid[grid.size() - 1][col_idx] = input_line[col_idx];
      }
   }
   size_t grid_height = grid.size(), grid_width = grid[0].size();

   size_t max_scenic_score = 0;
   for(int row = 0; row < grid_height; ++row) {
      std::cout << "row " << row << " :: " << grid[row][0] << '\n';
      for(int col = 0; col < grid_width; ++col) {
         // std::cout << "c" << col << " ";
         size_t scenic_score = 1;
         for(int offset = 1; ; ++offset) {
            if (row-offset <= 0 || grid[row][col] <= grid[row-offset][col]) {
               std::cout << 'b' << offset;
               scenic_score *= offset;
               break;
            }
         }
         for(int offset = 1; ; ++offset) {
            if (row+offset >= grid_width - 1 || grid[row][col] <= grid[row+offset][col]) {
               std::cout << 't' << offset;
               scenic_score *= offset;
               break;
            }
         }
         for(int offset = 1; ; ++offset) {
            if (col-offset <= 0 || grid[row][col] <= grid[row][col-offset]) {
               std::cout << 'l' << offset;
               scenic_score *= offset;
               break;
            }
         }
         for(int offset = 1; ; ++offset) {
            if (col+offset >= grid_width - 1 || grid[row][col] <= grid[row][col+offset]) {
               std::cout << 'r' << offset;
               scenic_score *= offset;
               break;
            }
         }
         std::cout << ' ';
         if (scenic_score > max_scenic_score) {
            std::cout << "\n> " << row << ' ' << col << " score=" << scenic_score << std::endl;
         }
         max_scenic_score = std::max(max_scenic_score, scenic_score);
      }
      std::cout << "\n\n";
   }

   std::cout << "height=" << grid_height << " width=" << grid_width
      << " bl=" << grid[0][0] << " br=" << grid[0][grid_width-1]
      << " tl=" << grid[grid_height-1][0] << " tr=" << grid[grid_height-1][grid_width-1] << '\n';

   std::cout << "Answer II: " << max_scenic_score << '\n';
   return 0;
}