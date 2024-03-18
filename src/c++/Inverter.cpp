#include <src/c++/Inverter.h>

void Inverter::display() const {
    mexPrintf("----------------------------------------------------------------------------------------------------\n");
    mexPrintf("| Invert Vector Field (2D C++)\n");
    mexPrintf("| Author: T.J.W. Draper\n");
    mexPrintf("| Year: 2024\n|\n");
    mexPrintf("||--------------------------------------------------\n");
    mexPrintf("||  dimensions: (%d %d)\n", this->dimin.x, this->dimin.y);
    mexPrintf("||  iterations: %d\n", this->niter);
    mexPrintf("||  convergence criterion: %.6f\n", this->convergence_criterion);
    mexPrintf("----------------------------------------------------------------------------------------------------\n\n");
}

Inverter::Inverter(const dim dimin, const unsigned int niter, const double convergence_criterion) {
    this->dimin = dimin;
    this->step = dim(1, dimin.x);
    this->sizein = dimin.x * dimin.y;

    this->niter = niter;
    this->convergence_criterion = convergence_criterion;

    this->displacement = new Motion(dimin);
    this->inverse_displacement = new Motion(dimin);

    Inverter::display();
}

Inverter::~Inverter() {
    delete this->displacement;
    delete this->inverse_displacement;
}

// Data I/O
void Inverter::set_vector_field(const double* field) {
    this->displacement->set_motion(field);

    // Set initial guess disp_u_inv = - disp_u;
    *this->inverse_displacement = *this->displacement * -1.0;
}

void Inverter::copy_inverted_vector_field_to_input(double* field) const {
    this->inverse_displacement->copy_motion_to_input(field);
}

// Invert the displacement field
void Inverter::invert() {
    const Motion& displacement = *this->displacement;
    Motion& inverse_displacement = *this->inverse_displacement;

    // Create auxiliary vector field
    Motion tmp_displacement(this->dimin);
    Motion dif_displacement(this->dimin);

    // Create array to track progress over iterations
    double* increment = new double[this->niter];

    for (unsigned int iter = 0; iter < this->niter; iter++) {
        tmp_displacement = inverse_displacement;

        // Fixed-point iteration step
        inverse_displacement.interpolate(displacement, tmp_displacement);
        inverse_displacement *= -1.0;

        // Calculate progress
        dif_displacement = tmp_displacement - inverse_displacement;
        increment[iter] = dif_displacement.norm();

        mexPrintf("Iteration %d - increment: %.3f\n", iter, increment[iter]);

        // Check for convergence
        if (iter > 0) {
            double relative_increment = ( increment[iter-1] - increment[iter] ) / increment[0];
            if (relative_increment < this->convergence_criterion) {
                mexPrintf("Convergence criterion reached\n");
                break;
            }
        }

    }

    // Free memory
    delete[] increment;

    // Done
    return;
}