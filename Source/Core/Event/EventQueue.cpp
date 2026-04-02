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

            m_Buffer[m_BufferIndex].Container.push(
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

            if (!m_Buffer[m_BufferIndex].Container.empty())
            {
                if (auto& tail = m_Buffer[m_BufferIndex].Container.back().Packet; tail.index() == packet.index())
                {
                    tail = std::move(packet);
                    return;
                }
            }

            m_Buffer[m_BufferIndex].Container.push(
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
        std::lock_guard const lock{ m_Mutex };

        // UIスレッドで溜めたデータを「Allocatorごと」奪い去る
        auto container = std::move(m_Buffer[m_BufferIndex].Container);

        // 空になった場所に、正しい Allocator（Storageへのポインタ）を再装填
        m_Buffer[m_BufferIndex].Container = Container{ m_Buffer[m_BufferIndex].Allocator };

        // 次の Push 用にインデックスを切り替える
        m_BufferIndex = (m_BufferIndex + 1) % m_Buffer.size();

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
