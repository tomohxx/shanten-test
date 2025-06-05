# Shanten Test

## ビルド

事前に[shanten-number](https://github.com/tomohxx/shanten-number)をインストールしておく.

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

## 構成

| 実行ファイル | 説明                                                     |
| :----------- | :------------------------------------------------------- |
| test1        | すべての手牌の向聴数を正確に計算できるかテストする[^1]   |
| test2        | すべての手牌の向聴数を正確に計算できるかテストする[^2]   |
| test3        | あら氏のテストケースを使用して向聴数の計算速度を測定する |

[^1]: 長時間かかるため実行は推奨しない.
[^2]: N枚版Nyantenの逆変換を使用して並列に実行する.

## 使用方法

### 精度をテストする場合

test1またはtest2を実行する.

### 計算速度を測定する場合

1. [resources](resources)にあら氏のテストケース[^3]を格納する.
     - p_normal_10000.txt
     - p_hon_10000.txt
     - p_tin_10000.txt
     - p_koku_10000.txt
2. test3を実行する.

[^3]: https://mahjong.ara.black/etc/shanten/shanten9.htm から入手する.

## ライセンス

MIT License.
