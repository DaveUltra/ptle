#pragma once

#include "EUIObjectNode.h"


class EUIHud : public EUIObjectNode
{
public:

		/// Presumably :
		/// - 0 : Hidden
		/// - 1 : Entering screen
		/// - 2 : Visible
		/// - 3 : Leaving screen
	int m_state;

		/// Controls state transition.
	bool m_hidden;

	void* unknown;
};
