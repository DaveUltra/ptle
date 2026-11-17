#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"


class EUIModel
{
public:

	uint32_t m_modelCRC;
	class ERModel* m_model;
	TreeMap m_treeMap;
};
