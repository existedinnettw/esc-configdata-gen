#include <esc-configdata.hpp>
#include <util.hpp>

#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/Generic.hpp>
#include <rfl/json.hpp>
#include <rfl/toml.hpp>

#include <bit> //bit_cast
#include <format>
#include <iostream>
#include <string>

using ::testing::Test;

// Helper function to convert uint8_t to binary string
std::string
uint8ToBinaryString(uint8_t value)
{
  std::string result;
  for (int i = 7; i >= 0; --i) {
    result += ((value >> i) & 1) ? '1' : '0';
  }
  return result;
}

// Helper function to convert uint16_t to binary string
std::string
uint16ToBinaryString(uint16_t value)
{
  std::string result;
  for (int i = 15; i >= 0; --i) {
    result += ((value >> i) & 1) ? '1' : '0';
  }
  return result;
}


TEST(EscConfigDataTest, GenFunction)
{
  auto pdi_control = PDI_control{ PDI_control_type::spi };

  auto esc_config = ESC_config{ .al_status_reg_set_by_al_control = true,
                                .enhanced_link_detection_enable = false,
                                .distibuted_clock_sync_out_enable = true,
                                .distibuted_clock_latch_in_enable = true,
                                .enhanced_link_port0 = false,
                                .enhanced_link_port1 = false,
                                .enhanced_link_port2 = false,
                                .enhanced_link_port3 = false };
  std::cout << "0x" << binaryToHexString(uint8ToBinaryString(to_raw(pdi_control)))
            << binaryToHexString(uint8ToBinaryString(to_raw(esc_config))) << '\n';

  // 0x0150:0x0151
  auto pdi_config = PDI_SPI_config{
    .spi_mode = SPI_mode::spi_mode_0,
    .spi_irq_driver_polarity = Output_driver_polarity::push_pull_active_high,
    .spi_sel_polarity = SPI_SEL_polarity::active_low,
    .spi_data_out_sample_mode = SPI_data_out_sample_mode::normal_sample,
  };
  auto sync_sig_latch_mode = Sync_signal_latch_mode{
    .sync0_out_driver_polarity = Output_driver_polarity::push_pull_active_high,
    .latch0_to_sync0_config = true,
    .latch0_map_to_al_request = true,
    .sync1_out_driver_polarity = Output_driver_polarity::push_pull_active_high,
    .latch1_to_sync1_config = true,
    .latch1_map_to_al_request = true,
  };
  std::cout << "0x" << binaryToHexString(uint8ToBinaryString(to_raw(pdi_config)))
            << binaryToHexString(uint8ToBinaryString(to_raw(sync_sig_latch_mode))) << '\n';

  // 0x0982:0x0983
  Sync_signal_pulse_length sync_signal_pulse_length = 10000; // 1000 * 10ns
  std::cout << std::format("0x{:04x}\n", sync_signal_pulse_length);
  EXPECT_EQ(to_raw(sync_signal_pulse_length), 10000);

  // 0x0152:0x0153
  std::cout << std::format("0x{:04x}\n", 0);

  // 0x0012:0x0013

  // reserved
  // reserved
  // checksum
}


TEST(EscConfigDataTest, DecodeFunction)
{
  uint8_t raw_pdi = 0x03;
  uint8_t raw_latch = 0x44;

  PDI_SPI_config pdi_spi_config = from_raw<PDI_SPI_config>(raw_pdi);
  Sync_signal_latch_mode sync_sig_latch_mode = from_raw<Sync_signal_latch_mode>(raw_latch);

  EXPECT_EQ(pdi_spi_config.spi_mode, SPI_mode::spi_mode_3);
  EXPECT_EQ(pdi_spi_config.spi_irq_driver_polarity, Output_driver_polarity::push_pull_active_low);

  // reflect-cpp?
  // print(pdi_spi_config);
  std::cout << rfl::toml::write(pdi_spi_config) << "\n\n";
  std::cout << rfl::toml::write(sync_sig_latch_mode) << "\n\n";
}


TEST(EscConfigDataTest, SII_config_data_to_raw_test)
{
  SII_config_data sii_config_data = {
    .pdi_control = PDI_control{ PDI_control_type::spi },
    .esc_config =
      ESC_config{
                               .al_status_reg_set_by_al_control = true,
                               .enhanced_link_detection_enable = false,
                               .distibuted_clock_sync_out_enable = true,
                               .distibuted_clock_latch_in_enable = true,
                               .enhanced_link_port0 = false,
                               .enhanced_link_port1 = false,
                               .enhanced_link_port2 = false,
                               .enhanced_link_port3 = false,
                               },
    .pdi_spi_config =
      PDI_SPI_config{
                               .spi_mode = SPI_mode::spi_mode_0,
                               .spi_irq_driver_polarity = Output_driver_polarity::push_pull_active_high,
                               .spi_sel_polarity = SPI_SEL_polarity::active_low,
                               .spi_data_out_sample_mode = SPI_data_out_sample_mode::normal_sample,
                               },
    .sync_signal_latch_mode =
      Sync_signal_latch_mode{
                               .sync0_out_driver_polarity = Output_driver_polarity::push_pull_active_high,
                               .latch0_to_sync0_config = true,
                               .latch0_map_to_al_request = true,
                               .sync1_out_driver_polarity = Output_driver_polarity::push_pull_active_high,
                               .latch1_to_sync1_config = true,
                               .latch1_map_to_al_request = true,
                               },
    .sync_signal_pulse_length = 10000, // 1000 * 10ns
  };
  // print raw data
  std::cout << "Raw SII_config_data:\n";
  auto raw_bits = to_raw(sii_config_data);
  std::cout << "0b" << raw_bits.to_string() << "\n";
  std::cout << "0x" << binaryToHexString(raw_bits.to_string()) << "\n\n";

  EXPECT_EQ(binaryToHexString(raw_bits.to_string()), "050D08EE1027");

  // std::cout << rfl::toml::write(sii_config_data) << "\n\n";
}

TEST(EscConfigDataTest, SII_config_data_from_raw_test)
{
  std::string raw_hex = "050C08EE1027";
  std::string raw_bin = hexToBinaryString(raw_hex);
  SII_config_data_bits raw_bits = SII_config_data_bits(raw_bin);

  std::string repeat_raw_hex = binaryToHexString(raw_bits.to_string());
  EXPECT_EQ(raw_hex, repeat_raw_hex);

  SII_config_data sii_config_data = from_raw(raw_bits);
  std::cout << "Decoded SII_config_data:\n";
  std::cout << rfl::toml::write(sii_config_data) << "\n\n";

  // then back to raw_hex
  auto repeat_raw_bits = to_raw(sii_config_data);
  std::string repeat_raw_bin = repeat_raw_bits.to_string();
  std::string repeat_raw_hex2 = binaryToHexString(repeat_raw_bin);
  EXPECT_EQ(repeat_raw_hex, repeat_raw_hex2);
}

TEST(UtilTest, ReverseBlocksOrderInToml)
{
  // Test with a simple TOML with multiple sections
  std::string input = R"(top_level_var = 42

[section_a]
var_a = "value_a"
another_var = true

[section_b]
var_b = "value_b"
num_var = 123

[section_c]
var_c = "value_c")";

  std::string expected = R"(top_level_var = 42

[section_c]
var_c = "value_c"

[section_b]
num_var = 123
var_b = "value_b"

[section_a]
another_var = true
var_a = "value_a")";

  std::string result = reverseBlocksOrderInToml(input);
  EXPECT_EQ(expected, result);
}

TEST(UtilTest, ReverseBlocksOrderInTomlEmpty)
{
  std::string input = "";
  std::string result = reverseBlocksOrderInToml(input);
  EXPECT_EQ(input, result);
}

TEST(UtilTest, ReverseBlocksOrderInTomlSingleSection)
{
  std::string input = R"([section]
var = "value")";

  std::string result = reverseBlocksOrderInToml(input);
  EXPECT_EQ(input, result);
}