#include "Pch.hpp"
#include "CommandQueue.hpp"

// Internal Project
#include "CommandEnvelope.hpp"
#include "Packets/CommandPacket.hpp"

// C++ Standard Libraries
#include <queue>
#include <semaphore>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Command
{

    /// @brief 
    /// @param packet 
    /// @return 
    auto CommandQueue::Push(Packets::CommandPacket&& packet) -> void
    {
        {
            std::lock_guard const lock{ m_Mutex };

            m_Container.push(
            {
                .Packet = std::move(packet),
                .Signal = nullptr
            });
        }

        m_WakeupEvent.SetEvent();
    }

    /// @brief 
    /// @param packet 
    /// @return 
    auto CommandQueue::PushSync(Packets::CommandPacket&& packet) -> void
    {
        std::binary_semaphore signal{ 0 };

        {
            std::lock_guard const lock{ m_Mutex };

            m_Container.push(
            {
                .Packet = std::move(packet),
                .Signal = &signal
            });
        }

        m_WakeupEvent.SetEvent();

        signal.acquire();
    }

    /// @brief 
    /// @return
    [[nodiscard]]
    auto CommandQueue::PopAll() -> Container
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
    auto CommandQueue::GetWakeupEventHandle() const -> HANDLE
    {
        return m_WakeupEvent.get();
    }

} // namespace N503::System
