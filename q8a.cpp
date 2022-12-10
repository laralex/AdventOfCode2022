#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

// Utility hash function: to be able to store a pair of coordinates in a hash set
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

// Try to put the `element` in the hash set
// return `true` if the `element` didn't exist before
// Time complexity: O(1), amortized
bool put_if_new(Coords element, CoordsSet& counted_already) {
   if (counted_already.find(element) == counted_already.end()) {
      counted_already.emplace(std::move(element));
      return true;
   }
   return false;
}

const static size_t HEIGHT_LIMIT = 10; // 0, 1, ..., 8, 9 -- (total of 10 variants)

int main() {
   std::ifstream input_file("data/q8.txt");
   std::string input_line;
   input_file >> input_line; // read the first line to find width of the forest
   size_t grid_width = input_line.size();

   CoordsSet visible_trees_coords{}; // record of already counted trees

   // for each column, store the biggest height found so far
   std::vector<char> columns_max_height(grid_width, '0');
   for (size_t col_idx = 0; col_idx < grid_width; ++col_idx) {
      columns_max_height[col_idx] = input_line[col_idx]; // initialization with the first row 
      visible_trees_coords.emplace(0, col_idx);
   }
   size_t n_visible = grid_width; // the first row is visible

   // for each column, for each height variant (0 to 9), 
   // store the latest row where such height was found
   std::vector<std::array<int, HEIGHT_LIMIT>> columns_height_record(grid_width, {-1});
   // same, but for the current row
   std::array<int, HEIGHT_LIMIT> current_row_height_record;

   // solving the task for all 4 sides in one sweep through the grid,
   // without storing all of it
   // time complexity: O(W*H + height_limit*H),
   //   or O(W*H) with constant height_limit == 10,
   //   where H is grid_height, W is grid_width
   size_t row_idx;
   for(size_t row_idx = 1; !input_file.eof(); ++row_idx) {
      // read a new row, reset the row record of heights
      input_file >> input_line;
      std::cout << "y=" << row_idx << " :: " << input_line << '\n';
      char row_max_height { 0 };
      current_row_height_record.fill(-1);

      // scan the row
      // time complexity: O(W)
      for (size_t col_idx = 0; col_idx < grid_width; ++col_idx) {
         // for the current tree we've already seen trees above & to the left, thus
         // - the current tree is visible from the top edge,
         //   if it has the maximum height so far in the current column
         if (input_line[col_idx] > columns_max_height[col_idx]) {
            columns_max_height[col_idx] = input_line[col_idx];
            bool is_new = put_if_new({row_idx, col_idx},visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "t(x=" << col_idx << " h=" << input_line[col_idx] << ") ";
         }
         // - the current tree is visible from the left edge,
         //   if it has the maximum height so far in the current row
         if (input_line[col_idx] > row_max_height) {
            row_max_height = input_line[col_idx];
            bool is_new = put_if_new({row_idx, col_idx}, visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "l(x=" << col_idx << " h=" << row_max_height << ") ";
         }

         // remember the coordinates as the lastest coordinates
         // where we found the current tree's height
         size_t height_index = input_line[col_idx] - '0';
         current_row_height_record[height_index] = col_idx;
         columns_height_record[col_idx][height_index] = row_idx;
      }
      std::cout << '\n';

      // only after reading the whole row
      // we can check visibility from the right edge
      // time complexity: O(height_limit), or O(1) for constant height_limit==10
      {
         // the idea: start from the column where we last saw the biggest tree on this row,
         // call this column a "cutoff", becaues this tree is visible, 
         // and all trees to the left are invisble

         // get the column where we last saw the tree with lower height
         // if the column is to the left of the cutoff, go to even lower height and repeat
         // if the column is to the right of the cutoff, then this tree is visible,
         // and this column becomes a new cutoff, continue with lower heights
         int current_height = row_max_height - '0' + 1;
         size_t left_cutoff_col_idx = 0;
         while(current_height-- > 0 && left_cutoff_col_idx < grid_width - 1){
            if (   current_row_height_record[current_height] == -1 // never saw
                || current_row_height_record[current_height] < left_cutoff_col_idx) {
               continue;
            }
            left_cutoff_col_idx = current_row_height_record[current_height];

            // count the visible tree
            bool is_new = put_if_new({row_idx, left_cutoff_col_idx}, visible_trees_coords);
            n_visible += is_new;
            std::cout << (is_new ? "*" : "") << "r(x=" << left_cutoff_col_idx << " h=" << current_height <<  ") ";
         }
      }

      std::cout << "\n\n";
   }

   size_t grid_height = row_idx;

   // similarly to the check of visibility from the right,
   // we check visibility from the bottom (for each column)
   // Time complexity: O(W*height_limit)
   for (size_t col_idx = 0; col_idx < columns_height_record.size(); ++col_idx) {
      std::cout << "x=" << col_idx << " :: ";
      int current_height = columns_max_height[col_idx] - '0' + 1;
      const auto& col_last_heights = columns_height_record[col_idx];
      int bottom_cutoff_row_idx = 0;
      while(current_height-- > 0 && bottom_cutoff_row_idx < grid_height - 1){
         if (   col_last_heights[current_height] == -1 // never saw
             || col_last_heights[current_height] < bottom_cutoff_row_idx) {
            continue;
         }
         bottom_cutoff_row_idx = col_last_heights[current_height];

         // O(log((H+W)*max_height))
         bool is_new = put_if_new({bottom_cutoff_row_idx, col_idx}, visible_trees_coords);
         n_visible += is_new;
         std::cout << (is_new ? "*" : "") << "b(y=" << bottom_cutoff_row_idx << " h=" << current_height <<  ") ";
      }
      std::cout << '\n';
   }

   std::cout << "Answer Part I : " << n_visible << '\n';
   return 0;
}