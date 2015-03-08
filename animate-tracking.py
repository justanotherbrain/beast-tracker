from matplotlib.pylab import *
import time
import numpy as np


ion()

eye_positions = np.genfromtxt("jester001.csv",dtype=float,delimiter=',')
pos, = plot(eye_positions[0][0],eye_positions[0][1],'+')
pos.set_markersize(15)

pos.axes.set_xlim(0,1600)
pos.axes.set_ylim(0,800)

for i in range(len(eye_positions)):
	pos.set_xdata(eye_positions[i][0])
	pos.set_ydata(eye_positions[i][1])
	
	draw()
	time.sleep(.01)


