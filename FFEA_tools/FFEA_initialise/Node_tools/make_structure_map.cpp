#include <cmath>
#include <iostream>
#include <fstream>
#include <istream>
#include <cstdlib>
#include <string>
#include "Vectors.hpp"
#include "Matrices.hpp"
#define MAX_FNAME_SIZE 255

using namespace std;

// Class for storing node connectivities (FFEA elements)
class tet_element
{
	public:
		
		// Constructors/Destructors
		tet_element() {
			num_nodes = 4;
			n = new int[num_nodes];
			node = new vector3*[num_nodes];
			for(int i = 0; i < num_nodes; ++i) {
				n[i] = 0;
				node[i] = NULL;
			}
		}
		
		tet_element(int num_nodes) {
			this->num_nodes = num_nodes;
			n = new int[num_nodes];
			node = new vector3*[num_nodes];
			for(int i = 0; i < num_nodes; ++i) {
				n[i] = 0;
				node[i] = NULL;
			}
		}
		
		tet_element(int n0, int n1, int n2, int n3, vector3 *nodes) {
			num_nodes = 4;
			n = new int[num_nodes];
			node = new vector3*[num_nodes];
			n[0] = n0;
			n[1] = n1;
			n[2] = n2;
			n[3] = n3;
			node[0] = &nodes[n[0]];
			node[1] = &nodes[n[1]];
			node[2] = &nodes[n[2]];
			node[3] = &nodes[n[3]];
		}
		
		tet_element(int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, vector3 *nodes) {
			num_nodes = 10;
			n = new int[num_nodes];
			node = new vector3*[num_nodes];
			n[0] = n0;
			n[1] = n1;
			n[2] = n2;
			n[3] = n3;
			n[4] = n4;
			n[5] = n5;
			n[6] = n6;
			n[7] = n7;
			n[8] = n8;
			n[9] = n9;
			node[0] = &nodes[n[0]];
			node[1] = &nodes[n[1]];
			node[2] = &nodes[n[2]];
			node[3] = &nodes[n[3]];
			node[4] = &nodes[n[4]];
			node[5] = &nodes[n[5]];
			node[6] = &nodes[n[6]];
			node[7] = &nodes[n[7]];
			node[8] = &nodes[n[8]];
			node[9] = &nodes[n[9]];
		}
		
		~tet_element() {
			for(int i = 0; i < num_nodes; ++i) {
				n[i] = 0;
				node[i] = NULL;
			}
		}
		
		// Member functions
		void set_nodes_linear(int n0, int n1, int n2, int n3, vector3 *nodes) {
			n[0] = n0;
			n[1] = n1;
			n[2] = n2;
			n[3] = n3;
			node[0] = &nodes[n[0]];
			node[1] = &nodes[n[1]];
			node[2] = &nodes[n[2]];
			node[3] = &nodes[n[3]];
		}
		
		vector3 get_face_normal(int index) {
			vector3 a, b, normal;
			if(index == 0) {
				a = *node[2] - *node[0];
				b = *node[1] - *node[2];
			} else if(index == 1) {
				a = *node[2] - *node[1];
				b = *node[3] - *node[2];
			} else if(index == 2) {
				a = *node[3] - *node[0];
				b = *node[2] - *node[3];
			} else if(index == 3) {
				a = *node[1] - *node[0];
				b = *node[3] - *node[1];
			} else {
				cout << "Error. Invalid index, " << index << endl;
				exit(0);
			}
			
			normal = a.cross(b);
			normal.normalise();
			return normal;
		}
		
		vector3 get_face_centroid(int index) {
			vector3 midpoint;
			if(index == 0) {
				midpoint = *node[0] + *node[1] + *node[2];
				midpoint *= 1.0 / 3.0;
			} else if(index == 1) {
				midpoint = *node[1] + *node[2] + *node[3];
				midpoint *= 1.0 / 3.0;
			} else if(index == 2) {
				midpoint = *node[0] + *node[2] + *node[3];
				midpoint *= 1.0 / 3.0;
			} else if(index == 3) {
				midpoint = *node[0] + *node[1] + *node[3];
				midpoint *= 1.0 / 3.0;
			} else {
				cout << "Error. Invalid index, " << index << endl;
				exit(0);
			}
			
			return midpoint;
		}
		
		vector3 get_centroid() {
			vector3 centroid = *node[0] + *node[1] + *node[2] + *node[3];
			centroid *= 0.25;
			return centroid;
		}

		int get_nearest_node_index(vector3 anode) {
			
			int nearest_node;
			double shortest_distance;
			vector3 distance;
			
			shortest_distance = INFINITY;
			for(int i = 0; i < 4; ++i) {
				distance = *node[i] - anode;
				if (distance.r < shortest_distance) {
					shortest_distance = distance.r;
					nearest_node = i;
				}
			}
			return nearest_node;
		}

		// Data members
		int num_nodes;
		int *n;
		vector3 **node;
};

int get_num_nodes_from_file(string node_fname) {
	
	// Open file
	ifstream node_file;
	node_file.open(node_fname.c_str());
	
	string line;
	
	// Check correct file type
	getline(node_file, line);
	if (line.compare("ffea node file") != 0) {
		cout << "Error. Expected 'ffea node file', got '" << line << "'. Possibly not an ffea node file." << endl;
		exit(0);
	}
	
	// Get num_nodes
	int num_nodes;
	node_file >> line >> num_nodes;
	
	node_file.close();
	return num_nodes;
}

int get_num_elements_from_file(string top_fname) {
	
	// Open file
	ifstream top_file;
	top_file.open(top_fname.c_str());
	
	string line;
	
	// Check correct file type
	getline(top_file, line);
	if (line.compare("ffea topology file") != 0) {
		cout << "Error. Expected 'ffea topology file', got '" << line << "'. Possibly not an ffea topology file." << endl;
		exit(0);
	}
	
	// Get num_elements
	int num_elements;
	top_file >> line >> num_elements;
	
	top_file.close();
	return num_elements;
}

void extract_and_create_nodes(string node_fname, vector3 *node) {

	// Open file
	ifstream node_file;
	node_file.open(node_fname.c_str());
	
	string line;
	
	// Check correct file type
	getline(node_file, line);
	if (line.compare("ffea node file") != 0) {
		cout << "Error. Expected 'ffea node file', got '" << line << "'. Possibly not an ffea node file." << endl;
		exit(0);
	}
	
	// Get num_nodes and build node_list
	int num_nodes, num_surface_nodes, num_interior_nodes;
	node_file >> line >> num_nodes;
	node_file >> line >> num_surface_nodes;
	node_file >> line >> num_interior_nodes;

	// Read in surface nodes
	int i;
	double x, y, z;
	getline(node_file, line);
	getline(node_file, line);
	for(i = 0; i < num_surface_nodes; ++i) {
		node_file >> x >> y >> z;
		node[i].set_pos(x, y, z);
	}
	
	// Read in interior nodes
	getline(node_file, line);
	getline(node_file, line);
	for(i = 0; i < num_interior_nodes; ++i) {
		node_file >> x >> y >> z;
		node[i + num_surface_nodes].set_pos(x, y, z);
	}
	
	// Close and return pointer
	node_file.close();
	return;
}

void extract_and_create_topologies(string top_fname, tet_element *elem, vector3 *node) {

	// Open file
	ifstream top_file;
	top_file.open(top_fname.c_str());
	
	string line;
	
	// Check correct file type
	getline(top_file, line);
	if (line.compare("ffea topology file") != 0) {
		cout << "Error. Expected 'ffea topology file', got '" << line << "'. Possibly not an ffea topology file." << endl;
		exit(0);
	}
	
	// Get num_elements and build node_list
	int num_elements, num_surface_elements, num_interior_elements;
	top_file >> line >> num_elements;
	top_file >> line >> num_surface_elements;
	top_file >> line >> num_interior_elements;

	// Read in surface elements
	int i;
	int n[10];
	getline(top_file, line);
	getline(top_file, line);
	for(i = 0; i < num_surface_elements; ++i) {
		top_file >> n[0] >> n[1] >> n[2] >> n[3] >> n[4] >> n[5] >> n[6] >> n[7] >> n[8] >> n[9];
		elem[i].set_nodes_linear(n[0], n[1], n[2], n[3], node);
	}
	
	// Read in interior elements
	getline(top_file, line);
	getline(top_file, line);
	for(i = 0; i < num_interior_elements; ++i) {
		top_file >> n[0] >> n[1] >> n[2] >> n[3] >> n[4] >> n[5] >> n[6] >> n[7] >> n[8] >> n[9];
		elem[i + num_surface_elements].set_nodes_linear(n[0], n[1], n[2], n[3], node);
	}
	
	// Close and return pointer
	top_file.close();
	return;
}

void print_map_to_file(string map_fname, double **map, int num_nodes_from, int num_nodes_to, int sparsity) {

	int i, j;

	// Open file
	ofstream map_file;
	map_file.open(map_fname.c_str());

	// Write initial stuff
	ffea node file
	map_file << "ffea kinetic conformation mapping file" << endl;
	if(sparsity == 1) {
		map_file << "(sparse representation)" << endl;
	} else {
		map_file << "(dense representation)" << endl;
	}
	map_file << "num_nodes_from " << num_nodes_from << endl;
	map_file << "num_nodes_to " << num_nodes_to << endl;
	map_file << "map:" << endl;
	
	for(i = 0; i < num_nodes_to; ++i) {
		for(j = 0; j < num_nodes_from; ++j) {
			map_file << map[i][j] << " ";
		}
		map_file << endl;
	}
	return;
}

tet_element * find_containing_element(vector3 node, tet_element *top, int num_elements) {
	
	int i, j, num_faces_behind, check;
	double angle;
	vector3 face_normal, face_centroid, node_to_face;
	check = 0;

	for(i = 0; i < num_elements; ++i) {

		// Check if in element by calculating face normals and node-face_center vectors
		num_faces_behind = 0;
		for(j = 0; j < 4; ++j) {

			face_normal = top[i].get_face_normal(j);
			face_centroid = top[i].get_face_centroid(j);
			node_to_face = face_centroid - node;
			node_to_face.normalise();

			angle = acos(face_normal.dot(node_to_face));
			if(angle > M_PI / 2.0 || angle < M_PI / -2.0) {

				// Not in this element
				break;
			} else {
				num_faces_behind++;
			}
		}

		// If behind all faces, node is in element
		if(num_faces_behind == 4) {		
			return &top[i];
		}
	}

	// If we didn't trigger return condition, node is outside element
	return NULL;
}

tet_element * get_nearest_element(vector3 node, tet_element *top, int num_elements) {
	
	int i, j;
	double shortest_distance;
	vector3 distance;
	tet_element *nearest_element;
	shortest_distance = INFINITY;
	for(i = 0; i < num_elements; ++i) {
		for(j = 0; j < 4; ++j) {
			distance = *top[i].node[j] - node;
			if(distance.r < shortest_distance) {
				shortest_distance = distance.r;
				nearest_element = &top[i];
			}
		}
	}

	return nearest_element;
}

void get_single_node_map(vector3 node, tet_element *elem, double *map) {

	vector3 a, b, c, n, coeff;
	matrix33 M, M_inv;

	// Get the required vectors and make them a matrix
	a = *elem->node[1] - *elem->node[0];
	b = *elem->node[2] - *elem->node[0];
	c = *elem->node[3] - *elem->node[0];
	n = node - *elem->node[0];
	M.set(a, b, c);
	M_inv = M.get_inverse();
	coeff = M_inv.apply(n);
	map[0] = 1 - coeff.x - coeff.y - coeff.z;
	map[1] = coeff.x;
	map[2] = coeff.y;
	map[3] = coeff.z;
	return;
}

tet_element * make_containing_element(tet_element *old_containing_element, int nearest_node_index, int new_node_index, vector3 *from_node) {

	int i, new_node[4];
	tet_element *new_containing_element;
	new_containing_element = new tet_element;

	// Get and set new nodes
	for(i = 0; i < 4; ++i) {
		if(i == nearest_node_index) {
			new_node[i] = new_node_index;
		} else {
			new_node[i] = old_containing_element->n[i];
		}
	}
	new_containing_element->set_nodes_linear(new_node[0], new_node[1], new_node[2], new_node[3], from_node);
	return new_containing_element;
}

void add_map_to_big_map(double *little_map, double **big_map, int to, tet_element *containing_elem) {

	int i;
	for(i = 0; i < 4; ++i) {
		big_map[to][containing_elem->n[i]] = little_map[i];
	}
	return;
}

int main(int argc, char **argv) {

	// Check for the right input
	if(argc != 6) {
		cout << "Usage ./" << argv[0] << " [INPUT from .node] [INPUT from .top] [INPUT to .node] [INPUT to .top] [OUTPUT .map fname]" << endl;
		exit(0);
	}

	// Extract input
	string from_node_fname, to_node_fname, from_top_fname, to_top_fname, output_map_fname;
	from_node_fname = argv[1];
	to_node_fname = argv[3];
	from_top_fname = argv[2];
	to_top_fname = argv[4];
	output_map_fname = argv[5];

	// Create node lists and topologies and map
	int num_elements_from, num_elements_to, num_nodes_from, num_nodes_to;
	double **map;
	vector3 *from_node, *to_node;
	tet_element *from_top, *to_top;
	
	num_nodes_from = get_num_nodes_from_file(from_node_fname);
	num_nodes_to = get_num_nodes_from_file(to_node_fname);
	num_elements_from = get_num_elements_from_file(from_top_fname);
	num_elements_to = get_num_elements_from_file(to_top_fname);

	// Extra work node in from_node
	from_node = new vector3[num_nodes_from + 1];
	to_node = new vector3[num_nodes_to];
	from_top = new tet_element[num_elements_from];
	to_top = new tet_element[num_elements_to];
	
	extract_and_create_nodes(from_node_fname, from_node);
	extract_and_create_nodes(to_node_fname, to_node);
	extract_and_create_topologies(from_top_fname, from_top, from_node);
	extract_and_create_topologies(to_top_fname, to_top, to_node);

	map = new double*[num_nodes_to];
	for(int i = 0; i < num_nodes_to; ++i) {
		map[i] = new double[num_nodes_from];
	}

	// Find nearest linear!! node, and therefore nearest element / containing element
	int i, nearest_node_index, check;
	tet_element *containing_element, *temp_element;
	double single_map[4];
	vector3 nearest_node, translation;

	if(num_nodes_to > 20) {
		check = num_nodes_to / 20;
	} else {
		check = 1;
	}
	for(i = 0; i < num_nodes_to; ++i) {

		// Output check
		if(i % check == 0) {
			cout << "Num elements left to map: " << num_nodes_to - i << endl;
		}

		// Find containing element
		containing_element = find_containing_element(to_node[i], from_top, num_elements_from);
		if(containing_element == NULL) {
			
			// Get nearest node from nearest element
			containing_element = get_nearest_element(to_node[i], from_top, num_elements_from);
	
		} 

		// Get position of to_node[i] as a function of the nodes of this element
		get_single_node_map(to_node[i], containing_element, single_map);
			
		// Add this data to final map
		add_map_to_big_map(single_map, map, i, containing_element);
		
	}

	// Print out the whole map
	print_map_to_file(output_map_fname, map, num_nodes_from, num_nodes_to);
	return 0;
}
