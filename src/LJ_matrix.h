#ifndef LJMATRIXHINCLUDED
#define LJMATRIXHINCLUDED

#define LJI(A,B)	((A) * num_vdw_face_types + (B))

class LJ_pair
{
	public:
		scalar vdw_eps;
		scalar vdw_r_eq;
};

class LJ_matrix
{
	public:
		LJ_matrix()
		{
			params = NULL;
			num_vdw_face_types = 0;
		}

		int init(const char *vdw_params_fname)
		{
			FILE *in = NULL;
			const int max_line_size = 50;
			char line[max_line_size];

			if((in = fopen(vdw_params_fname, "r")) == NULL) {
				FFEA_FILE_ERROR_MESSG(vdw_params_fname)
			}
			printf("\tReading in vdw forcefield parameters file: %s\n", vdw_params_fname);

			// first line should be the file type "ffea vdw forcefield params file"
			if(fgets (line, max_line_size, in) == NULL) {
				fclose(in);
				FFEA_ERROR_MESSG("Error reading first line of ffea vdw forcefield params file\n")
			}
			if(strcmp(line, "ffea vdw forcefield params file\n") != 0) {
				fclose(in);
				FFEA_ERROR_MESSG("This is not a 'ffea vdw forcefield params file' (read '%s') \n", line)
			}

			// read in the number of nodes in the file
			if(fscanf(in, "num_vdw_face_types %d\n", &num_vdw_face_types) != 1) {
				fclose(in);
				FFEA_ERROR_MESSG("Error reading number of vdw face types\n")
			}
			printf("\t\tNumber of vdw face types = %d\n", num_vdw_face_types);

			// Allocate the memory for all these LJ pairs
			params = new LJ_pair[num_vdw_face_types * num_vdw_face_types];
			if(params == NULL) {
				fclose(in);
				FFEA_ERROR_MESSG("Unable to allocate memory for LJ matrix.\n")
			}

			// Fill the matrix
			scalar vdw_eps=0.0, vdw_r_eq=0.0;
			for(int i = 0; i < num_vdw_face_types; i++) {
				for(int j = 0; j < num_vdw_face_types; j++) {
					if(fscanf(in, " (%le,%le) ", &vdw_eps, &vdw_r_eq) != 2) {
						fclose(in);
						FFEA_ERROR_MESSG("Error reading from ffea vdw forcefield params file at (%d,%d)\n", i,j);
					}

                                	if(vdw_eps <= 0) {
                                        	FFEA_ERROR_MESSG("Required: 'vdw_eps' must be greater than 0 if you wish to use vdw (calc_vdw is 1)\n")
                                	}
                                	if(vdw_r_eq <= 0) {
                                        	FFEA_ERROR_MESSG("Required: 'vdw_r_eq' must be greater than 0 if you wish to use vdw (calc_vdw is 1)\n")
                                	}

					params[LJI(i, j)].vdw_eps = vdw_eps;
					params[LJI(i, j)].vdw_r_eq = vdw_r_eq;
				}
			}

			fclose(in);

			printf("\t\tRead %d vdw forcefeild parameter entries from %s\n", num_vdw_face_types * num_vdw_face_types, vdw_params_fname);

			return FFEA_OK;
		}

 		void get_LJ_params(int type1, int type2, scalar *vdw_eps, scalar *vdw_r_eq)
		{
			if (type1 < 0 || type1 > num_vdw_face_types - 1) {
				printf("Fuck1 %d %d\n", type1, num_vdw_face_types - 1);
				return;
			}
			if (type2 < 0 || type2 > num_vdw_face_types - 1) {
				printf("Fuck2 %d %d\n", type2, num_vdw_face_types - 1);
				return;
			}

			LJ_pair *p = &(params[LJI(type1, type2)]);
			*vdw_eps = p->vdw_eps;
			*vdw_r_eq = p->vdw_r_eq;
		}

		int get_num_types()
		{
			return num_vdw_face_types;
		}

	private:
		LJ_pair *params;
		int num_vdw_face_types;
};


#endif
