#include "GizmodPlugin.h"

#include "gizmod/Gizmod.h"

#include <sstream>


GizmodPlugin* GizmodPlugin::m_instance = 0;


GizmodPlugin::GizmodPlugin()
	: m_versionMajor( Gizmod::VERSION_MAJOR )
	, m_versionMinor( Gizmod::VERSION_MINOR )
	, m_versionPatch( Gizmod::VERSION_PATCH )
{
	m_instance = this;
}

std::wstring GizmodPlugin::getConfigDirectory() const
{
	std::string cname = this->getSystemName();
	std::wstring name( cname.begin(), cname.end() );

	return L"config/" + name + L'/';
}

std::string GizmodPlugin::getVersionString() const
{
	std::stringstream ss;
	ss << m_versionMajor << '.' << m_versionMinor << '.' << m_versionPatch;
	return ss.str();
}
