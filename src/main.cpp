#include "esc-configdata.hpp"
#include "util.hpp"

#include <rfl.hpp>
#include <rfl/toml.hpp>

#include <argparse/argparse.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int
main(int argc, char** argv)
{
  argparse::ArgumentParser parser(argv[0]);

  auto& group = parser.add_mutually_exclusive_group(true);
  group.add_argument("-e", "--encode").flag().help("encode config from TOML format (stdin) to raw ESC config data");
  group.add_argument("-d", "--decode").help("decode raw ESC config data in hex format to TOML format");

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    return 1;
  }


  if (parser.is_used("encode")) {
    // Parse TOML from stdin
    SII_config_data config;
    try {
      std::stringstream buffer;
      buffer << std::cin.rdbuf();
      std::string input = buffer.str();
      if (input.empty()) {
        std::cerr << "Input is empty." << std::endl;
        return 1;
      }
      auto config_opt = rfl::toml::read<SII_config_data>(input);
      if (!config_opt) {
        std::cerr << "Failed to parse TOML." << std::endl;
        return 1;
      }
      config = config_opt.value();
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse TOML: " << e.what() << std::endl;
      return 1;
    }
    auto raw_bits = to_raw(config);
    std::string hex = binaryToHexString(raw_bits.to_string());
    std::cout << hex << std::endl;
  } else if (parser.is_used("decode")) {

    std::string raw_hex = parser.get<std::string>("decode"); // e.g.0x050C08EE1027
    std::string raw_bin = hexToBinaryString(raw_hex);
    SII_config_data_bits raw_bits = SII_config_data_bits(raw_bin);

    SII_config_data config = from_raw(raw_bits);
    std::string toml = rfl::toml::write(config);
    std::cout << reverseBlocksOrderInToml(toml) << std::endl;
  }

  return 0;
}
