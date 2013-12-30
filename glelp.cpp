/*
* GLELP
* Anders Malm 2012
*/

#define define_WGL_ARB_extensions_string 1

#include "glelp.h"

namespace glelp
{
	bool loadExtensions()
	{
		if(!initExtensions())
			return false;

		return initWindowsExtensions();
	}

	const char* wext;
	/*
		Quick hack for comparing two strings so we can live without std libraries
	*/
	bool compare(const char* str1, const char* str2)
	{
		unsigned int i = 0;
		while(true)
		{
			char c1 = str1[i];
			char c2 = str2[i];

			if(c1 != c2) return false;
			if((c1 == 0) && (c2 == 0)) return true;

			i++;
		}
	}

	/*
		Check each extensiom
	*/
	bool findExtension(const char* extension, const char* extensions)
	{
		unsigned char ci = 0;
		char current[256];
		while(true)
		{
			while((*extensions) != ' ')
			{
				if((*extensions) == 0)
					return false;
				
				current[ci] = (*extensions);

				extensions++;
				ci++;
			}

			current[ci] = 0;

			if(compare(current, extension))
				return true;

			extensions++;
			ci = 0;
		}
	}

	bool checkAvailable(const char* extension)
	{
		// Needed on windows
		if(wglGetExtensionsStringARB == 0)
		{
			wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

			HWND hWnd = GetActiveWindow();
			HDC hDC = GetDC(hWnd);
			wext = wglGetExtensionsStringARB(hDC);
		}

		if(glGetStringi == 0 )
		{
			if(findExtension(extension, (char*)glGetString(GL_EXTENSIONS)))
				return true;
		}
		else
		{
			int num;
			glGetIntegerv(GL_NUM_EXTENSIONS, &num);

			for (int i = 0; i < num; i++)
				if(compare(extension, (const char*)glGetStringi(GL_EXTENSIONS, i)))
					return true;
		}
		if (findExtension(extension, (char*)wext))
			return true;

		return false;
	}

	/*
	* Checks if the asked for version is less or equal compared
	* to the version provided by the driver
	*/
	bool checkVersion(char major, char minor)
	{
		// this might be happening too quickly..
		if (glGetString == 0)
		{
			glGetString = (PFNGLGETSTRINGPROC)GetProcAddress(GetModuleHandle(TEXT("opengl32.dll")), "glGetString");
		}

		const GLubyte* version = glGetString(GL_VERSION);
		int GLmajor = version[0] - '0';
		int GLminor = version[2] - '0';

		if(major > GLmajor)
			return false;

		if(minor > GLminor)
		{
			if( major < GLmajor)
				return true;
			else
				return false;
		}

		return true;
	}

}
