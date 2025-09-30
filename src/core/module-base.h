#pragma once
#include "common.h"
#include "event-listener-support.h"

namespace Core
{
    class ModuleBase : public EventListenerSupport
    {
    public:
        virtual ~ModuleBase() override = default;

        virtual bool Attach() = 0;
        virtual void Detach() = 0;

    protected:
        ModuleBase() = default;
    };
}
