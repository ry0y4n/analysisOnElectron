import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.patches import Patch


fig = plt.figure(figsize=(12, 8))

ax = fig.add_subplot(111)
sns.set(style="whitegrid", palette="pastel", color_codes=True)

patch1 = Patch(
    facecolor="y", edgecolor='y', label='M'
)
patch2 = Patch(
    facecolor="b", edgecolor='b', label='not M'
)

# Load the example tips dataset
data = pd.read_csv("fixation_list.csv")

# Draw a nested violinplot and split the violins for easier comparison
sns.violinplot(x="fixation_object", y="fixation_time", hue="class",
               split=True, inner="quart",
               palette={"M": "y", "Z": "b"},
               data=data)
sns.despine(left=True)

#ax.legend(labels=["M", "not M"], handles=[patch1, patch2], title="class", title_fontsize=20, bbox_to_anchor=(0.8, 0.8, 0.15, 0.2), loc='lower left', mode="expand", borderaxespad=0.0, fontsize=20)
ax.set_xlabel("object", size=25)
ax.set_ylabel("fixation time", size=25)
plt.xticks([0, 1, 2], ["front", "plan", "right"])
plt.tick_params(labelsize=20)

plt.show()