
#include "gmock/gmock.h"
#include "gtest/gtest.h"

int main( int argc, char** argv )
{
  // Disables elapsed time by default.
  ::testing::GTEST_FLAG( print_time ) = false;

  // This allows the user to override the flag on the command line.
  ::testing::InitGoogleMock( &argc, argv );

  return RUN_ALL_TESTS();
}
