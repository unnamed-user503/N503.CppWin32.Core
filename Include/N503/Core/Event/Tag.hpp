#pragma once

// C++ Standard Libraries
#include <cstdint>
#include <concepts>

namespace N503::Core::Event
{

    /// @brief 
    enum class Tag
    {
        Root = 0,

        Window,     ///< @brief 
        Mouse,      ///< @brief 
        Keyboard,   ///< @brief 
        System,     ///< @brief 

        MaxTags = 64,
    };

    /// @brief 
    template <typename T>
    concept DataType = requires
    {
        { T::Tag } -> std::convertible_to<::N503::Core::Event::Tag>;
    };

} // namespace N503::Core::Event
