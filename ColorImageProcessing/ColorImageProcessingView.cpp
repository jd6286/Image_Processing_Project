
// ColorImageProcessingView.cpp: CColorImageProcessingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcessing.h"
#endif

#include "ColorImageProcessingDoc.h"
#include "ColorImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageProcessingView

IMPLEMENT_DYNCREATE(CColorImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CColorImageProcessingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorImageProcessingView::OnEqualImage)
	ON_COMMAND(IDM_GRAYSCALE, &CColorImageProcessingView::OnGrayscale)
	ON_COMMAND(IDM_ADD_IMAGE, &CColorImageProcessingView::OnAddImage)
	ON_COMMAND(IDM_REVERSE_IMAGE, &CColorImageProcessingView::OnReverseImage)
	ON_COMMAND(IDM_BW_IMAGE_127, &CColorImageProcessingView::OnBwImage127)
	ON_COMMAND(IDM_BW_IMAGE_AVG, &CColorImageProcessingView::OnBwImageAvg)
	ON_COMMAND(IDM_CONTRAST_IMAGE, &CColorImageProcessingView::OnContrastImage)
	ON_COMMAND(IDM_CHANGE_SAT, &CColorImageProcessingView::OnChangeSat)
	ON_COMMAND(IDM_PICK_ORANGE, &CColorImageProcessingView::OnPickOrange)
	ON_COMMAND(IDM_PARA_CAP, &CColorImageProcessingView::OnParaCap)
	ON_COMMAND(IDM_PARA_CUP, &CColorImageProcessingView::OnParaCup)
	ON_COMMAND(IDM_GAMMA_IMAGE, &CColorImageProcessingView::OnGammaImage)
	ON_COMMAND(IDM_ZOOM_OUT, &CColorImageProcessingView::OnZoomOut)
	ON_COMMAND(IDM_ZOOM_IN_FWD, &CColorImageProcessingView::OnZoomInFwd)
	ON_COMMAND(IDM_ZOOM_IN_BWD, &CColorImageProcessingView::OnZoomInBwd)
	ON_COMMAND(IDM_ROTATE_FWD, &CColorImageProcessingView::OnRotateFwd)
	ON_COMMAND(IDM_ROTATE_BWD, &CColorImageProcessingView::OnRotateBwd)
	ON_COMMAND(IDM_MOVE_IMAGE, &CColorImageProcessingView::OnMoveImage)
	ON_COMMAND(IDM_SYMMETRY_LR, &CColorImageProcessingView::OnSymmetryLR)
	ON_COMMAND(IDM_SYMMETRY_UD, &CColorImageProcessingView::OnSymmetryUD)
	ON_COMMAND(IDM_HISTO_STRETCH, &CColorImageProcessingView::OnHistoStretch)
	ON_COMMAND(IDM_END_IN, &CColorImageProcessingView::OnEndIn)
	ON_COMMAND(IDM_HISTO_EQUAL, &CColorImageProcessingView::OnHistoEqual)
	ON_COMMAND(IDM_EMBOSSING, &CColorImageProcessingView::OnEmbossing)
	ON_COMMAND(IDM_BLURRING, &CColorImageProcessingView::OnBlurring)
	ON_COMMAND(IDM_GAUSSIAN, &CColorImageProcessingView::OnGaussian)
	ON_COMMAND(IDM_SHARPENING_MID9, &CColorImageProcessingView::OnSharpeningMid9)
	ON_COMMAND(IDM_SHARPENING_MID5, &CColorImageProcessingView::OnSharpeningMid5)
	ON_COMMAND(IDM_HIGH_FREQ, &CColorImageProcessingView::OnHighFreq)
	ON_COMMAND(IDM_EDGE_VERTICAL, &CColorImageProcessingView::OnEdgeVertical)
	ON_COMMAND(IDM_EDGE_HORIZONTAL, &CColorImageProcessingView::OnEdgeHorizontal)
	ON_COMMAND(IDM_EDGE_HOMOGEN, &CColorImageProcessingView::OnEdgeHomogen)
	ON_COMMAND(IDM_EDGE_SUBTRACT, &CColorImageProcessingView::OnEdgeSubtract)
	ON_COMMAND(IDM_EDGE_REBERTS_V, &CColorImageProcessingView::OnEdgeRebertsV)
	ON_COMMAND(IDM_EDGE_ROBERTS_H, &CColorImageProcessingView::OnEdgeRobertsH)
	ON_COMMAND(IDM_EDGE_ROBERTS, &CColorImageProcessingView::OnEdgeRoberts)
	ON_COMMAND(IDM_EDGE_PREWITT_V, &CColorImageProcessingView::OnEdgePrewittV)
	ON_COMMAND(IDM_EDGE_PREWITT_H, &CColorImageProcessingView::OnEdgePrewittH)
	ON_COMMAND(IDM_EDGE_PREWITT, &CColorImageProcessingView::OnEdgePrewitt)
	ON_COMMAND(IDM_EDGE_SOBEL_V, &CColorImageProcessingView::OnEdgeSobelV)
	ON_COMMAND(IDM_EDGE_SOBEL_H, &CColorImageProcessingView::OnEdgeSobelH)
	ON_COMMAND(IDM_EDGE_SOBEL, &CColorImageProcessingView::OnEdgeSobel)
	ON_COMMAND(IDM_LAPLACIAN_MID4, &CColorImageProcessingView::OnLaplacianMid4)
	ON_COMMAND(IDM_LAPLACIAN_MID8, &CColorImageProcessingView::OnLaplacianMid8)
	ON_COMMAND(IDM_LAPLACIAN_MINUS8, &CColorImageProcessingView::OnLaplacianMinus8)
	ON_COMMAND(IDM_EDGE_LOG, &CColorImageProcessingView::OnEdgeLoG)
	ON_COMMAND(IDM_EDGE_DOG, &CColorImageProcessingView::OnEdgeDoG)
	ON_COMMAND(IDM_EMBOSSING_HSI, &CColorImageProcessingView::OnEmbossingHSI)
	ON_COMMAND(IDM_CHROMA_KEY, &CColorImageProcessingView::OnChromaKey)
	ON_COMMAND(IDM_CHROMA_KEY_BLUR, &CColorImageProcessingView::OnChromaKeyBlur)
	ON_COMMAND(IDM_CHROMA_KEY_MID, &CColorImageProcessingView::OnChromaKeyMid)
	ON_COMMAND(IDM_CHROMA_KEY_AVG, &CColorImageProcessingView::OnChromaKeyAvg)
END_MESSAGE_MAP()

// CColorImageProcessingView 생성/소멸

CColorImageProcessingView::CColorImageProcessingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageProcessingView::~CColorImageProcessingView()
{
}

BOOL CColorImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageProcessingView 그리기

void CColorImageProcessingView::OnDraw(CDC* pDC)
{

	///////////////
	///////////////
	// 화면 크기 조절
	/////////////
	///////////////

	///////////////////
	/// ** 더블 버퍼링 **
	///////////////////
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/////////////////////
	/// 성능 개선을 위한 더블 버퍼링 
	////////////////////
	int i, k;
	unsigned char R, G, B;
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_inW, pDoc->m_inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_inW, pDoc->m_inH, WHITENESS); // 흰색으로 초기화


	// 출력 영상의 크기를 자동 조절
	double MAXSIZE = 800;  // 필요시 실 모니터 또는 화면의 해상도에 따라서 변경 가능!
	int inH = pDoc->m_inH;
	int inW = pDoc->m_inW;
	double hop = 1.0; // 기본

	if (inH > MAXSIZE || inW > MAXSIZE) {
		// hop을 새로 계산.
		if (inW > inH)
			hop = (inW / MAXSIZE);
		else
			hop = (inH / MAXSIZE);

		inW = (int)(inW / hop);
		inH = (int)(inH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < inH; i++) {
		for (k = 0; k < inW; k++) {
			R = pDoc->m_inImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_inImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_inImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}

	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->m_inW, pDoc->m_inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	///////////////////

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_outW, pDoc->m_outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_outW, pDoc->m_outH, WHITENESS); // 흰색으로 초기화

	int outH = pDoc->m_outH;
	int outW = pDoc->m_outW;
	hop = 1.0; // 기본

	if (outH > MAXSIZE || outW > MAXSIZE) {
		// hop을 새로 계산.
		if (outW > outH)
			hop = (outW / MAXSIZE);
		else
			hop = (outH / MAXSIZE);

		outW = (int)(outW / hop);
		outH = (int)(outH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < outH; i++) {
		for (k = 0; k < outW; k++) {
			R = pDoc->m_outImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_outImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_outImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CColorImageProcessingView 인쇄

BOOL CColorImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorImageProcessingView 진단

#ifdef _DEBUG
void CColorImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageProcessingDoc)));
	return (CColorImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CColorImageProcessingView 메시지 처리기


void CColorImageProcessingView::OnEqualImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnGrayscale()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGrayscale();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnAddImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAddImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnReverseImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnReverseImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnBwImage127()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBwImage127();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnBwImageAvg()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBwImageAvg();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnContrastImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnContrastImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChangeSat()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChangeSat();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnPickOrange()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPickOrange();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnParaCap()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnParaCap();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnParaCup()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnParaCup();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnGammaImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnZoomOut()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomOut();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnZoomInFwd()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomInFwd();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnZoomInBwd()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomInBwd();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnRotateFwd()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateFwd();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnRotateBwd()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateBwd();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMoveImage()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMoveImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnSymmetryLR()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSymmetryLR();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnSymmetryUD()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSymmetryUD();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnHistoStretch()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoStretch();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEndIn()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEndIn();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnHistoEqual()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoEqual();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEmbossing()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbossing();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnBlurring()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBlurring();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnGaussian()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGaussian();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnSharpeningMid9()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSharpeningMid9();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnSharpeningMid5()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSharpeningMid5();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnHighFreq()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHighFreq();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeVertical()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeVertical();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeHorizontal()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeHorizontal();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeHomogen()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeHomogen();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeSubtract()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeSubtract();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeRebertsV()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeRebertsV();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeRobertsH()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeRobertsH();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeRoberts()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeRoberts();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgePrewittV()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgePrewittV();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgePrewittH()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgePrewittH();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgePrewitt()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgePrewitt();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeSobelV()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeSobelV();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeSobelH()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeSobelH();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeSobel()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeSobel();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnLaplacianMid4()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacianMid4();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnLaplacianMid8()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacianMid8();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnLaplacianMinus8()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacianMinus8();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeLoG()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeLoG();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEdgeDoG()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeDoG();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEmbossingHSI()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbossingHSI();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChromaKey()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChromaKey();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChromaKeyBlur()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChromaKeyBlur();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChromaKeyMid()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChromaKeyMid();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChromaKeyAvg()
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChromaKeyAvg();
	Invalidate(TRUE);
}
