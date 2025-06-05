// https://tomohxx.github.io/mahjong-algorithm-book/generating_hand/
#include "shanten.hpp"
#include <calsht.hpp>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>
constexpr int MAX_TIDS = 34;
constexpr int NUM_TIDS = 34;
constexpr int NUM_TILES = 13;
// constexpr int NUM_TILES = 14;
constexpr int NUM_CALLS = 4 - NUM_TILES / 3;
constexpr int MODE = 1;

void generate(const int n, const int m, std::function<void(std::vector<int>&)> func)
{
  static std::vector<int> hand(MAX_TIDS);

  if (n == NUM_TIDS) {
    func(hand);
  }
  else {
    for (int i = std::max(0, m - 4 * (NUM_TIDS - n - 1)); i <= std::min(4, m); ++i) {
      hand[n] = i;
      generate(n + 1, m - i, func);
    }
  }
}

int main()
{
  Calsht calsht;

  calsht.initialize(INDEX_FILE_PATH);

  uint64_t num_hands = 0ull;

  const auto start = std::chrono::system_clock::now();

  generate(0, NUM_TILES, [&calsht, &num_hands](std::vector<int>& hand) {
    const auto [sht, mode] = calsht(hand, NUM_TILES / 3, MODE);
    const auto shanten = calc_shanten(hand, NUM_CALLS);

    assert(sht - 1 == shanten);

    ++num_hands;
  });

  const auto end = std::chrono::system_clock::now();

  std::cout << "Number of hands: " << num_hands << std::endl;
  std::cout << "Time (msec.): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

  return EXIT_SUCCESS;
}
