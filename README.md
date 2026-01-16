# st-paths (s-t パス数え上げ, TdZdd)

## ビルド
`./tdzdd/include` に TdZdd を配置する前提です。別の場所にある場合は `INCLUDES` を指定してください。

```sh
make
```

例:

```sh
make INCLUDES=-I/path/to/tdzdd/include
```

## 実行
```sh
./path_count grid11x11.grh
```

出力は s-t パスの総数のみです。

## s-t パスをすべて出力（小規模グラフ用）
```sh
./path_count grid4x4.grh --all
```

1 行に 1 つのパスを、頂点番号を空白区切りで出力します。
