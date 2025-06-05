#include "shanten.hpp"
#include <iostream>
constexpr int MAX_BLOCKS = 4;
constexpr int MAX_SHANTEN = 8;
constexpr int NUM_TIDS = 34;

struct Result {
  int num_meld = 0;    // 面子数
  int num_premeld = 0; // 搭子数
  int num_pair = 0;    // 雀頭数
};

int get_blocks(const Result& result)
{
  return result.num_meld + result.num_premeld;
}

int calc_shanten(const Result& result)
{
  return MAX_SHANTEN - result.num_meld * 2 - result.num_premeld - result.num_pair;
}

int calc_lower_bound(const Result& result)
{
  return MAX_BLOCKS - result.num_meld - result.num_pair;
}

bool is_suit(int index)
{
  return index < 27;
}

bool is_honor(int index)
{
  return !is_suit(index);
}

void cut_meld(std::vector<int>& hand,
              const std::vector<int>& original,
              Result& result,
              int& min_shanten,
              int pos_pair,
              int tid);
void cut_premeld(std::vector<int>& hand,
                 const std::vector<int>& original,
                 Result& result,
                 int& min_shanten,
                 int pos_pair,
                 int tid,
                 int lower_bound);
void cut_single_tile_for_pair(std::vector<int>& hand,
                              const std::vector<int>& original,
                              Result& result,
                              int& min_shanten);
void cut_single_tile_for_meld(std::vector<int>& hand,
                              const std::vector<int>& original,
                              Result& result,
                              int& min_shanten);
void cut_single_tile_for_pair_and_meld(std::vector<int>& hand,
                                       const std::vector<int>& original,
                                       Result& result,
                                       int& min_shanten);

int calc_shanten(std::vector<int>& hand, const int num_calls)
{
  int min_shanten = MAX_SHANTEN;
  const auto original = hand;
  Result result{num_calls, 0, 0};

  for (int tid = 0; tid < NUM_TIDS; ++tid) {
    if (hand[tid] >= 2) {
      ++result.num_pair;
      hand[tid] -= 2;
      cut_meld(hand, original, result, min_shanten, tid, 0);
      --result.num_pair;
      hand[tid] += 2;
    }
  }

  cut_meld(hand, original, result, min_shanten, -1, 0);

  return min_shanten;
}

void cut_meld(std::vector<int>& hand,
              const std::vector<int>& original,
              Result& result,
              int& min_shanten,
              const int pos_pair,
              const int tid)
{
  if (tid >= NUM_TIDS) {
    const int lower_bound = calc_lower_bound(result);

    return cut_premeld(hand, original, result, min_shanten, pos_pair, 0, lower_bound);
  }

  // 刻子
  if (hand[tid] >= 3) {
    ++result.num_meld;
    hand[tid] -= 3;
    cut_meld(hand, original, result, min_shanten, pos_pair, tid);
    --result.num_meld;
    hand[tid] += 3;
  }

  // 順子
  if (is_suit(tid) && tid % 9 < 7 && hand[tid] > 0 && hand[tid + 1] > 0 && hand[tid + 2] > 0) {
    ++result.num_meld;
    --hand[tid];
    --hand[tid + 1];
    --hand[tid + 2];
    cut_meld(hand, original, result, min_shanten, pos_pair, tid);
    --result.num_meld;
    ++hand[tid];
    ++hand[tid + 1];
    ++hand[tid + 2];
  }

  cut_meld(hand, original, result, min_shanten, pos_pair, tid + 1);
}

void cut_premeld(std::vector<int>& hand,
                 const std::vector<int>& original,
                 Result& result,
                 int& min_shanten,
                 const int pos_pair,
                 const int tid,
                 const int lower_bound)
{
  // 枝刈り
  if (min_shanten <= lower_bound) {
    return;
  }

  if (tid >= NUM_TIDS) {
    if ((result.num_meld == 4 && result.num_premeld == 0 && result.num_pair == 0) ||
        (result.num_meld == 3 && result.num_premeld == 1 && result.num_pair == 0)) {
      // 孤立牌不足パターン1: 孤立牌 -> 雀頭
      return cut_single_tile_for_pair(hand, original, result, min_shanten);
    }
    else if (result.num_meld == 3 && result.num_premeld == 0 && result.num_pair == 1) {
      // 孤立牌不足パターン2: 孤立牌 -> 面子
      return cut_single_tile_for_meld(hand, original, result, min_shanten);
    }
    else if (result.num_meld == 3 && result.num_premeld == 0 && result.num_pair == 0) {
      // 孤立牌不足パターン3: 孤立牌1 -> 雀頭, 孤立牌2 -> 面子
      return cut_single_tile_for_pair_and_meld(hand, original, result, min_shanten);
    }
    else {
      // 孤立牌十分
      min_shanten = std::min(min_shanten, calc_shanten(result));

      return;
    }
  }

  if (get_blocks(result) < 4) {
    // 対子
    if (hand[tid] == 2 && tid != pos_pair) {
      ++result.num_premeld;
      hand[tid] -= 2;
      cut_premeld(hand, original, result, min_shanten, pos_pair, tid, lower_bound);
      --result.num_premeld;
      hand[tid] += 2;
    }

    // 両面/辺張搭子
    if (is_suit(tid) && tid % 9 < 8 && hand[tid] > 0 && hand[tid + 1] > 0) {
      ++result.num_premeld;
      --hand[tid];
      --hand[tid + 1];
      cut_premeld(hand, original, result, min_shanten, pos_pair, tid, lower_bound);
      --result.num_premeld;
      ++hand[tid];
      ++hand[tid + 1];
    }

    // 嵌張搭子
    if (is_suit(tid) && tid % 9 < 7 && hand[tid] > 0 && hand[tid + 2] > 0) {
      ++result.num_premeld;
      --hand[tid];
      --hand[tid + 2];
      cut_premeld(hand, original, result, min_shanten, pos_pair, tid, lower_bound);
      --result.num_premeld;
      ++hand[tid];
      ++hand[tid + 2];
    }
  }

  cut_premeld(hand, original, result, min_shanten, pos_pair, tid + 1, lower_bound);
}

void cut_single_tile_for_pair(std::vector<int>& hand,
                              const std::vector<int>& original,
                              Result& result,
                              int& min_shanten)
{
  for (int tid = 0; tid < NUM_TIDS; ++tid) {
    if (hand[tid] > 0 && original[tid] < 3) {
      min_shanten = std::min(min_shanten, calc_shanten(result));

      return;
    }
  }

  min_shanten = std::min(min_shanten, calc_shanten(result) + 1);
}

void cut_single_tile_for_meld(std::vector<int>& hand,
                              const std::vector<int>& original,
                              Result& result,
                              int& min_shanten)
{
  for (int tid = 0; tid < NUM_TIDS; ++tid) {
    if ((is_suit(tid) && hand[tid] > 0) || (is_honor(tid) && hand[tid] > 0 && original[tid] < 3)) {
      min_shanten = std::min(min_shanten, calc_shanten(result));

      return;
    }
  }

  min_shanten = std::min(min_shanten, calc_shanten(result) + 1);
}

void cut_single_tile_for_pair_and_meld(std::vector<int>& hand,
                                       const std::vector<int>& original,
                                       Result& result,
                                       int& min_shanten)
{
  int cnt = 0;

  for (int tid = 0; tid < NUM_TIDS; ++tid) {
    if ((is_suit(tid) && hand[tid] > 0) || (is_honor(tid) && hand[tid] > 0 && original[tid] < 3)) {
      ++cnt;
    }
  }

  min_shanten = std::min(min_shanten, calc_shanten(result) + (cnt >= 2 ? 0 : 1));
}
