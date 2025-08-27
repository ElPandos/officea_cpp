#ifndef __3D_OBJECT_H
#define __3D_OBJECT_H

#include "General.hpp"
#include "UserInput.hpp"

enum xObjType
{
	Circle = 0,
	Triangle = 1,
	Square = 2,
	Cube = 3,
	Sphere = 4,
	Pyramid = 5,
	Model = 6
};

struct xObect
{
	string sName;
	xObjType xType;
	CUserInput* pInput;
	int iClipWidth;
	int iClipHeight;
};

// ---------------------------------

class CObject
{

public:

	CObject() {}
	~CObject() {}

	xObjType GetType() { }
	void Add(xObect* xEntity);
	void Remove(xObjType xType, string sName);
	
private:

	double dArea;

	vector<xObect*> m_aObjects;

};

class CTriangle: public CObject
{

public:

	CTriangle() {}
	~CTriangle() {}

	void Volume();

	virtual xObjType GetType() { return m_xObject.xType; }
	
private:

	xObect m_xObject;

};

#endif __3D_OBJECT_H