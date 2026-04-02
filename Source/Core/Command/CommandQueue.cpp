#include "Pch.hpp"
#include "CommandQueue.hpp"

// Internal Project
#include "CommandEnvelope.hpp"
#include "Packets/CommandPacket.hpp"

// C++ Standard Libraries
#include <cstddef>
#include <queue>
#include <semaphore>
#include <utility>

#ifdef _DEBUG
#include <format>
#include <string>
#endif

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Command
{

    /// @brief 
    /// @param packet 
    /// @return 
    auto CommandQueue::Push(Packets::CommandPacket&& packet) -> void
    {
#ifdef _DEBUG
        {
            const std::size_t currentSize = m_Container.size();
            const std::size_t capacity = 4096; // プールのサイズ

            if (currentSize > capacity * 0.8) // 80%を超えたら警告
            {
                ::OutputDebugStringA(std::format("CommandQueue is congesting! Current: {}, TypeIndex: {}\n", currentSize, packet.index()).data());
            }
        }
#endif
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

#ifdef _DEBUG
        {
            const std::size_t currentSize = m_Container.size();
            const std::size_t capacity = 4096; // プールのサイズ

            if (currentSize > capacity * 0.8) // 80%を超えたら警告
            {
                ::OutputDebugStringA(std::format("CommandQueue is congesting! Current: {}, TypeIndex: {}\n", currentSize, packet.index()).data());
            }
        }
#endif
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
    auto CommandQueue::IsEmpty() -> bool
    {
        return m_Container.empty();
    }

    /// @brief 
    /// @return 
    [[nodiscard]]
    auto CommandQueue::GetWakeupEventHandle() const -> HANDLE
    {
        return m_WakeupEvent.get();
    }

} // namespace N503::System
