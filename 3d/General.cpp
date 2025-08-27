#include "General.hpp"

string GetDirectory(string sPath)
{
	sPath = string ( sPath.rbegin(), sPath.rend() );

	int iFoundPos = sPath.find(WIN_SLASH);

	if (iFoundPos != 0)
		sPath = sPath.substr(iFoundPos); 

	sPath = string ( sPath.rbegin(), sPath.rend() );

	return sPath;
}

string Int2Str(int iValue)
{
	stringstream xStream;
	
	xStream << iValue;

	return xStream.str();
}