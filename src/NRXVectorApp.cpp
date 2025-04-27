#include "pch.h"
#include "framework.h"
#include "NRXVectorApp.h"
#include "Manipulator.h"

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

void testNrxCmd();
void getPolylineVertex();
bool getFilePath(const NCHAR *&path);
bool getFileNameAndPath(std::wstring &name, std::wstring &path);
void openPLineFromBlock();

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

  if (ncdbOpenNcDbEntity(pEnt, plineId, NcDb::kForRead) != Nano::eOk)
  {
    ncutPrintf(L"\nНе удалось открыть entity!\n");
    return;
  }

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
    ncutPrintf(L"\nВыбранный примитив не является 3D полилинией!\n");
    pEnt->close();
    return;
  }

  NcGeMatrix3d ucsMatrix;
  if (ncedGetCurrentUCS(ucsMatrix) == Nano::eOk)
  {
    NcGeVector3d xAxis;
    NcGeVector3d yAxis;
    NcGeVector3d zAxis;
    NcGePoint3d  origin;
    ucsMatrix.getCoordSystem(origin, xAxis, yAxis, zAxis);

    ncutPrintf(L"\nModel Coordinate System (MCS):");
    ncutPrintf(L"\nOrigin: X=%.6f, Y=%.6f, Z=%.6f", origin.x, origin.y, origin.z);
    ncutPrintf(L"\nX-Axis: X=%.6f, Y=%.6f, Z=%.6f", xAxis.x, xAxis.y, xAxis.z);
    ncutPrintf(L"\nY-Axis: X=%.6f, Y=%.6f, Z=%.6f", yAxis.x, yAxis.y, yAxis.z);
    ncutPrintf(L"\nZ-Axis: X=%.6f, Y=%.6f, Z=%.6f", zAxis.x, zAxis.y, zAxis.z);
  }
  else
  {
    ncutPrintf(L"\nОшибка: Не удалось получить систему координат.");
  }
}

bool getFilePath(const NCHAR *&path)
{
  NcDbDatabase* pDb = ncdbHostApplicationServices()->workingDatabase();

  if (!pDb)
  {
    ncutPrintf(L"\nОшибка: Не удалось получить доступ к базе данных!");
    return false;
  }

  const NCHAR* filePath;
  if (pDb->getFilename(filePath) != Nano::eOk)
  {
    ncutPrintf(L"\nОшибка: Не удалось получить имя файла!");
    return false;
  }

  path = filePath;
}

bool getFileNameAndPath(std::wstring& name, std::wstring& path)
{
  const NCHAR *ncpath;
  getFilePath(ncpath);

  std::wstring strFilePath(ncpath);
  std::wstring fileName;
  int pos = strFilePath.find_last_of('\\');
  if (pos != -1)
  {
    fileName = strFilePath.substr(pos + 1);
  }

  name = fileName;
  path = strFilePath;

  return true;
}

void openPLineFromBlock() 
{
  NcDbObjectId entId;
  nds_name selObj;
  nds_point pt;

  if (ncedEntSel(L"\nВыберите блок-манипулятор (содержащий 3д полилинию-скелет: ", selObj, pt) != RTNORM)
  {
    ncutPrintf(L"\nОшибка: Не удалось выбрвть блок!");
    return;
  }

  NcDbObject* pObj;
  if (ncdbGetObjectId(entId, selObj))
  {
    ncutPrintf(L"\nОшибка: Не удалось получить id объекта!\n");
    return;
  }

  if (ncdbOpenNcDbObject(pObj, entId, NcDb::kForRead) != Nano::eOk)
  {
    ncutPrintf(L"\nОшибка: Не удалось открыть примитив!");
    return;
  }

  NcDbBlockReference* pBlockRef = NcDbBlockReference::cast(pObj);
  if (!pBlockRef)
  {
    ncutPrintf(L"\nОшибка: выбранный примитив не является блоком!");
    pObj->close();
    return;
  }  

  NcDbObjectId blockTableId = pBlockRef->blockTableRecord();
  NcDbBlockTableRecord* pBlockTable;
  if (ncdbOpenNcDbObject((NcDbObject*&)pBlockTable, blockTableId, NcDb::kForRead) != Nano::eOk)
  {
    ncutPrintf(L"\nОшибка: Не удалось открыть Block Table Record!");
    pBlockRef->close();
    return;
  }

  NcDbBlockTableRecordIterator* pIter;
  if (pBlockTable->newIterator(pIter) != Nano::eOk)
  {
    pBlockTable->close();
    pBlockRef->close();
    ncutPrintf(L"\nОшибка: Не удалось открыть Block Table Record Iterator!");
    return;
  }

  NCHAR* blockName;
  pBlockTable->getName(blockName);
  ncutPrintf(blockName);
  Manipulator manip;

  for (; !pIter->done(); pIter->step())
  {
    NcDbEntity* pEnt;
    if (pIter->getEntity(pEnt, AcDb::kForRead) == Acad::eOk)
    {
      NcDb3dPolyline* p3dPoly = NcDb3dPolyline::cast(pEnt);
      if (p3dPoly)
      {
        ncutPrintf(L"\nНайдена 3D полилиния в блоке:");

        NcDbObjectIterator* pVertIter = p3dPoly->vertexIterator();
        for (; !pVertIter->done(); pVertIter->step())
        {
          NcDb3dPolylineVertex* pVertex;
          if (ncdbOpenNcDbObject((NcDbObject*&)pVertex, pVertIter->objectId(), NcDb::kForRead) == Nano::eOk)
          {
            NcGePoint3d pos = pVertex->position();
            ncutPrintf(L"\nКоординаты вершины: X=%f, Y=%f, Z=%f", pos.x, pos.y, pos.z);
            pVertex->close();

            manip.addPointToSkelet(pos.x, pos.y, pos.z);
          }
        }
        delete pVertIter;
      }
      pEnt->close();
    }
  }

  delete pIter;
  pBlockTable->close();
  pBlockRef->close();

  NcGeMatrix3d ucsMatrix;
  if (ncedGetCurrentUCS(ucsMatrix) == Nano::eOk)
  {
    NcGeVector3d xAxis;
    NcGeVector3d yAxis;
    NcGeVector3d zAxis;
    NcGePoint3d  origin;
    ucsMatrix.getCoordSystem(origin, xAxis, yAxis, zAxis);

    manip.setOrigin(origin.x, origin.y, origin.z);

    std::wstring nameStr(blockName);
    manip.setName(nameStr);

    std::wstring filename, filepath;
    getFileNameAndPath(filename, filepath);
    ncutPrintf(filename.c_str());
    ncutPrintf(filepath.c_str());

    manip.setFileName(filename);
    manip.setFilePath(filepath);

    manip.writeToJSON();

    ncutPrintf(L"\nСистема координат модели (MCS):");
    ncutPrintf(L"\nOrigin: X=%.6f, Y=%.6f, Z=%.6f", origin.x, origin.y, origin.z);
    ncutPrintf(L"\nX-Axis: X=%.6f, Y=%.6f, Z=%.6f", xAxis.x, xAxis.y, xAxis.z);
    ncutPrintf(L"\nY-Axis: X=%.6f, Y=%.6f, Z=%.6f", yAxis.x, yAxis.y, yAxis.z);
    ncutPrintf(L"\nZ-Axis: X=%.6f, Y=%.6f, Z=%.6f", zAxis.x, zAxis.y, zAxis.z);
  }
  else
  {
    ncutPrintf(L"\nОшибка: Не удалось получить матрицу координат!");
  }
}

void testNrxCmd()
{
  ncutPrintf(L"Module is working!");
}

#pragma region NRXinit
void initApp()
{
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXTEST",
    L"MYNRXRTEST",
    ACRX_CMD_TRANSPARENT,
    testNrxCmd);
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXPLINE3DVERTEX",
    L"MYNRXPLINE3DVERTEX",
    ACRX_CMD_TRANSPARENT,
    getPolylineVertex);
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXOPENBLOCK",
    L"MYNRXOPENBLOCK",
    ACRX_CMD_TRANSPARENT,
    openPLineFromBlock);
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