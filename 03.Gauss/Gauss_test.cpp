#include <gtest/gtest.h>
#include "Gauss_solve.h"

TEST(GaussSolve, TrivialSystem)
{
    GaussMatrix ab(2, 3);
    ab << 2.0, 1.0, 5.0, 1.0, -3.0, -1.0;

    GaussVector x = Gauss_solve(ab);

    ASSERT_EQ(x.size(), 2);
    EXPECT_NEAR(x(0), 2.0, 1e-6);
    EXPECT_NEAR(x(1), 1.0, 1e-6);
}

TEST(GaussSolve, SingularMatrix)
{
    GaussMatrix ab(2, 3);
    ab << 1.0, 2.0, 3.0, 2.0, 4.0, 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}
