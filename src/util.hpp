#pragma once

#include <string>

/**
 * @brief Reverse blocks order in a TOML.
 * @details
 * This function more like a hotfix for reflectcpp::toml::write
 * which does not write blocks in the order they are defined.
 */
std::string
reverseBlocksOrderInToml(const std::string& toml);