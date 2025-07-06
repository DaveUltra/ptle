#include "GizmodPlugin.h"


GizmodPlugin* GizmodPlugin::m_instance = 0;


GizmodPlugin::GizmodPlugin()
{
	m_instance = this;
}

std::wstring GizmodPlugin::getConfigDirectory() const
{
	std::string cname = this->getSystemName();
	std::wstring name( cname.begin(), cname.end() );

	return L"config/" + name + L'/';
}
