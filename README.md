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

[^1]: 長時間かかるため実行は推奨しない
[^2]: N枚版Nyantenの逆変換を使用して並列に実行する

## ライセンス

MIT License.
