#include "shanten.hpp"
#include <array>
#include <calsht.hpp>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif
static_assert(NUM_THREADS > 0);
constexpr int MAX_TIDS = 34;
constexpr int NUM_TIDS = 34;
constexpr int NUM_TILES = 13;
// constexpr int NUM_TILES = 14;
constexpr int NUM_CALLS = 4 - NUM_TILES / 3;
constexpr int MODE = 1;
using Table1 = std::array<std::array<uint64_t, NUM_TILES + 1>, NUM_TIDS + 1>;

void build_table1(Table1& tbl1)
{
  tbl1[NUM_TIDS].fill(0);
  tbl1[NUM_TIDS][NUM_TILES] = 1ull;

  for (int i = NUM_TIDS - 1; i >= 0; --i) {
    for (int n = 0; n <= std::min(4 * i, NUM_TILES); ++n) {
      for (int c = 0; c <= std::min(NUM_TILES - n, 4); ++c) {
        tbl1[i][n] += tbl1[i + 1][n + c];
      }
    }
  }
}

std::vector<int> decode(const uint64_t hash, const Table1& tbl1)
{
  std::vector<int> hand(MAX_TIDS);
  uint64_t h = 0ull;
  unsigned int n = 0u;

  for (int i = 0; i < NUM_TIDS; ++i) {
    for (int c = 0; c <= 4; ++c) {
      const auto hh = tbl1[i + 1][n];

      if (h + hh <= hash) {
        h += hh;
        ++n;
      }
      else {
        hand[i] = c;
        break;
      }
    }
  }

  return hand;
}

void test(uint64_t begin, const uint64_t end, const Table1& table1, const Calsht& calsht)
{
  for (; begin < end; ++begin) {
    auto hand = decode(begin, table1);
    const auto [sht, mode] = calsht(hand, NUM_TILES / 3, MODE);
    const auto shanten = calc_shanten(hand, NUM_CALLS);

    assert(sht - 1 == shanten);
  }
}

int main()
{
  std::cout << "Number of threads: " << NUM_THREADS << std::endl;

  Calsht calsht;

  calsht.initialize(INDEX_FILE_PATH);

  Table1 table1{};

  table1[NUM_TIDS][NUM_TILES] = 1ull;

  build_table1(table1);

  const uint64_t num_hands = table1[0][0];

  std::cout << "Number of hands: " << num_hands << std::endl;

  std::vector<std::thread> threads;

  const uint64_t step = num_hands / NUM_THREADS;

  const auto start = std::chrono::system_clock::now();

  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.emplace_back(test, step * i, step * (i + 1), std::cref(table1), std::cref(calsht));
  }

  test(step * NUM_THREADS, num_hands, table1, calsht);

  for (auto& thread : threads) {
    thread.join();
  }

  const auto end = std::chrono::system_clock::now();

  std::cout << "Time (msec.): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

  return EXIT_SUCCESS;
}
