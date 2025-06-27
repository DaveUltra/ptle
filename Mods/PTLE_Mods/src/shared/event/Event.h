#pragma once


class Event
{
public:

	Event() : m_cancel(false) {

	}

	inline bool isCancelled() const { return m_cancel; }
	inline void setCancelled( bool b ) { m_cancel = true; }

private:

	bool m_cancel;
};
