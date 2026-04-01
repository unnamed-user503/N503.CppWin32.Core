#pragma once

// Internal Project
#include "MessageContext.hpp"

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Message::Processors
{

    /// @brief
    class WindowLifecycleTracker final
    {
    public:
        /// @brief
        WindowLifecycleTracker() = default;

        /// @brief
        ~WindowLifecycleTracker() = default;

    public:
        /// @brief
        /// @param 
        /// @return
        auto Process(MessageContext&) -> bool;

        /// @brief 
        /// @param 
        /// @return 
        auto OnNCCreate(MessageContext&) -> bool;

        /// @brief 
        /// @param 
        /// @return 
        auto OnNCDestroy(MessageContext&) -> bool;

    private:
        /// @brief 
        std::uint32_t m_WindowPopulationCount{ 0 };
    };

} // namespace N503::Core::Message::Processors
