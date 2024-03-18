#include <iostream>
#include <cstring>
#include <stdexcept>
#include <math.h>
#include <mex.h>

// Constructors and deconstructors
template <class T>
Field<T>::Field(const dim dimin) {
    // Set the dimensions of the field
    this->dimin = dimin;
    this->sizein = dimin.x * dimin.y;
    this->step = dim(1, dimin.x);

    // Allocate memory for the field and set values to zero
    try {
        this->field = new T[this->sizein];
        memset(this->field, 0, this->sizein*sizeof(T));
    }
    catch (const std::bad_alloc& e) {
        const std::string mes = "Error in Field<T>::Field(const dim dimin): " + 
            std::string(e.what()) + 
            std::string("\n");
        mexErrMsgTxt(mes.c_str());
    }
}

template <class T>
Field<T>::Field(const Field<T>& fieldin) {
    // Set the dimensions of the field
    this->dimin  = fieldin.get_dimensions();
    this->sizein = fieldin.get_size();
    this->step   = fieldin.get_step();

    // Allocate memory for the field and copy values from input
    try {
        this->field = new T[this->sizein];
        memcpy(this->field, fieldin.get_field(), this->sizein*sizeof(T));
    }
    catch (const std::bad_alloc& e) {
        const std::string mes = "Error in Field<T>::Field(const Field& fieldin): " + 
            std::string(e.what()) + 
            std::string("\n");
        mexErrMsgTxt(mes.c_str());
    }
}

template <class T>
Field<T>::~Field() {
    delete[] this->field;
}

// Getters and setters
template <class T>
T* Field<T>::get_field() const {
    return this->field;
}

template <class T>
dim Field<T>::get_dimensions() const {
    return this->dimin;
}

template <class T>
dim Field<T>::get_step() const {
    return this->step;
}

template <class T>
unsigned int Field<T>::get_size() const {
    return this->sizein;
}

// Overload operators
template <class T>
Field<T>& Field<T>::operator=(const Field<T>& fieldin) {
    if (this->dimin != fieldin.get_dimensions()) {
        throw std::invalid_argument("input argument has to have same dimensions as target");
    }
    memcpy(this->field, fieldin.get_field(), this->sizein*sizeof(T));
    return *this;
}

template <class T>
Field<T>& Field<T>::operator+=(const Field<T>& fieldin) {
    if (this->dimin != fieldin.get_dimensions()) {
        throw std::invalid_argument("input argument has to have same dimensions as target");
    }
    // Get a copy to the pointer to the content of input
    T *datain = fieldin.get_field();

    // Iterate over voxels, add together
    for (unsigned int i = 0 ; i < this->sizein; i++) {
        this->field[i] += datain[i];
    }

    // Done
    return *this;
}

template <class T>
Field<T> Field<T>::operator+(const Field<T>& fieldin) const {
    if (this->dimin != fieldin.get_dimensions()) {
        throw std::invalid_argument("input argument has to have same dimensions as target");
    }
    // Create output field as copy of input
    Field<T> fieldout(*this);

    // Add input to it
    fieldout += fieldin;

    // Done
    return fieldout;
}

template <class T>
Field<T>& Field<T>::operator-=(const Field<T>& fieldin) {
    if (this->dimin != fieldin.get_dimensions()) {
        throw std::invalid_argument("input argument has to have same dimensions as target");
    }
    // Get a copy to the pointer to the content of input
    T *datain = fieldin.get_field();

    // Iterate over voxels, add together
    for (unsigned int i = 0; i < this->sizein; i++) {
        this->field[i] -= datain[i];
    }

    // Done
    return *this;
}

template <class T>
Field<T> Field<T>::operator-(const Field<T>& fieldin) const {
    if (this->dimin != fieldin.get_dimensions()) {
        throw std::invalid_argument("input argument has to have same dimensions as target");
    }
    // Create output field as copy of input
    Field<T> fieldout(*this);

    // Add input to it
    fieldout -= fieldin;

    // Done
    return fieldout;
}

template <class T>
Field<T>& Field<T>::operator*=(const double& val) {
    // Iterate over voxels, add together
    for (unsigned int i = 0; i < this->sizein; i++) {
        this->field[i] *= val;
    }

    // Done
    return *this;
}

template <class T>
Field<T> Field<T>::operator*(const double& val) const {
    // Create output field as copy of input
    Field<T> fieldout(*this);

    // Add input to it
    fieldout *= val;

    // Done
    return fieldout;
}
