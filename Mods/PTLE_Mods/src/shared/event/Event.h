#pragma once

#include <vector>

#define DECLARE_EVENT( name ) \
	class I ## name ## Listener \
	{ \
	public: \
		virtual void on ## name( class name ## Event& e ) = 0; \
	};


template<class ICallbackType>
class Event
{
	friend class PitfallPlugin;
	friend class PitfallEvents;

public:

	typedef ICallbackType ICallbackType;
	typedef std::vector<ICallbackType*> CallbackList;


public:

	Event() : m_cancel(false) {

	}

	inline bool isCancelled() const { return m_cancel; }
	inline void setCancelled( bool b ) { m_cancel = true; }


private:

	bool m_cancel;
};
