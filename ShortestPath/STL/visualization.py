import numpy
from stl import mesh
from mpl_toolkits import mplot3d
from matplotlib import pyplot
import sys

dir = 'STL/'
fileName = sys.argv[1]

your_mesh = mesh.Mesh.from_file(fileName)

def read_file(file_name):
	f = open(file_name,"r")
	points = []
	for line in f:
		pointsLine = line.replace('\n', '')
		pointsV = pointsLine.split(' ')
		currentPointA = (float(pointsV[0]), float(pointsV[1]), float(pointsV[2]))
		currentPointB = (float(pointsV[3]), float(pointsV[4]), float(pointsV[5]))
		points.append([currentPointA, currentPointB])
	f.close()
	return points


figure = pyplot.figure()
axes = mplot3d.Axes3D(figure)


collection = mplot3d.art3d.Poly3DCollection(your_mesh.vectors)
collection.set_facecolor((1,1,1,0.0))
collection.set_edgecolor((0, 0, 0, 0.7))
collection.set_linewidth(0.3)


path_collection = mplot3d.art3d.Poly3DCollection(read_file(dir + "path.txt"))
path_collection.set_edgecolor('green')
path_collection.set_linewidth(2)

           
axes.add_collection3d(collection)
axes.add_collection3d(path_collection)


scale = your_mesh.points.flatten(-5)
axes.auto_scale_xyz(scale, scale, scale)
axes.can_zoom()


pyplot.show()