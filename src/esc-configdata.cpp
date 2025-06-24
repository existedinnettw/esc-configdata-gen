#include "esc-configdata.hpp"

Byte
to_byte(const PDI_control& pdi_control)
{
  return Byte(static_cast<uint8_t>(pdi_control.type));
}
template<>
PDI_control
from_byte<PDI_control>(const Byte& byte)
{
  return PDI_control{ .type = static_cast<PDI_control_type>(byte.to_ulong()) };
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
Byte
to_byte(const ESC_config& esc_config)
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
  return Byte(*reinterpret_cast<uint8_t*>(&bits));
}
template<>
ESC_config
from_byte<ESC_config>(const Byte& byte)
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
