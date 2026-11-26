#pragma once

#include "Event.h"

#include "ptle/EScriptContext.h"


DECLARE_EVENT( ExecuteScript );

class ExecuteScriptEvent : public Event<IExecuteScriptListener>
{
public:

	ExecuteScriptEvent( EScriptContext* ctx )
		: m_context( ctx )
	{
	}

	EScriptContext* getScriptContext() const { return m_context; }
	ERScript* getScript() const { return m_context->m_script; }
	EInstance* getContextOwner() const { return m_context->m_owningInstance; }


private:

	EScriptContext* m_context;
};
