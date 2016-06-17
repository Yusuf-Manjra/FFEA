#include "SimulationParams.h"

SimulationParams::SimulationParams() {

    // Constructor used to give default values where necessary

    // Defaulted (into SI units)
    dt = 1e-14 / mesoDimensions::time;
    num_steps = 1e11;
    check = 10000;
    kT = 4.11e-21 / mesoDimensions::Energy;
    max_iterations_cg = 1000;
    epsilon2 = 0.01;
    rng_seed = time(NULL);
    calc_vdw = 1;
    sticky_wall_xz = 0;
    vdw_type = "steric";
    vdw_steric_factor = 1e-2;
    move_into_box = 1;
    es_update = 1;
    kappa = 1e9 * mesoDimensions::length;
    es_h = 3;

    calc_noise = 1;
    calc_es = 0;
    dielec_ext = 1;
    epsilon_0 = 1;
    calc_stokes = 1;
    stokes_visc = 1e-3 / (mesoDimensions::pressure * mesoDimensions::time);
    calc_kinetics = 0;
    kinetics_update = 0;
    calc_preComp = 0;


    wall_x_1 = WALL_TYPE_PBC;
    wall_x_2 = WALL_TYPE_PBC;
    wall_y_1 = WALL_TYPE_PBC;
    wall_y_2 = WALL_TYPE_PBC;
    wall_z_1 = WALL_TYPE_PBC;
    wall_z_2 = WALL_TYPE_PBC;

    // Initialised to zero or equivalent for later initialisation
    restart = -1;
    num_blobs = 0;
    num_conformations = NULL;
    num_states = NULL;
    state_array_size = 0;
    conformation_array_size = 0;
    es_N_x = -1;
    es_N_y = -1;
    es_N_z = -1;

    trajectory_out_fname_set = 0;
    kinetics_out_fname_set = 0;
    measurement_out_fname_set = 0;
    binding_params_fname_set = 0;
    vdw_params_fname_set = 0;

    sprintf(trajectory_out_fname, "\n");
    sprintf(kinetics_out_fname, "\n");
    sprintf(temp_fname, "\n");
    measurement_out_fname = NULL;
    sprintf(vdw_params_fname, "\n");
    sprintf(binding_params_fname, "\n");
}

SimulationParams::~SimulationParams() {
    dt = 0;
    num_steps = -1;
    check = 0;
    num_blobs = 0;
    delete[] num_conformations;
    num_conformations = NULL;
    delete[] num_states;
    num_states = NULL;
    state_array_size = 0;
    conformation_array_size = 0;
    rng_seed = 0;
    kT = 0;
    max_iterations_cg = 0;
    epsilon2 = 0;
    es_update = 0;
    kinetics_update = 0;
    es_N_x = -1;
    es_N_y = -1;
    es_N_z = -1;
    move_into_box = 0;
    es_h = 0;
    kappa = 0;
    dielec_ext = 0;
    epsilon_0 = 0;
    restart = 0;
    calc_vdw = -1;
    vdw_type = "";
    calc_es = 0;
    calc_noise = 0;
    calc_preComp = 0;
    calc_kinetics = 0;

    wall_x_1 = -1;
    wall_x_2 = -1;
    wall_y_1 = -1;
    wall_y_2 = -1;
    wall_z_1 = -1;
    wall_z_2 = -1;

    sticky_wall_xz = 0;

    calc_stokes = 0;
    stokes_visc = -1;

    vdw_steric_factor = 0;

    trajectory_out_fname_set = 0;
    kinetics_out_fname_set = 0;
    measurement_out_fname_set = 0;
    binding_params_fname_set = 0;
    vdw_params_fname_set = 0;

    sprintf(trajectory_out_fname, "\n");
    sprintf(kinetics_out_fname, "\n");
    sprintf(temp_fname, "\n");
    delete[] measurement_out_fname;
    sprintf(binding_params_fname, "\n");
    sprintf(vdw_params_fname, "\n");
}

int SimulationParams::extract_params(vector<string> script_vector) {

	// Extract param string from script string
	vector<string> param_vector;
	FFEA_input_reader *paramreader = new FFEA_input_reader();
	paramreader->extract_block("param", 0, script_vector, &param_vector);	

	// Parse the section
	vector<string>::iterator it;
	string lrvalue[2];
	for(it = param_vector.begin(); it != param_vector.end(); ++it) {
		paramreader->parse_tag(*it, lrvalue);

		// Assign if possible
		if(assign(lrvalue[0], lrvalue[1]) != 0) {
			FFEA_error_text();
			cout << "Assignment of parameter " << lrvalue[0] << " = " << lrvalue[1] << " failed" << endl;
			return FFEA_ERROR;
		}
	}
	return FFEA_OK;
}

int SimulationParams::assign(string lvalue, string rvalue) {
	
	// Carry out parameter assignments
	if (lvalue == "restart") {
		restart = atoi(rvalue.c_str());
		cout << "\tSetting " << lvalue << " = " << restart << endl;

	} else if (lvalue == "dt") {
		dt = atof(rvalue.c_str());
		cout << "\tSetting " << lvalue << " = " << dt << endl;
                dt /= mesoDimensions::time;

	} else if (lvalue == "epsilon") {
		epsilon2 = atof(rvalue.c_str());
		epsilon2 *= epsilon2;
		cout << "\tSetting " << lvalue << " = " << epsilon2 << endl;

	} else if (lvalue == "num_steps") {

		// convert to float first so that user may write numbers of the form 1e4 for 10000 etc
	        num_steps = (long long) (atof(rvalue.c_str()));
        	cout << "\tSetting " << lvalue << " = " << num_steps << endl;

   	} else if (lvalue == "max_iterations_cg") {
		max_iterations_cg = atoi(rvalue.c_str());
		cout << "\tSetting " << lvalue << " = " << max_iterations_cg << endl;

    	} else if (lvalue == "check") {
       		check = (int) atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << check << endl;

	} else if (lvalue == "num_blobs") {
        	num_blobs = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << num_blobs << endl;

    	} else if (lvalue == "num_conformations") {

		// Split rvalue into a vector
		vector<string> conformation_vector;
		rvalue = boost::erase_last_copy(boost::erase_first_copy(rvalue, "("), ")");
		boost::split(conformation_vector, rvalue, boost::is_any_of(","));

		// Now assign them to an array
		vector<string>::iterator it;
		conformation_array_size = conformation_vector.size();
		num_conformations = new int[conformation_array_size];
		int i = 0;
		for(it = conformation_vector.begin(); it != conformation_vector.end(); ++it) {
			num_conformations[i] = atoi((*it).c_str());
			cout << "\tSetting Blob " << i << ", " << lvalue << " = " << num_conformations[i] << endl;
			i++;
		}

        } else if (lvalue == "num_states") {

		// Split rvalue into a vector
		vector<string> state_vector;
		rvalue = boost::erase_last_copy(boost::erase_first_copy(rvalue, "("), ")");
		boost::split(state_vector, rvalue, boost::is_any_of(","));

		// Now assign them to an array
		vector<string>::iterator it;
		state_array_size = state_vector.size();
        	num_states = new int[state_array_size];
		int i = 0;
		for(it = state_vector.begin(); it != state_vector.end(); ++it) {
			num_states[i] = atoi((*it).c_str());
			cout << "\tSetting Blob " << i << ", " << lvalue << " = " << num_states[i] << endl;
			i++;
		}

    	} else if (lvalue == "es_update") {
        	es_update = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << es_update << endl;

    	} else if (lvalue == "kinetics_update") {
        	kinetics_update = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << kinetics_update << endl;

    	} else if (lvalue == "es_N_x") {
        	es_N_x = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << es_N_x << endl;

	} else if (lvalue == "es_N_y") {
        	es_N_y = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << es_N_y << endl;

	} else if (lvalue == "es_N_z") {
        	es_N_z = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << es_N_z << endl;

	} else if (lvalue == "move_into_box") {
        	move_into_box = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << move_into_box << endl;

	} else if (lvalue == "sticky_wall_xz") {
        	sticky_wall_xz = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << sticky_wall_xz << endl;
    
	} else if (lvalue == "es_h") {
        	es_h = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << es_h << endl;
    
	} else if (lvalue == "rng_seed") {
		if(rvalue == "time") {
			rng_seed = time(NULL);
		} else {
			rng_seed = atoi(rvalue.c_str());
		}
        	cout << "\tSetting " << lvalue << " = " << rng_seed << endl;

    	} else if (lvalue == "kT") {
        	kT = atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << kT << endl;
                kT /= mesoDimensions::Energy;

	} else if (lvalue == "kappa") {
        	kappa = atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << kappa << endl;
        	kappa *= mesoDimensions::length;

    	} else if (lvalue == "dielec_ext") {
        	dielec_ext = atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << dielec_ext << endl;

  	} else if (lvalue == "epsilon_0") {
        	epsilon_0 = atof(rvalue.c_str()); // relative permittivity
        	cout << "\tSetting " << lvalue << " = " << epsilon_0 << endl;

    	} else if (lvalue == "calc_vdw") {
        	calc_vdw = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_vdw << endl;

    	} else if (lvalue == "vdw_type") {
        	vdw_type = rvalue;
        	cout << "\tSetting " << lvalue << " = " << vdw_type << endl;

	} else if (lvalue == "calc_es") {
        	calc_es = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_es << endl;

	} else if (lvalue == "calc_noise") {
        	calc_noise = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_noise << endl;

	} else if (lvalue == "calc_preComp") {
        	calc_preComp = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_preComp << endl;

	} else if (lvalue == "calc_kinetics") {
        	calc_kinetics = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_kinetics << endl;

	} else if (lvalue == "calc_stokes" || lvalue == "do_stokes") {
        	calc_stokes = atoi(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << calc_stokes << endl;

	} else if (lvalue == "stokes_visc") {
        	stokes_visc = atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << stokes_visc << endl;
                stokes_visc /= mesoDimensions::pressure * mesoDimensions::time;

	} else if (lvalue == "vdw_steric_factor") {
        	vdw_steric_factor = atof(rvalue.c_str());
        	cout << "\tSetting " << lvalue << " = " << vdw_steric_factor << endl;

	} else if (lvalue == "wall_x_1") {
		if (rvalue == "PBC") {
			wall_x_1 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_x_1 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_x_1 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_x_1 << endl;

	} else if (lvalue == "wall_x_2") {
		if (rvalue == "PBC") {
			wall_x_2 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_x_2 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_x_2 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_x_2 << endl;

	} else if (lvalue == "wall_y_1") {
		if (rvalue == "PBC") {
			wall_y_1 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_y_1 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_y_1 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_y_1 << endl;

	} else if (lvalue == "wall_y_2") {
		if (rvalue == "PBC") {
			wall_y_2 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_y_2 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_y_2 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_y_2 << endl;

	} else if (lvalue == "wall_z_1") {
		if (rvalue == "PBC") {
			wall_z_1 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_z_1 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_z_1 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_z_1 << endl;

	} else if (lvalue == "wall_z_2") {
		if (rvalue == "PBC") {
			wall_z_2 = WALL_TYPE_PBC;
		} else if (rvalue == "HARD") {
			wall_z_2 = WALL_TYPE_HARD;
		} else if (rvalue == "STOP") {
			wall_z_2 = WALL_TYPE_STOP;
		}
		cout << "\tSetting " << lvalue << " = " << wall_z_2 << endl;

    	} else if (lvalue == "trajectory_out_fname") {
		if (rvalue.length() >= MAX_FNAME_SIZE) {
			FFEA_ERROR_MESSG("trajectory_out_fname is too long. Maximum filename length is %d characters.\n", MAX_FNAME_SIZE - 1)
		}
		sprintf(trajectory_out_fname, "%s", rvalue.c_str());
        	trajectory_out_fname_set = 1;
		cout << "\tSetting " << lvalue << " = " << trajectory_out_fname << endl;

    	} else if (lvalue == "measurement_out_fname") {
		if (rvalue.length() >= MAX_FNAME_SIZE) {
			FFEA_ERROR_MESSG("measurement_out_fname is too long. Maximum filename length is %d characters.\n", MAX_FNAME_SIZE - 1)
		}
		sprintf(temp_fname, "%s", rvalue.c_str());
	        measurement_out_fname_set = 1;
		cout << "\tSetting " << lvalue << " = " << temp_fname << endl;

    	} else if (lvalue == "kinetics_out_fname") {
		if (rvalue.length() >= MAX_FNAME_SIZE) {
			FFEA_ERROR_MESSG("kinetics_out_fname is too long. Maximum filename length is %d characters.\n", MAX_FNAME_SIZE - 1)
		}
		sprintf(kinetics_out_fname, "%s", rvalue.c_str());
	        kinetics_out_fname_set = 1;
		cout << "\tSetting " << lvalue << " = " << kinetics_out_fname << endl;
   
	} else if (lvalue == "vdw_forcefield_params") {
		if (rvalue.length() >= MAX_FNAME_SIZE) {
			FFEA_ERROR_MESSG("vdw_forcefield_params is too long. Maximum filename length is %d characters.\n", MAX_FNAME_SIZE - 1)
		}
		sprintf(vdw_params_fname, "%s", rvalue.c_str());
        	vdw_params_fname_set = 1;
		cout << "\tSetting " << lvalue << " = " << vdw_params_fname << endl;

	} else if (lvalue == "binding_site_params") {
		if (rvalue.length() >= MAX_FNAME_SIZE) {
			FFEA_ERROR_MESSG("binding_site_params is too long. Maximum filename length is %d characters.\n", MAX_FNAME_SIZE - 1)
		}
		sprintf(binding_params_fname, "%s", rvalue.c_str());
        	binding_params_fname_set = 1;
		cout << "\tSetting " << lvalue << " = " << binding_params_fname << endl;

    	} else if (lvalue == "stress_out_fname") {
		cout << lvalue << " no longer recognised" << endl;
	} else {
        	FFEA_error_text();
        	cout << "Error: '" << lvalue << "' is not a recognised lvalue" << endl;
        	cout << "Recognised lvalues are:" << endl;
        	cout << "dt\n\tepsilon\n\tnum_steps\n\tmax_iterations_cg\n\tcheck\n\tes_update\n\ttrajectory_out_fname\n\tmeasurement_out_fname\n\tstress_out_fname\n\tes_N_x\n\tes_N_y\n\tes_N_z\n\tes_h\n\trng_seed\n\tkT\n\tkappa\n\tdielec_ext\tepsilon_0\n\trestart\n\tcalc_vdw\n\tcalc_noise\n\tcalc_preComp\n" << endl;
        	return FFEA_ERROR;
   	}

	return FFEA_OK;
}

// rename oFile to something else. 
int SimulationParams::checkFileName(string oFile){


    if (b_fs::exists(oFile))    // does oFile actually exist?
    {
        int cnt = 1;
        b_fs::path fs_oFile = oFile;
        cout << "oFile: " << oFile << endl;
        string base = "__" + fs_oFile.filename().string() + "__bckp.";
        if (fs_oFile.parent_path().string().size() != 0)
          base = fs_oFile.parent_path().string() + "/" + base;

        string bckp = base + boost::lexical_cast<string>(cnt);
        while (b_fs::exists(bckp)) {
          cnt += 1;
          string s_cnt = boost::lexical_cast<string>(cnt);
          bckp = base + s_cnt;
        }
        FFEA_CAUTION_MESSG("Moving %s to %s\n", oFile.c_str(), bckp.c_str()); 
        // cout << "FFEA: moving " << oFile << " to " << bckp << "\n";
        b_fs::rename(oFile, bckp.c_str());
    } 
      

    return FFEA_OK; 
}

int SimulationParams::validate() {

    if (restart != 0 && restart != 1) {
        FFEA_ERROR_MESSG("Required: Restart flag, 'restart', must be 0 (false) or 1 (true).\n");
    }

    if (num_steps < 0) {
        FFEA_ERROR_MESSG("Required: Number of time steps, 'num_steps', must be greater than or equal to 0.\n");
    }
    if (num_blobs <= 0) {
        FFEA_ERROR_MESSG("\tRequired: Number of Blobs, 'num_blobs', must be greater than 0.\n");
    }

    if (conformation_array_size != num_blobs) {
	FFEA_ERROR_MESSG("\tRequired: Number of Conformations, 'num_conformations', must have 'num_blobs' elements. We read %d elements but only %d blobs\n", conformation_array_size, num_blobs);
    }

    if (kappa < 0) {
        FFEA_ERROR_MESSG("Required: Inverse Debye Screening length, 'kappa', cannot be negative.\n");
    }

    if (dielec_ext <= 0) {
        FFEA_ERROR_MESSG("Required: Exterior dielectric constant, 'dielec_ext', must be greater than 0.\n");
    }

    if (epsilon_0 <= 0) {
        FFEA_ERROR_MESSG("Required: Permittivity of free space, 'epsilon_0', must be greater than 0.\n");
    }

    if (calc_vdw != 0 && calc_vdw != 1) {
        FFEA_ERROR_MESSG("Required: 'calc_vdw', must be 0 (no) or 1 (yes).\n");
    }

    if (calc_vdw == 1) {
        if (vdw_params_fname_set == 0) {
            FFEA_ERROR_MESSG("VdW forcefield params file name required (vdw_forcefield_params).\n");
        }
    }
 
    if (vdw_type != "lennard-jones" && vdw_type != "steric" && vdw_type != "ljsteric") {
        FFEA_ERROR_MESSG("Optional: 'vdw_type', must be either 'steric' (default), 'lennard-jones' or 'ljsteric' (both methods combined).\n");
    }

    if (calc_preComp != 0 && calc_preComp != 1) {
        FFEA_ERROR_MESSG("Required: 'calc_preComp', must be 0 (no) or 1 (yes).\n");
    }

    if (calc_es != 0 && calc_es != 1) {
        FFEA_ERROR_MESSG("Required: 'calc_es', must be 0 (no) or 1 (yes).\n");
    }

    if (calc_kinetics != 0 && calc_kinetics != 1) {
        FFEA_ERROR_MESSG("Required: 'calc_kinetics', must be 0 (no) or 1 (yes).\n");
    }
    
    if (calc_vdw == 1 or calc_es == 1) {
    	if (es_N_x < 1) {
        	printf("\tFRIENDLY WARNING: Length of the nearest neighbour lookup grid, 'es_N_x', is less than 1. Will assign default value to encompass whole system.\n");
    	} else if (es_N_y < 1) {
        	printf("\tFRIENDLY WARNING: Length of the nearest neighbour lookup grid, 'es_N_y', is less than 1. Will assign default value to encompass whole system.\n");
    	} else if (es_N_z < 1) {
        	printf("\tFRIENDLY WARNING: Length of the nearest neighbour lookup grid, 'es_N_z', is less than 1. Will assign default value to encompass whole system.\n");
    	} else {
        	if (es_h <= 0) {
            		FFEA_ERROR_MESSG("Required: Nearest neighbour lookup grid cell dimension, 'es_h', must be greater than 0.\n");
        	}
    	}

	if (move_into_box != 0 && move_into_box != 1) {
		FFEA_ERROR_MESSG("'move_into_box' must all be either 0 (system centroid preserved) or 1 (system centroid will move to centroid of simulation box).\n")
	}

    } else {
		printf("\tFRIENDLY WARNING: No electrostatic or vdw interactions will be simulated\n");
		es_N_x = 0;
		es_N_y = 0;
		es_N_z = 0;
    }

    if (calc_noise != 0 && calc_noise != 1) {
        FFEA_ERROR_MESSG("Required: 'calc_noise', must be 0 (no) or 1 (yes).\n");
    }

    if (trajectory_out_fname_set == 0) {
        FFEA_ERROR_MESSG("Trajectory output file name required (trajectory_out_fname).\n");
    }

    if (measurement_out_fname_set == 0) {
        FFEA_ERROR_MESSG("Measurement output file name required (measurement_out_fname).\n");
    } else {
        measurement_out_fname = new char*[num_blobs + 1];
	vector<string> all;
	char temp[MAX_FNAME_SIZE];
	string orig, base, ext;
	orig = string(temp_fname);
	boost::split(all, temp_fname, boost::is_any_of("."));
	ext = "." + all.at(all.size() - 1);
	base = boost::erase_last_copy(orig, ext);

        for (int i = 0; i < num_blobs + 1; ++i) {
            measurement_out_fname[i] = new char[MAX_FNAME_SIZE];
            sprintf(measurement_out_fname[i], base.c_str());

            if (i < num_blobs) {
                sprintf(temp, "_blob%d", i);
                strcat(measurement_out_fname[i], temp);
            } else {
                strcat(measurement_out_fname[i], "_world");
            }
            strcat(measurement_out_fname[i], ext.c_str());


        }
    }

    // check if the output files exists, and if so, rename it. 
    if (restart == 0) {
      for (int i = 0; i < num_blobs + 1; ++i) {
        checkFileName(boost::lexical_cast<string>(measurement_out_fname[i])); 
      }
      checkFileName(boost::lexical_cast<string>(trajectory_out_fname));
      checkFileName(boost::lexical_cast<string>(kinetics_out_fname));
    } 

    if (calc_stokes == 1 && stokes_visc <= 0) {
        FFEA_ERROR_MESSG("calc_stokes flag is set, so stokes_visc must be set to a value greater than 0.\n");
    }

    if (vdw_steric_factor < 0) {
       printf("\tFRIENDLY WARNING: Beware, vdw_steric_factor is negative.\n");
    } 

    if (calc_kinetics == 1) {
	if(kinetics_update <= 0) {
		//FFEA_ERROR_MESSG("\tRequired: If 'calc_kinetics' = 1, then 'kinetics_update' must be greater than 0.\n");
		cout << "\tDefaulting 'kinetics_update' to " << check << endl;
		kinetics_update = check;
	} //else if (kinetics_update <= check) {

		// This could be fixed by changing how the output files are printed. Fix later, busy now!
		//FFEA_CAUTION_MESSG("\t'kinetics_update' < 'check'. A kinetic switch therefore maybe missed i.e. not printed to the output files.\n")
	//}

	// num_conformations[i] can be > num_states[i], so long as none of the states reference an out of bounds conformation

    } else {
	for(int i = 0; i < num_blobs; ++i) {
		if(num_conformations[i] != 1) {
			FFEA_CAUTION_MESSG("\tNumber of Conformations, 'num_conformations[%d]', not equal to 1. Only first conformation will be loaded.\n", i);
			num_conformations[i] = 1;
		}
	}
    }
    printf("...done\n");

    printf("Parameters:\n");
    printf("\trestart = %d\n", restart);
    printf("\tdt = %e\n", dt*mesoDimensions::time);
    printf("\tnum_steps = %lld\n", num_steps);
    printf("\tcheck = %d\n", check);
    printf("\tnum_blobs = %d\n", num_blobs);
    for (int i = 0; i < num_blobs; ++i) {
        printf("\tBlob %d:\n", i);
        printf("\t\tnum_conformations = %d\n", num_conformations[i]);
	if (calc_kinetics == 1) {
	        printf("\t\tnum_states = %d\n", num_states[i]);
    	}
    }
    printf("\trng_seed = %d\n", rng_seed);
    printf("\tkT = %e\n", kT*mesoDimensions::Energy);
    printf("\ttrajectory_out_fname = %s\n", trajectory_out_fname);
    for (int i = 0; i < num_blobs + 1; ++i) {
        printf("\tmeasurement_out_fname %d = %s\n", i, measurement_out_fname[i]);
    }
    printf("\tkinetics_out_fname = %s\n", kinetics_out_fname);
    printf("\tvdw_forcefield_params = %s\n", vdw_params_fname);
    printf("\tmax_iterations_cg = %d\n", max_iterations_cg);
    printf("\tepsilon2 = %e\n", epsilon2);
    printf("\tes_update = %d\n", es_update);
    printf("\tes_N_x = %d\n", es_N_x);
    printf("\tes_N_y = %d\n", es_N_y);
    printf("\tes_N_z = %d\n", es_N_z);
    printf("\tes_h = %e x inverse kappa\n", es_h);
    printf("\tkappa = %e\n", kappa/mesoDimensions::length);
    printf("\tepsilon_0 = %e\n", epsilon_0);
    printf("\tdielec_ext = %e\n", dielec_ext);
    printf("\tcalc_vdw = %d\n", calc_vdw);
    printf("\tvdw_type = %s\n", vdw_type.c_str());
    printf("\tcalc_es = %d\n", calc_es);
    printf("\tcalc_noise = %d\n", calc_noise);
    printf("\tcalc_kinetics = %d\n", calc_kinetics);
    printf("\tcalc_preComp = %d\n", calc_preComp);
    printf("\tcalc_stokes = %d\n", calc_stokes);
    printf("\tstokes_visc = %f\n", stokes_visc*mesoDimensions::pressure*mesoDimensions::time);
    printf("\tcalc_kinetics = %d\n", calc_kinetics);

    if(calc_kinetics == 1 && binding_params_fname_set == 1) {
	printf("\tbinding_params_fname = %s\n", binding_params_fname);
    }
    if(calc_vdw == 1 && (vdw_type == "steric" || vdw_type == "ljsteric")) {
        printf("\tvdw_steric_factor = %e\n", vdw_steric_factor);
    }
    return FFEA_OK;
}

int SimulationParams::get_max_num_states() {
	
    int i, max_num_states = 0;
    for(i = 0; i < num_blobs; ++i) {
	if(num_states[i] > max_num_states) {
	    max_num_states = num_states[i];
	} 
    }
    return max_num_states;
}

void SimulationParams::write_to_file(FILE *fout) {

	// This should be getting added to the top of the measurement_world file!!

	// Then, every parameter (if anyone hates this goddamn class in the future, please make a Param struct / dictionary thing so we can just loop over all parameters)
	fprintf(fout, "Parameters:\n");
    	fprintf(fout, "restart = %d\n", restart);
    	fprintf(fout, "dt = %e\n", dt*mesoDimensions::time);
    	fprintf(fout, "num_steps = %lld\n", num_steps);
    	fprintf(fout, "check = %d\n", check);
	fprintf(fout, "rng_seed = %d\n", rng_seed);
	fprintf(fout, "kT = %e\n", kT*mesoDimensions::Energy);

    	fprintf(fout, "calc_vdw = %d\n", calc_vdw);
    	fprintf(fout, "vdw_type = %s\n", vdw_type.c_str());
    	fprintf(fout, "calc_es = %d\n", calc_es);
    	fprintf(fout, "calc_noise = %d\n", calc_noise);
    	fprintf(fout, "calc_kinetics = %d\n", calc_kinetics);
    	fprintf(fout, "calc_preComp = %d\n", calc_preComp);
    	fprintf(fout, "calc_stokes = %d\n", calc_stokes);
    	fprintf(fout, "stokes_visc = %e\n", stokes_visc*mesoDimensions::pressure*mesoDimensions::time);

    	fprintf(fout, "es_update = %d\n", es_update);
    	fprintf(fout, "es_N_x = %d\n", es_N_x);
   	fprintf(fout, "es_N_y = %d\n", es_N_y);
    	fprintf(fout, "es_N_z = %d\n", es_N_z);
    	fprintf(fout, "es_h = %f\n", es_h);
	fprintf(fout, "move_into_box = %d\n", move_into_box);

    	fprintf(fout, "kappa = %e\n", kappa/mesoDimensions::length);
    	fprintf(fout, "epsilon_0 = %e\n", epsilon_0);
    	fprintf(fout, "dielec_ext = %e\n", dielec_ext);

	fprintf(fout, "max_iterations_cg = %d\n", max_iterations_cg);
    	fprintf(fout, "epsilon2 = %e\n", epsilon2);
	fprintf(fout, "num_blobs = %d\n", num_blobs);
	fprintf(fout, "num_conformations = (");
    	for (int i = 0; i < num_blobs; ++i) {
        	fprintf(fout, "%d", num_conformations[i]);
		if(i == num_blobs - 1) {
			fprintf(fout, ")");
		} else {
			fprintf(fout, ",");
		}
    	}
	fprintf(fout, "\n");

	if (calc_kinetics == 1) {
		fprintf(fout, "num_states = (");
		for (int i = 0; i < num_blobs; ++i) {
			fprintf(fout, "%d", num_states[i]);
			if(i == num_blobs - 1) {
				fprintf(fout, ")");
			} else {
				fprintf(fout, ",");
			}
		}
		fprintf(fout, "\n");
	}

    	/*fprintf(fout, "trajectory_out_fname = %s\n", trajectory_out_fname);
    	for (int i = 0; i < num_blobs + 1; ++i) {
    	    fprintf(fout, "measurement_out_fname %d = %s\n", i, measurement_out_fname[i]);
    	}
        if(calc_kinetics == 1) {
	    	fprintf(fout, "kinetics_out_fname = %s\n", kinetics_out_fname);
	}*/
}
