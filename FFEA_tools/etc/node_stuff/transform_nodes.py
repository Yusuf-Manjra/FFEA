import os, sys
import math
from Vectors import *

def mat_mult(mat, vec):
	new_vec = [0.0,0.0,0.0]
	for i in range(3):
		for j in range(3):
			new_vec[i] = new_vec[i] + mat[i][j] * vec[j]
	return new_vec

def node_translate(nodes, list_of_nodes, trans):
	for i in list_of_nodes:
		nodes[i].x += trans.x
		nodes[i].y += trans.y
		nodes[i].z += trans.z

def node_rotate(nodes, list_of_nodes, rot, cent):
	rot.x = math.radians(rot.x)
	rot.y = math.radians(rot.y)
	rot.z = math.radians(rot.z)
	rot_mat = [[0.0, 0.0, 0.0],[0.0, 0.0, 0.0],[0.0, 0.0, 0.0]]
	rot_mat[0][0] = math.cos(rot.y) * math.cos(rot.z)
	rot_mat[0][1] = math.cos(rot.x) * math.sin(rot.z) + math.cos(rot.z) * math.sin(rot.x) * math.sin(rot.y)
	rot_mat[0][2] = math.sin(rot.x) * math.sin(rot.z) - math.cos(rot.x) * math.sin(rot.y) * math.cos(rot.z)
	rot_mat[1][0] = -math.cos(rot.y) * math.sin(rot.z)
	rot_mat[1][1] = math.cos(rot.x) * math.cos(rot.z) - math.sin(rot.x) * math.sin(rot.y) * math.sin(rot.z)
	rot_mat[1][2] = math.sin(rot.x) * math.cos(rot.z) + math.cos(rot.x) * math.sin(rot.y) * math.sin(rot.z)
	rot_mat[2][0] = math.sin(rot.y)
	rot_mat[2][1] = -math.sin(rot.x) * math.cos(rot.y)
	rot_mat[2][2] = math.cos(rot.x) * math.cos(rot.y)

	node_vec = [0.0, 0.0, 0.0]
	for i in range(num_nodes):
		node_vec[0] = nodes[i].x - cent.x
		node_vec[1] = nodes[i].y - cent.y
		node_vec[2] = nodes[i].z - cent.z

		new_node_vec = mat_mult(rot_mat, node_vec)

		nodes[i].x = new_node_vec[0] + cent.x
		nodes[i].y = new_node_vec[1] + cent.y
		nodes[i].z = new_node_vec[2] + cent.z

def write_output(node_fname):
	outfile = open(node_fname, "w")
	outfile.write("ffea node file\n")
	outfile.write("num_nodes " + str(num_nodes) + "\n")
	outfile.write("num_surface_nodes " + str(num_surface_nodes) + "\n")
	outfile.write("num_interior_nodes " + str(num_interior_nodes) + "\n")
	outfile.write("surface nodes:\n")
	for i in range(num_surface_nodes):
		outfile.write(str(node[i].x) + " " + str(node[i].y) + " " + str(node[i].z) + "\n")
	outfile.write("interior nodes:\n")
	for i in range(num_interior_nodes):
		outfile.write(str(node[i + num_surface_nodes].x) + " " + str(node[i + num_surface_nodes].y) + " " + str(node[i + num_surface_nodes].z) + "\n")

if len(sys.argv) != 3:
	sys.exit("Usage: python transform_nodes.py [INPUT NODE FILE] [OUTPUT NODE FILE]")

node_lines = open(sys.argv[1], "r")
node = []

node_lines.readline() # ffea node file
num_nodes = int(node_lines.readline().split()[1])
num_surface_nodes = int(node_lines.readline().split()[1])
num_interior_nodes = int(node_lines.readline().split()[1])

node_lines.readline() # surface nodes:
for i in range(num_surface_nodes):
	sline = node_lines.readline().split()
	node.append(vector3(float(sline[0]), float(sline[1]), float(sline[2])))

node_lines.readline() # interior nodes:
for i in range(num_interior_nodes):
	sline = node_lines.readline().split()
	node.append(vector3(float(sline[0]), float(sline[1]), float(sline[2])))


list_of_nodes = []

# How do we decide which nodes to transform?
method = raw_input("Which nodes to transform? All (a), choose radially (r) or choose_by_volume (v)?:")

if method == "a" or method == "A":
	for i in range(len(node)):
		list_of_nodes.append(i)

elif method == "r" or method == "R":
	origin = vector3(0.0, 0.0, 0.0)
	origin.x = input("Input radial center x coordinate:")
	origin.y = input("Input radial center y coordinate:")
	origin.z = input("Input radial center z coordinate:")
	min_radius = input("Input minimum radius limit:")
	max_radius = input("Input maximum radius limit:")
	print min_radius * 5

	for i in range(len(node)):
		radius = node[i] - origin
		if radius.mag() > min_radius and radius.mag() < max_radius:
			list_of_nodes.append(i)

elif method == "v" or method == "V":	
	sys.exit("Not fixed yet. Come back soon!")
else:
	sys.exit("Invalid choice. Bye.")

print "Num nodes in selection = " + str(len(list_of_nodes)) + " of a possible " + str(len(node))

# Actual transform
done = 0
while(done == 0):
	transform = raw_input("Would you like to translate (t), rotate (r) or finish (f)?:")
	if(transform == "t"):
		translation = vector3(0.0, 0.0, 0.0)
		translation.x = input("Enter x translation:")
		translation.y = input("Enter y translation:")
		translation.z = input("Enter z translation:")
		print node[0].x
		node_translate(node, list_of_nodes, translation)
		print node[0].x

	elif(transform == "r"):
		rotation = vector3(0.0, 0.0, 0.0)
		center = vector3(0.0, 0.0, 0.0)
		rotation.x = input("Enter angle (in degrees) for x rotation:")
		rotation.y = input("Enter angle (in degrees) for y rotation:")
		rotation.z = input("Enter angle (in degrees) for z rotation:")
		center.x = input("Enter center of rotation, x:")
		center.y = input("Enter center of rotation, y:")
		center.z = input("Enter center of rotation, z:")

		node_rotate(node, list_of_nodes, rotation, center)
	elif(transform == "f"):
		write_output(sys.argv[2])
		done = 1
	else:
		print "Please enter either 'translate', 'rotate' or 'finish'\n"
