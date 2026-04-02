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
        std::lock_guard const lock{ m_Mutex };

        // UIスレッドで溜めたデータを「Allocatorごと」奪い去る
        auto container = std::move(m_Container);

        // 空になった場所に、正しい Allocator（Storageへのポインタ）を再装填
        m_Container = Container{ m_Allocator };

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
