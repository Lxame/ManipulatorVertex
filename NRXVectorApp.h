// NRXVectorApp.h: основной файл заголовка для библиотеки DLL NRXVectorApp
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы

#include "arxHeaders.h"
#include "AcExtensionModule.h"


// CNRXVectorAppApp
// Реализацию этого класса см. в файле NRXVectorApp.cpp
//

class CNRXVectorAppApp : public CWinApp
{
public:
	CNRXVectorAppApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
