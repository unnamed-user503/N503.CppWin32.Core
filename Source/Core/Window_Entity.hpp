#pragma once

// Internal Project
#include <N503/Core/Window.hpp>
#include <N503/Core/Event/Tag.hpp>

// External Project
#include <N503/Event.hpp>   // IWYU pragma: export
#include <N503/Memory.hpp>  // IWYU pragma: export

// C++ Standard Libraries
#include <memory>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core
{

    /// @brief 
    struct Window::Entity final
    {
        /// @brief 
        HWND m_Handle{ nullptr };

        // --- Cache ---

        // --- Events ---

        /// @brief 
        using EventTag = Event::Tag;

        /// @brief 
        using EventNode = N503::Event::Node<EventTag>;

        /// @brief 
        using EventAllocator = N503::Memory::Storage::Pool<EventNode>;

        /// @brief 
        using EventRegistry = N503::Event::Registry<EventTag, EventAllocator>;

        /// @brief 
        std::unique_ptr<EventRegistry> m_EventRoot;

        /// @brief 
        Entity() : m_EventRoot{ std::make_unique<EventRegistry>(Event::Tag::Root, 4096) }
        {
        }
    };

} // namespace N503::Core
