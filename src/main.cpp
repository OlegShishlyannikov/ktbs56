#include "core.hpp"

int main( int argc, char * argv[] )
{
  core core;
  core.load_drivers();
  core.run();
  return 0;
}
