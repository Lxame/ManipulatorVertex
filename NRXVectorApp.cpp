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

void helloNrxCmd();
void manipulatorVector();
void getPolylineVertex();
bool getFilePath(const NCHAR *&path);
bool getFileNameAndPath(std::wstring &name, std::wstring &path);

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

  AcGeMatrix3d ucsMatrix;

  // Get the UCS transformation matrix
  if (ncedGetCurrentUCS(ucsMatrix) == Nano::eOk)
  {
    // Extract coordinate system axes from the matrix
    AcGeVector3d xAxis;  // X-Axis direction
    AcGeVector3d yAxis;  // Y-Axis direction
    AcGeVector3d zAxis;  // Z-Axis direction
    AcGePoint3d  origin; // Origin of the UCS
    ucsMatrix.getCoordSystem(origin, xAxis, yAxis, zAxis);

    acutPrintf(_T("\nModel Coordinate System (MCS):"));
    acutPrintf(_T("\nOrigin: X=%.6f, Y=%.6f, Z=%.6f"), origin.x, origin.y, origin.z);
    acutPrintf(_T("\nX-Axis: X=%.6f, Y=%.6f, Z=%.6f"), xAxis.x, xAxis.y, xAxis.z);
    acutPrintf(_T("\nY-Axis: X=%.6f, Y=%.6f, Z=%.6f"), yAxis.x, yAxis.y, yAxis.z);
    acutPrintf(_T("\nZ-Axis: X=%.6f, Y=%.6f, Z=%.6f"), zAxis.x, zAxis.y, zAxis.z);
  }
  else
  {
    acutPrintf(_T("\nFailed to get UCS Matrix."));
  }

}

bool getFilePath(const NCHAR *&path)
{
  NcDbDatabase* pDb = ncdbHostApplicationServices()->workingDatabase();

  if (!pDb)
  {
    ncutPrintf(L"\nFailed to get the active database.");
    return false;
  }

  // Get full file path
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
  int pos = strFilePath.find_last_of('\\'); // Find last backslash for Windows paths
  if (pos != -1)
  {
    fileName = strFilePath.substr(pos + 1);
  }

  name = fileName;
  path = strFilePath;

  return true;
}

void openLineFromBlock() 
{
  AcDbObjectId entId;
  nds_name selObj;
  nds_point pt;

  if (ncedEntSel(L"\nSelect a Block Reference: ", selObj, pt) != RTNORM)
  {
    acutPrintf(L"\nNo entity selected.");
    return;
  }

  AcDbObject* pObj;

  if (ncdbGetObjectId(entId, selObj))
  {
    ncutPrintf(L"\nНе удалось получить id объекта!\n");
    return;
  }

  if (acdbOpenAcDbObject(pObj, entId, AcDb::kForRead) != Acad::eOk)
  {
    acutPrintf(_T("\nFailed to open entity."));
    return;
  }

  AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pObj);
  if (!pBlockRef)
  {
    acutPrintf(_T("\nSelected entity is not a Block Reference."));
    pObj->close();
    return;
  }
  

  // Get Block Table Record (definition of the block)
  AcDbObjectId blockTableId = pBlockRef->blockTableRecord();
  AcDbBlockTableRecord* pBlockTable;

  if (acdbOpenAcDbObject((AcDbObject*&)pBlockTable, blockTableId, AcDb::kForRead) != Acad::eOk)
  {
    acutPrintf(_T("\nFailed to open Block Table Record."));
    pBlockRef->close();
    return;
  }

  // Iterate through entities in the block
  AcDbBlockTableRecordIterator* pIter;
  if (pBlockTable->newIterator(pIter) != Acad::eOk)
  {
    pBlockTable->close();
    pBlockRef->close();
    return;
  }

  NCHAR* blockName;
  pBlockTable->getName(blockName);
  ncutPrintf(blockName);

  Manipulator manip;

  for (; !pIter->done(); pIter->step())
  {
    AcDbEntity* pEnt;
    if (pIter->getEntity(pEnt, AcDb::kForRead) == Acad::eOk)
    {
      AcDb3dPolyline* p3dPoly = AcDb3dPolyline::cast(pEnt);
      if (p3dPoly)
      {
        acutPrintf(_T("\nFound 3D Polyline in Block:"));

        // Iterate through vertices
        AcDbObjectIterator* pVertIter = p3dPoly->vertexIterator();
        for (; !pVertIter->done(); pVertIter->step())
        {
          AcDb3dPolylineVertex* pVertex;
          if (acdbOpenAcDbObject((AcDbObject*&)pVertex, pVertIter->objectId(), AcDb::kForRead) == Acad::eOk)
          {
            AcGePoint3d pos = pVertex->position();
            acutPrintf(_T("\nVertex at: X=%f, Y=%f, Z=%f"), pos.x, pos.y, pos.z);
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

  AcGeMatrix3d ucsMatrix;

  // Get the UCS transformation matrix
  if (ncedGetCurrentUCS(ucsMatrix) == Nano::eOk)
  {
    // Extract coordinate system axes from the matrix
    AcGeVector3d xAxis;  // X-Axis direction
    AcGeVector3d yAxis;  // Y-Axis direction
    AcGeVector3d zAxis;  // Z-Axis direction
    AcGePoint3d  origin; // Origin of the UCS
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

    acutPrintf(_T("\nModel Coordinate System (MCS):"));
    acutPrintf(_T("\nOrigin: X=%.6f, Y=%.6f, Z=%.6f"), origin.x, origin.y, origin.z);
    acutPrintf(_T("\nX-Axis: X=%.6f, Y=%.6f, Z=%.6f"), xAxis.x, xAxis.y, xAxis.z);
    acutPrintf(_T("\nY-Axis: X=%.6f, Y=%.6f, Z=%.6f"), yAxis.x, yAxis.y, yAxis.z);
    acutPrintf(_T("\nZ-Axis: X=%.6f, Y=%.6f, Z=%.6f"), zAxis.x, zAxis.y, zAxis.z);
  }
  else
  {
    acutPrintf(_T("\nFailed to get UCS Matrix."));
  }
}

void helloNrxCmd()
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
  ncedRegCmds->addCommand(L"MYNRXCOMMANDS_GROUP",
    L"_MYNRXOPENBLOCK",
    L"MYNRXOPENBLOCK",
    ACRX_CMD_TRANSPARENT,
    openLineFromBlock);
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