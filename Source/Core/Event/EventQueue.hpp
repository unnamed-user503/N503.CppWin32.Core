#pragma once

// Internal Project
#include "Data/EventPacket.hpp"
#include "EventEnvelope.hpp"

// External Project
#include <N503/Memory.hpp>

// C++ Standard Libraries
#include <array>
#include <cstddef>
#include <cstdint>
#include <list>
#include <mutex>
#include <queue>
#include <tuple>
#include <utility>
#include <variant>

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <wil/resource.h>
#include <algorithm>

namespace N503::Core::Event
{
    
    /// @brief 
    class EventQueue final
    {
    public:
        /// @brief 
        using Element = EventEnvelope;

        /// @brief 
        struct Slot { alignas(void*) std::byte data[sizeof(Element) + 32]; };

        /// @brief 
        using Storage = ::N503::Memory::Storage::Pool<Slot>;

        /// @brief 
        using Allocator = ::N503::Memory::StorageAllocator<Element, Storage>;

        /// @brief 
        using Strategy = std::list<Element, Allocator>;

        /// @brief 
        using Container = std::queue<Element, Strategy>;

    public:
        /// @brief 
        /// @tparam T 
        template <typename TDataType>
        class EventBorrower
        {
        public:
            /// @brief 
            /// @param mutex 
            /// @param queue 
            EventBorrower(std::unique_lock<std::mutex>&& lock, Container& queue) : m_Lock(std::move(lock))
            {
                if (!queue.empty() && std::holds_alternative<TDataType>(queue.back().Packet))
                {
                    m_Target = &std::get<TDataType>(queue.back().Packet);
                }
            }

            /// @brief 
            /// @param  
            EventBorrower(const EventBorrower&) = delete;

            /// @brief 
            /// @param  
            /// @return 
            auto operator=(const EventBorrower&) -> EventBorrower& = delete;

            /// @brief 
            /// @return 
            auto operator->() -> TDataType*
            {
                return m_Target;
            }

            /// @brief 
            explicit operator bool() const
            {
                return m_Target != nullptr;
            }

        private:
            /// @brief 
            std::unique_lock<std::mutex> m_Lock;

            /// @brief 
            TDataType* m_Target = nullptr;
        };

    public:
        /// @brief 
        EventQueue() = default;

        /// @brief 
        ~EventQueue() = default;

        /// @brief 
        /// @param  
        EventQueue(const EventQueue&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(const EventQueue&) -> EventQueue& = delete;

        /// @brief 
        /// @param  
        EventQueue(EventQueue&&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(EventQueue&&) -> EventQueue& = delete;

    public:
        /// @brief 
        /// @param 
        /// @param 
        /// @return 
        auto Push(HWND, Data::EventPacket&&) -> void;

        /// @brief 
        /// @param 
        /// @param 
        /// @return 
        auto PushOrUpdate(HWND, Data::EventPacket&&) -> void;

        /// @brief 
        /// @return 
        [[nodiscard]]
        auto PopAll() -> Container;

        /// @brief 
        /// @return 
        [[nodiscard]]
        auto GetWakeupEventHandle() const -> HANDLE;

    public:
        /// @brief 
        /// @tparam T 
        /// @return 
        template <typename TDataType>
        auto TryBorrowBack() -> EventBorrower<TDataType>
        {
            // ロックを奪取する
            std::unique_lock<std::mutex> lock{ m_Mutex };

            // ロックした状態で Container を特定し、lock ごと Borrower に渡す
            return EventBorrower<TDataType>{ std::move(lock), m_Buffer[m_BufferIndex].Container };
        }

    private:
        /// @brief 
        struct Buffer
        {
            /// @brief 
            typename EventQueue::Storage Storage{ 64 };  ///< Allocator より先に宣言する必要がある事に注意

            /// @brief 
            typename EventQueue::Allocator Allocator{ &Storage };  ///< Container より先に宣言する必要がある事に注意

            /// @brief 
            typename EventQueue::Container Container{ Allocator };
        };

        /// @brief 
        std::array<Buffer, 2> m_Buffer{};

        /// @brief 
        std::uint32_t m_BufferIndex{ 0 };

        /// @brief 
        wil::unique_event_nothrow m_WakeupEvent{ ::CreateEventA(nullptr, FALSE, FALSE, nullptr) };

        /// @brief 
        std::mutex m_Mutex;
    };

} // namespace N503::Core
