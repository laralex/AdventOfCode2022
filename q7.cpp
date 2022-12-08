#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <stack>

struct FileMeta {
   size_t parent_id, size;
   std::string name;
};

int main() {
   std::ifstream input_file("data/q7.txt");
   std::string command, arg1, arg2;
   size_t incremental_dir_id = 0, current_dir_id = 0;
   std::unordered_map<size_t, FileMeta> dir_meta;
   dir_meta[current_dir_id] = {0, 0, "/"};
   while(!input_file.eof()) {
      input_file >> command >> command;
      // changing directory
      if (command == "cd") {
         input_file >> arg1;
         if (arg1 == "/") {
            current_dir_id = 0;
         } else if (arg1 == "..") {
            current_dir_id = dir_meta[current_dir_id].parent_id;
         } else {
            dir_meta[incremental_dir_id] = {current_dir_id, 0, std::move(arg1)};
            current_dir_id = incremental_dir_id;
            ++incremental_dir_id;
         }
         auto& current_dir_meta = dir_meta[current_dir_id];
         std::cout << "$ " << command << ' '
               << current_dir_id << ' '
               << current_dir_meta.name << ' '
               << current_dir_meta.size << '\n' << '\n';
      // remembering directory info (unless already visited)
      } else if (command == "ls" && dir_meta[current_dir_id].size == 0) { 
         size_t current_dir_size = 0;
         while(!(input_file.eof() || input_file.peek() == '$')) {
            input_file >> arg1 >> arg2; // size and filename
            input_file.ignore(1);
            std::cout << arg1 << ' ' << arg2 << '\n';
            if (arg1 != "dir") {
               size_t file_size = std::stoi(arg1);
               current_dir_size += file_size;
            }
         }
         auto& current_dir_meta = dir_meta[current_dir_id];
         current_dir_meta.size += current_dir_size;
         size_t unwind_dir_id = current_dir_meta.parent_id;
         while(true) {
            dir_meta[unwind_dir_id].size += current_dir_size;
            if (unwind_dir_id == 0) {
               break;
            }
            unwind_dir_id = dir_meta[unwind_dir_id].parent_id;
         }
         std::cout << "$ " << command << ' '
               << current_dir_id << ' '
               << current_dir_meta.name << ' '
               << current_dir_meta.size << '\n' << '\n';
      }
   }

   size_t sum_size_sub_100k = 0;
   size_t need_to_free = std::max(0, 30000000 - (70000000 - (int)dir_meta[0].size));
   size_t min_delete_size = dir_meta[0].size;
   for (auto& p : dir_meta) {
      size_t dir_size = p.second.size;
      if (dir_size <= 100000) {
         sum_size_sub_100k += dir_size;
      }
      if (dir_size >= need_to_free) {
         min_delete_size = std::min(min_delete_size, dir_size);
      }
   }
   std::cout << "Answer I : " << sum_size_sub_100k << '\n';
   std::cout << "Answer II: " << min_delete_size << '\n';
   return 0;
}