#pragma once

/**
 * @see ESC Section I – Technology ch 11.1 SII EEPROM Content
 * @details
 * only >=cpp26 have bit-filed reflection
 */

#include <bit> //bit_cast
#include <bitset>
#include <cstdint>
#include <iomanip> // For std::setw and std::setfill
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
// #include <rfl.hpp>

template<typename T>
T
from_raw(const uint8_t& byte);

/**
 * @see ESC Section II – Register Description ch 2.25 PDI Control (0x0140)
 */
enum class PDI_control_type : uint8_t
{
  deactivated = 0x00,
  di4 = 0x01,
  do4 = 0x02,
  di2_do2 = 0x03,
  dio = 0x04,
  spi = 0x05,
  oversample_io = 0x06,
  ecat_bridge = 0x07,
  async_mcu_16bit = 0x08,
  async_mcu_8bit = 0x09,
  sync_mcu_16bit = 0x0A,
  sync_mcu_8bit = 0x0B,
  di32_do0 = 0x0C,
  di24_do8 = 0x0D,
  di16_do16 = 0x0E,
  di8_do24 = 0x0F,
  di0_do32 = 0x10,
  onchip_bus = 0x80,
  // other reserved
};
struct PDI_control
{
  PDI_control_type type;
};
uint8_t
to_raw(const PDI_control& pdi_control);

/**
 * @see ESC Section II – Register Description 2.26 ESC Configuration (0x0141)
 */
struct ESC_config
{
  /**
   * 0: AL status register has to be set by PDI
   * 1: AL status register will be set to value written to AL control register
   */
  bool al_status_reg_set_by_al_control; // else set by PDI

  /**
   * 0: disabled (if bits [7:4]=0)
   * 1: enabled at all ports (overrides bits [7:4])
   */
  bool enhanced_link_detection_enable;

  bool distibuted_clock_sync_out_enable;
  bool distibuted_clock_latch_in_enable;
  bool enhanced_link_port0;
  bool enhanced_link_port1;
  bool enhanced_link_port2;
  bool enhanced_link_port3;
};
uint8_t
to_raw(const ESC_config& esc_config);

/**
 * @see ESC Section II – Register Description 2.28 PDI Configuration (0x0150:0x0153)
 */
// class PDI_config;

enum class SPI_mode : uint8_t
{
  spi_mode_0 = 0x00, // CPOL=0, CPHA=0, most used

  spi_mode_1 = 0x01, // CPOL=0, CPHA=1
  spi_mode_2 = 0x02, // CPOL=1, CPHA=0
  spi_mode_3 = 0x03, // CPOL=1, CPHA=1
};
enum class Output_driver_polarity : uint8_t
{
  push_pull_active_low = 0x00,
  open_drain_active_low = 0x01,
  push_pull_active_high = 0x02,
  open_source_active_high = 0x03,
};

enum class SPI_SEL_polarity : bool
{
  active_low = 0x00,
  active_high = 0x01,
};

enum class SPI_data_out_sample_mode : bool
{
  normal_sample = 0x00, // SPI_DO and SPI_DI are sampled at the same SPI_CLK edge
  late_sample = 0x01,   // SPI_DO and SPI_DI are sampled at different SPI_CLK edges
};

/**
 * @see ESC Section II – Register Description 2.28.2 PDI SPI Configuration
 */
struct PDI_SPI_config
{
  SPI_mode spi_mode;
  Output_driver_polarity spi_irq_driver_polarity;
  SPI_SEL_polarity spi_sel_polarity;
  SPI_data_out_sample_mode spi_data_out_sample_mode;
  // uint8_t : 2; // reserved
};
uint8_t
to_raw(const PDI_SPI_config& pdi_spi_config);

/**
 * @see ESC Section II – Register Description 2.28.7 Sync/Latch Configuration
 */
struct Sync_signal_latch_mode
{
  Output_driver_polarity sync0_out_driver_polarity;
  bool latch0_to_sync0_config; // true: sync0, false: latch0
  bool latch0_map_to_al_request;
  Output_driver_polarity sync1_out_driver_polarity;
  bool latch1_to_sync1_config; // true: sync1, false: latch1
  bool latch1_map_to_al_request;
};
uint8_t
to_raw(const Sync_signal_latch_mode& sync_signal_latch_mode);

/**
 * @brief
 * Pulse Length of SyncSignals 10ns per unit
 * @details
 * Pulse length of SyncSignals (in Units of 10ns)
 * 0 --> Acknowledge mode: SyncSignal will be cleared by reading SYNC[1:0] Status register
 * @see Pulse Length of SyncSignals (0x0982:0x983)
 */
// using Sync_signal_pulse_length = uint16_t;
typedef uint16_t Sync_signal_pulse_length;
uint16_t
to_raw(const Sync_signal_pulse_length& sync_signal_pulse_length);

struct SII_config_data
{
  // word 0
  PDI_control pdi_control;
  ESC_config esc_config;

  // word 1
  std::optional<PDI_SPI_config> pdi_spi_config;
  Sync_signal_latch_mode sync_signal_latch_mode;

  // word 2
  Sync_signal_pulse_length sync_signal_pulse_length;

  // word 3
  // Extended PDI Configuration (0x0152:0x0153)

  // word 4
  // Configured_Station_alias configured_station_alias;

  // word 5
  // uint16_t reserved1; // reserved

  // word 6
  // uint16_t reserved2; // reserved

  // word 7
  // uint16_t checksum; // checksum
};

using SII_config_data_bits = std::bitset<3 * sizeof(uint16_t) * 8>;

SII_config_data_bits
to_raw(const SII_config_data& sii_config_data);

SII_config_data
from_raw(const SII_config_data_bits& bits);

std::string
binaryToHexString(const std::string& binaryString);

std::string
hexToBinaryString(const std::string& hexString);