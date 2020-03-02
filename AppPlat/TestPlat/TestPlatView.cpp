
// TestPlatView.cpp : CTestPlatView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TestPlat.h"
#endif

#include "TestPlatDoc.h"
#include "TestPlatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestPlatView

IMPLEMENT_DYNCREATE(CTestPlatView, CView)

BEGIN_MESSAGE_MAP(CTestPlatView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestPlatView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTestPlatView ����/����

CTestPlatView::CTestPlatView()
{
	// TODO: �ڴ˴���ӹ������

}

CTestPlatView::~CTestPlatView()
{
}

BOOL CTestPlatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTestPlatView ����

void CTestPlatView::OnDraw(CDC* /*pDC*/)
{
	CTestPlatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTestPlatView ��ӡ


void CTestPlatView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestPlatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTestPlatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTestPlatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CTestPlatView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestPlatView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestPlatView ���

#ifdef _DEBUG
void CTestPlatView::AssertValid() const
{
	CView::AssertValid();
}

void CTestPlatView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestPlatDoc* CTestPlatView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestPlatDoc)));
	return (CTestPlatDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestPlatView ��Ϣ�������
