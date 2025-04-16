#pragma once

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
