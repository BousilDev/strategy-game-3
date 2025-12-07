#pragma once

/**
 * @file resource.hpp
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

  Resource& operator+=(const Resource& other) {
    if (type == other.type) {
      amount += other.amount;
    }
    return *this;
  }

  Resource operator+(const Resource& other) const {
    Resource result = *this;
    result += other;
    return result;
  }

  Resource& operator*(unsigned int factor) {
    amount *= factor;
    return *this;
  }
};

} // namespace core
