## エフェクター再現プロジェクト

# 概要

このプロジェクトは、マイクの入力にエフェクトをリアルタイムでかけることを実現するためのものです。

# インストール

Visual Studio 2022
C++ Compiler
PortAudio Library インストール先: https://www.portaudio.com/docs/v19-doxydocs/index.html

# 使い方
Project2/Project2/test.cppをVS 2022でコンパイル、実行

# 今後の計画
エフェクタを組み込む。リバーブかな

# 仕組み
1フレームごとのbufferの大きさをFRAMES_PER_BUFFERで指定している
1フレームが満タンになる度にpatestCallback関数が実行される
オーディオデータを保存しているメモリの先頭アドレスはinputBufferでずっと変わらない　→　なので前のデータを使おうとすると

# 課題
p135. コムフィルタと全域通過フィルタの違い(z^-d[0]と^-d[1]の違い)がわからない
リバーブをIIRフィルタを実装したい。
前のデータを使おうとすると無限にメモリが必要なのでは?という疑問があります。