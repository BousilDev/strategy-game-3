#pragma once

/**
 * @file Resource.hpp
 * @brief Defines the Resource type representing game materials or currencies.
 */

#include <string>

namespace core {

/**
 * @enum ResourceType
 * @brief Simple enumerator for different types of resources. Length should match kNResourceTypes
 * defined in Constants.cpp
 */
enum ResourceType {
  kGold,
  kWood,
  kMetal,
  kFood
};

/**
 * @struct Resource
 * @brief The type and amount of a game resource.
 */
struct Resource {
  ResourceType type; ///< Name of the type
  unsigned int amount; ///< Quantity of the resource

  Resource() = default;
  Resource(ResourceType type, int amount) : type(type), amount(amount) {}
};

} // namespace core
