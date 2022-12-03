#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int get_move_score(char move) {
   return move - 'A' + 1;
}

char get_winning_move(char opponent_move) {
   return (opponent_move - 'A' + 1) % 3 + 'A';
}

char get_loosing_move(char opponent_move) {
   return (opponent_move - 'A' + 2) % 3 + 'A';
}

int main() {
   std::ifstream input_file("data/q2.txt");
   std::vector<long long> elves_sums;
   char their_move, our_action, our_move;
   long long sum_score = 0;
   while(input_file >> their_move) {
      input_file >> our_action;
      size_t match_score = 0;
      if (our_action == 'X') {
         our_move = get_loosing_move(their_move);
      } else if (our_action == 'Y') {
         our_move = their_move;
         match_score = 3; // draw
      } else {
         our_move = get_winning_move(their_move);
         match_score = 6; // win
      }
      sum_score += match_score + get_move_score(our_move);
   }
   std::cout << sum_score;
   return 0;
}