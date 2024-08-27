
// ColorImageProcessingDoc.h: CColorImageProcessingDoc 클래스의 인터페이스
//


#pragma once


class CColorImageProcessingDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingDoc() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CColorImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;
	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;
	int m_inH = 0;
	int m_inW = 0;
	int m_outH = 0;
	int m_outW = 0;
	unsigned char** OnMalloc2D(int h, int w);
	void OnFree2D(unsigned char** memory, int h);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	void OnFreeOutImage();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnEqualImage();
	void OnGrayscale();
	void OnAddImage();
	unsigned char OnClamping(double value);
	void OnReverseImage();
	void OnBwImage127();
	void OnBwImageAvg();
	void OnContrastImage();
	void OnChangeSat();
	double* RGB2HSI(int R, int G, int B);
	unsigned char* HSI2RGB(double H, double S, double I);
	void OnPickOrange();
	void OnParaCap();
	void OnParaCup();
	void OnGammaImage();
	void OnZoomOut();
	void OnZoomInFwd();
	void OnZoomInBwd();
	void OnRotateFwd();
	void OnRotateBwd();
	void OnMoveImage();
	void OnSymmetryLR();
	void OnSymmetryUD();
	void OnHistoStretch();
	int OnMaxVal(unsigned char** image, int h, int w);
	int OnMinVal(unsigned char** image, int h, int w);
	void OnEndIn();
	void OnHistoEqual();
	void OnEmbossing();
	double** OnMallocDouble2D(int h, int w);
	void OnFreeDouble2D(double** memory, int h);
	void OnBlurring();
	void OnGaussian();
	void OnSharpeningMid9();
	void OnSharpeningMid5();
	void OnHighFreq();
	void OnEdgeVertical();
	void OnEdgeHorizontal();
	void OnEdgeHomogen();
	double doubleAbs(double value);
	void OnEdgeSubtract();
	void OnEdgeRebertsV();
	void OnEdgeRobertsH();
	void OnEdgeRoberts();
	void OnEdgePrewittV();
	void OnEdgePrewittH();
	void OnEdgePrewitt();
	void OnEdgeSobelV();
	void OnEdgeSobelH();
	void OnEdgeSobel();
	void OnLaplacianMid4();
	void OnLaplacianMid8();
	void OnLaplacianMinus8();
	void OnEdgeLoG();
	void OnEdgeDoG();
	void OnEmbossingHSI();
	void OnChromaKey();
	void OnChromaKeyBlur();
	void OnChromaKeyMid();
	void OnChromaKeyAvg();
};
