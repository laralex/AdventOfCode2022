#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <queue>
#include <functional>

using ll = long long;

struct MonkeyState {
   std::vector<ll> items;
   std::function<ll(ll)> operation;
   size_t operation_counter;
   size_t test_denominator, test_true_monkey_idx, test_false_monkey_idx;
};

// interpreted as operation with "old" value, rather than an actual number
constexpr static int FAKE_ARGUMENT = 42424242;

ll solve(size_t n_rounds, size_t worry_denominator, bool print_steps, bool print_interactive) {

   // read monkeys attributes
   std::ifstream input_file("data/q11.txt");
   std::vector<MonkeyState> monkeys;
   while (!input_file.eof()) {
      size_t n_start_items;
      input_file >> n_start_items;

      MonkeyState state;
      size_t worry_level;
      for (int i = 0; i < n_start_items; ++i) {
         input_file >> worry_level;
         state.items.push_back(worry_level);
      }

      char operation;
      size_t operation_arg;
      input_file >> operation >> operation_arg;
      std::function<ll(ll, ll)> operation_fn;
      switch (operation) {
         case '+': operation_fn = std::plus<ll>(); break;
         case '-': operation_fn = std::minus<ll>(); break;
         case '*': operation_fn = std::multiplies<ll>(); break;
         case '/': operation_fn = std::divides<ll>(); break;
      }

      input_file >> state.test_denominator >> state.test_true_monkey_idx >> state.test_false_monkey_idx;

      if (operation_arg != FAKE_ARGUMENT) {
         state.operation = std::bind(operation_fn, std::placeholders::_1, operation_arg);
      } else {
         //auto reduce_arg = [&](ll a){ return (a % state.test_denominator == 0 ? state.test_denominator : 1); };
         // state.operation = std::bind(operation_fn, std::placeholders::_1, std::bind(reduce_arg, std::placeholders::_1));
         state.operation = std::bind(operation_fn, std::placeholders::_1, std::placeholders::_1);
      }
      state.operation_counter = 0;

      monkeys.emplace_back(state);
   }

   // start rounds
   for (int round = 0; round < n_rounds; ++round) {
      for (int monkey_i = 0; monkey_i < monkeys.size(); ++monkey_i) {
         if (print_steps) { std::cout << "Monkey " << monkey_i << ' '; }
         auto& monkey = monkeys[monkey_i];
         monkey.operation_counter += monkey.items.size();
         for (auto item_worry : monkey.items) {
            ll new_worry = (monkey.operation(item_worry) / worry_denominator); // % monkey.test_denominator;
            size_t destination_monkey_idx = new_worry % monkey.test_denominator
               ? monkey.test_false_monkey_idx
               : monkey.test_true_monkey_idx;
            if (print_steps) {
               std::cout << " (" << item_worry << " op "
                  << new_worry << " -> " << destination_monkey_idx << ")";
            }
            monkeys[destination_monkey_idx].items.push_back(new_worry);
         }
         monkey.items.clear();
         if (print_steps) { std::cout << '\n'; }
      }
   }

   if (print_steps) {
      for(const auto& monkey : monkeys) {
         std::cout << "Monkey inspected " << monkey.operation_counter << " items\n";
      }
   }

   // inspect the most active monkeys
   std::sort(monkeys.begin(), monkeys.end(),
      [](const auto& a, const auto& b) {
         return a.operation_counter > b.operation_counter;
      });

   std::cout << monkeys[0].operation_counter << ' ' << monkeys[1].operation_counter << '\n';
   return monkeys[0].operation_counter * monkeys[1].operation_counter;
}

int main() {
   std::cout << "Answer for Part 1: " << solve(20, 3, false, false) << '\n';
   std::cout << "Answer for Part 2: " << solve(10000, 1, false, false) << '\n';
   return 0;
}