// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// GDAL_S57View.cpp : CGDAL_S57View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GDAL_S57.h"
#endif

#include "GDAL_S57Doc.h"
#include "GDAL_S57View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ogr_api.h"
#include "ogrsf_frmts.h"

// CGDAL_S57View

IMPLEMENT_DYNCREATE(CGDAL_S57View, CView)

BEGIN_MESSAGE_MAP(CGDAL_S57View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGDAL_S57View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CGDAL_S57View 생성/소멸

CGDAL_S57View::CGDAL_S57View()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	_putenv("RETURN_LINKAGES=ON,RETURN_PRIMITIVES=ON");
	CPLSetConfigOption("OGR_S57_OPTIONS", "RETURN_LINKAGES=ON,RETURN_PRIMITIVES=ON");

	OGRRegisterAll();
	
	CString encFilePathName("..\\KR4G1E30.000");
	OGRDataSource* pDS =  OGRSFDriverRegistrar::Open(_bstr_t(encFilePathName));
	
	int cnt = pDS->GetLayerCount();
	
	for (int i = 0; i < cnt; i++)
	{
		OGRLayer* layer = pDS->GetLayer(i);
		

		CString strLayerName;
		strLayerName = layer->GetName();

		if (strLayerName.Compare(_T("M_QUAL"))) continue;
		OutputDebugString(_bstr_t(layer->GetName()) + _T("\n"));
		OGRFeatureDefn* fDefn = layer->GetLayerDefn();

		layer->ResetReading();
		layer->GetFeatureCount();
		

		OGRFeature* fe = nullptr;
		while (fe = (layer->GetNextFeature()))
		{
			int fcnt = fe->GetFieldCount();
				
			for (int j = 0; j < fcnt; j++)
			{
				OGRFieldDefn* fd = fe->GetFieldDefnRef(j);
				OutputDebugString(_bstr_t(fd->GetNameRef()) + _T("\n"));
			}

			OGRGeometry* geo = fe->GetGeometryRef();
			OGRLineString* line = (OGRLineString*)geo;



		
		}
		
	}
	OGRDataSource::DestroyDataSource(pDS);

	m_bLMove = false;
}

CGDAL_S57View::~CGDAL_S57View()
{
	OGRCleanupAll();;
}

BOOL CGDAL_S57View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGDAL_S57View 그리기

void CGDAL_S57View::OnDraw(CDC* pDC)
{
	CGDAL_S57Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	pDC->Ellipse(m_ptCurrent.x - 5, m_ptCurrent.y - 5, m_ptCurrent.x + 5, m_ptCurrent.y + 5);
}


// CGDAL_S57View 인쇄


void CGDAL_S57View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGDAL_S57View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGDAL_S57View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGDAL_S57View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CGDAL_S57View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGDAL_S57View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGDAL_S57View 진단

#ifdef _DEBUG
void CGDAL_S57View::AssertValid() const
{
	CView::AssertValid();
}

void CGDAL_S57View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDAL_S57Doc* CGDAL_S57View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDAL_S57Doc)));
	return (CGDAL_S57Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDAL_S57View 메시지 처리기


void CGDAL_S57View::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ptCurrent = point;

	if (nFlags & MK_LBUTTON)
	{
		m_bLMove = true;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	}
	else
	{
		m_bLMove = false;
	}

	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}


BOOL CGDAL_S57View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLMove)
	{
		OutputDebugString(_T("A\n"));
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CGDAL_S57View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}


void CGDAL_S57View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}
