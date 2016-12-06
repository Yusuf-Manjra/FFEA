#include "LJ_matrix.h"

LJ_matrix::LJ_matrix() {
    params = NULL;
    num_vdw_face_types = 0;
}

int LJ_matrix::init(string vdw_params_fname, string vdw_type) {

    int err;
    // In that case we do not need an input parameter file, 
    //      but just to initialise a number of values
    if (vdw_type == "steric") {
       err = init_steric(); 
    } else {
       err = init_lj(vdw_params_fname); 
    } 
    return err; 
} 

int LJ_matrix::init_steric() {

    num_vdw_face_types = 1;
    // Allocate the memory for that LJ pair
    params = new LJ_pair[num_vdw_face_types * num_vdw_face_types];
    if (params == NULL) {
        FFEA_ERROR_MESSG("Unable to allocate memory for LJ matrix.\n")
    }
    params[LJI(0, 0)].vdw_eps = 0; 
    params[LJI(0, 0)].vdw_r_eq = 0; 

    return FFEA_OK; 
}

int LJ_matrix::init_lj(string vdw_params_fname) {
    FILE *in = NULL;
    const int max_line_size = 50;
    char line[max_line_size];

    if ((in = fopen(vdw_params_fname.c_str(), "r")) == NULL) {
        FFEA_FILE_ERROR_MESSG(vdw_params_fname.c_str())
    }
    printf("\tReading in vdw forcefield parameters file: %s\n", vdw_params_fname.c_str());

    // first line should be the file type "ffea vdw forcefield params file"
    if (fgets(line, max_line_size, in) == NULL) {
        fclose(in);
        FFEA_ERROR_MESSG("Error reading first line of ffea vdw forcefield params file\n")
    }
    if (strcmp(line, "ffea vdw forcefield params file\n") != 0) {
        fclose(in);
        FFEA_ERROR_MESSG("This is not a 'ffea vdw forcefield params file' (read '%s') \n", line)
    }

    // read in the number of nodes in the file
    if (fscanf(in, "num_vdw_face_types %d\n", &num_vdw_face_types) != 1) {
        fclose(in);
        FFEA_ERROR_MESSG("Error reading number of vdw face types\n")
    }
    printf("\t\tNumber of vdw face types = %d\n", num_vdw_face_types);

    // Allocate the memory for all these LJ pairs
    params = new LJ_pair[num_vdw_face_types * num_vdw_face_types];
    if (params == NULL) {
        fclose(in);
        FFEA_ERROR_MESSG("Unable to allocate memory for LJ matrix.\n")
    }

    // Fill the matrix
    double vdw_eps = 0.0, vdw_r_eq = 0.0;
    for (int i = 0; i < num_vdw_face_types; i++) {
        for (int j = 0; j < num_vdw_face_types; j++) {
            if (fscanf(in, " (%le,%le) ", &vdw_eps, &vdw_r_eq) != 2) {
                fclose(in);
                FFEA_ERROR_MESSG("Error reading from ffea vdw forcefield params file at (%d,%d)\n", i, j);
            }

            if (vdw_eps <= 0) {
                FFEA_ERROR_MESSG("Required: 'vdw_eps' must be greater than 0 if you wish to use vdw (calc_vdw is 1)\n")
            }
            if (vdw_r_eq <= 0) {
                FFEA_ERROR_MESSG("Required: 'vdw_r_eq' must be greater than 0 if you wish to use vdw (calc_vdw is 1)\n")
            }

            params[LJI(i, j)].vdw_eps = vdw_eps * mesoDimensions::area * mesoDimensions::area / mesoDimensions::Energy ;
            params[LJI(i, j)].vdw_r_eq = vdw_r_eq / mesoDimensions::length ;
        }
    }

    fclose(in);

    printf("\t\tRead %d vdw forcefield parameter entries from %s\n", num_vdw_face_types * num_vdw_face_types, vdw_params_fname.c_str());

    return FFEA_OK;
}

void LJ_matrix::get_LJ_params(int type1, int type2, scalar *vdw_eps, scalar *vdw_r_eq) {
    if (type1 < 0 || type1 > num_vdw_face_types - 1) {
        printf("Frog1 %d %d\n", type1, num_vdw_face_types - 1);
        return;
    }
    if (type2 < 0 || type2 > num_vdw_face_types - 1) {
        printf("Frog2 %d %d\n", type2, num_vdw_face_types - 1);
        return;
    }

    LJ_pair *p = &(params[LJI(type1, type2)]);
    *vdw_eps = p->vdw_eps;
    *vdw_r_eq = p->vdw_r_eq;
}

int LJ_matrix::get_num_types() {
    return num_vdw_face_types;
}

