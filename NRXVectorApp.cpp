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

void helloNrxCmd();
void manipulatorVector();
void addToModelSpace(NcDbObject &objId, NcDbEntity *pEnt);
void getPolylineVertex();

void manipulatorVector()
{
  nds_name ss, ent_name;
  NRX::Int32 ssLen;
  NcDbObjectId entId;
  NcDbEntity* pEnt;
  int ret = ncedSSGet(L"", NULL, NULL, NULL, ss);

  if (ret != RTNORM)
  {
    ncutPrintf(L"\nОшибка выбора примитивов!\n");
    return;
  }

  if (ncedSSLength(ss, &ssLen) != RTNORM)
  {
    ncutPrintf(L"\nОшибка получения количества примитивов!\n");
    return;
  }

  for (NRX::Int32 i = 0; i < ssLen; ++i)
  {
    if (ncedSSName(ss, i, ent_name) != RTNORM)
    {
      ncutPrintf(L"\nОшибка получения имени приитива %d\n", i);
      return;
    }
    if (ncdbGetObjectId(entId, ent_name) != Nano::eOk)
    {
      ncutPrintf(L"\nОшибка получения id приитива %d\n", i);
      return;
    }
    if (ncdbOpenNcDbEntity(pEnt, entId, NcDb::kForWrite, true) == Nano::eOk)
    {
      ncutPrintf(L"\nПримитив открыт!\n");
    }
    else
    {
      ncutPrintf(L"\nОшибка открытия приитива %d\n", i);
      return;
    }
  }

  ncedSSFree(ss);
}

void getPolylineVertex()
{
  nds_name polyline;
  nds_point pt;
  if (ncedEntSel(L"Выберите полилинию", polyline, pt) != RTNORM)
  {
    ncutPrintf(L"\nОшибка выбора!\n");
    return;
  }

  NcDbObjectId plineId;
  NcDbEntity *pEnt;
  if (ncdbGetObjectId(plineId, polyline))
  {
    ncutPrintf(L"\nНе удалось получить id объекта!\n");
    return;
  }

  if (ncdbOpenNcDbEntity(pEnt, plineId, NcDb::kForRead) == Nano::eOk)
  {
    if (NcDb3dPolyline* pline = NcDb3dPolyline::cast(pEnt))
    {
      NcDbObjectIterator* iter = pline->vertexIterator();
      NcDbObjectIdArray vertexArray;
      for (iter->start(); !iter->done(); iter->step())
      {
        vertexArray.append(iter->objectId());
        NcDb3dPolylineVertex* pVertex;
        if (ncdbOpenNcDbObject((NcDbObject*&)pVertex, iter->objectId(), NcDb::kForRead) == Nano::eOk)
        {
          NcGePoint3d pos = pVertex->position();
          ncutPrintf(L"\nКоординаты вершины: X=%f, Y=%f, Z=%f", pos.x, pos.y, pos.z);
          pVertex->close();
        }
        else
        {
          ncutPrintf(L"\nНе удалось получить координаты вершины!");
        }
      }
      delete iter;
    }
    else
    {
      ncutPrintf(L"\nВыбранный примитив не является прямой!\n");
      pEnt->close();
      return;
    }
  }
  else
  {
    ncutPrintf(L"\nНе удалось открыть entity!\n");
    return;
  }

}

void helloNrxCmd()
{
  ncutPrintf(L"Waaaaaauuuuuu!!!");
}

#pragma region NRXinit
void initApp()
{
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXTEST",
    L"MYNRXRTEST",
    ACRX_CMD_TRANSPARENT,
    helloNrxCmd);
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXVECTOR",
    L"MYNRXVECTOR",
    ACRX_CMD_TRANSPARENT,
    manipulatorVector);
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXPLINE3DVERTEX",
    L"MYNRXPLINE3DVERTEX",
    ACRX_CMD_TRANSPARENT,
    getPolylineVertex);
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