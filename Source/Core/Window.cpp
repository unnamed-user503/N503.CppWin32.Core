#include "Pch.hpp"
#include <N503/Core/Window.hpp>

// Internal Project
#include "Command/CommandQueue.hpp"
#include "Command/Packets/CreateWindowCommand.hpp"
#include "Command/Packets/DestroyWindowCommand.hpp"
#include "Command/Packets/SetWindowTitleCommand.hpp"
#include "CoreEngine.hpp"
#include "Window_Entity.hpp"
#include <N503/Core/Event/Tag.hpp>

// External Project
#include <N503/Event.hpp>

// C++ Standard Libraries
#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core
{

    /// @brief 
    Window::Window(std::string_view title, std::uint32_t width, std::uint32_t height)
    {
        m_Entity = std::make_unique<Window::Entity>();

        auto packet = Command::Packets::CreateWindowCommand
        {
            .Parent = nullptr,
            .Title  = title.data(),
            .Width  = width,
            .Height = height,
            .Result = &m_Entity->m_Handle
        };

        CoreEngine::Instance().GetCommandQueue().PushSync(std::move(packet));

        if (m_Entity->m_Handle)
        {
            // Windowクラスのインスタンス化が出来ていない状態でWN_CREATEを受信できるようにするメリットがないのでここでポインタを埋め込むだけでよい。
            ::SetWindowLongPtrW(m_Entity->m_Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_Entity.get()));
        }
        else
        {
            m_Entity.reset();
        }
    }

    /// @brief 
    Window::~Window()
    {
        Destroy();
    }

    /// @brief 
    /// @param  
    Window::Window(Window&&) = default;

    /// @brief 
    /// @param  
    /// @return 
    auto Window::operator=(Window&&) -> Window& = default;

    // --- Methods ---

    /// @brief 
    /// @return 
    auto Window::WaitForQuit() -> void
    {
        CoreEngine::Instance().AwaitThreadCompletion();
    }

    /// @brief 
    /// @return 
    auto Window::Destroy() -> void
    {
        if (!m_Entity || !m_Entity->m_Handle)
        {
            return;
        }

        auto packet = Command::Packets::DestroyWindowCommand
        {
            .Target = m_Entity->m_Handle,
        };

        // 非同期処理でコマンドを投げるので、先にこちら側の操作を無効化できるように先にm_Handleをnullptrに設定する
        m_Entity->m_Handle = nullptr;

        CoreEngine::Instance().GetCommandQueue().Push(std::move(packet));
    }

    /// @brief 
    /// @param title 
    /// @return 
    auto Window::SetTitle(std::string_view title) -> void
    {
        if (!m_Entity || !m_Entity->m_Handle)
        {
            return;
        }

        auto packet = Command::Packets::SetWindowTitleCommand
        {
            .Target = m_Entity->m_Handle,
            .Title  = title.data()
        };

        CoreEngine::Instance().GetCommandQueue().Push(std::move(packet));
    }

    // --- Events ---

    /// @brief 
    namespace
    {
        /// @brief 
        /// @tparam TEvent 
        /// @param root 
        /// @param handler 
        template <Event::DataType TEvent>
        void RegisterEventHandler(Window::Entity::EventRegistry* root, std::function<bool(TEvent&)> handler)
        {
            if (!root)
            {
                return;
            }

            root->AddChild(TEvent::Tag, [delegate = std::move(handler)](const auto& visitor)
            {
                if (auto* data = visitor.template As<TEvent>())
                {
                    if (delegate(*const_cast<TEvent*>(data)))
                    {
                        visitor.Stop();
                    }
                }
            });
        }
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::WindowActivated&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::WindowClose&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::WindowMoved&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::WindowSized&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::MousePressed&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::MouseReleased&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::MouseMoved&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::MouseWheel&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::KeyDown&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::KeyUp&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

    /// @brief 
    /// @param handler 
    /// @return 
    auto Window::OnEvent(std::function<bool(Events::KeyInput&)> handler) -> void 
    {
        RegisterEventHandler(m_Entity->m_EventRoot.get(), std::move(handler));
    }

} // namespace N503::System
