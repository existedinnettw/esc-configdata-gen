#include "esc-configdata.hpp"
#include <cstring>  // for memcpy

uint8_t
to_raw(const PDI_control& pdi_control)
{
  return static_cast<uint8_t>(pdi_control.type);
}
template<>
PDI_control
from_raw<PDI_control>(const uint8_t& byte)
{
  return PDI_control{ .type = static_cast<PDI_control_type>(byte) };
}


struct ESC_config_bits
{
  /**
   * 0: AL status register has to be set by PDI
   * 1: AL status register will be set to value written to AL control register
   */
  bool al_status_reg_set_by_al_control : 1; // else set by PDI

  /**
   * 0: disabled (if bits [7:4]=0)
   * 1: enabled at all ports (overrides bits [7:4])
   */
  bool enhanced_link_detection_enable : 1;

  bool distibuted_clock_sync_out_enable : 1;
  bool distibuted_clock_latch_in_enable : 1;
  bool enhanced_link_port0 : 1;
  bool enhanced_link_port1 : 1;
  bool enhanced_link_port2 : 1;
  bool enhanced_link_port3 : 1;
};
uint8_t
to_raw(const ESC_config& esc_config)
{
  ESC_config_bits bits = {
    .al_status_reg_set_by_al_control = esc_config.al_status_reg_set_by_al_control,
    .enhanced_link_detection_enable = esc_config.enhanced_link_detection_enable,
    .distibuted_clock_sync_out_enable = esc_config.distibuted_clock_sync_out_enable,
    .distibuted_clock_latch_in_enable = esc_config.distibuted_clock_latch_in_enable,
    .enhanced_link_port0 = esc_config.enhanced_link_port0,
    .enhanced_link_port1 = esc_config.enhanced_link_port1,
    .enhanced_link_port2 = esc_config.enhanced_link_port2,
    .enhanced_link_port3 = esc_config.enhanced_link_port3,
  };
  return *reinterpret_cast<uint8_t*>(&bits);
}
template<>
ESC_config
from_raw<ESC_config>(const uint8_t& byte)
{
  ESC_config_bits bits = *reinterpret_cast<const ESC_config_bits*>(&byte);
  return ESC_config{
    .al_status_reg_set_by_al_control = bits.al_status_reg_set_by_al_control,
    .enhanced_link_detection_enable = bits.enhanced_link_detection_enable,
    .distibuted_clock_sync_out_enable = bits.distibuted_clock_sync_out_enable,
    .distibuted_clock_latch_in_enable = bits.distibuted_clock_latch_in_enable,
    .enhanced_link_port0 = bits.enhanced_link_port0,
    .enhanced_link_port1 = bits.enhanced_link_port1,
    .enhanced_link_port2 = bits.enhanced_link_port2,
    .enhanced_link_port3 = bits.enhanced_link_port3,
  };
}

struct PDI_SPI_config_bits
{
  uint8_t spi_mode : 2;
  uint8_t spi_irq_driver_polarity : 2;
  uint8_t spi_sel_polarity : 1;
  uint8_t spi_data_out_sample_mode : 1;
  uint8_t reserved : 2;
};

uint8_t
to_raw(const PDI_SPI_config& cfg)
{
  PDI_SPI_config_bits bits = { .spi_mode = static_cast<uint8_t>(cfg.spi_mode),
                               .spi_irq_driver_polarity = static_cast<uint8_t>(cfg.spi_irq_driver_polarity),
                               .spi_sel_polarity = static_cast<uint8_t>(cfg.spi_sel_polarity),
                               .spi_data_out_sample_mode = static_cast<uint8_t>(cfg.spi_data_out_sample_mode),
                               .reserved = 0 };
  return *reinterpret_cast<uint8_t*>(&bits);
}

template<>
PDI_SPI_config
from_raw<PDI_SPI_config>(const uint8_t& byte)
{
  PDI_SPI_config_bits bits = *reinterpret_cast<const PDI_SPI_config_bits*>(&byte);
  return PDI_SPI_config{ .spi_mode = static_cast<SPI_mode>(bits.spi_mode),
                         .spi_irq_driver_polarity = static_cast<Output_driver_polarity>(bits.spi_irq_driver_polarity),
                         .spi_sel_polarity = static_cast<SPI_SEL_polarity>(bits.spi_sel_polarity),
                         .spi_data_out_sample_mode =
                           static_cast<SPI_data_out_sample_mode>(bits.spi_data_out_sample_mode) };
}

// ---- Sync_signal_latch_mode ----

struct Sync_signal_latch_mode_bits
{
  uint8_t sync0_out_driver_polarity : 2;
  uint8_t latch0_to_sync0_config : 1;
  uint8_t latch0_map_to_al_request : 1;
  uint8_t sync1_out_driver_polarity : 2;
  uint8_t latch1_to_sync1_config : 1;
  uint8_t latch1_map_to_al_request : 1;
};

uint8_t
to_raw(const Sync_signal_latch_mode& cfg)
{
  Sync_signal_latch_mode_bits bits = { .sync0_out_driver_polarity = static_cast<uint8_t>(cfg.sync0_out_driver_polarity),
                                       .latch0_to_sync0_config = static_cast<uint8_t>(cfg.latch0_to_sync0_config),
                                       .latch0_map_to_al_request = static_cast<uint8_t>(cfg.latch0_map_to_al_request),
                                       .sync1_out_driver_polarity = static_cast<uint8_t>(cfg.sync1_out_driver_polarity),
                                       .latch1_to_sync1_config = static_cast<uint8_t>(cfg.latch1_to_sync1_config),
                                       .latch1_map_to_al_request = static_cast<uint8_t>(cfg.latch1_map_to_al_request) };
  return *reinterpret_cast<uint8_t*>(&bits);
}

template<>
Sync_signal_latch_mode
from_raw<Sync_signal_latch_mode>(const uint8_t& byte)
{
  Sync_signal_latch_mode_bits bits = *reinterpret_cast<const Sync_signal_latch_mode_bits*>(&byte);
  return Sync_signal_latch_mode{ .sync0_out_driver_polarity =
                                   static_cast<Output_driver_polarity>(bits.sync0_out_driver_polarity),
                                 .latch0_to_sync0_config = static_cast<bool>(bits.latch0_to_sync0_config),
                                 .latch0_map_to_al_request = static_cast<bool>(bits.latch0_map_to_al_request),
                                 .sync1_out_driver_polarity =
                                   static_cast<Output_driver_polarity>(bits.sync1_out_driver_polarity),
                                 .latch1_to_sync1_config = static_cast<bool>(bits.latch1_to_sync1_config),
                                 .latch1_map_to_al_request = static_cast<bool>(bits.latch1_map_to_al_request) };
}

uint16_t
to_raw(const Sync_signal_pulse_length& sync_signal_pulse_length)
{
  return static_cast<uint16_t>(sync_signal_pulse_length);
}

// Primary template declaration for from_raw
template<typename T>
T
from_raw(const uint16_t& bits);

// Specialization for Sync_signal_pulse_length
template<>
Sync_signal_pulse_length
from_raw<Sync_signal_pulse_length>(const uint16_t& bits)
{
  return static_cast<Sync_signal_pulse_length>(bits);
}


/**
 * Write field bits into a bitset using memcpy (little-endian)
 * @param bits the bitset to write to
 * @param offset the current byte offset from the start
 * @param value the value to write
 */
template<typename T>
void
write_field_bits(SII_config_data_bits& bits, size_t& offset, const T& value)
{
  auto raw_value = to_raw(value);
  size_t field_size = sizeof(raw_value);
  
  // Create a temporary array to hold the bytes
  uint8_t temp_bytes[sizeof(raw_value)];
  memcpy(temp_bytes, &raw_value, field_size);
  
  // Copy each byte bit by bit into the bitset (from MSB side like original algorithm)
  for (size_t byte_idx = 0; byte_idx < field_size; ++byte_idx) {
    uint8_t byte_val = temp_bytes[byte_idx];
    size_t bit_start = (bits.size() - (offset + byte_idx + 1) * 8);
    for (size_t bit_idx = 0; bit_idx < 8; ++bit_idx) {
      bits[bit_start + bit_idx] = (byte_val >> bit_idx) & 1;
    }
  }
  
  offset += field_size;
}

SII_config_data_bits
to_raw(const SII_config_data& sii_config_data)
{
  SII_config_data_bits bits{ 0 };
  size_t offset = 0;  // byte offset

  // PDI_control (1 byte)
  write_field_bits(bits, offset, sii_config_data.pdi_control);

  // ESC_config (1 byte)
  write_field_bits(bits, offset, sii_config_data.esc_config);

  // PDI_SPI_config (optional, 1 byte if present, else 0)
  if (sii_config_data.pdi_spi_config.has_value()) {
    write_field_bits(bits, offset, sii_config_data.pdi_spi_config.value());
  } else {
    // If not present, write 0s for the PDI_SPI_config field
    write_field_bits(bits, offset, PDI_SPI_config{});
  }

  // Sync_signal_latch_mode (1 byte)
  write_field_bits(bits, offset, sii_config_data.sync_signal_latch_mode);

  // Sync_signal_pulse_length (2 bytes, little-endian)
  write_field_bits(bits, offset, sii_config_data.sync_signal_pulse_length);

  return bits;
}

// Helper function to extract uint8_t and uint16_t from bitset using memcpy (little-endian)
template<typename T>
T
extract_field(const SII_config_data_bits& bits, size_t& offset)
{
  T value = 0;
  size_t field_size = sizeof(T);
  uint8_t temp_bytes[sizeof(T)];
  
  // Extract bytes bit by bit (from MSB side, matching write_field_bits)
  for (size_t byte_idx = 0; byte_idx < field_size; ++byte_idx) {
    uint8_t byte_val = 0;
    size_t bit_start = bits.size() - (offset + byte_idx + 1) * 8;
    for (size_t bit_idx = 0; bit_idx < 8; ++bit_idx) {
      if (bits[bit_start + bit_idx]) {
        byte_val |= (1 << bit_idx);
      }
    }
    temp_bytes[byte_idx] = byte_val;
  }
  
  // Use memcpy to reconstruct the value (respects host endianness)
  memcpy(&value, temp_bytes, sizeof(value));
  
  offset += field_size;
  return value;
}

SII_config_data
from_raw(const SII_config_data_bits& bits)
{
  size_t offset = 0;  // byte offset

  // PDI_control
  uint8_t pdi_control_bits = extract_field<uint8_t>(bits, offset);
  PDI_control pdi_control = from_raw<PDI_control>(pdi_control_bits);

  // ESC_config
  uint8_t esc_config_bits = extract_field<uint8_t>(bits, offset);
  ESC_config esc_config = from_raw<ESC_config>(esc_config_bits);

  // PDI_SPI_config (optional)
  uint8_t pdi_spi_config_bits = extract_field<uint8_t>(bits, offset);
  PDI_SPI_config pdi_spi_config_val = from_raw<PDI_SPI_config>(pdi_spi_config_bits);
  std::optional<PDI_SPI_config> pdi_spi_config;
  if (pdi_spi_config_bits != 0)
    pdi_spi_config = pdi_spi_config_val;

  // Sync_signal_latch_mode
  uint8_t sync_signal_latch_mode_bits = extract_field<uint8_t>(bits, offset);
  Sync_signal_latch_mode sync_signal_latch_mode = from_raw<Sync_signal_latch_mode>(sync_signal_latch_mode_bits);

  // Sync_signal_pulse_length (2 bytes, little-endian)
  uint16_t sync_signal_pulse_length_bits = extract_field<uint16_t>(bits, offset);
  Sync_signal_pulse_length sync_signal_pulse_length = from_raw<Sync_signal_pulse_length>(sync_signal_pulse_length_bits);

  return SII_config_data{ .pdi_control = pdi_control,
                          .esc_config = esc_config,
                          .pdi_spi_config = pdi_spi_config,
                          .sync_signal_latch_mode = sync_signal_latch_mode,
                          .sync_signal_pulse_length = sync_signal_pulse_length };
}

std::string
binaryToHexString(const std::string& binaryString)
{
  if (binaryString.empty()) {
    return "";
  }

  // Pad the binary string with leading zeros so its length is a multiple of 4
  std::string paddedBinaryString = binaryString;
  int paddingNeeded = (4 - (paddedBinaryString.length() % 4)) % 4;
  paddedBinaryString.insert(0, paddingNeeded, '0');

  std::stringstream ss;
  ss << std::hex << std::uppercase; // Set output format to hexadecimal and uppercase

  for (size_t i = 0; i < paddedBinaryString.length(); i += 4) {
    std::string fourBits = paddedBinaryString.substr(i, 4);
    // Convert the 4-bit binary string to an unsigned long
    unsigned long decimalValue = std::bitset<4>(fourBits).to_ulong();
    // Output the hexadecimal representation of the decimal value
    ss << static_cast<char>(decimalValue + (decimalValue < 10 ? '0' : 'A' - 10));
  }

  return ss.str();
}
std::string
hexToBinaryString(const std::string& hexString)
{
  if (hexString.empty()) {
    return "";
  }

  std::string binaryString;
  for (char c : hexString) {
    // Convert each hex character to its binary representation
    unsigned long decimalValue = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;
    std::bitset<4> bits(decimalValue);
    binaryString += bits.to_string();
  }

  return binaryString;
}
