import os, sys

if len(sys.argv) != 3:
	sys.exit("make_default_script.py [BASE] [OUTPUT DEFAULT SCRIPT]")

print "Running: make_default_script.py"

base = sys.argv[1]
out_fname = sys.argv[2]

traj_fname = base + "_trajectory.out"
meas_fname = base + "_measurement.out"
stress_fname = base + "_stress.out"
nodes_fname = base + ".node"
top_fname = base + ".top"
surf_fname = base + ".surf"
mat_fname = base + ".mat"
stokes_fname = base + ".stokes"
vdw_fname = base + ".vdw"
pin_fname = base + ".pin"
force_fname = base + ".force"
lj_fname = base + ".lj"

script_txt = "<param>\n"
script_txt += "	<restart = 0>\n"
script_txt += "	<dt = 1e-14>\n"
script_txt += "	<kT = 4.11e-21>\n"
script_txt += "	<check = 10000>\n"
script_txt += "	<num_steps = 1e11>\n"
script_txt += "	<rng_seed = time>\n"
script_txt += "	<trajectory_out_fname = " + traj_fname + ">\n"
script_txt += "	<measurement_out_fname = " + meas_fname + ">\n"
script_txt += "	<stress_out_fname = " + stress_fname + ">\n"
script_txt += " <vdw_forcefield_params = " + lj_fname + ">\n"
script_txt += "	<epsilon = 0.01>\n"
script_txt += "	<max_iterations_cg = 1000>\n"
script_txt += "	<kappa = 1e9>\n"
script_txt += "	<epsilon_0 = 1>\n"
script_txt += "	<dielec_ext = 1>\n"
script_txt += "	<do_stokes = 1>\n"
script_txt += "	<stokes_visc = 1e-06>\n"
script_txt += "	<calc_vdw = 0>\n"
script_txt += " <calc_noise = 1>\n"
script_txt += "	<calc_es = 0>\n"
script_txt += "	<es_update = 1>\n"
script_txt += "	<es_N_x = 0>\n"
script_txt += "	<es_N_y = 0>\n"
script_txt += "	<es_N_z = 0>\n"
script_txt += "	<sticky_wall_xz = 0>\n"
script_txt += "	<wall_x_1 = PBC>\n"
script_txt += "	<wall_x_2 = PBC>\n"
script_txt += "	<wall_y_1 = PBC>\n"
script_txt += "	<wall_y_2 = PBC>\n"
script_txt += "	<wall_z_1 = PBC>\n"
script_txt += "	<wall_z_2 = PBC>\n"
script_txt += "	<es_h = 3>\n"
script_txt += "	<num_blobs = 1>\n"
script_txt += "	<num_conformations = (1)>\n"
script_txt += "	<num_states = (1)>\n"
script_txt += "</param>\n"
script_txt += "\n"
script_txt += "<system>\n"
script_txt += "	<blob>\n"
script_txt += "		<conformation>\n"
script_txt += "			<state = DYNAMIC>\n"
script_txt += "			<nodes = " + nodes_fname + ">\n"
script_txt += "			<topology = " + top_fname + ">\n"
script_txt += "			<surface = " + surf_fname + ">\n"
script_txt += "			<material = " + mat_fname + ">\n"
script_txt += "			<stokes = " + stokes_fname + ">\n"
script_txt += "			<vdw = " + vdw_fname + ">\n"
script_txt += "			<pin = " + pin_fname + ">\n"
script_txt += "			<solver = CG>\n"
script_txt += "			<scale = 1e-10>\n"
script_txt += "			<centroid_pos = (0.0,0.0,0.0)>\n"
script_txt += "			<velocity = (0.0,0.0,0.0)>\n"
script_txt += "		</conformation>\n"
script_txt += "	</blob>\n"
script_txt += "</system>\n"

with open(out_fname, "w") as outfile:
	outfile.write(script_txt)

print "Done. --> make_default_script.py"
