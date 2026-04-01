#pragma once

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    enum class MouseButton : std::int32_t
    {
        None,   ///< @brief
        Left,   ///< @brief
        Right,  ///< @brief
        Middle, ///< @brief
        X1,     ///< @brief
        X2      ///< @brief
    };

} // namespace N503::Core::Event::Data
