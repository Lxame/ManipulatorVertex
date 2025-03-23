#include "pch.h"
#include "framework.h"
#include "NRXVectorApp.h"

#pragma region MFCinit
BEGIN_MESSAGE_MAP(CNRXVectorAppApp, CWinApp)
END_MESSAGE_MAP()

CNRXVectorAppApp::CNRXVectorAppApp()
{

}

CNRXVectorAppApp theApp;

BOOL CNRXVectorAppApp::InitInstance()
{
  CWinApp::InitInstance();

  return TRUE;
}
#pragma endregion

void helloNrxCmd()
{
  ncutPrintf(L"Waaaaaauuuuuu!!!");
}

#pragma region NRXinit
void initApp()
{
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXVECTOR",
    L"MYNRXVECTOR",
    ACRX_CMD_TRANSPARENT,
    helloNrxCmd);
}

void uninitApp()
{
  ncedRegCmds->removeGroup(L"MYNRXCOMMANDS_GROUP");
}

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
#pragma endregion