#pragma once

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    enum class ModifierKeyState : std::uint32_t
    {
        None    = 0,      ///< @brief
        LButton = 1 << 0, ///< @brief
        RButton = 1 << 1, ///< @brief
        MButton = 1 << 2, ///< @brief
        Shift   = 1 << 3, ///< @brief
        Control = 1 << 4, ///< @brief
        Alt     = 1 << 5, ///< @brief
        Super   = 1 << 6, ///< @brief
    };

    // --- ビット演算子のオーバーロード ---

    /// @brief
    /// @param lhs
    /// @param rhs
    /// @return
    [[nodiscard]]
    inline constexpr auto operator|(const ModifierKeyState lhs, const ModifierKeyState rhs) noexcept -> ModifierKeyState
    {
        return static_cast<ModifierKeyState>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
    }

    /// @brief
    /// @param lhs
    /// @param rhs
    /// @return
    inline auto operator|=(ModifierKeyState& lhs, const ModifierKeyState rhs) noexcept -> ModifierKeyState&
    {
        lhs = lhs | rhs;
        return lhs;
    }

    /// @brief
    /// @param lhs
    /// @param rhs
    /// @return
    [[nodiscard]]
    inline constexpr auto operator&(const ModifierKeyState lhs, const ModifierKeyState rhs) noexcept -> ModifierKeyState
    {
        return static_cast<ModifierKeyState>(static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
    }

    /// @brief
    /// @param state
    /// @param flag
    /// @return
    [[nodiscard]]
    inline constexpr auto HasFlag(const ModifierKeyState state, const ModifierKeyState flag) noexcept -> bool
    {
        return (state & flag) == flag;
    }

} // namespace N503::Core::Event::Data
