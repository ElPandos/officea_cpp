#include "App.hpp"
#include "General.hpp"


int main(int argc, char** argv)
{
	CApp* pApp = new CApp(GetDirectory(argv[0]));
	delete pApp;

	return 0;
}

