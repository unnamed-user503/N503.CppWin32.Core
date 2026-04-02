#include "Pch.hpp"
#include "CommandDispatcher.hpp"

// Internal Project
#include "CommandExecutor.hpp"
#include "CommandQueue.hpp"

// C++ Standard Libraries
#include <variant>

namespace N503::Core::Command
{

    /// @brief 
    /// @return 
    auto CommandDispatcher::Dispatch(CommandQueue& commandQueue, CommandExecutor& commandExecutor) -> void
    {
        auto queue = commandQueue.PopAll();

        while (!queue.empty())
        {
            auto command = queue.front();

            auto delegate = [this, &commandExecutor](auto&& concreteCommand)
            {
                commandExecutor(concreteCommand);
            };

            std::visit(delegate, command.Packet);

            if (command.Signal)
            {
                command.Signal->release();
            }

            queue.pop();
        }
    }

};
