#include "Pch.hpp"
#include "CommandExecutor.hpp"

// Internal Project
#include "../Message/MessageDispatcher.hpp"
#include "Packets/CreateWindowCommand.hpp"
#include "Packets/DestroyWindowCommand.hpp"
#include "Packets/SetWindowTitleCommand.hpp"
#include "Packets/StopEngineCommand.hpp"

// C++ Standard Libraries
#include <string>
#include <string_view>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Command
{

    namespace
    {
        /// @brief 
        /// @param utf8 
        /// @return 
        auto TranscodeUtf8ToWide(const std::string_view& utf8) -> std::wstring
        {
            if (utf8.empty())
            {
                return {};
            }

            int desired = ::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), -1, nullptr, 0);
            if (desired == 0)
            {
                return {};
            }

            std::wstring result(desired, 0);
            ::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), -1, &result[0], desired);

            result.resize(desired - 1); 
            return result;
        }
    }

    /// @brief 
    /// @param command 
    /// @return 
    auto CommandExecutor::operator()(const Packets::CreateWindowCommand& command) -> void
    {
        static const auto isRegistered = [&]() -> bool
        {
            auto windowClass = WNDCLASSEXW{};
            auto windowClassName = L"N503.CppWin32.Core.Window";

            if (::GetClassInfoExW(::GetModuleHandleW(nullptr), windowClassName, &windowClass))
            {
                return true;
            }

            ::SecureZeroMemory(&windowClass, sizeof(WNDCLASSEXW));

            windowClass.cbSize        = sizeof(WNDCLASSEXW);
            windowClass.style         = CS_HREDRAW | CS_VREDRAW;
            windowClass.hInstance     = ::GetModuleHandleW(nullptr);
            windowClass.lpszClassName = windowClassName;
            windowClass.lpfnWndProc   = Message::MessageDispatcher::OSMessageCallback;
            windowClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
            windowClass.hCursor       = ::LoadCursorW(nullptr, IDC_ARROW);

            return ::RegisterClassExW(&windowClass) != 0;
        }();

        if (!isRegistered)
        {
            return;
        }

        auto hwnd = ::CreateWindowExW(
            0,
            L"N503.CppWin32.Core.Window",
            TranscodeUtf8ToWide(command.Title).data(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<int>(command.Width), 
            static_cast<int>(command.Height),
            command.Parent,
            nullptr,
            ::GetModuleHandleW(nullptr),
            nullptr
        );

        if (hwnd)
        {
            *command.Result = hwnd;
        }
        else
        {
            return;
        }

        ::ShowWindow(hwnd, SW_SHOW);
    }

    /// @brief 
    /// @param command 
    /// @return 
    auto CommandExecutor::operator()(const Packets::DestroyWindowCommand& command) -> void
    {
        if (command.Target && IsWindow(command.Target))
        {
            ::DestroyWindow(command.Target);
        }
    }

    /// @brief 
    /// @param command 
    /// @return 
    auto CommandExecutor::operator()(const Packets::SetWindowTitleCommand& command) -> void
    {
        if (command.Target)
        {
            ::SetWindowTextW(command.Target, TranscodeUtf8ToWide(command.Title).data());
        }
    }

    /// @brief 
    /// @param command 
    /// @return 
    auto CommandExecutor::operator()(const Packets::StopEngineCommand& command) -> void
    {
        ::PostQuitMessage(command.ExitCode);
    }

};
