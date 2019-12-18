#include "vec.h"
#include "gtest/gtest.h"

#include "BmpEncoder.h"

namespace tapley {

TEST(QAvec, equality)
{
	Vec3 a(1.0, 1.0, 1.0);
	Vec3 b(2.0, 2.0, 2.0);
	Vec3 c(1.0, 1.0, 1.0);
	EXPECT_NE(a, b);
	EXPECT_EQ(a, c);
}

TEST(QAvec, length)
{
	Vec3 a(3.0, 10.0, 12.0);
	const auto expected = std::sqrt(std::pow(3.0, 2.0) + std::pow(10.0, 2.0) + std::pow(12.0, 2.0));
	EXPECT_NEAR(expected, a.length(), 0.000001);
}

}