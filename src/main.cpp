#include "esc-configdata.hpp"

#include <argparse/argparse.hpp>

#include <iostream>
#include <string>
#include <vector>

int
main(int argc, char** argv)
{
  argparse::ArgumentParser parser(argv[0]); // argv 0 is program name

  // parser.add_argument("--type")
  //   .default_value(std::string{ "et1100" })
  //   .choices("et1100", "lan9252", "ax58100")
  //   .help("choose ESC type");

  auto& group = parser.add_mutually_exclusive_group(true);
  group.add_argument("encode").help("encode config file in TOML format to raw ESC config data");
  group.add_argument("-d", "--decode").help("decode raw ESC config data in hex format to TOML format");

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  

  return 0;
}
