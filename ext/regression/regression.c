#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <ruby.h>

//----------------------------------------------------
//
// METHOD:  polyfit
//
// INPUTS:  dependentValues[0..(countOfElements-1)]
//          independentValues[0...(countOfElements-1)]
//          countOfElements
//          order - Order of the polynomial fitting
//
// OUTPUTS: coefficients[0..order] - indexed by term
//               (the (coef*x^3) is coefficients[3])
//
// HOMEPAGE: https://github.com/natedomin/polyfit.git
//
//----------------------------------------------------
int polyfit(const double* const dependentValues,
            const double* const independentValues,
            unsigned int        countOfElements,
            unsigned int        order,
            double*             coefficients)
{
    // Declarations...
    // ----------------------------------
    const unsigned int maxNoOfElements = 50; // Arbitrary
    enum {maxOrder = 5};

    double B[maxOrder+1] = {0.0f};
    double P[((maxOrder+1) * 2)+1] = {0.0f};
    double A[(maxOrder + 1)*2*(maxOrder + 1)] = {0.0f};

    double x, y, powx;

    unsigned int ii, jj, kk;

    // Verify initial conditions....
    // ----------------------------------

    // This method requires that the countOfElements >
    // (order+1)
    if (countOfElements <= order)
        return -1;

    // This method has imposed an arbitrary bound of
    // order <= maxOrder.  Increase maxOrder if necessary.
    if (order > maxOrder)
        return -1;

    // This method has imposed an arbitrary bound of
    // countOfElements <= maxNoOfElements
    if (countOfElements > maxNoOfElements)
        return -1;

    // Begin Code...
    // ----------------------------------

    // Identify the column vector
    for (ii = 0; ii < countOfElements; ii++)
    {
        x    = dependentValues[ii];
        y    = independentValues[ii];
        powx = 1;

        for (jj = 0; jj < (order + 1); jj++)
        {
            B[jj] = B[jj] + (y * powx);
            powx  = powx * x;
        }
    }

    // Initialize the PowX array
    P[0] = countOfElements;

    // Compute the sum of the Powers of X
    for (ii = 0; ii < countOfElements; ii++)
    {
        x    = dependentValues[ii];
        powx = dependentValues[ii];

        for (jj = 1; jj < ((2 * (order + 1)) + 1); jj++)
        {
            P[jj] = P[jj] + powx;
            powx  = powx * x;
        }
    }

    // Initialize the reduction matrix
    //
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            A[(ii * (2 * (order + 1))) + jj] = P[ii+jj];
        }

        A[(ii*(2 * (order + 1))) + (ii + (order + 1))] = 1;
    }

    // Move the Identity matrix portion of the redux matrix
    // to the left side (find the inverse of the left side
    // of the redux matrix
    for (ii = 0; ii < (order + 1); ii++)
    {
        x = A[(ii * (2 * (order + 1))) + ii];
        if (x != 0)
        {
            for (kk = 0; kk < (2 * (order + 1)); kk++)
            {
                A[(ii * (2 * (order + 1))) + kk] =
                    A[(ii * (2 * (order + 1))) + kk] / x;
            }

            for (jj = 0; jj < (order + 1); jj++)
            {
                if ((jj - ii) != 0)
                {
                    y = A[(jj * (2 * (order + 1))) + ii];
                    for (kk = 0; kk < (2 * (order + 1)); kk++)
                    {
                        A[(jj * (2 * (order + 1))) + kk] =
                            A[(jj * (2 * (order + 1))) + kk] -
                            y * A[(ii * (2 * (order + 1))) + kk];
                    }
                }
            }
        }
        else
        {
            // Cannot work with singular matrices
            return -1;
        }
    }

    // Calculate and Identify the coefficients
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            x = 0;
            for (kk = 0; kk < (order + 1); kk++)
            {
                x = x + (A[(ii * (2 * (order + 1))) + (kk + (order + 1))] *
                    B[kk]);
            }
            coefficients[ii] = x;
        }
    }

    return 0;
}

static VALUE
rb_polyfit(VALUE rb_self, VALUE rb_x_array, VALUE rb_y_array, VALUE rb_order)
{
  VALUE rb_c_array;

  char status;
  unsigned int i, order, length, order_length;
  double *x_array, *y_array, *c_array;

  Check_Type(rb_x_array, T_ARRAY);
  Check_Type(rb_y_array, T_ARRAY);

  order = NUM2INT(rb_order);
  order_length = order + 1;
  length = (unsigned int)RARRAY_LEN(rb_x_array);

  rb_c_array = rb_ary_new2(order_length);

  if (!length)
    return rb_c_array;

  c_array = malloc(order_length * sizeof(double));
  x_array = malloc(length * sizeof(double));
  y_array = malloc(length * sizeof(double));

  for(i = 0; i < length; ++i)
  {
    x_array[i] = NUM2DBL( rb_ary_entry(rb_x_array, i) );
    y_array[i] = NUM2DBL( rb_ary_entry(rb_y_array, i) );
  }

  status = polyfit(x_array, y_array, length, order, c_array);

  free(x_array);
  free(y_array);

  if (!status)
  {
    for(i = 0; i < order_length; ++i)
    {
      rb_ary_store(rb_c_array, i, rb_float_new(c_array[i]));
    }
  }

  return rb_c_array;
}

void Init_regression(void)
{
  VALUE rb_mRegression = rb_define_module("Regression");
  rb_define_module_function(rb_mRegression, "polyfit", rb_polyfit, 3);
}
