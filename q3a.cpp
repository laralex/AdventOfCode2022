#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <set>

int get_priority(char item) {
   if (item >= 'A' && item <= 'Z') {
      return 27 + item - 'A';
   }
   return item - 'a' + 1;
}

int main() {
   std::ifstream input_file("data/q3.txt");
   long long sum_priority = 0;
   std::string two_rucksacks;
   while(input_file >> two_rucksacks) {
      auto second_rucksack_begin = two_rucksacks.begin() + two_rucksacks.size() / 2;
      std::set<char> first_rucksack(two_rucksacks.begin(), second_rucksack_begin);
      for (auto it = second_rucksack_begin; it != two_rucksacks.end(); ++it) {
         if (first_rucksack.find(*it) != first_rucksack.end()) {
            std::cout << two_rucksacks << ' ' <<  *it  << ' ' << get_priority(*it) << std::endl;
            sum_priority += get_priority(*it);
            break;
         }
      }
   }
   std::cout << sum_priority;
   return 0;
}