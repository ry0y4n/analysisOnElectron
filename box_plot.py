import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

"""
df = pd.read_csv("fixation_list.csv")
print(df)

colors = ["windows blue", "amber", "greyish", "faded green", "dusty purple"]

sns.boxplot(x="class", y="fixation_time", palette=sns.color_palette("hls", 8), data=df)
sns.swarmplot(x="class", y="fixation_time",palette=sns.color_palette("RdBu", n_colors=7), data=df)
"""
fig = plt.figure(figsize=(12, 8))

ax = fig.add_subplot(111)

sns.set(style="whitegrid", palette="muted", color_codes=True)

# Load the example tips dataset
data = pd.read_csv("raw_result.csv", usecols=[1,2,3,4,5,6])

# Draw a nested boxplot to show bills by day and time
sns.boxplot(x="class", y="transition", palette=["g", "r"], data=data)

#ax.legend(loc=1, bbox_to_anchor=(0.5, 0.5, 0.5, 0.5), borderaxespad=0.1)
#ax.legend(bbox_to_anchor=(0.8, 0.8, 0.1, 0.2), loc='lower left', mode="expand", borderaxespad=0.0, fontsize=16)
ax.set_xlabel("class", size=25)
ax.set_ylabel("transition", size=25)

#plt.xticks([0, 1, 2], ["front", "plan", "right"])
plt.tick_params(labelsize=20)

plt.show()