#include <esc-configdata.hpp>

#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

#include <bit> //bit_cast
#include <format>
#include <iostream>

using ::testing::Test;

TEST(EscConfigDataTest, GenFunction)
{
  auto pdi_control = PDI_control::spi;

  auto esc_config = ESC_config{ .al_status_reg_set_by_al_control = false,
                                .enhanced_link_detection_enable = true,
                                .distibuted_clock_sync_out_enable = true,
                                .distibuted_clock_latch_in_enable = true,
                                .enhanced_link_port0 = false,
                                .enhanced_link_port1 = false,
                                .enhanced_link_port2 = false,
                                .enhanced_link_port3 = false };
  std::cout << std::format(
    "0x{:02x}{:02x}\n", magic_enum::enum_integer(pdi_control), std::bit_cast<std::uint8_t>(esc_config));

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
    .latch0_map_to_al_request = false,
    .sync1_out_driver_polarity = Output_driver_polarity::push_pull_active_high,
    .latch1_to_sync1_config = true,
    .latch1_map_to_al_request = false,
  };
  std::cout << std::format(
    "0x{:02x}{:02x}\n", std::bit_cast<std::uint8_t>(pdi_config), std::bit_cast<std::uint8_t>(sync_sig_latch_mode));

  // 0x0982:0x0983
  Sync_signal_pulse_length sync_signal_pulse_length = 0x1027;
  std::cout << std::format("0x{:04x}\n", sync_signal_pulse_length);

  // 0x0152:0x0153
  std::cout << std::format("0x{:04x}\n", 0);

  // 0x0012:0x0013

  // reserved
  // reserved
  // checksum
}

void print(const PDI_SPI_config& cfg) {
    std::cout << "spi_mode: " << magic_enum::enum_name(cfg.spi_mode) << '\n';
    std::cout << "spi_irq_driver_polarity: " << magic_enum::enum_name(cfg.spi_irq_driver_polarity) << '\n';
    std::cout << "spi_sel_polarity: " << magic_enum::enum_name(cfg.spi_sel_polarity) << '\n';
    std::cout << "spi_data_out_sample_mode: " << magic_enum::enum_name(cfg.spi_data_out_sample_mode) << '\n';
}
void print(const Sync_signal_latch_mode& cfg) {
    std::cout << "sync0_out_driver_polarity: " << magic_enum::enum_name(cfg.sync0_out_driver_polarity) << '\n';
    std::cout << "latch0_to_sync0_config: " << cfg.latch0_to_sync0_config << '\n';
    std::cout << "latch0_map_to_al_request: " << cfg.latch0_map_to_al_request << '\n';
    std::cout << "sync1_out_driver_polarity: " << magic_enum::enum_name(cfg.sync1_out_driver_polarity) << '\n';
    std::cout << "latch1_to_sync1_config: " << cfg.latch1_to_sync1_config << '\n';
    std::cout << "latch1_map_to_al_request: " << cfg.latch1_map_to_al_request << '\n';
}

TEST(EscConfigDataTest, DecodeFunction)
{
  uint8_t raw_pdi = 0x03;
  uint8_t raw_latch = 0x44;

  auto pdi_spi_config = std::bit_cast<PDI_SPI_config>(raw_pdi);
  auto sync_sig_latch_mode = std::bit_cast<Sync_signal_latch_mode>(raw_latch);

  // reflect-cpp?
  print(pdi_spi_config);
  print(sync_sig_latch_mode);
}