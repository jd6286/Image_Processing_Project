
// ColorImageProcessingView.h: CColorImageProcessingView 클래스의 인터페이스
//

#pragma once


class CColorImageProcessingView : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingView() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingView)

// 특성입니다.
public:
	CColorImageProcessingDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CColorImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualImage();
	afx_msg void OnGrayscale();
	afx_msg void OnAddImage();
	afx_msg void OnReverseImage();
	afx_msg void OnBwImage127();
	afx_msg void OnBwImageAvg();
	afx_msg void OnContrastImage();
	afx_msg void OnChangeSat();
	afx_msg void OnPickOrange();
	afx_msg void OnParaCap();
	afx_msg void OnParaCup();
	afx_msg void OnGammaImage();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomInFwd();
	afx_msg void OnZoomInBwd();
	afx_msg void OnRotateFwd();
	afx_msg void OnRotateBwd();
	afx_msg void OnMoveImage();
	afx_msg void OnSymmetryLR();
	afx_msg void OnSymmetryUD();
	afx_msg void OnHistoStretch();
	afx_msg void OnEndIn();
	afx_msg void OnHistoEqual();
	afx_msg void OnEmbossing();
	afx_msg void OnBlurring();
	afx_msg void OnGaussian();
	afx_msg void OnSharpeningMid9();
	afx_msg void OnSharpeningMid5();
	afx_msg void OnHighFreq();
	afx_msg void OnEdgeVertical();
	afx_msg void OnEdgeHorizontal();
	afx_msg void OnEdgeHomogen();
	afx_msg void OnEdgeSubtract();
	afx_msg void OnEdgeRebertsV();
	afx_msg void OnEdgeRobertsH();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnEdgePrewittV();
	afx_msg void OnEdgePrewittH();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeSobelV();
	afx_msg void OnEdgeSobelH();
	afx_msg void OnEdgeSobel();
	afx_msg void OnLaplacianMid4();
	afx_msg void OnLaplacianMid8();
	afx_msg void OnLaplacianMinus8();
	afx_msg void OnEdgeLoG();
	afx_msg void OnEdgeDoG();
	afx_msg void OnEmbossingHSI();
	afx_msg void OnChromaKey();
	afx_msg void OnChromaKeyBlur();
	afx_msg void OnChromaKeyMid();
	afx_msg void OnChromaKeyAvg();
};

#ifndef _DEBUG  // ColorImageProcessingView.cpp의 디버그 버전
inline CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const
   { return reinterpret_cast<CColorImageProcessingDoc*>(m_pDocument); }
#endif

