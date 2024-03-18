#include <src/c++/Motion.h>

#include <mex.h>
#include <math.h>

// Constructors and deconstructors
Motion::Motion(const dim dimin) : VectorField(dimin) {}

Motion::Motion(const Motion& motion) : VectorField(motion) {}

Motion::~Motion() {}

// Getters and setters
vector2d* Motion::get_motion() const {
    return this->VectorField::get_field();
}

void Motion::set_motion(const double* motion) {
    const dim& dimin = this->dimin;
    const dim& step = this->step;
    const unsigned int& sizein = this->sizein;

    unsigned int idx;
    for (unsigned int i = 0; i < dimin.x; i++) {
        for (unsigned int j = 0; j < dimin.y; j++) {
            idx = i * step.x + j * step.y;

            this->field[idx].x = motion[idx + 0 * sizein];
            this->field[idx].y = motion[idx + 1 * sizein];
        }
    }

    // Done
    return;
}

void Motion::reset() {
    memset(this->field, 0, this->sizein*sizeof(vector2d));
}

// Copy to input
void Motion::copy_motion_to_input(double* motion) const {
    // Get the dimensions of the Motion
    const dim& dimin = this->dimin;
    const dim& step = this->step;
    const unsigned int& sizein = this->sizein;

    // Store the input in the object
    for (unsigned int i = 0; i < dimin.x; i++) {
        for (unsigned int j = 0; j < dimin.y; j++) {
            motion[i*step.x + j*step.y + 0*sizein] = this->field[i*step.x + j*step.y].x;
            motion[i*step.x + j*step.y + 1*sizein] = this->field[i*step.x + j*step.y].y;
        }
    }

    // Done
    return;
}

// Interpolate
void Motion::interpolate(const Motion& motion, const Motion& displaced_grid) {
    const vector2d* grid = displaced_grid.get_motion();
    const vector2d* mo = motion.get_motion();

    // Get the dimensions of the motion field
    const dim& dimin = this->dimin;
    const dim& step = this->step;
    

    unsigned int idx;
    unsigned int idx_interpolated;
    for (unsigned int i = 0; i < dimin.x; i++) {
        for (unsigned int j = 0; j < dimin.y; j++) {
            idx = i * step.x + j * step.y;

            // Calculate the coordinates to sample from
            double px = i + grid[idx].x; int dx = std::floor(px); double fx = px - dx;
            double py = j + grid[idx].y; int dy = std::floor(py); double fy = py - dy;

            // Check if sampled coordinates are within image bounds
            if ((dx < 0) || (dx > dimin.x-1) ||
                (dy < 0) || (dy > dimin.y-1)) {
                this->field[idx] = mo[idx];
                continue;
            }

            idx_interpolated = dx * step.x + dy * step.y;

            // Sample from nearest points
            vector2d val = mo[idx_interpolated] * (1-fx) * (1-fy);
            double weight = (1-fx) * (1-fy);
            if (dx < dimin.x-1) {
                val += mo[idx_interpolated + step.x] * fx * (1-fy);
                weight += fx * (1-fy);
            }
            if (dy < dimin.y-1) {
                val += mo[idx_interpolated + step.y] * (1-fx) * fy;
                weight += (1-fx) * fy;
            }
            if ((dx < dimin.x-1) && (dy < dimin.y-1)) {
                val += mo[idx_interpolated + step.x + step.y] * fx * fy;
                weight += fx * fy;
            }

            if (weight != 0) {
                this->field[idx] = val / weight;
            }
        }
    }

    // Done
    return;
}

// Get some motion field properties
double Motion::norm() const {
    double norm = 0.0f;
    for (unsigned int i = 0; i < this->sizein; i++) {
        norm += std::pow(this->field[i].x, 2) + std::pow(this->field[i].y, 2);
    }
    return norm;
}

double Motion::maxabs() const {
    double maxabs = 0.0f;
    for (unsigned int i = 0; i < this->sizein; i++) {
        double normsq = std::pow(this->field[i].y, 2) + std::pow(this->field[i].y, 2);
        maxabs = std::max(maxabs, normsq);
    }
    return std::sqrt(maxabs);
}


// Overload operator=
Motion& Motion::operator=(const Motion& motion) {
    VectorField::operator=(motion);
    // if (this->dimin != motion.get_dimensions()) {
    //     throw std::invalid_argument("Motion::operator=(const Motion& motion) input argument has to have same dimensions as target");
    // }

    // // Copy the contents from the input to object
    // try {
    //     memcpy(this->field, motion.get_field(), this->sizein*sizeof(vector2d));
    // }
    // catch(const std::bad_alloc& e) {
    //     const std::string mes = "Error in Motion::operator=(const Motion& motion) " + 
    //         std::string(e.what()) + 
    //         std::string("\n");
    //     mexErrMsgTxt(mes.c_str());
    // }

    // Done
    return *this;
}

// Overload operators from base class
Motion Motion::operator+(const Motion& motion) const {
    Motion motion_out(*this);
    motion_out.VectorField::operator+=(motion);
    return motion_out;
}

Motion& Motion::operator+=(const Motion& motion) {
    VectorField::operator+=(motion);
    return *this;
}

Motion Motion::operator-(const Motion& motion) const {
    Motion motion_out(*this);
    motion_out.VectorField::operator-=(motion);
    return motion_out;
}

Motion& Motion::operator-=(const Motion& motion) {
    VectorField::operator-=(motion);
    return *this;
}

Motion Motion::operator*(const double& val) const {
    Motion motion_out(*this);
    motion_out.VectorField::operator*=(val);
    return motion_out;
}

Motion& Motion::operator*=(const double& val) {
    VectorField::operator*=(val);
    return *this;
}