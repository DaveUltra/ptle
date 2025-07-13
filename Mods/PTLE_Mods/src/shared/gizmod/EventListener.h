#pragma once

#include "event/CollectExplorerEvent.h"
#include "event/CollectIdolEvent.h"
#include "event/CollectItemEvent.h"
#include "event/EntitySpawnEvent.h"
#include "event/LevelLoadedEvent.h"
#include "event/LoadLevelEvent.h"
#include "event/ShamanPurchaseEvent.h"


class EventListener
{
public:

	template<class EventType>
	inline void registerEvent( typename EventType::ICallbackType* cb );

	template<class EventType>
	inline void invokeEvent( EventType& event );


private:

	// Creating a callback list for every type of event.
#	define GIZMOD_EVENT(name) name##Event::CallbackList m_event##name;
#	include "GizmodEvents.inl"
};


// Definitions for registerEvent() and invokeEvent().
#define GIZMOD_EVENT(name) \
	template<> \
	inline void EventListener::registerEvent<name##Event>( typename name##Event::ICallbackType* cb ) \
	{ \
		m_event##name.push_back( cb ); \
	} \
	\
	template<> \
	inline void EventListener::invokeEvent<name##Event>( name##Event& event ) \
	{ \
		for ( const auto& cb : m_event##name ) { cb->on##name( event ); } \
	}
#include "GizmodEvents.inl"

