#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>


// main (input_file, dataset_name, output_file)
int main (int argc, char* argv[])
{
    hid_t       file, space, dset, 
                avfile_id, avdataset_id, avdataspace_id;          /* Handles */
    herr_t      status;
    hsize_t     dims[3], avdims[2];
    double      *rdata;
    int         i, j, t;
    double      **avgpwr;

    if (argc != 4) {
        printf("Incorrect Number of arguments\n");
        exit(0);
    }
    printf("Opening dataset \"%s\" from file \"%s\" ...\n", argv[2], argv[1]);

    printf("Opening file...\n");
    file = H5Fopen (argv[1], H5F_ACC_RDONLY, H5P_DEFAULT);
    printf("Loading dataset\n");
    dset = H5Dopen (file, argv[2], H5P_DEFAULT);


    space = H5Dget_space (dset);
    H5Sget_simple_extent_dims(space, dims, NULL);

    printf("Dimensions: (x: %llu, y: %llu, t: %llu)\n", dims[0],dims[1], dims[2]);


    avgpwr = (double **) malloc (dims[0] * sizeof (double *));
    avgpwr[0] = (double *) malloc (dims[0] * dims[1] * sizeof (double));
    for (i=1; i<dims[0]; i++)
        avgpwr[i] = avgpwr[0] + i * dims[1];



    rdata = (double *) malloc(dims[0]*dims[1]*dims[2]*sizeof(double));
    if (rdata == NULL) {
        printf("Error allocating memory, rdata\n");
        exit(0);
    }

    status = H5Dread (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                rdata);


    /* Zero the avg array */
    
    for (i = 0; i < dims[0]; i++)
    {
        for (j = 0; j < dims[1]; j++)
        {
            avgpwr[i][j] = 0;
        }
        
    }
    
    /*
    * Avg the data
    */

   for (t = 0; t < dims[2]; t++)
   {
       for (i = 0; i < dims[0]; i++)
       {
           for (j = 0; j < dims[1]; j++)
           {
               avgpwr[i][j] += *(rdata + i*dims[1]*dims[2] + j*dims[2] + t);
           }
           
       }
       
   }
   
    for (i = 0; i < dims[0]; i++)
    {
        for (j = 0; j < dims[1]; j++)
        {
            avgpwr[i][j] /= (double)dims[2];
        }
        
    }


    /* Save avg data to H5 file */

    printf("Saving to \"%s\"\n");
    avfile_id = H5Fcreate(argv[3], H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    avdims[0] = dims[0];
    avdims[1] = dims[1];
    avdataspace_id = H5Screate_simple(2, avdims, NULL);
    avdataset_id = H5Dcreate(avfile_id, "/avg", H5T_IEEE_F64LE, avdataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(avdataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL,
            H5P_DEFAULT, avgpwr[0]);

    printf("Avg values written to file \"%s\"\n", AVFILE);

    status = H5Dclose(avdataset_id);
    status = H5Sclose(avdataspace_id);
    status = H5Fclose(avfile_id);    

    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Fclose (file);

    return 0;
}
