#pragma once

// Internal Project
#include <N503/Core/Event/Data/KeyboardButtonPressEventData.hpp>
#include <N503/Core/Event/Data/KeyboardButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/KeyboardCharEventData.hpp>
#include <N503/Core/Event/Data/MouseButton.hpp>
#include <N503/Core/Event/Data/MouseButtonPressEventData.hpp>
#include <N503/Core/Event/Data/MouseButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/MouseMoveEventData.hpp>
#include <N503/Core/Event/Data/MouseWheelEventData.hpp>
#include <N503/Core/Event/Data/WindowActivateEventData.hpp>
#include <N503/Core/Event/Data/WindowCloseEventData.hpp>
#include <N503/Core/Event/Data/WindowMoveEventData.hpp>
#include <N503/Core/Event/Data/WindowSizeEventData.hpp>

// External Project
#include <N503/Abi/Api.hpp>

// C++ Standard Libraries
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

// Platform/Thirdparty Libraries

namespace N503::Core
{

    /// @brief 
    namespace Events
    {
        using WindowActivated = ::N503::Core::Event::Data::WindowActivateEventData;
        using WindowClose     = ::N503::Core::Event::Data::WindowCloseEventData;
        using WindowMoved     = ::N503::Core::Event::Data::WindowMoveEventData;
        using WindowSized     = ::N503::Core::Event::Data::WindowSizeEventData;
        using MousePressed    = ::N503::Core::Event::Data::MouseButtonPressEventData;
        using MouseReleased   = ::N503::Core::Event::Data::MouseButtonReleaseEventData;
        using MouseMoved      = ::N503::Core::Event::Data::MouseMoveEventData;
        using MouseWheel      = ::N503::Core::Event::Data::MouseWheelEventData;
        using KeyDown         = ::N503::Core::Event::Data::KeyboardButtonPressEventData;
        using KeyUp           = ::N503::Core::Event::Data::KeyboardButtonReleaseEventData;
        using KeyInput        = ::N503::Core::Event::Data::KeyboardCharEventData;
        using MouseButton     = ::N503::Core::Event::Data::MouseButton;
    };

    /// @brief 
    class N503_API Window final
    {
    public:
        /// @brief 
        /// @param title 
        /// @param width 
        /// @param height 
        explicit Window(std::string_view title = "", std::uint32_t width = 720, std::uint32_t height = 480);

        /// @brief 
        ~Window();

        /// @brief 
        /// @param  
        Window(const Window&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(const Window&) -> Window& = delete;

        /// @brief 
        /// @param  
        Window(Window&&);

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(Window&&) -> Window&;

    public:
        /// @brief 
        /// @return 
        auto WaitForQuit() -> void;

        /// @brief 
        /// @return 
        auto Destroy() -> void;

        /// @brief 
        /// @param title 
        /// @return 
        auto SetTitle(std::string_view title) -> void;

    public:
        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::WindowActivated&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::WindowClose&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::WindowMoved&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::WindowSized&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::MousePressed&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::MouseReleased&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::MouseMoved&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::MouseWheel&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::KeyDown&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::KeyUp&)>) -> void;

        /// @brief 
        /// @param  
        /// @return 
        auto OnEvent(std::function<bool(Events::KeyInput&)>) -> void;

    public:
        /// @brief 実装の詳細を隠蔽するための不透明な構造体。
        struct Entity;

#ifdef N503_DLL_EXPORTS
        /// @brief 内部の実装実体（Entity）への参照を取得します。
        /// @note このメソッドはライブラリ内部（DLL境界の内側）でのみ使用されます。
        /// @return Entity を管理する unique_ptr への参照。
        [[nodiscard]]
        auto GetEntity() -> std::unique_ptr<Entity>&
        {
            return m_Entity;
        }
#endif

    private:
#pragma warning(push)
#pragma warning(disable : 4251) // DLL境界を越える際の unique_ptr に関する警告を抑制
        /// @brief 
        std::unique_ptr<Entity> m_Entity;
#pragma warning(pop)
    };

} // namespace N503::Core
