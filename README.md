# analysisOnElectron

**卒業研究での視線解析プロセスをGUIで実行するため開発したMacOS&WindowsOS向けデスクトップアプリです．**

# 注意次項

このアプリは卒研の終盤で思いつき，**2週間弱**で開発したものです．それゆえいくつかの不都合があります．．．

- Path管理がまずい
  - 動的な指定に対応していません．なので`bat`，`sh`ファイルのPathが`users/momosuke/~~~`になっています．なので`clone`したらすぐに該当部分を自分のユーザー名に書き換えてください．
- パッケージング化に対応できていない
  - 上の問題に付随して`exe`ファイルにパッケージングすると実行ファイルのPathが上手くいかないです．なので現状，ローカルサーバー上での用途のみを推奨しています．


# Requirement
> 解析プロセスを実行するために必要なライブラリ・モジュールを以下に列挙する

* matplotlib
* seaborn
* sklearn
* pandas


# Installation

Requirementで列挙したライブラリなどのインストール方法として，ここでは[Anaconda](https://www.anaconda.com/)での環境構築を強く推奨する．ドキュメントは豊富なので各自OSに合わせたインストール方法を調べて実行してください．

※WindowsOSを使っている場合は，Anaconda環境をグローバルに使えるようにする必要があります(ここでのグローバルとは，どこからでもAnacondaモジュール・ライブラリが使用可能な環境を指す(`import matplotlib`などができるかどうか))．

そのための情報をQiitaに流したのでよければ参考にしてください．

[windowsでanaconda環境をコマンドプロンプトからでも使用可能にする](https://qiita.com/momosuke/items/fd6f8f9d01d2f57be90e)


# To Use

このレポジトリをクローンして実行するためには[Git](https://git-scm.com)と[Node.js](https://nodejs.org/en/download/)(which comes with [npm](http://npmjs.com))があなたのコンピュータに必要です．

以下をコマンドラインで入力してください．

```bash
# Clone this repository
git clone https://github.com/ry0y4n/analysisOnElectron
# Go into the repository
cd analysisOnElectron
# Install dependencies
npm install
# Run the app
npm start
```

Note: もしあなたがLinux Bash for Windowsを使っているなら，[このガイドを見てください](https://www.howtogeek.com/261575/how-to-run-graphical-linux-desktop-applications-from-windows-10s-bash-shell/)それか`node` をコマンドプロンプトから使用してください．

# Author

**[モモスケ](https://twitter.com/AnoTensai)**

米子高専山口研究室の学生は連絡(実装したい機能やその他やりたいことについて)してくれれば返答します．

# License
ライセンスを明示する

**analysisOnElectron**is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).