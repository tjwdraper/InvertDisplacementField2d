#include <iostream>
#include <math.h>
#include <cstring>
#include <mex.h>
#include <src/c++/Inverter.h>
#include <src/c++/coord2d.h>

static Inverter* myInverter = NULL;
static dim dimin;
static mwSize* dimin_mw;


void
mexFunction (int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[])
{
    // Set registration parameters
    if ((nlhs == 0) && (nrhs == 3) && (myInverter == NULL)) {
        // Load variables from matlab
        double *tmp;
        tmp = mxGetPr(prhs[0]);
        int dimx = (int) tmp[0];
        int dimy = (int) tmp[1];
        dimin = dim(dimx, dimy);

        tmp = mxGetPr(prhs[1]);
        const unsigned int niter = (unsigned int) tmp[0];

        tmp = mxGetPr(prhs[2]);
        const double convergence_criterion = (double) tmp[0];

        // Set Inverter object
        myInverter = new Inverter(dimin, niter, convergence_criterion);

        // Set output dimensions
        dimin_mw = new mwSize[3];
        dimin_mw[0] = dimx;
        dimin_mw[1] = dimy;
        dimin_mw[2] = 2;
    }

    // Invert vector field
    else if ((nlhs == 0) && (nrhs == 1) && (myInverter != NULL)) {
        // Create pointer to MATLAB input
        double *displacement;
        displacement = mxGetPr(prhs[0]);

        // Set vector field in Inverter object
        myInverter->set_vector_field(displacement);

        // Invert the vector field
        myInverter->invert();
    }

    // Get the inverted motion field from object
    else if ((nlhs == 1) && (nrhs == 0) && (myInverter != NULL)) {
        // Create output array and pointer to data
        plhs[0] = mxCreateNumericArray(3, dimin_mw, mxDOUBLE_CLASS, mxREAL);
        double *inverse_displacement = mxGetPr(plhs[0]);

        // Set the values from inverted vector field in Inverter object
        myInverter->copy_inverted_vector_field_to_input(inverse_displacement);
    }

    // Free Inverter object
    else if ((nlhs == 0) && (nrhs == 0) && (myInverter != NULL)) {
        delete myInverter;
        myInverter = NULL;

        delete[] dimin_mw;
    }

    // Invalid arguments
    else {
        mexErrMsgTxt("Error invalid number of input and output variables given.\n");
    }

    return;
}