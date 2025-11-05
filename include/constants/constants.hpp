#pragma once

/**
 * @file constants.hpp
 * @brief Defines the Constants namespace which declares all global constants.
 */

namespace constants {
const int kNumberOfResourceTypes = 4;
const int kNumberOfTerrainTypes = 4;
const bool debug = true;
const std::string debug_prefix = "###: ";
const int kInitWindowWidth = 800;
const int kInitWindowHeight = 600;
const std::string faultySavesPath = "tests/faultySaves/";
} // namespace constants