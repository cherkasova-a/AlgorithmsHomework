#include "Gauss_solve.h"
#include "util.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <matrix.csv>\n";
        return 1;
    }

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        GaussVector x = Gauss_solve(ab);
        std::cout << x << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
