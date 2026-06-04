#include <stdexcept>
#include <cmath>
#include "Gauss_solve.h"

GaussVector Gauss_solve(GaussMatrix &ab)
{
    int n = ab.rows();
    int m = ab.cols();

    for (int i = 0; i < n; ++i)
    {
        int pivot_row = i;
        double max_val = std::abs(ab(i, i));
        for (int r = i + 1; r < n; ++r)
        {
            double val = std::abs(ab(r, i));
            if (val > max_val)
            {
                max_val = val;
                pivot_row = r;
            }
        }

        if (max_val < 1e-9)
        {
            throw std::runtime_error("Matrix is singular or nearly singular");
        }

        if (pivot_row != i)
        {
            ab.row(i).swap(ab.row(pivot_row));
        }

        for (int r = i + 1; r < n; ++r)
        {
            double factor = ab(r, i) / ab(i, i);
            ab.row(r).tail(m - i) -= factor * ab.row(i).tail(m - i);
        }
    }

    GaussVector x = GaussVector::Zero(n);
    for (int i = n - 1; i >= 0; --i)
    {
        double sum = ab(i, m - 1);
        if (i < n - 1)
        {
            sum -= ab.row(i).segment(i + 1, n - 1 - i).dot(x.tail(n - 1 - i));
        }
        x(i) = sum / ab(i, i);
    }

    return x;
}
