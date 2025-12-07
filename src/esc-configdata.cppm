module;
#include <bit> //bit_cast
#include <bitset>
#include <cstdint>
#include <iomanip> // For std::setw and std::setfill
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

export module esc_configdata;

export template<typename T>
T
from_raw(const uint8_t& byte);

export enum class PDI_control_type
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
};

export struct PDI_control
{
  PDI_control_type type;
};

export uint8_t
to_raw(const PDI_control& pdi_control);

export struct ESC_config
{
  bool al_status_reg_set_by_al_control; // else set by PDI
  bool enhanced_link_detection_enable;
  bool distibuted_clock_sync_out_enable;
  bool distibuted_clock_latch_in_enable;
  bool enhanced_link_port0;
  bool enhanced_link_port1;
  bool enhanced_link_port2;
  bool enhanced_link_port3;
};
export uint8_t
to_raw(const ESC_config& esc_config);

export enum class SPI_mode
{
  spi_mode_0 = 0x00,
  spi_mode_1 = 0x01,
  spi_mode_2 = 0x02,
  spi_mode_3 = 0x03,
};
export enum class Output_driver_polarity
{
  push_pull_active_low = 0x00,
  open_drain_active_low = 0x01,
  push_pull_active_high = 0x02,
  open_source_active_high = 0x03,
};

export enum class SPI_SEL_polarity
{
  active_low = 0x00,
  active_high = 0x01,
};

export enum class SPI_data_out_sample_mode
{
  normal_sample = 0x00,
  late_sample = 0x01,
};

export struct PDI_SPI_config
{
  SPI_mode spi_mode;
  Output_driver_polarity spi_irq_driver_polarity;
  SPI_SEL_polarity spi_sel_polarity;
  SPI_data_out_sample_mode spi_data_out_sample_mode;
};
export uint8_t
to_raw(const PDI_SPI_config& pdi_spi_config);

export struct Sync_signal_latch_mode
{
  Output_driver_polarity sync0_out_driver_polarity;
  bool latch0_to_sync0_config;
  bool latch0_map_to_al_request;
  Output_driver_polarity sync1_out_driver_polarity;
  bool latch1_to_sync1_config;
  bool latch1_map_to_al_request;
};
export uint8_t
to_raw(const Sync_signal_latch_mode& sync_signal_latch_mode);

export typedef uint16_t Sync_signal_pulse_length;
export uint16_t
to_raw(const Sync_signal_pulse_length& sync_signal_pulse_length);

export struct SII_config_data
{
  PDI_control pdi_control;
  ESC_config esc_config;
  std::optional<PDI_SPI_config> pdi_spi_config;
  Sync_signal_latch_mode sync_signal_latch_mode;
  Sync_signal_pulse_length sync_signal_pulse_length;
};

export using SII_config_data_bits = std::bitset<3 * sizeof(uint16_t) * 8>;

export SII_config_data_bits
to_raw(const SII_config_data& sii_config_data);

export SII_config_data
from_raw(const SII_config_data_bits& bits);


export std::string
binaryToHexString(const std::string& binaryString);

export std::string
hexToBinaryString(const std::string& hexString);

// Implementation belongs to module implementation unit (src/esc-configdata.cpp)
