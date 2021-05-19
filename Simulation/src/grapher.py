import pandas as pd
import matplotlib.pyplot as plt
from pandas.core import indexing
DENSITY_FILEs = ["Analysis.csv"]

plt.rcParams.update({'font.size': 20})

for INPUT_FILE in DENSITY_FILEs:
    df = pd.read_csv(INPUT_FILE)
    df.sort_values(by = ['Optimal'], inplace=True)
    df.reset_index(inplace = True)
    df.plot(y = ['Optimal','Experimental'])
    plt.ylabel('Distance (in block units)')
    plt.xlabel('Trials')
    plt.xticks([0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
    plt.yticks([x for x in range(0, 261, 20)])
    plt.grid(b=True, which='major', color='#666666', linestyle='-')
    plt.minorticks_on()
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
    fig1 = plt.gcf()
    fig1.set_size_inches(20, 10)
    plt.savefig(INPUT_FILE[:-4]+".jpg")
