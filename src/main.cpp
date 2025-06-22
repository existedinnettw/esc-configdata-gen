#include "esc-configdata.hpp"

#include <argparse/argparse.hpp>

#include <string>
#include <vector>

int
main(int argc, char** argv)
{
  argparse::ArgumentParser parser(argv[0]); // argv 0 is program name
  parser.add_argument("--version").default_value(false).implicit_value(true).help("Show version information");
  parser.add_argument("--help").default_value(false).implicit_value(true).help("Show help message");

  return 0;
}
