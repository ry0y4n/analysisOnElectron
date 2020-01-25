import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

#df = pd.read_csv("raw_result.csv", encoding="utf_8", usecols=[1,2,3,4,5,6])
df = pd.read_csv("fixation_list.csv")

#print(df)
#print(type(df))
"""
sns.relplot(x="fixation_start", y="fixation_time", hue="class", size="fixation_count",
            sizes=(40, 400), alpha=.5, palette="muted",
            height=6, data=df)
"""
sns.relplot(x="fixation_start", y="fixation_time", hue="class", kind="line", data=df)


plt.show()
