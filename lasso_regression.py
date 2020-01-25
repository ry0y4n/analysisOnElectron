# 各モジュールのインポート
from sklearn.linear_model import Lasso
import pandas as pd
import matplotlib.pyplot as plt

# 図の台紙となるfigの作成
fig = plt.figure(figsize=(12, 8))
# 台紙の上に付箋的な役割のaxを作成（この付箋に点をプロットなり，描画していくイメージ）
ax = fig.add_subplot(111)

# fixation_list.csvを読み込む（dfはデータの略）
df = pd.read_csv("fixation_list.csv")
df["fixation_time"] = df["fixation_time"] / 1000

# dfの中からM科のデータを取り出し，x_mとy_mに代入
x_m = df[df["class"] == "M"][["fixation_start"]]
y_m = df[df["class"] == "M"]["fixation_time"]

# dfの中からM科以外のデータを取り出し，x_mとy_mに代入
x_z = df[df["class"] == "Z"][["fixation_start"]]
y_z = df[df["class"] == "Z"]["fixation_time"]

# Lasso回帰モデルのインスタンスの作成
lss = Lasso()
# インスタンスを利用して学習（ここがLasso回帰）
lss.fit(x_m, y_m)
# 学習したモデルを使って，元データx_mから予測データy_m_predを算出
y_m_pred = lss.predict(x_m)

# 上と同様にM科以外のデータもLasso回帰して予測データを算出
lss2 = Lasso()
lss2.fit(x_z, y_z)
y_z_pred = lss2.predict(x_z)

# 算出したデータを描画．plotは点を繋いだ直線をプロットする
plt.plot(x_m, y_m_pred, color="tomato", label="M", linewidth=10, zorder=0)
plt.plot(x_z, y_z_pred, color="cyan", label="not M", linewidth=10, zorder=0)

# 算出したデータを描画．scatterは点をプロットする（散布図）
plt.scatter(x_m, y_m_pred, color="darkred", linewidth=10, zorder=1, alpha=0.7, marker="o")
plt.scatter(x_z, y_z_pred, color="midnightblue", linewidth=10, zorder=1, alpha=0.7, marker="D")

# ラベルの値とサイズを設定する．
plt.xlabel("fixation start time (s)", size=25)
plt.ylabel("fixation time (s)", size=25)

# 目盛の値を設定する
plt.xticks([0, 10, 20, 30, 40, 50, 60])
#plt.yticks([3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0])

# 目盛の値のサイズを設定する
plt.tick_params(labelsize=20)

# 凡例のタイトル，位置，サイズなどの設定
#ax.legend(title="class", title_fontsize=20, bbox_to_anchor=(0.7, 0.8, 0.15, 0.2), loc='lower left', mode="expand", borderaxespad=0.0, fontsize=20)

# 図として出力するための関数
#plt.get_current_fig_manager().full_screen_toggle()
plt.show()