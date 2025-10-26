#pragma once

#include "EInstance.h"

#include "ptle/containers/LinkedList/LinkedList.h"


class EUIObjectNode : public EInstance
{
public:

	LinkedList m_children;
	EUIObjectNode* m_parent;
	LinkedListElem* m_navigation;

	uint32_t m_flags;
	uint32_t m_flags2;

	char padding3[0x04];

	float unknown1;
	float unknown2;
	float unknown3;
	float m_xPos;
	float unknown5;
	float m_yPos;
	float unknown7;

	char padding4[0x0C];

	char* m_uiTypeName;
	char* unknown8;


	virtual void func_unknown26();
	virtual void SetPosition( float* );
	virtual void func_unknown28( float* );
	virtual void func_unknown29( float* );
	virtual void func_unknown30( int, int, int );
	virtual void func_unknown31();
	virtual void func_unknown32();
	virtual void func_unknown33();
	virtual bool func_unknown34();
	virtual void func_unknown35();
	virtual void GetPosition( float* out );
	virtual void func_unknown37();
	virtual void AddChild( EUIObjectNode* );
	virtual void func_unknown39();
};
