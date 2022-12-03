#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <set>

int get_priority(char item) {
   if (item >= 'A' && item <= 'Z') {
      return 27 + item - 'A';
   }
   return item - 'a' + 1;
}

void count_rucksack_items(const std::string& rucksack, std::unordered_map<char, int>& counts) {
   std::set<char> unique_items {rucksack.begin(), rucksack.end()};
   for (auto v : unique_items) {
      ++counts[v];
   }
}

int main() {
   std::ifstream input_file("data/q3.txt");
   long long sum_priority = 0;
   std::string rucksack;
   std::unordered_map<char, int> counts;
   while(input_file >> rucksack) {
      count_rucksack_items(rucksack, counts);
      for (int i = 0; i < 2; ++i) {
         input_file >> rucksack;
         count_rucksack_items(rucksack, counts);
      }
      for (auto &[item,cnt] : counts) {
         if (cnt == 3) {
            std::cout << item << ' ' << get_priority(item) << '\n';
            sum_priority += get_priority(item);
            break;
         }
      }
      counts.clear();
   }
   std::cout << sum_priority;
   return 0;
}