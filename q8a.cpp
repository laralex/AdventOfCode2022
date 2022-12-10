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
   input_file >> input_line;
   size_t grid_width = input_line.size();
   size_t n_visible = grid_width; // first row

   CoordsSet visible_trees_coords;

   // first row contains maximum heights so far, complexity: O(W), where W is grid_width
   std::vector<char> bottom2top_max_height(grid_width);
   for (size_t col_idx = 0; col_idx < grid_width; ++col_idx) {
      bottom2top_max_height[col_idx] = input_line[col_idx];
      visible_trees_coords.emplace(0, col_idx);
   }

   // complexity: O(max_height*W), where max_height is 10
   std::vector<std::array<size_t, 10>> last_height_bottom_location(grid_width);
   for(auto& heights : last_height_bottom_location) {
      heights.fill((size_t)(-1));
   }

   // solving the task for all 4 sides in one sweep through the grid,
   // without storing all of it

   // complexity: O( [ W + max_height ] * H * log((H+W)*max_height) ), where H is grid_height
   std::array<size_t, 10> last_height_left_location;
   size_t row_idx;
   for(row_idx = 1; !input_file.eof(); ++row_idx) {
      input_file >> input_line;
      std::cout << "y=" << row_idx << " :: " << input_line << '\n';
      char left2right_max_height { '0'-1 };
      last_height_left_location.fill((size_t)(-1));

      // complexity: O(grid_width)
      for (size_t col_idx = 0; col_idx < grid_width; ++col_idx) {
         if (input_line[col_idx] > bottom2top_max_height[col_idx]) {
            // bottom to top
            // O(log((H+W)*max_height))
            bool is_new = put_if_new({row_idx, col_idx},visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "bt(x=" << col_idx << " h=" << input_line[col_idx] << ") ";

            bottom2top_max_height[col_idx] = input_line[col_idx];
         }
         if (input_line[col_idx] > left2right_max_height) {
            // left to right
            // O(log((H+W)*max_height))
            bool is_new = put_if_new({row_idx, col_idx}, visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "lr(x=" << col_idx << " h=" << left2right_max_height << ") ";

            left2right_max_height = input_line[col_idx];
         }
         size_t ch_idx = input_line[col_idx] - '0';
         last_height_left_location[ch_idx] = col_idx;
         last_height_bottom_location[col_idx][ch_idx] = row_idx;
      }
      std::cout << '\n';

      // right to left, complexity: O(max_height * log((H+W)*max_height))
      {
         int current_height = 10;
         size_t left_cutoff_col_idx = 0;
         while(current_height-- > 0 && left_cutoff_col_idx < grid_width - 1){
            if (   last_height_left_location[current_height] == (size_t)(-1)
                || last_height_left_location[current_height] < left_cutoff_col_idx) {
               continue;
            }
            left_cutoff_col_idx = last_height_left_location[current_height];

            // O(log((H+W)*max_height))
            bool is_new = put_if_new({row_idx, left_cutoff_col_idx}, visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "rl(x=" << left_cutoff_col_idx << " h=" << current_height <<  ") ";
         }
      }

      std::cout << "\n\n";
   }

   size_t grid_height = row_idx;

   // top to bottom: O(grid_width*max_height*log(max_height))
   for (size_t col_idx = 0; col_idx < last_height_bottom_location.size(); ++col_idx) {
      std::cout << "x=" << col_idx << " :: ";
      int current_height = 10;
      const auto& col_last_heights = last_height_bottom_location[col_idx];
      size_t bottom_cutoff_row_idx = 0;
      while(current_height-- > 0 && bottom_cutoff_row_idx < grid_height - 1){
         if (   col_last_heights[current_height] == (size_t)(-1)
             || col_last_heights[current_height] < bottom_cutoff_row_idx) {
            continue;
         }
         bottom_cutoff_row_idx = col_last_heights[current_height];

         // O(log((H+W)*max_height))
         bool is_new = put_if_new({bottom_cutoff_row_idx, col_idx}, visible_trees_coords); 
         n_visible += is_new;
         std::cout << (is_new ? "*" : "") << "tb(y=" << bottom_cutoff_row_idx << " h=" << current_height <<  ") ";
      }
      std::cout << '\n';
   }

   std::cout << "Answer I : " << n_visible << '\n';
   return 0;
}