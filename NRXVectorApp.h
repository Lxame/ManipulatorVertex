#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"

#include "arxHeaders.h"
#include "AcExtensionModule.h"

class CNRXVectorAppApp : public CWinApp
{
public:
	CNRXVectorAppApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
