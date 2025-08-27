#include "Object.hpp"

void CObject::Add(xObect* xEntity)
{
	m_aObjects.push_back(xEntity);
}

void CObject::Remove(xObjType xType, string sName)
{
	for(int i=0; i<m_aObjects.size(); i++)
	{
		if (m_aObjects[i]->xType == xType && m_aObjects[i]->sName == sName)
		{
			m_aObjects.erase(m_aObjects.begin()+i,m_aObjects.begin()+i+1);
		}
	}
}

