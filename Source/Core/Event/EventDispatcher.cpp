#include "Pch.hpp"
#include "EventDispatcher.hpp"

// Internal Project
#include "../Window_Entity.hpp"
#include "EventQueue.hpp"
#include <N503/Core/Window.hpp>
#include <N503/Core/Event/Tag.hpp>

// External Project
#include <N503/Event.hpp>
#include <N503/Event/Visitor.hpp>

// C++ Standard Libraries
#include <algorithm>
#include <cstdint>
#include <utility>
#include <variant>

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <type_traits>

namespace N503::Core::Event
{

    /// @brief 
    /// @return 
    auto EventDispatcher::Dispatch(EventQueue& eventQueue) -> void
    {
        auto queue = eventQueue.PopAll();

        while (!queue.empty())
        {
            auto& event = queue.front();

            auto delegate = [&event](auto&& data)
            {
                auto entity = reinterpret_cast<Window::Entity*>(::GetWindowLongPtrW(event.Target, GWLP_USERDATA));

                if (!entity)
                {
                    return;
                }

                using Type = std::decay_t<decltype(data)>;
                static_assert(Event::DataType<Type>, "Variantに含まれるすべてのイベント構造体は 'static constexpr Tag' を定義する必要があります。");

                auto visitor = ::N503::Event::Visitor<Tag>{ Type::Tag, std::move(data) };

                entity->m_EventRoot->Accept(visitor);
            };

            std::visit(delegate, event.Packet);

            if (event.Signal)
            {
                event.Signal->release();
            }

            queue.pop();
        }
    }

};
