#include "shanten.hpp"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
constexpr int NUM_TIDS = 34;
constexpr int NUM_TILES = 14;
constexpr size_t NUM_RECORDS = 10000u;
using HandShanten = std::pair<std::vector<int>, int>;

void read(std::vector<HandShanten>& hand_shanten, std::ifstream& fin)
{
  assert(hand_shanten.size() == NUM_RECORDS);

  int tmp;

  for (std::size_t i = 0u; i < NUM_RECORDS; ++i) {
    for (int j = 0; j < NUM_TILES; ++j) {
      fin >> tmp;
      ++hand_shanten[i].first[tmp];
    }

    fin >> hand_shanten[i].second >> tmp >> tmp;
  }
}

void test(std::vector<HandShanten>& hand_shanten)
{
  assert(hand_shanten.size() == NUM_RECORDS);

  for (std::size_t i = 0u; i < NUM_RECORDS; ++i) {
    assert(calc_shanten(hand_shanten[i].first) == hand_shanten[i].second);
  }
}

int main()
{
  {
    std::ifstream fin("../resources/p_normal_10000.txt");

    if (!fin) return EXIT_FAILURE;

    std::vector<HandShanten> hand_shanten{NUM_RECORDS, {std::vector<int>(NUM_TIDS, 0), 0}};

    read(hand_shanten, fin);

    const auto start = std::chrono::system_clock::now();

    test(hand_shanten);

    const auto end = std::chrono::system_clock::now();

    std::cout << "[normal] " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " msec.\n";
  }

  {
    std::ifstream fin("../resources/p_tin_10000.txt");

    if (!fin) return EXIT_FAILURE;

    std::vector<HandShanten> hand_shanten{NUM_RECORDS, {std::vector<int>(NUM_TIDS, 0), 0}};

    read(hand_shanten, fin);

    const auto start = std::chrono::system_clock::now();

    test(hand_shanten);

    const auto end = std::chrono::system_clock::now();

    std::cout << "[tinitu] " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " msec.\n";
  }

  {
    std::ifstream fin("../resources/p_hon_10000.txt");

    if (!fin) return EXIT_FAILURE;

    std::vector<HandShanten> hand_shanten{NUM_RECORDS, {std::vector<int>(NUM_TIDS, 0), 0}};

    read(hand_shanten, fin);

    const auto start = std::chrono::system_clock::now();

    test(hand_shanten);

    const auto end = std::chrono::system_clock::now();

    std::cout << "[honitu] " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " msec.\n";
  }

  {
    std::ifstream fin("../resources/p_koku_10000.txt");

    if (!fin) return EXIT_FAILURE;

    std::vector<HandShanten> hand_shanten{NUM_RECORDS, {std::vector<int>(NUM_TIDS, 0), 0}};

    read(hand_shanten, fin);

    const auto start = std::chrono::system_clock::now();

    test(hand_shanten);

    const auto end = std::chrono::system_clock::now();

    std::cout << "[kokusi] " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " msec.\n";
  }

  return EXIT_SUCCESS;
}
