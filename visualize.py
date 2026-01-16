import pandas as pd
import matplotlib.pyplot as pl
from matplotlib.ticker import MultipleLocator

df = pd.read_csv('results.csv')
fig, ax = pl.subplots(1, 3, figsize=(14, 4))
for i in ax:
    i.margins(x=0)
    i.xaxis.set_major_locator(MultipleLocator(1))
df.pivot_table(index='Nthreads',
               columns='Algorithm',
               values='Time(ms)').plot(ax=ax[0],
                                       grid=True, title='Time')
df.pivot_table(index='Nthreads',
               columns='Algorithm',
               values='SpeedUp').plot(ax=ax[1],
                                      grid=True, title='SpeedUp')
df.pivot_table(index='Nthreads',
               columns='Algorithm',
               values='Efficiency').plot(ax=ax[2],
                                         grid=True, title='Efficiency')
pl.show()
