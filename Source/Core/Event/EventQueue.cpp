#include "Pch.hpp"
#include "EventQueue.hpp"

// Internal Project
#include "Data/EventPacket.hpp"
#include "EventEnvelope.hpp"

// External Project

// C++ Standard Libraries
#include <list>
#include <mutex>
#include <queue>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Event
{

    /// @brief 
    /// @param target 
    /// @param packet 
    /// @return 
    auto EventQueue::Push(HWND target, Data::EventPacket&& packet) -> void
    {
        {
            std::lock_guard const lock{ m_Mutex };

            m_Container.push(
            {
                .Packet = std::move(packet),
                .Signal = nullptr,
                .Target = target
            });
        }

        m_WakeupEvent.SetEvent();
    }

    /// @brief 
    /// @param target 
    /// @param packet 
    /// @return 
    auto EventQueue::PushOrUpdate(HWND target, Data::EventPacket&& packet) -> void
    {
        {
            std::lock_guard const lock{ m_Mutex };

            if (!m_Container.empty())
            {
                if (auto& tail = m_Container.back().Packet; tail.index() == packet.index())
                {
                    tail = std::move(packet);
                    return;
                }
            }

            m_Container.push(
            {
                .Packet = std::move(packet),
                .Signal = nullptr,
                .Target = target
            });
        }

        m_WakeupEvent.SetEvent();
    }

    /// @brief 
    /// @return
    [[nodiscard]]
    auto EventQueue::PopAll() -> Container
    {
        auto container = Container{ Strategy{ m_Allocator } };
        {
            std::lock_guard const lock{ m_Mutex };
            std::swap(m_Container, container);
        }
        return container;
    }

    /// @brief 
    /// @return 
    [[nodiscard]]
    auto EventQueue::GetWakeupEventHandle() const -> HANDLE
    {
        return m_WakeupEvent.get();
    }

} // namespace N503::System
