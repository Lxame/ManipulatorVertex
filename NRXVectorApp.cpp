// NRXVectorApp.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "NRXVectorApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//

// CNRXVectorAppApp

BEGIN_MESSAGE_MAP(CNRXVectorAppApp, CWinApp)
END_MESSAGE_MAP()


// Создание CNRXVectorAppApp

CNRXVectorAppApp::CNRXVectorAppApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CNRXVectorAppApp

CNRXVectorAppApp theApp;


// Инициализация CNRXVectorAppApp

BOOL CNRXVectorAppApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void helloNrxCmd()
{
  ncutPrintf(L"Waaaaaauuuuuu!!!");
}

void initApp()
{
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXCOMMAND",
    L"MYNRXCOMMAND",
    ACRX_CMD_TRANSPARENT,
    helloNrxCmd);
}

void uninitApp()
{
  ncedRegCmds->removeGroup(L"MYNRXCOMMANDS_GROUP");
}

// EntryPoint
extern "C" __declspec(dllexport) NcRx::AppRetCode
ncrxEntryPoint(NcRx::AppMsgCode msg, void* pkt)
{
  switch (msg)
  {
  case NcRx::kInitAppMsg:
    initApp();
    break;
  case NcRx::kUnloadAppMsg:
    uninitApp();
  }
  return NcRx::kRetOK;
}
