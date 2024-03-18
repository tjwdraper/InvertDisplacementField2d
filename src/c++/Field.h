#ifndef _FIELD_H_
#define _FIELD_H_

#include <src/c++/coord2d.h>

template <class T>
class Field {
    public:
        // Constructors and deconstructors
        Field(const dim dimin);
        Field (const Field<T>& fieldin);
        virtual ~Field();

        // Getters and setters
        T* get_field() const;
        dim get_dimensions() const;
        dim get_step() const;
        unsigned int get_size() const;

        // Overload operators
        Field<T>& operator=(const Field<T>& fieldin);
        Field<T> operator+(const Field<T>& fieldin) const;
        Field<T>& operator+=(const Field<T>& fieldin);
        Field<T> operator-(const Field<T>& fieldin) const;
        Field<T>& operator-=(const Field<T>& fieldin);

        Field<T> operator*(const double& val) const;
        Field<T>& operator*=(const double& val);

    protected:
        T *field;
        dim dimin;
        unsigned int sizein;
        dim step;
};

#include <src/c++/Field.tpp>

typedef Field<double> ScalarField;
typedef Field<vector2d> VectorField;

#endif