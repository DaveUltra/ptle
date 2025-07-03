#pragma once

#include "event/CollectExplorerEvent.h"
#include "event/CollectIdolEvent.h"
#include "event/EntitySpawnEvent.h"
#include "event/LoadLevelEvent.h"


class PitfallEvents
{
public:

	template<class EventType>
	inline void registerEvent( typename EventType::ICallbackType* cb );

	template<class EventType>
	inline void invokeEvent( EventType& event );


private:

	// Creating a callback list for every type of event.
#	define PITFALL_EVENT(name) name##Event::CallbackList m_event##name;
#	include "PitfallEvents.inl"
};


// Definitions for registerEvent() and invokeEvent().
#define PITFALL_EVENT(name) \
	template<> \
	inline void PitfallEvents::registerEvent<name##Event>( typename name##Event::ICallbackType* cb ) \
	{ \
		m_event##name.push_back( cb ); \
	} \
	\
	template<> \
	inline void PitfallEvents::invokeEvent<name##Event>( name##Event& event ) \
	{ \
		for ( const auto& cb : m_event##name ) { cb->on##name( event ); } \
	}
#include "PitfallEvents.inl"
