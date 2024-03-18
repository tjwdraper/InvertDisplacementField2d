#ifndef _MOTION_H_
#define _MOTION_H_

#include <src/c++/Field.h>

class Motion : public VectorField {
    public:
        // Constructors and deconstructors
        Motion(const dim dimin);
        Motion(const Motion& motion);
        ~Motion() override;

        // Getters and setters
        vector2d* get_motion() const;
        void set_motion(const double* motion);
        void reset();

        // Copy to input
        void copy_motion_to_input(double* motion) const;

        // Interpolate
        void interpolate(const Motion& motion, const Motion& displaced_grid);

        // Get some motion field properties
        double norm() const;
        double maxabs() const;

        // Overload operator=
        Motion& operator=(const Motion& motion);
        Motion operator+(const Motion& motion) const;
        Motion& operator+=(const Motion& motion);
        Motion operator-(const Motion& motion) const;
        Motion& operator-=(const Motion& motion);

        Motion operator*(const double& val) const;
        Motion& operator*=(const double& val);
};

#endif