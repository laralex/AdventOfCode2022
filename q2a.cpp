#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int get_move_score(char move) {
   return move - 'A' + 1;
}

int main() {
   std::ifstream input_file("data/q2.txt");
   std::vector<long long> elves_sums;
   char their_move, our_move;
   long long sum_score = 0;
   while(input_file >> their_move) {
      input_file >> our_move;
      our_move -= 'X' - 'A'; // turn into A B C
      size_t diff = our_move - their_move;
      size_t match_score = 0;
      if (diff == 0) {
         match_score += 3; // draw
      } else if (diff == 1 || diff + 3 == 1) {
         match_score += 6; // win
      }
      sum_score += match_score + get_move_score(our_move);
   }
   std::cout << sum_score;
   return 0;
}