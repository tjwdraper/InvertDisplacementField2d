#ifndef _INVERTER_H_
#define _INVERTER_H_

#include <src/c++/coord2d.h>
#include <src/c++/Motion.h>

class Inverter {
    public:
        Inverter(const dim dimin, const unsigned int niter, const double convergence_criterion);
        ~Inverter();

        // Data I/O
        void set_vector_field(const double* field);

        void copy_inverted_vector_field_to_input(double* field) const;

        // Invert the motion field
        void invert();

    private:
        void display() const;

        dim dimin;
        dim step;
        unsigned int sizein;

        unsigned int niter;
        double convergence_criterion;

        Motion* displacement;
        Motion* inverse_displacement;
};

#endif