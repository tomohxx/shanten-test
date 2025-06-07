# Shanten Test

ブロック分解方式の向聴数(シャンテン数)計算アルゴリズムの修正案とテスト.

## 修正案

[麻雀C言語プログラム集](https://web.archive.org/web/20190616213620/http://cmj3.web.fc2.com/)に記載のアルゴリズムをベースに孤立牌不足に対応できるように修正する.

- 4枚使いを双碰待ち(シャンポン待ち・シャボ待ち)とみなさい.
- 孤立牌不足になり得る手牌では孤立牌を数える.
- 枝刈りを導入して高速化する.

## 性能

### 正確

13枚と14枚の手牌で正確に向聴数を計算できることをテスト済.

### 高速

一手当たりの計算時間は1μs - 2μs.

### 省メモリ

計算済テーブルを使用しない.

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

1. [resources](resources)ディレクトリに以下のあら氏のテストケース[^3]を格納する.
   - p_normal_10000.txt
   - p_hon_10000.txt
   - p_tin_10000.txt
   - p_koku_10000.txt
2. test3を実行する.

    実行例(@m5zn.2xlarge):
    ```
    $ ./test3
    [normal] 14 msec.
    [tinitu] 38 msec.
    [honitu] 17 msec.
    [kokusi] 10 msec.
    ```

[^3]: https://mahjong.ara.black/etc/shanten/shanten9.htm から入手する.

## 参考

- https://zenn.dev/tomohxx/articles/aecace4e3a3bc1 (孤立牌不足について)
- https://github.com/tomohxx/shanten-number (より高速な向聴数計算ツール)

## ライセンス

MIT License.
