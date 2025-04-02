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

//void getPolylineVertex()
//{
//  nds_name polyline;
//  nds_point pt;
//  if (ncedEntSel(L"Выберите полилинию", polyline, pt) != RTNORM)
//  {
//    ncutPrintf(L"\nОшибка выбора!\n");
//    return;
//  }
//
//  NcDbObjectId plineId;
//  NcDbEntity* pEnt;
//  if (ncdbGetObjectId(plineId, polyline))
//  {
//    ncutPrintf(L"\nНе удалось получить id объекта!\n");
//    return;
//  }
//
//  if (ncdbOpenNcDbEntity(pEnt, plineId, NcDb::kForRead) != Nano::eOk)
//  {
//    ncutPrintf(L"\nНе удалось открыть entity!\n");
//    return;
//  }
//
//  if (NcDb3dPolyline* pline = NcDb3dPolyline::cast(pEnt))
//  {
//    NcDbObjectIterator* iter = pline->vertexIterator();
//    NcDbObjectIdArray vertexArray;
//    for (iter->start(); !iter->done(); iter->step())
//    {
//      vertexArray.append(iter->objectId());
//      NcDb3dPolylineVertex* pVertex;
//      if (ncdbOpenNcDbObject((NcDbObject*&)pVertex, iter->objectId(), NcDb::kForRead) == Nano::eOk)
//      {
//        NcGePoint3d pos = pVertex->position();
//        ncutPrintf(L"\nКоординаты вершины: X=%f, Y=%f, Z=%f", pos.x, pos.y, pos.z);
//        pVertex->close();
//      }
//      else
//      {
//        ncutPrintf(L"\nНе удалось получить координаты вершины!");
//      }
//    }
//    delete iter;
//  }
//  else
//  {
//    ncutPrintf(L"\nВыбранный примитив не является 3D полилинией!\n");
//    pEnt->close();
//    return;
//  }
//
//  AcGeMatrix3d ucsMatrix;
//
//  // Get the UCS transformation matrix
//  if (ncedGetCurrentUCS(ucsMatrix) == Nano::eOk)
//  {
//    // Extract coordinate system axes from the matrix
//    AcGeVector3d xAxis;  // X-Axis direction
//    AcGeVector3d yAxis;  // Y-Axis direction
//    AcGeVector3d zAxis;  // Z-Axis direction
//    AcGePoint3d  origin; // Origin of the UCS
//    ucsMatrix.getCoordSystem(origin, xAxis, yAxis, zAxis);
//
//    acutPrintf(_T("\nModel Coordinate System (MCS):"));
//    acutPrintf(_T("\nOrigin: X=%.6f, Y=%.6f, Z=%.6f"), origin.x, origin.y, origin.z);
//    acutPrintf(_T("\nX-Axis: X=%.6f, Y=%.6f, Z=%.6f"), xAxis.x, xAxis.y, xAxis.z);
//    acutPrintf(_T("\nY-Axis: X=%.6f, Y=%.6f, Z=%.6f"), yAxis.x, yAxis.y, yAxis.z);
//    acutPrintf(_T("\nZ-Axis: X=%.6f, Y=%.6f, Z=%.6f"), zAxis.x, zAxis.y, zAxis.z);
//  }
//  else
//  {
//    acutPrintf(_T("\nFailed to get UCS Matrix."));
//  }
//
//}