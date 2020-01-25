import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from matplotlib.patches import Patch

#df = pd.read_csv("raw_result_remodel.csv", encoding="utf_8", usecols=[1,2,3,4,5,6])
df = pd.read_csv("fixation_list.csv", usecols=[1,2,3,4,5])

sns.set(style="whitegrid", palette="pastel", color_codes=True)

patch1 = Patch(
    facecolor="r",edgecolor='r',label='M'
)
patch2 = Patch(
    facecolor="b" ,edgecolor='b',label='not M'
)

grid = sns.pairplot(df, hue="class", palette=["r", "b"], markers=["o", "x"])

#grid.fig.legend(labels=["M", "not M"], handles=[patch1, patch2], title="class", bbox_to_anchor=(0.92, 0.925, 0.07, 0.15), loc='lower left', mode="expand", borderaxespad=0.0, fontsize=16, frameon=False)
grid.fig.axes[0].set_ylabel("fixation start (s)", size=20)
grid.fig.axes[4].set_ylabel("fixation time (s)", size=20)
grid.fig.axes[8].set_ylabel("fixation object", size=20)
grid.fig.axes[12].set_ylabel("fixation count", size=20)

grid.fig.axes[12].set_xlabel("fixation start (s)", size=20)
grid.fig.axes[13].set_xlabel("fixation time (s)", size=20)
grid.fig.axes[14].set_xlabel("fixation object", size=20)
grid.fig.axes[15].set_xlabel("fixation count", size=20)

grid.fig.axes[0].tick_params(labelsize=15)
grid.fig.axes[4].tick_params(labelsize=15)
grid.fig.axes[8].tick_params(labelsize=15)
grid.fig.axes[12].tick_params(labelsize=15)
grid.fig.axes[13].tick_params(labelsize=15)
grid.fig.axes[14].tick_params(labelsize=15)
grid.fig.axes[15].tick_params(labelsize=15)

grid.fig.axes[0].set(yticks=[0, 20, 40, 60])
grid.fig.axes[4].set(yticks=[0, 4, 8, 12])
grid.fig.axes[8].set(yticks=[0, 1, 2])
grid.fig.axes[12].set(yticks=[0, 2, 4, 6, 8])

grid.fig.axes[12].set(xticks=[0, 20, 40, 60])
grid.fig.axes[13].set(xticks=[0, 4, 8, 12])
grid.fig.axes[14].set(xticks=[0, 1, 2])
grid.fig.axes[15].set(xticks=[0, 2, 4, 6, 8])

plt.show()

"""
df = pd.read_csv("raw_result_remodel.csv", encoding="utf_8", usecols=[1,2,3,4,5,6])

print(df)
print(type(df))

g = sns.PairGrid(df, hue="class", palette="husl")
g.map_diag(sns.kdeplot)
g.map_offdiag(sns.kdeplot, n_levels=6)

plt.show()
"""
