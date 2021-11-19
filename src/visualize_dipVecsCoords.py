import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import sys
import subprocess

def set_axes_equal(ax, zoom=0.5):
    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    x_middle = np.mean(x_limits)
    y_range = abs(y_limits[1] - y_limits[0])
    y_middle = np.mean(y_limits)
    z_range = abs(z_limits[1] - z_limits[0])
    z_middle = np.mean(z_limits)

    # The plot bounding box is a sphere in the sense of the infinity
    # norm, hence I call half the max range the plot radius. zoom = 0.5
    plot_radius = zoom*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])

def get_cmap(n, name='hsv'):
    # Returns a function that maps each index in n to a distinct color
    return plt.cm.get_cmap(name, n)

def make_movie_all(numElem, chainSize, steps, runs):
	filename_movie = "../res/dipVecsCoords_all_c"+str(chainSize)+"_e"+str(numElem)+"_r"+str(runs)+"_s"+str(steps)
	filename = "../res/snaps/frame_"+"_c"+str(chainSize)+"_e"+str(numElem)+"_r"+str(runs)+"_s"+str(steps)

	#subprocess.call('rm ../res/snaps/*', shell=True)

	step = 0
	with open(filename_movie+".txt") as f:
		while step < steps:
			if step == 0:
				line = f.readline()
			if line[:4] == '#run':
				step += 1
				line = f.readline()

				coords = []
				while line[:4] != '#run':
					if line[0] not in ['#', ' ', '\n']:
						line = line[:-1]
						coords.append(list(map(float, line.split('\t'))))
					line = f.readline()
					if not line:
						break

				coords = np.array(coords)

				fig = plt.figure()
				ax = fig.add_subplot(111, projection='3d')
				cmap = get_cmap(chainSize)
				for c in range(chainSize):
					if c == 0:
						ax.scatter(coords[:numElem**3,1], coords[:numElem**3,2], coords[:numElem**3,3], color=cmap(c))
					elif c == chainSize-1:
						ax.scatter(coords[numElem**3*c:,1], coords[numElem**3*c:,2], coords[numElem**3*c:,3], color=cmap(c))
					else:
						ax.scatter(coords[numElem**3*c:numElem**3*(c+1),1], coords[numElem**3*c:numElem**3*(c+1),2], coords[numElem**3*c:numElem**3*(c+1),3], color=cmap(c))
				ax.set_xlabel('x')
				ax.set_ylabel('y')
				ax.set_zlabel('z')
				set_axes_equal(ax, zoom=0.1)
				#plt.show()

				fig.savefig(filename+"_zoom_%03d.png"%(step-1), dpi=300)
				plt.close(fig)

	subprocess.call("ffmpeg -y -framerate 10 -i "+filename+"_zoom_%03d.png -pix_fmt yuv420p -vf scale=1280:-2 "+filename_movie+"_zoom_movie.mp4", shell=True)
	subprocess.call("ffmpeg -y -i "+filename_movie+"_zoom_movie.mp4 -vf 'fps=10,scale=340:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse' -loop 0 "+filename_movie+"_zoom_movie.gif", shell=True)

def main(numElem, name, chainSize, steps, runs):
	if name == 'all':
		make_movie_all(numElem, chainSize, steps, runs)

	else:
		filename = "../res/dipVecsCoords_"+name+"_c"+str(chainSize)+"_e"+str(numElem)+"_r"+str(runs)+"_s"+str(steps)
		coords = []
		with open(filename+".txt") as f:
			for line in f:
				if line[0] not in ['#', ' ', '\n']:
					line = line[:-1]
					coords.append(list(map(float, line.split('\t'))))

		coords = np.array(coords)

		fig = plt.figure()
		ax = fig.add_subplot(111, projection='3d')
		cmap = get_cmap(chainSize)
		for c in range(chainSize):
			if c == 0:
				ax.scatter(coords[:numElem**3,0], coords[:numElem**3,1], coords[:numElem**3,2], color=cmap(c))
			elif c == chainSize-1:
				ax.scatter(coords[numElem**3*c:,0], coords[numElem**3*c:,1], coords[numElem**3*c:,2], color=cmap(c))
			else:
				ax.scatter(coords[numElem**3*c:numElem**3*(c+1),0], coords[numElem**3*c:numElem**3*(c+1),1], coords[numElem**3*c:numElem**3*(c+1),2], color=cmap(c))
		ax.set_xlabel('x')
		ax.set_ylabel('y')
		ax.set_zlabel('z')
		set_axes_equal(ax, zoom=0.06)
		#ax.view_init(elev=ax.elev+10., azim=ax.azim)
		fig.savefig(filename+".png", dpi=300)
		plt.show()
		#plt.close(fig)

if __name__ == "__main__":
	main(int(sys.argv[1]), sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5]))

