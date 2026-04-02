#pragma once

// Internal Project
#include "CommandEnvelope.hpp"
#include "Packets/CommandPacket.hpp"

// External Project
#include <N503/Memory.hpp>

// C++ Standard Libraries
#include <cstddef>
#include <list>
#include <mutex>
#include <queue>

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <wil/resource.h>

namespace N503::Core::Command
{

    /// @brief 
    class CommandQueue final
    {
    public:
        /// @brief 
        using Element = CommandEnvelope;

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
        CommandQueue() = default;

        /// @brief 
        ~CommandQueue() = default;

        /// @brief 
        /// @param  
        CommandQueue(const CommandQueue&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(const CommandQueue&) -> CommandQueue& = delete;

        /// @brief 
        /// @param  
        CommandQueue(CommandQueue&&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(CommandQueue&&) -> CommandQueue& = delete;

    public:
        /// @brief 
        /// @param  
        /// @return 
        auto Push(Packets::CommandPacket&&) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto PushSync(Packets::CommandPacket&&) -> void;

        /// @brief 
        /// @return 
        [[nodiscard]]
        auto PopAll() -> Container;

        /// @brief 
        /// @return 
        [[nodiscard]]
        auto IsEmpty() -> bool;

        /// @brief 
        /// @return 
        [[nodiscard]]
        auto GetWakeupEventHandle() const -> HANDLE;

    public:
        /// @brief 
        Storage m_Storage{ 4096 };  ///< Allocator / Container より先に宣言する必要がある事に注意

        /// @brief 
        Allocator m_Allocator{ &m_Storage };

        /// @brief 
        Container m_Container{ m_Allocator };

        /// @brief 
        wil::unique_event_nothrow m_WakeupEvent{ ::CreateEventA(nullptr, FALSE, FALSE, nullptr) };

        /// @brief 
        std::mutex m_Mutex;

    };

} // namespace N503::Core
