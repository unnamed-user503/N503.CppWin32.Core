#include "Pch.hpp"
#include "EventQueue.hpp"

// Internal Project
#include "Data/EventPacket.hpp"
#include "EventEnvelope.hpp"

// C++ Standard Libraries
#include <cstddef>
#include <mutex>
#include <queue>
#include <utility>

#ifdef _DEBUG
#include <format>
#include <string>
#endif

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
#ifdef _DEBUG
        {
            const std::size_t currentSize = m_Buffer[m_BufferIndex].Container.size();
            const std::size_t capacity = 4096; // プールのサイズ

            if (currentSize >= capacity * 0.8) // 80%を超えたら警告
            {
                ::OutputDebugStringA(std::format("EventQueue is congesting! Current: {}, TypeIndex: {}\n", currentSize, packet.index()).data());
            }
        }
#endif
        {
            std::lock_guard const lock{ m_Mutex };

            auto envelope = EventEnvelope
            {
                .Packet = std::move(packet),
                .Target = target
            };

            m_Buffer[m_BufferIndex].Container.push(std::move(envelope));
        }

        m_WakeupEvent.SetEvent();
    }

    /// @brief 
    /// @param target 
    /// @param packet 
    /// @return 
    auto EventQueue::PushOrUpdate(HWND target, Data::EventPacket&& packet) -> void
    {
#ifdef _DEBUG
        {
            const std::size_t currentSize = m_Buffer[m_BufferIndex].Container.size();
            const std::size_t capacity = 4096; // プールのサイズ

            if (currentSize >= capacity * 0.8) // 80%を超えたら警告
            {
                ::OutputDebugStringA(std::format("EventQueue is congesting! Current: {}, TypeIndex: {}\n", currentSize, packet.index()).data());
            }
        }
#endif
        {
            std::lock_guard const lock{ m_Mutex };

            if (!m_Buffer[m_BufferIndex].Container.empty())
            {
                auto& tail = m_Buffer[m_BufferIndex].Container.back().Packet;

                if (tail.index() == packet.index())
                {
                    tail = std::move(packet);
                    return;
                }
            }

            auto envelope = EventEnvelope
            {
                .Packet = std::move(packet),
                .Target = target
            };

            m_Buffer[m_BufferIndex].Container.push(std::move(envelope));
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
