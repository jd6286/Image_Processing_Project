
// ColorImageProcessingDoc.cpp: CColorImageProcessingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcessing.h"
#endif

#include "ColorImageProcessingDoc.h"

#include <propkey.h>
#include "ConstantDlg.h"
#include "HeightWidth.h"
#include "HighLow.h"
#include <math.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorImageProcessingDoc

IMPLEMENT_DYNCREATE(CColorImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CColorImageProcessingDoc 생성/소멸

CColorImageProcessingDoc::CColorImageProcessingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageProcessingDoc::~CColorImageProcessingDoc()
{
}

BOOL CColorImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorImageProcessingDoc serialization

void CColorImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CColorImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColorImageProcessingDoc 진단

#ifdef _DEBUG
void CColorImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorImageProcessingDoc 명령


unsigned char** CColorImageProcessingDoc::OnMalloc2D(int h, int w)
{
	unsigned char** memory;
	memory = new unsigned char* [h];
	for (int i = 0; i < h; i++)
		memory[i] = new unsigned char[w];
	return memory;
}


void CColorImageProcessingDoc::OnFree2D(unsigned char** memory, int h)
{
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		delete memory[i];
	delete[] memory;
}


BOOL CColorImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 기존 메모리 해제
	if (m_inImageR != NULL) {
		OnFree2D(m_inImageR, m_inH);
		OnFree2D(m_inImageG, m_inH);
		OnFree2D(m_inImageB, m_inH);
		m_inImageR = m_inImageG = m_inImageB = NULL;
		m_inH = m_inW = 0;

		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}

	CImage image;
	image.Load(lpszPathName);
	// 입력 영상 크기 알아내기
	m_inH = image.GetHeight();
	m_inW = image.GetWidth();
	// 메모리 할당
	m_inImageR = OnMalloc2D(m_inH, m_inW);
	m_inImageG = OnMalloc2D(m_inH, m_inW);
	m_inImageB = OnMalloc2D(m_inH, m_inW);
	// CImage의 객체값 -> 메모리
	COLORREF px; //한 픽셀에 대한 정보 color reference
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			px = image.GetPixel(k, i); //입력받을 때는 반대로 입력받음
			m_inImageR[i][k] = GetRValue(px);
			m_inImageG[i][k] = GetGValue(px);
			m_inImageB[i][k] = GetBValue(px);
		}
	}

	return TRUE;
}


void CColorImageProcessingDoc::OnCloseDocument()
{
	OnFree2D(m_inImageR, m_inH);
	OnFree2D(m_inImageG, m_inH);
	OnFree2D(m_inImageB, m_inH);

	OnFree2D(m_outImageR, m_outH);
	OnFree2D(m_outImageG, m_outH);
	OnFree2D(m_outImageB, m_outH);

	CDocument::OnCloseDocument();
}


void CColorImageProcessingDoc::OnFreeOutImage()
{
	if (m_outImageR != NULL) {
		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}
}


BOOL CColorImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (m_outImageR == NULL)
		return FALSE;

	CImage image;
	image.Create(m_outW, m_outH, 32);//32비트형

	unsigned char R, G, B;
	COLORREF px;
	for (int i = 0; i < m_outW; i++) {
		for (int k = 0; k < m_outH; k++) {
			R = m_outImageR[k][i];
			G = m_outImageG[k][i];
			B = m_outImageB[k][i];
			px = RGB(R, G, B);
			image.SetPixel(i, k, px);
		}
	}
	image.Save(lpszPathName, Gdiplus::ImageFormatPNG);
	MessageBox(NULL, L"저장", L"성공", NULL);
	return TRUE;
}


void CColorImageProcessingDoc::OnEqualImage()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnGrayscale()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double avg;
	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
		}
	}
}

unsigned char CColorImageProcessingDoc::OnClamping(double value)
{
	unsigned char retVal;
	if (value > 255)
		value = 255;
	if (value < 0)
		value = 0;
	retVal = (unsigned char)value;
	return retVal;
}

void CColorImageProcessingDoc::OnAddImage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		double valueR, valueG, valueB;
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				valueR = m_inImageR[i][k] + dlg.m_constant;
				valueG = m_inImageG[i][k] + dlg.m_constant;
				valueB = m_inImageB[i][k] + dlg.m_constant;
				m_outImageR[i][k] = OnClamping(valueR);
				m_outImageG[i][k] = OnClamping(valueG);
				m_outImageB[i][k] = OnClamping(valueB);
			}
		}
	}
}

void CColorImageProcessingDoc::OnReverseImage()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - m_inImageR[i][k];
			m_outImageG[i][k] = 255 - m_inImageG[i][k];
			m_outImageB[i][k] = 255 - m_inImageB[i][k];
		}
	}
}



void CColorImageProcessingDoc::OnBwImage127()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 출력 이미지 영상 처리 <핵심 알고리즘>
	double avgRGB;
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			avgRGB = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			if (avgRGB > 128)
				avgRGB = 255;
			else
				avgRGB = 0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avgRGB;
		}
	}
}


void CColorImageProcessingDoc::OnBwImageAvg()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 출력 이미지 영상 처리 <핵심 알고리즘>
	double sum = 0;
	double avg;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			sum += (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
		}
	}
	avg = sum / (m_inH * m_inW);
	double avgRGB;
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			avgRGB = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			if (avgRGB > avg)
				avgRGB = 255;
			else
				avgRGB = 0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avgRGB;
		}
	}
}


void CColorImageProcessingDoc::OnContrastImage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		double valueR, valueG, valueB;
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				valueR = m_inImageR[i][k] * dlg.m_constant;
				valueG = m_inImageG[i][k] * dlg.m_constant;
				valueB = m_inImageB[i][k] * dlg.m_constant;
				m_outImageR[i][k] = OnClamping(valueR);
				m_outImageG[i][k] = OnClamping(valueG);
				m_outImageB[i][k] = OnClamping(valueB);
			}
		}
	}
}


void CColorImageProcessingDoc::OnChangeSat()
{
	//기존에 있던 코드
// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HSI 모델 값
			// H(색상) : 0 ~ 360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255 

			// RGB -> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];

			// 채도(S) 흐리게
			S -= 0.2;
			if (S < 0)
				S = 0.0;

			// HSI -> RGB
			unsigned char* rgb = HSI2RGB(H, S, I);
			R = rgb[0];
			G = rgb[1];
			B = rgb[2];

			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
		}
	}
}


double* CColorImageProcessingDoc::RGB2HSI(int R, int G, int B)
{
	double H, S, I;
	double* HSI = new double[3];
	double min_value, angle;
	I = (R + G + B) / 3.0; // 밝기
	if ((R == G) && (G == B)) { // 그레이
		S = 0.0;
		H = 0.0;
	}
	else {

		min_value = min(min(R, G), B); //최소값 추출
		angle = (R - 0.5 * G - 0.5 * B) / (double)sqrt((R - G) * (R - G) + (R - B) * (G - B));

		H = (double)acos(angle) * 57.29577951;
		S = 1.0f - (3.0 / (R + G + B)) * min_value;
	}
	if (B > G) H = 360. - H;

	HSI[0] = H;
	HSI[1] = S;
	HSI[2] = I;

	return HSI;
}


unsigned char* CColorImageProcessingDoc::HSI2RGB(double H, double S, double I)
{
	double R, G, B;
	unsigned char* RGB = new unsigned char[3];
	double angle1, angle2, scale;

	if (I == 0.0) { // Black
		RGB[0] = 0;
		RGB[1] = 0;
		RGB[2] = 0;
		return RGB;
	}

	if (H <= 0.0) H += 360.0f;

	scale = 3.0 * I;
	if (H <= 120.0)
	{
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		B = (1.0 - S) / 3.0f;
		R = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		G = 1.0 - R - B;
		B *= scale;
		R *= scale;
		G *= scale;
	}


	else if ((H > 120.0) && (H <= 240.0)) {
		H -= 120.0;
		angle1 = H * 0.017453293;

		angle2 = (60.0 - H) * 0.017453293;
		R = (1.0 - S) / 3.0;
		G = (double)(1.0f + (S * cos(angle1) / cos(angle2))) / 3.0;
		B = 1.0 - R - G;
		R *= scale;
		G *= scale;
		B *= scale;
	}
	else {
		H -= 240.0;
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		G = (1.0f - S) / 3.0;
		B = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		R = 1.0 - G - B;

		R *= scale;
		G *= scale;
		B *= scale;
	}

	RGB[0] = (unsigned char)R;
	RGB[1] = (unsigned char)G;
	RGB[2] = (unsigned char)B;
	return RGB;
}


void CColorImageProcessingDoc::OnPickOrange()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HSI 모델 값
			// H(색상) : 0 ~ 360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255 

			// RGB -> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];

			// 오렌지 추출 (H : 8~20)
			if (8 <= H && H <= 20) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
			}
		}
	}
}


void CColorImageProcessingDoc::OnParaCap()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	double valueR, valueG, valueB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			valueR = 255.0 - 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			valueG = 255.0 - 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			valueB = 255.0 - 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			m_outImageR[i][k] = OnClamping(valueR);
			m_outImageG[i][k] = OnClamping(valueG);
			m_outImageB[i][k] = OnClamping(valueB);
		}
	}
}


void CColorImageProcessingDoc::OnParaCup()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	double valueR, valueG, valueB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			valueR = 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			valueG = 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			valueB = 255.0 * pow((m_inImageR[i][k] / 127.0 - 1.0), 2);
			m_outImageR[i][k] = OnClamping(valueR);
			m_outImageG[i][k] = OnClamping(valueG);
			m_outImageB[i][k] = OnClamping(valueB);
		}
	}
}


void CColorImageProcessingDoc::OnGammaImage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 영상처리 알고리즘
		double valueR, valueG, valueB;
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				valueR = pow(m_inImageR[i][k], 1 / dlg.m_constant);
				valueG = pow(m_inImageG[i][k], 1 / dlg.m_constant);
				valueB = pow(m_inImageB[i][k], 1 / dlg.m_constant);
				m_outImageR[i][k] = OnClamping(valueR);
				m_outImageG[i][k] = OnClamping(valueG);
				m_outImageB[i][k] = OnClamping(valueB);
			}
		}
	}
}


void CColorImageProcessingDoc::OnZoomOut()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		int scale = (int)dlg.m_constant;
		m_outH = (int)(m_inH / scale);
		m_outW = (int)(m_inW / scale);

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[(int)(i / scale)][(int)(k / scale)] = m_inImageR[i][k];
				m_outImageG[(int)(i / scale)][(int)(k / scale)] = m_inImageG[i][k];
				m_outImageB[(int)(i / scale)][(int)(k / scale)] = m_inImageB[i][k];
			}
		}
	}
}


void CColorImageProcessingDoc::OnZoomInFwd()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		int scale = (int)dlg.m_constant;
		m_outH = (int)(m_inH * scale);
		m_outW = (int)(m_inW * scale);

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[(int)(i * scale)][(int)(k * scale)] = m_inImageR[i][k];
				m_outImageG[(int)(i * scale)][(int)(k * scale)] = m_inImageG[i][k];
				m_outImageB[(int)(i * scale)][(int)(k * scale)] = m_inImageB[i][k];
			}
		}
	}
}


void CColorImageProcessingDoc::OnZoomInBwd()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		int scale = (int)dlg.m_constant;
		m_outH = (int)(m_inH * scale);
		m_outW = (int)(m_inW * scale);

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				m_outImageR[i][k] = m_inImageR[(int)(i / scale)][(int)(k / scale)];
				m_outImageG[i][k] = m_inImageG[(int)(i / scale)][(int)(k / scale)];
				m_outImageB[i][k] = m_inImageB[(int)(i / scale)][(int)(k / scale)];
			}
		}
	}
}


void CColorImageProcessingDoc::OnRotateFwd()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		double degree = dlg.m_constant;
		double radian = degree * 3.141592 / 180.0;
		int cx = m_inH / 2;
		int cy = m_inW / 2;

		for (int xs = 0; xs < m_inH; xs++) {
			for (int ys = 0; ys < m_inW; ys++) {
				int xd = (int)(cos(radian) * (xs - cx) - sin(radian) * (ys - cy)); //  xd = cos*xs - sin*ys
				int yd = (int)(sin(radian) * (xs - cx) + cos(radian) * (ys - cy)); //  yd = sin*xs + cos*ys
				xd += cx;
				yd += cy;

				if ((0 <= xd && xd < m_outH) && (0 <= yd && yd < m_outW)) {
					m_outImageR[xd][yd] = m_inImageR[xs][ys];
					m_outImageG[xd][yd] = m_inImageG[xs][ys];
					m_outImageB[xd][yd] = m_inImageB[xs][ys];
				}
			}
		}
	}
}


void CColorImageProcessingDoc::OnRotateBwd()
{
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		double degree = dlg.m_constant;
		double radian = degree * 3.141592 / 180.0;
		int cx = m_inH / 2;
		int cy = m_inW / 2;

		for (int xd = 0; xd < m_outH; xd++) {
			for (int yd = 0; yd < m_outW; yd++) {
				int xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cy)); // xd = cos*xs - sin*ys
				int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cy)); // yd = sin*xs + cos*ys
				xs += cx;
				ys += cy;

				if ((0 <= xs && xs < m_outH) && (0 <= ys && ys < m_outW)) {
					m_outImageR[xd][yd] = m_inImageR[xs][ys];
					m_outImageG[xd][yd] = m_inImageG[xs][ys];
					m_outImageB[xd][yd] = m_inImageB[xs][ys];
				}
			}
		}
	}
}


void CColorImageProcessingDoc::OnMoveImage()
{
	CHeightWidth dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 출력 이미지 영상 처리 <핵심 알고리즘> 
		int moveH = dlg.m_height;
		int moveW = dlg.m_width;
		if (m_inH < moveH) moveH = 0;
		if (m_inW < moveW) moveW = 0;
		for (int i = moveH; i < m_inH; i++) {
			for (int k = moveW; k < m_inW; k++) {
				m_outImageR[i][k] = m_inImageR[i - moveH][k - moveW];
				m_outImageG[i][k] = m_inImageG[i - moveH][k - moveW];
				m_outImageB[i][k] = m_inImageB[i - moveH][k - moveW];
			}
		}
	}
}


void CColorImageProcessingDoc::OnSymmetryLR()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i][m_inW - k - 1];
			m_outImageG[i][k] = m_inImageG[i][m_inW - k - 1];
			m_outImageB[i][k] = m_inImageB[i][m_inW - k - 1];
		}
	}
}


void CColorImageProcessingDoc::OnSymmetryUD()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[m_inH - i - 1][k];
			m_outImageG[i][k] = m_inImageG[m_inH - i - 1][k];
			m_outImageB[i][k] = m_inImageB[m_inH - i - 1][k];
		}
	}
}


int CColorImageProcessingDoc::OnMaxVal(unsigned char** image, int h, int w)
{
	int high = image[0][0];
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			if (image[i][k] > high)
				high = image[i][k];
		}
	}
	return high;
}


int CColorImageProcessingDoc::OnMinVal(unsigned char** image, int h, int w)
{
	int low = image[0][0];
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			if (image[i][k] < low)
				low = image[i][k];
		}
	}
	return low;
}


void CColorImageProcessingDoc::OnHistoStretch()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 최대값, 최소값 -> 맨 첫 번째 값으로 초기화
	int highR = OnMaxVal(m_inImageR, m_inH, m_inW);
	int lowR = OnMinVal(m_inImageR, m_inH, m_inW);
	int highG = OnMaxVal(m_inImageG, m_inH, m_inW);
	int lowG = OnMinVal(m_inImageG, m_inH, m_inW);
	int highB = OnMaxVal(m_inImageB, m_inH, m_inW);
	int lowB = OnMinVal(m_inImageB, m_inH, m_inW);

	// val = (old - low) / (high - low) * 255.0
	int oldR, oldG, oldB;
	double valueR, valueG, valueB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			oldR = m_inImageR[i][k];
			oldG = m_inImageG[i][k];
			oldB = m_inImageB[i][k];
			valueR = (int)((double)(oldR - lowR) / (double)(highR - lowR) * 255.0);
			valueG = (int)((double)(oldG - lowG) / (double)(highG - lowG) * 255.0);
			valueB = (int)((double)(oldB - lowB) / (double)(highB - lowB) * 255.0);
			m_outImageR[i][k] = OnClamping(valueR);
			m_outImageG[i][k] = OnClamping(valueG);
			m_outImageB[i][k] = OnClamping(valueB);
		}
	}
}




void CColorImageProcessingDoc::OnEndIn()
{
	CHighLow dlg;
	if (dlg.DoModal() == IDOK) {
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정 -> 알고리즘에 따름
		m_outH = m_inH;
		m_outW = m_inW;

		//메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 최대값, 최소값 -> 맨 첫 번째 값으로 초기화
		int highR = OnClamping(OnMaxVal(m_inImageR, m_inH, m_inW) + dlg.m_high);
		int lowR = OnClamping(OnMinVal(m_inImageR, m_inH, m_inW) - dlg.m_low);
		int highG = OnClamping(OnMaxVal(m_inImageG, m_inH, m_inW) + dlg.m_high);
		int lowG = OnClamping(OnMinVal(m_inImageG, m_inH, m_inW) - dlg.m_low);
		int highB = OnClamping(OnMaxVal(m_inImageB, m_inH, m_inW) + dlg.m_high);
		int lowB = OnClamping(OnMinVal(m_inImageB, m_inH, m_inW) - dlg.m_low);

		// val = (old - low) / (high - low) * 255.0
		int oldR, oldG, oldB;
		double valueR, valueG, valueB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				oldR = m_inImageR[i][k];
				oldG = m_inImageG[i][k];
				oldB = m_inImageB[i][k];
				valueR = (int)((double)(oldR - lowR) / (double)(highR - lowR) * 255.0);
				valueG = (int)((double)(oldG - lowG) / (double)(highG - lowG) * 255.0);
				valueB = (int)((double)(oldB - lowB) / (double)(highB - lowB) * 255.0);
				m_outImageR[i][k] = OnClamping(valueR);
				m_outImageG[i][k] = OnClamping(valueG);
				m_outImageB[i][k] = OnClamping(valueB);
			}
		}
	}
}


void CColorImageProcessingDoc::OnHistoEqual()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 영상처리 알고리즘
	// 1단계 : 빈도수 세기(=히스토그램) histo[256]
	int histoR[256] = { 0, };
	int histoG[256] = { 0, };
	int histoB[256] = { 0, };
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			histoR[m_inImageR[i][k]]++;
			histoG[m_inImageG[i][k]]++;
			histoB[m_inImageB[i][k]]++;
		}
	// 2단계 : 누적히스토그램 생성
	int sumHistoR[256] = { 0, };
	int sumHistoG[256] = { 0, };
	int sumHistoB[256] = { 0, };
	sumHistoR[0] = histoR[0];
	sumHistoG[0] = histoG[0];
	sumHistoB[0] = histoB[0];
	for (int i = 1; i < 256; i++) {
		sumHistoR[i] = sumHistoR[i - 1] + histoR[i];
		sumHistoG[i] = sumHistoG[i - 1] + histoG[i];
		sumHistoB[i] = sumHistoB[i - 1] + histoB[i];
	}
	// 3단계 : 정규화된 히스토그램 생성  normalHisto = sumHisto * (1.0 / (inH*inW) ) * 255.0;
	double normalHistoR[256] = { 1.0, };
	double normalHistoG[256] = { 1.0, };
	double normalHistoB[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		normalHistoR[i] = sumHistoR[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoG[i] = sumHistoG[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoB[i] = sumHistoB[i] * (1.0 / (m_inH * m_inW)) * 255.0;
	}
	// 4단계 : inImage를 정규화된 값으로 치환
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = (unsigned char)normalHistoR[m_inImageR[i][k]];
			m_outImageG[i][k] = (unsigned char)normalHistoG[m_inImageG[i][k]];
			m_outImageB[i][k] = (unsigned char)normalHistoB[m_inImageB[i][k]];
		}
	}
}


double** CColorImageProcessingDoc::OnMallocDouble2D(int h, int w)
{
	double** retMemory;
	retMemory = new double* [h];
	for (int i = 0; i < h; i++)
		retMemory[i] = new double[w];
	return retMemory;
}


void CColorImageProcessingDoc::OnFreeDouble2D(double** memory, int h)
{
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		delete memory[i];
	delete memory;
}



void CColorImageProcessingDoc::OnEmbossing()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{-1.0, 0.0, 0.0},
		{ 0.0, 0.0, 0.0},
		{ 0.0, 0.0, 1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}	

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}
	
	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127.0;
			tmpOutImageG[i][k] += 127.0;
			tmpOutImageB[i][k] += 127.0;
		}
	}
		
	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}



void CColorImageProcessingDoc::OnEmbossingHSI()
{
	// 기존 메모리 해제
	OnFreeOutImage();
	// 중요! 출력 이미지 크기 결정 --> 알고리즘에 따름...
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	// ** 진짜 영상처리 알고리즘 **
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {  // 엠보싱 마스크
		{ -1.0, 0.0, 0.0 },
		{  0.0, 0.0, 0.0 },
		{  0.0, 0.0, 1.0 } };

	// 임시 메모리 할당
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);

	tmpInImageH = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	tmpInImageS = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	tmpInImageI = OnMallocDouble2D(m_inH + 2, m_inW + 2);

	tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 메모리 초기화 (127)
	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	// 입력메모리 --> 임시입력메모리
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}

	///////// RGB 모델 --> HSI 모델 ///////////////
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k]; G = tmpInImageG[i][k]; B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H; 
			tmpInImageS[i][k] = S; 
			tmpInImageI[i][k] = I;
		}
	}
	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.	
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double S_VALUE = 0.0;
			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S_VALUE += tmpInImageI[i + m][k + n] * mask[m][n];
			tmpInImageI[i][k] = S_VALUE;
		}
	}

	// 후처리 (마스크의 합계에 따라서 127을 더할지 결정)
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageI[i][k] += 127;
		}

	////// HSI --> RGB ////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; S = tmpInImageS[i][k]; I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			tmpOutImageR[i][k] = rgb[0]; tmpOutImageG[i][k] = rgb[1]; tmpOutImageB[i][k] = rgb[2];
		}


	// 임시 출력 이미지 ---> 출력 이미지
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}


	// 임시 메모리 해제
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpInImageH, m_inH + 2);
	OnFreeDouble2D(tmpInImageS, m_inH + 2);
	OnFreeDouble2D(tmpInImageI, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnBlurring()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 1. / 9, 1. / 9, 1. / 9},
						  { 1. / 9, 1. / 9, 1. / 9},
						  { 1. / 9, 1. / 9, 1. / 9} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnGaussian()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 1. / 16, 1. / 8, 1. / 16},
						  { 1. /  8, 1. / 4, 1. /  8},
						  { 1. / 16, 1. / 8, 1. / 16} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnSharpeningMid9()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { -1.0, -1.0, -1.0},
						  { -1.0,  9.0, -1.0},
						  { -1.0, -1.0, -1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnSharpeningMid5()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 0.0,-1.0, 0.0},
						  {-1.0, 5.0,-1.0},
						  { 0.0,-1.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnHighFreq()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { {-1. / 9,-1. / 9,-1. / 9},
						  {-1. / 9, 8. / 9,-1. / 9},
						  {-1. / 9,-1. / 9,-1. / 9} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeVertical()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 0.0, 0.0, 0.0},
						  {-1.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeHorizontal()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 0.0,-1.0, 0.0},
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double SumR, SumG, SumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			SumR = SumG = SumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					SumR += tmpInImageR[i + m][k + n] * mask[m][n];
					SumG += tmpInImageG[i + m][k + n] * mask[m][n];
					SumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = SumR;
			tmpOutImageG[i][k] = SumG;
			tmpOutImageB[i][k] = SumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


double CColorImageProcessingDoc::doubleAbs(double value)
{
	if (value >= 0) return value;
	else return -value;
}


void CColorImageProcessingDoc::OnEdgeHomogen()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 임시 입력 메모리 할당
	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double maxR, maxG, maxB;
	double valueR, valueG, valueB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			maxR = maxG = maxB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					valueR = doubleAbs(tmpInImageR[i + 1][k + 1] - tmpInImageR[i + m][k + n]);
					valueG = doubleAbs(tmpInImageR[i + 1][k + 1] - tmpInImageR[i + m][k + n]);
					valueB = doubleAbs(tmpInImageR[i + 1][k + 1] - tmpInImageR[i + m][k + n]);
					if (valueR >= maxR)
						maxR = valueR;
					if (valueG >= maxG)
						maxG = valueG;
					if (valueB >= maxB)
						maxB = valueB;
				}
			}
			tmpOutImageR[i][k] = maxR;
			tmpOutImageG[i][k] = maxG;
			tmpOutImageB[i][k] = maxB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}



void CColorImageProcessingDoc::OnEdgeSubtract()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 임시 입력 메모리 할당
	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double maxR, maxG, maxB;
	double maskR[4] = { 0, };
	double maskG[4] = { 0, };
	double maskB[4] = { 0, };
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			maxR = maxG = maxB = 0.0;
			maskR[0] = doubleAbs(tmpInImageR[i][k] - tmpInImageR[i + 2][k + 2]);
			maskR[1] = doubleAbs(tmpInImageR[i][k + 1] - tmpInImageR[i + 2][k + 1]);
			maskR[2] = doubleAbs(tmpInImageR[i][k + 2] - tmpInImageR[i + 2][k]);
			maskR[3] = doubleAbs(tmpInImageR[i + 1][k] - tmpInImageR[i + 1][k + 2]);
			maskG[0] = doubleAbs(tmpInImageG[i][k] - tmpInImageG[i + 2][k + 2]);
			maskG[1] = doubleAbs(tmpInImageG[i][k + 1] - tmpInImageG[i + 2][k + 1]);
			maskG[2] = doubleAbs(tmpInImageG[i][k + 2] - tmpInImageG[i + 2][k]);
			maskG[3] = doubleAbs(tmpInImageG[i + 1][k] - tmpInImageG[i + 1][k + 2]);
			maskB[0] = doubleAbs(tmpInImageB[i][k] - tmpInImageB[i + 2][k + 2]);
			maskB[1] = doubleAbs(tmpInImageB[i][k + 1] - tmpInImageB[i + 2][k + 1]);
			maskB[2] = doubleAbs(tmpInImageB[i][k + 2] - tmpInImageB[i + 2][k]);
			maskB[3] = doubleAbs(tmpInImageB[i + 1][k] - tmpInImageB[i + 1][k + 2]);
			for (int m = 0; m < 3; m++) {
				if (maskR[m] >= maxR) 
					maxR = maskR[m];
				if (maskG[m] >= maxG)
					maxG = maskG[m];
				if (maskB[m] >= maxB)
					maxB = maskB[m];
			}
			tmpOutImageR[i][k] = maxR;
			tmpOutImageG[i][k] = maxG;
			tmpOutImageB[i][k] = maxB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeRebertsV()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 0.0, 0.0,-1.0},
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeRobertsH()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { {-1.0, 0.0, 0.0},
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeRoberts()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double maskV[3][3] = { { 0.0, 0.0,-1.0},
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	double maskH[3][3] = { {-1.0, 0.0, 0.0},
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	double mask[3][3];
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			mask[i][k] = maskV[i][k] + maskH[i][k];
		}
	}

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgePrewittV()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 1.0, 0.0,-1.0},
						  { 1.0, 0.0,-1.0},
						  { 1.0, 0.0,-1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgePrewittH()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { {-1.0,-1.0,-1.0},
						  { 0.0, 0.0, 0.0},
						  { 1.0, 1.0, 1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgePrewitt()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double maskV[3][3] = { { 1.0, 0.0,-1.0},
						  { 1.0, 0.0,-1.0},
						  { 1.0, 0.0,-1.0} };
	double maskH[3][3] = { {-1.0,-1.0,-1.0},
						  { 0.0, 0.0, 0.0},
						  { 1.0, 1.0, 1.0} };
	double mask[3][3];
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			mask[i][k] = maskV[i][k] + maskH[i][k];
		}
	}

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeSobelV()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 1.0, 0.0,-1.0},
						  { 2.0, 0.0,-2.0},
						  { 1.0, 0.0,-1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeSobelH()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { {-1.0,-2.0,-1.0},
						  { 0.0, 0.0, 0.0},
						  { 1.0, 2.0, 1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeSobel()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double maskV[3][3] = { { 1.0, 0.0,-1.0},
						  { 2.0, 0.0,-2.0},
						  { 1.0, 0.0,-1.0} };
	double maskH[3][3] = { {-1.0,-2.0,-1.0},
						  { 0.0, 0.0, 0.0},
						  { 1.0, 2.0, 1.0} };
	double mask[3][3];
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			mask[i][k] = maskV[i][k] + maskH[i][k];
		}
	}

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnLaplacianMid4()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 0.0,-1.0, 0.0},
						  {-1.0, 4.0,-1.0},
						  { 0.0,-1.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnLaplacianMid8()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { -1.0,-1.0, -1.0},
						  { -1.0, 8.0, -1.0},
						  { -1.0,-1.0, -1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnLaplacianMinus8()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[3][3] = { { 1.0, 1.0, 1.0},
						  { 1.0,-8.0, 1.0},
						  { 1.0, 1.0, 1.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 2);
	OnFreeDouble2D(tmpInImageG, m_inH + 2);
	OnFreeDouble2D(tmpInImageB, m_inH + 2);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeLoG()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[5][5] = { { 0.0, 0.0, -1.0, 0.0, 0.0},
						  { 0.0,-1.0, -2.0,-1.0, 0.0},
						  {-1.0,-2.0, 16.0,-2.0,-1.0},
						  { 0.0,-1.0, -2.0,-1.0, 0.0},
						  { 0.0, 0.0, -1.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 4, m_inW + 4);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 4, m_inW + 4);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 4, m_inW + 4);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 4);
	OnFreeDouble2D(tmpInImageG, m_inH + 4);
	OnFreeDouble2D(tmpInImageB, m_inH + 4);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEdgeDoG()
{
	// 기존 메모리 해제
	OnFreeOutImage();

	// 출력 이미지 크기 결정 -> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;

	//메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	double mask[9][9] = { { 0.0, 0.0, 0.0,-1.0,-1.0,-1.0, 0.0, 0.0, 0.0},
						  { 0.0,-2.0,-3.0,-3.0,-3.0,-3.0,-3.0,-2.0, 0.0},
						  { 0.0,-3.0,-2.0,-1.0,-1.0,-1.0,-2.0,-3.0, 0.0},
						  { 0.0,-3.0,-1.0, 9.0, 9.0, 9.0,-1.0,-3.0, 0.0},
						  {-1.0,-3.0,-1.0, 9.0,19.0, 9.0,-1.0,-3.0,-1.0},
						  { 0.0,-3.0,-1.0, 9.0, 9.0, 9.0,-1.0,-3.0, 0.0},
						  { 0.0,-3.0,-2.0,-1.0,-1.0,-1.0,-2.0,-3.0, 0.0},
						  { 0.0,-2.0,-3.0,-3.0,-3.0,-3.0,-3.0,-2.0, 0.0},
						  { 0.0, 0.0, 0.0,-1.0,-1.0,-1.0, 0.0, 0.0, 0.0} };

	double** tmpInImageR = OnMallocDouble2D(m_inH + 8, m_inW + 8);
	double** tmpInImageG = OnMallocDouble2D(m_inH + 8, m_inW + 8);
	double** tmpInImageB = OnMallocDouble2D(m_inH + 8, m_inW + 8);
	double** tmpOutImageR = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageG = OnMallocDouble2D(m_outH, m_outW);
	double** tmpOutImageB = OnMallocDouble2D(m_outH, m_outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = 127;
			tmpInImageG[i][k] = 127;
			tmpInImageB[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 ***
	double sumR, sumG, sumB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
			sumR = sumG = sumB = 0.0;
			for (int m = 0; m < 9; m++) {
				for (int n = 0; n < 9; n++) {
					sumR += tmpInImageR[i + m][k + n] * mask[m][n];
					sumG += tmpInImageG[i + m][k + n] * mask[m][n];
					sumB += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = sumR;
			tmpOutImageG[i][k] = sumG;
			tmpOutImageB[i][k] = sumB;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
			m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
			m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
		}
	}
	OnFreeDouble2D(tmpInImageR, m_inH + 8);
	OnFreeDouble2D(tmpInImageG, m_inH + 8);
	OnFreeDouble2D(tmpInImageB, m_inH + 8);
	OnFreeDouble2D(tmpOutImageR, m_outH);
	OnFreeDouble2D(tmpOutImageG, m_outH);
	OnFreeDouble2D(tmpOutImageB, m_outH);
}


// 크로마키 알고리즘 (보정 X)
void CColorImageProcessingDoc::OnChromaKey()
{
	// 사용자에게 새로운 이미지를 선택하도록 함
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("이미지 파일 (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|모든 파일 (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) {
		
		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정
		m_outH = m_inH;
		m_outW = m_inW;

		// 출력 이미지 메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		//****************************************** 1. 배경 이미지 로드 ****************************************** 

		// 배경 이미지 불러오기
		CString newImagePath = dlg.GetPathName();
		CImage newImage;
		newImage.Load(newImagePath);

		// 이미지 크기 알아내기
		int newWidth = newImage.GetWidth();
		int newHeight = newImage.GetHeight();

		// 배경 이미지 메모리 할당
		double** newInImageR = OnMallocDouble2D(m_inH, m_inW);
		double** newInImageG = OnMallocDouble2D(m_inH, m_inW);
		double** newInImageB = OnMallocDouble2D(m_inH, m_inW);

		//****************************************** 2. 크기 조정(양선형 보간법) ******************************************

		// 이미지 크기에 따른 확대 또는 축소 비율 계산
		double scaleX = (double)m_outW / newWidth;
		double scaleY = (double)m_outH / newHeight;

		// 배경 이미지 크기 조정
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				// 보간을 통해 배경 이미지에서 해당 위치의 픽셀 값을 가져옴
				double x = k / scaleX;
				double y = i / scaleY;

				// 양선형 보간법을 적용하여 배경 이미지에서의 픽셀 값을 계산
				int x1 = (int)x;
				int y1 = (int)y;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				// 각 꼭지점의 색상 값 가져오기
				COLORREF p11 = newImage.GetPixel(x1, y1);
				COLORREF p12 = newImage.GetPixel(x1, y2);
				COLORREF p21 = newImage.GetPixel(x2, y1);
				COLORREF p22 = newImage.GetPixel(x2, y2);

				// 양선형 보간을 통해 새로운 픽셀 값 계산
				double weight1 = (x2 - x) * (y2 - y);
				double weight2 = (x - x1) * (y2 - y);
				double weight3 = (x2 - x) * (y - y1);
				double weight4 = (x - x1) * (y - y1);

				int r = (int)(GetRValue(p11) * weight1 + GetRValue(p21) * weight2 + GetRValue(p12) * weight3 + GetRValue(p22) * weight4);
				int g = (int)(GetGValue(p11) * weight1 + GetGValue(p21) * weight2 + GetGValue(p12) * weight3 + GetGValue(p22) * weight4);
				int b = (int)(GetBValue(p11) * weight1 + GetBValue(p21) * weight2 + GetBValue(p12) * weight3 + GetBValue(p22) * weight4);

				COLORREF px = RGB(r, g, b);

				newInImageR[i][k] = GetRValue(px);
				newInImageG[i][k] = GetGValue(px);
				newInImageB[i][k] = GetBValue(px);
			}
		}

		//****************************************** 3. 크로마키 알고리즘 ******************************************

		// 크로마키 범위 설정
		const int hue_green_min = 90; // 녹색 색상 최소값
		const int hue_green_max = 150; // 녹색 색상 최대값
		const double saturation_min = 0.3; // 채도 최소값

		// HSI 변환
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				double* hsi = RGB2HSI(m_inImageR[i][k], m_inImageG[i][k], m_inImageB[i][k]);
				double H = hsi[0];
				double S = hsi[1];
				double I = hsi[2];

				// 녹색 범위 내의 픽셀을 배경 이미지로 대입
				if (H >= hue_green_min && H <= hue_green_max &&
					S >= saturation_min) {
					// 이미지 범위를 벗어나는 경우 무시
					if (i < newHeight && k < newWidth) {
						// 배경 이미지의 RGB 값을 가져와서 대입
						m_outImageR[i][k] = newInImageR[i][k];
						m_outImageG[i][k] = newInImageG[i][k];
						m_outImageB[i][k] = newInImageB[i][k];
					}
				}
				else {
					// 녹색 범위 외의 픽셀은 원본 이미지를 유지
					m_outImageR[i][k] = m_inImageR[i][k];
					m_outImageG[i][k] = m_inImageG[i][k];
					m_outImageB[i][k] = m_inImageB[i][k];
				}

				delete[] hsi; // 동적으로 할당된 HSI 메모리 해제
			}
		}

		// 배경 이미지 메모리 할당 해제
		OnFreeDouble2D(newInImageR, m_inH);
		OnFreeDouble2D(newInImageG, m_inH);
		OnFreeDouble2D(newInImageB, m_inH);
	}
}

// 크로마키 보정 알고리즘 (블러링 필터)
void CColorImageProcessingDoc::OnChromaKeyBlur()
{
	// 사용자에게 새로운 이미지를 선택하도록 함
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("이미지 파일 (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|모든 파일 (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) {

		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정
		m_outH = m_inH;
		m_outW = m_inW;

		// 메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		//****************************************** 1. 배경 이미지 로드, 크기 조정 ****************************************** 
		// 배경 이미지 불러오기
		CString newImagePath = dlg.GetPathName();
		CImage newImage;
		newImage.Load(newImagePath);

		// 이미지 크기 알아내기
		int newWidth = newImage.GetWidth();
		int newHeight = newImage.GetHeight();

		// 배경 이미지 메모리 할당
		double** newImageR = OnMallocDouble2D(m_inH, m_inW);
		double** newImageG = OnMallocDouble2D(m_inH, m_inW);
		double** newImageB = OnMallocDouble2D(m_inH, m_inW);

		// 이미지 크기에 따른 확대 또는 축소 비율 계산
		double scaleX = (double)m_outW / newWidth;
		double scaleY = (double)m_outH / newHeight;

		// 양선형 보간법을 사용하여 출력 이미지 생성
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				// 보간을 통해 새로운 이미지에서 해당 위치의 픽셀 값을 가져옴
				double x = k / scaleX;
				double y = i / scaleY;

				// 양선형 보간법을 적용하여 새로운 이미지에서의 픽셀 값을 계산
				int x1 = (int)x;
				int y1 = (int)y;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				// 각 꼭지점의 색상 값 가져오기
				COLORREF p11 = newImage.GetPixel(x1, y1);
				COLORREF p12 = newImage.GetPixel(x1, y2);
				COLORREF p21 = newImage.GetPixel(x2, y1);
				COLORREF p22 = newImage.GetPixel(x2, y2);

				// 양선형 보간을 통해 새로운 픽셀 값 계산
				double weight1 = (x2 - x) * (y2 - y);
				double weight2 = (x - x1) * (y2 - y);
				double weight3 = (x2 - x) * (y - y1);
				double weight4 = (x - x1) * (y - y1);

				int r = (int)(GetRValue(p11) * weight1 + GetRValue(p21) * weight2 + GetRValue(p12) * weight3 + GetRValue(p22) * weight4);
				int g = (int)(GetGValue(p11) * weight1 + GetGValue(p21) * weight2 + GetGValue(p12) * weight3 + GetGValue(p22) * weight4);
				int b = (int)(GetBValue(p11) * weight1 + GetBValue(p21) * weight2 + GetBValue(p12) * weight3 + GetBValue(p22) * weight4);

				COLORREF px = RGB(r, g, b);

				newImageR[i][k] = GetRValue(px);
				newImageG[i][k] = GetGValue(px);
				newImageB[i][k] = GetBValue(px);
			}
		}

		//****************************************** 2. 크로마키 알고리즘 ****************************************** 

		// 임시 메모리 할당
		// tmpImage -> 에지 검출 연산을 위한 배열
		// tmpOutImage -> 최종 연산을 위한 배열
		double** tmpImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageB = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageB = OnMallocDouble2D(m_inH, m_inW);

		// 크로마키 범위 설정
		const int hue_green_min = 90; // 녹색 색상 최소값
		const int hue_green_max = 150; // 녹색 색상 최대값
		const double saturation_min = 0.3; // 채도 최소값

		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				double* hsi = RGB2HSI(m_inImageR[i][k], m_inImageG[i][k], m_inImageB[i][k]);
				double H = hsi[0];
				double S = hsi[1];
				double I = hsi[2];

				// 녹색 범위 내의 픽셀을 배경 이미지로 대입
				if (H >= hue_green_min && H <= hue_green_max &&
					S >= saturation_min) {
					// 이미지 범위를 벗어나는 경우 무시
					if (i < newHeight && k < newWidth) {
						// 배경 이미지의 RGB 값을 가져와서 대입
						// tmpImage -> 에지 검출을 위해 배경 픽셀 제거
						// tmpOutImage -> 배경 이미지 대입(노이즈 있음)
						tmpImageR[i][k] = 0;
						tmpImageG[i][k] = 0;
						tmpImageB[i][k] = 0;
						tmpOutImageR[i][k] = newImageR[i][k];
						tmpOutImageG[i][k] = newImageG[i][k];
						tmpOutImageB[i][k] = newImageB[i][k];
					}
				}
				else {
					// 녹색 범위 외의 픽셀은 원본 이미지를 유지
					tmpImageR[i][k] = m_inImageR[i][k];
					tmpImageG[i][k] = m_inImageG[i][k];
					tmpImageB[i][k] = m_inImageB[i][k];
					tmpOutImageR[i][k] = m_inImageR[i][k];
					tmpOutImageG[i][k] = m_inImageG[i][k];
					tmpOutImageB[i][k] = m_inImageB[i][k];
				}

				delete[] hsi; // 동적으로 할당된 메모리 해제
			}
		}

		//****************************************** 3. 이진화 ****************************************** 
		// 에지 검출을 위해 배경과 객체를 이진화
		// 배경이 제거된 tmpImage에 대해 적용
		double sum = 0;
		double avg;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				sum += (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
			}
		}
		avg = sum / (m_inH * m_inW);
		double avgRGB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				avgRGB = (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
				if (avgRGB > avg)
					avgRGB = 255;
				else
					avgRGB = 0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avgRGB;
			}
		}

		////****************************************** 4. 에지 검출 ****************************************** 
		// 라플라시안 에지 기법 적용
		double mask[3][3] = { { 0.0,-1.0, 0.0},
						  {-1.0, 4.0,-1.0},
						  { 0.0,-1.0, 0.0} };

		// 회선 연산을 위한 메모리 할당
		double** edgeInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeOutImageR = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageG = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageB = OnMallocDouble2D(m_outH, m_outW);

		// 임시 입력 메모리를 초기화(127) : 필요시 평균값
		for (int i = 0; i < m_inH + 2; i++) {
			for (int k = 0; k < m_inW + 2; k++) {
				edgeInImageR[i][k] = 127;
				edgeInImageG[i][k] = 127;
				edgeInImageB[i][k] = 127;
			}
		}

		// 입력 이미지 --> 임시 입력 이미지
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				edgeInImageR[i + 1][k + 1] = tmpImageR[i][k];
				edgeInImageG[i + 1][k + 1] = tmpImageG[i][k];
				edgeInImageB[i + 1][k + 1] = tmpImageB[i][k];
			}
		}

		// *** 회선 연산 ***
		double sumR, sumG, sumB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
				// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
				sumR = sumG = sumB = 0.0;
				for (int m = 0; m < 3; m++) {
					for (int n = 0; n < 3; n++) {
						sumR += edgeInImageR[i + m][k + n] * mask[m][n];
						sumG += edgeInImageG[i + m][k + n] * mask[m][n];
						sumB += edgeInImageB[i + m][k + n] * mask[m][n];
					}
				}
				edgeOutImageR[i][k] = sumR;
				edgeOutImageG[i][k] = sumG;
				edgeOutImageB[i][k] = sumB;
			}
		}

		// 임시 출력 영상--> 출력 영상. 
		// tmpImage에 검출한 에지 정보를 저장
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				tmpImageR[i][k] = OnClamping(edgeOutImageR[i][k]);
				tmpImageG[i][k] = OnClamping(edgeOutImageG[i][k]);
				tmpImageB[i][k] = OnClamping(edgeOutImageB[i][k]);
			}
		}

		////****************************************** 5. 에지 라인 보정 ******************************************

		// 검출한 경계선 영역을 따라 배경 이미지 대입
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					tmpOutImageR[i][k] = newImageR[i][k];
					tmpOutImageG[i][k] = newImageG[i][k];
					tmpOutImageB[i][k] = newImageB[i][k];
				}
			}
		}

		// 블러링 기법 적용
		double maskB[3][3] = { { 1. / 9, 1. / 9, 1. / 9},
						  { 1. / 9, 1. / 9, 1. / 9},
						  { 1. / 9, 1. / 9, 1. / 9} };

		// 임시 입력 메모리를 초기화(127) : 필요시 평균값
		for (int i = 0; i < m_inH + 2; i++) {
			for (int k = 0; k < m_inW + 2; k++) {
				edgeInImageR[i][k] = 127;
				edgeInImageG[i][k] = 127;
				edgeInImageB[i][k] = 127;
			}
		}

		// 입력 이미지 --> 임시 입력 이미지
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				edgeInImageR[i + 1][k + 1] = tmpOutImageR[i][k];
				edgeInImageG[i + 1][k + 1] = tmpOutImageG[i][k];
				edgeInImageB[i + 1][k + 1] = tmpOutImageB[i][k];
			}
		}

		// *** 회선 연산 ***
		// 검출한 경계선 영역에 한해서 보정 적용
		double sumBR, sumBG, sumBB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					sumBR = sumBG = sumBB = 0.0;
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							sumBR += edgeInImageR[i + m][k + n] * maskB[m][n];
							sumBG += edgeInImageG[i + m][k + n] * maskB[m][n];
							sumBB += edgeInImageB[i + m][k + n] * maskB[m][n];
						}
					}
					edgeOutImageR[i][k] = sumBR;
					edgeOutImageG[i][k] = sumBG;
					edgeOutImageB[i][k] = sumBB;
				}
				else {
					edgeOutImageR[i][k] = tmpOutImageR[i][k];
					edgeOutImageG[i][k] = tmpOutImageG[i][k];
					edgeOutImageB[i][k] = tmpOutImageB[i][k];
				}

			}
		}


		// 보정 이후 클램핑 적용
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[i][k] = OnClamping(edgeOutImageR[i][k]);
				m_outImageG[i][k] = OnClamping(edgeOutImageG[i][k]);
				m_outImageB[i][k] = OnClamping(edgeOutImageB[i][k]);
			}
		}


		// 동적 메모리 할당 해제
		OnFreeDouble2D(edgeInImageR, m_inH + 2);
		OnFreeDouble2D(edgeInImageG, m_inH + 2);
		OnFreeDouble2D(edgeInImageB, m_inH + 2);
		OnFreeDouble2D(edgeOutImageR, m_outH);
		OnFreeDouble2D(edgeOutImageG, m_outH);
		OnFreeDouble2D(edgeOutImageB, m_outH);

		OnFreeDouble2D(newImageR, m_inH);
		OnFreeDouble2D(newImageG, m_inH);
		OnFreeDouble2D(newImageB, m_inH);
		OnFreeDouble2D(tmpImageR, m_inH);
		OnFreeDouble2D(tmpImageG, m_inH);
		OnFreeDouble2D(tmpImageB, m_inH);

	}
}

// 크로마키 보정 알고리즘(중간값 필터)
void CColorImageProcessingDoc::OnChromaKeyMid()
{
	// 사용자에게 새로운 이미지를 선택하도록 함
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("이미지 파일 (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|모든 파일 (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) {

		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정
		m_outH = m_inH;
		m_outW = m_inW;

		// 메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		//****************************************** 1. 배경 이미지 로드, 크기 조정 ****************************************** 
		// 배경 이미지 불러오기
		CString newImagePath = dlg.GetPathName();
		CImage newImage;
		newImage.Load(newImagePath);

		// 이미지 크기 알아내기
		int newWidth = newImage.GetWidth();
		int newHeight = newImage.GetHeight();

		// 배경 이미지 메모리 할당
		double** newImageR = OnMallocDouble2D(m_inH, m_inW);
		double** newImageG = OnMallocDouble2D(m_inH, m_inW);
		double** newImageB = OnMallocDouble2D(m_inH, m_inW);

		// 이미지 크기에 따른 확대 또는 축소 비율 계산
		double scaleX = (double)m_outW / newWidth;
		double scaleY = (double)m_outH / newHeight;

		// 양선형 보간법을 사용하여 출력 이미지 생성
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				// 보간을 통해 새로운 이미지에서 해당 위치의 픽셀 값을 가져옴
				double x = k / scaleX;
				double y = i / scaleY;

				// 양선형 보간법을 적용하여 새로운 이미지에서의 픽셀 값을 계산
				int x1 = (int)x;
				int y1 = (int)y;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				// 각 꼭지점의 색상 값 가져오기
				COLORREF p11 = newImage.GetPixel(x1, y1);
				COLORREF p12 = newImage.GetPixel(x1, y2);
				COLORREF p21 = newImage.GetPixel(x2, y1);
				COLORREF p22 = newImage.GetPixel(x2, y2);

				// 양선형 보간을 통해 새로운 픽셀 값 계산
				double weight1 = (x2 - x) * (y2 - y);
				double weight2 = (x - x1) * (y2 - y);
				double weight3 = (x2 - x) * (y - y1);
				double weight4 = (x - x1) * (y - y1);

				int r = (int)(GetRValue(p11) * weight1 + GetRValue(p21) * weight2 + GetRValue(p12) * weight3 + GetRValue(p22) * weight4);
				int g = (int)(GetGValue(p11) * weight1 + GetGValue(p21) * weight2 + GetGValue(p12) * weight3 + GetGValue(p22) * weight4);
				int b = (int)(GetBValue(p11) * weight1 + GetBValue(p21) * weight2 + GetBValue(p12) * weight3 + GetBValue(p22) * weight4);

				COLORREF px = RGB(r, g, b);

				newImageR[i][k] = GetRValue(px);
				newImageG[i][k] = GetGValue(px);
				newImageB[i][k] = GetBValue(px);
			}
		}

		//****************************************** 2. 크로마키 알고리즘 ****************************************** 

		// 임시 메모리 할당
		// tmpImage -> 에지 검출 연산을 위한 배열
		// tmpOutImage -> 최종 연산을 위한 배열
		double** tmpImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageB = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageB = OnMallocDouble2D(m_inH, m_inW);

		// 크로마키 범위 설정
		const int hue_green_min = 90; // 녹색 색상 최소값
		const int hue_green_max = 150; // 녹색 색상 최대값
		const double saturation_min = 0.3; // 채도 최소값

		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				double* hsi = RGB2HSI(m_inImageR[i][k], m_inImageG[i][k], m_inImageB[i][k]);
				double H = hsi[0];
				double S = hsi[1];
				double I = hsi[2];

				// 녹색 범위 내의 픽셀을 배경 이미지로 대입
				if (H >= hue_green_min && H <= hue_green_max &&
					S >= saturation_min) {
					// 이미지 범위를 벗어나는 경우 무시
					if (i < newHeight && k < newWidth) {
						// 배경 이미지의 RGB 값을 가져와서 대입
						// tmpImage -> 에지 검출을 위해 배경 픽셀 제거
						// tmpOutImage -> 배경 이미지 대입(노이즈 있음)
						tmpImageR[i][k] = 0;
						tmpImageG[i][k] = 0;
						tmpImageB[i][k] = 0;
						tmpOutImageR[i][k] = newImageR[i][k];
						tmpOutImageG[i][k] = newImageG[i][k];
						tmpOutImageB[i][k] = newImageB[i][k];
					}
				}
				else {
					// 녹색 범위 외의 픽셀은 원본 이미지를 유지
					tmpImageR[i][k] = m_inImageR[i][k];
					tmpImageG[i][k] = m_inImageG[i][k];
					tmpImageB[i][k] = m_inImageB[i][k];
					tmpOutImageR[i][k] = m_inImageR[i][k];
					tmpOutImageG[i][k] = m_inImageG[i][k];
					tmpOutImageB[i][k] = m_inImageB[i][k];
				}

				delete[] hsi; // 동적으로 할당된 메모리 해제
			}
		}

		//****************************************** 3. 이진화 ****************************************** 
		// 에지 검출을 위해 배경과 객체를 이진화
		// 배경이 제거된 tmpImage에 대해 적용
		double sum = 0;
		double avg;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				sum += (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
			}
		}
		avg = sum / (m_inH * m_inW);
		double avgRGB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				avgRGB = (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
				if (avgRGB > avg)
					avgRGB = 255;
				else
					avgRGB = 0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avgRGB;
			}
		}

		////****************************************** 4. 에지 검출 ****************************************** 
		// 라플라시안 에지 기법 적용
		double mask[3][3] = { { 0.0,-1.0, 0.0},
						  {-1.0, 4.0,-1.0},
						  { 0.0,-1.0, 0.0} };

		// 회선 연산을 위한 메모리 할당
		double** edgeInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeOutImageR = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageG = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageB = OnMallocDouble2D(m_outH, m_outW);

		// 임시 입력 메모리를 초기화(127) : 필요시 평균값
		for (int i = 0; i < m_inH + 2; i++) {
			for (int k = 0; k < m_inW + 2; k++) {
				edgeInImageR[i][k] = 127;
				edgeInImageG[i][k] = 127;
				edgeInImageB[i][k] = 127;
			}
		}

		// 입력 이미지 --> 임시 입력 이미지
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				edgeInImageR[i + 1][k + 1] = tmpImageR[i][k];
				edgeInImageG[i + 1][k + 1] = tmpImageG[i][k];
				edgeInImageB[i + 1][k + 1] = tmpImageB[i][k];
			}
		}

		// *** 회선 연산 ***
		double sumR, sumG, sumB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
				// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
				sumR = sumG = sumB = 0.0;
				for (int m = 0; m < 3; m++) {
					for (int n = 0; n < 3; n++) {
						sumR += edgeInImageR[i + m][k + n] * mask[m][n];
						sumG += edgeInImageG[i + m][k + n] * mask[m][n];
						sumB += edgeInImageB[i + m][k + n] * mask[m][n];
					}
				}
				edgeOutImageR[i][k] = sumR;
				edgeOutImageG[i][k] = sumG;
				edgeOutImageB[i][k] = sumB;
			}
		}

		// 임시 출력 영상--> 출력 영상. 
		// tmpImage에 검출한 에지 정보를 저장
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				tmpImageR[i][k] = OnClamping(edgeOutImageR[i][k]);
				tmpImageG[i][k] = OnClamping(edgeOutImageG[i][k]);
				tmpImageB[i][k] = OnClamping(edgeOutImageB[i][k]);
			}
		}

		////****************************************** 5. 에지 라인 보정 ******************************************

		// 검출한 경계선 영역을 따라 배경 이미지 대입
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					tmpOutImageR[i][k] = newImageR[i][k];
					tmpOutImageG[i][k] = newImageG[i][k];
					tmpOutImageB[i][k] = newImageB[i][k];
				}
			}
		}

		// 중간값 필터링 알고리즘
		for (int i = 1; i < m_inH - 1; i++) {
			for (int k = 1; k < m_inW - 1; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					// 주변 3x3 픽셀 값 가져오기
					unsigned char mR[9], mG[9], mB[9];
					int index = 0;
					for (int x = -1; x <= 1; x++) {
						for (int y = -1; y <= 1; y++) {
							mR[index] = tmpOutImageR[i + x][k + y];
							mG[index] = tmpOutImageG[i + x][k + y];
							mB[index] = tmpOutImageB[i + x][k + y];
							index++;
						}
					}
					// 중간값 찾기
					std::sort(mR, mR + 9);
					std::sort(mG, mG + 9);
					std::sort(mB, mB + 9);
					tmpOutImageR[i][k] = mR[4];
					tmpOutImageG[i][k] = mG[4];
					tmpOutImageB[i][k] = mB[4];
				}
			}
		}


		// 최종 보정 이후 클램핑 적용
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
				m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
				m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
			}
		}



		// 동적 메모리 할당 해제
		OnFreeDouble2D(edgeInImageR, m_inH + 2);
		OnFreeDouble2D(edgeInImageG, m_inH + 2);
		OnFreeDouble2D(edgeInImageB, m_inH + 2);
		OnFreeDouble2D(edgeOutImageR, m_outH);
		OnFreeDouble2D(edgeOutImageG, m_outH);
		OnFreeDouble2D(edgeOutImageB, m_outH);

		OnFreeDouble2D(newImageR, m_inH);
		OnFreeDouble2D(newImageG, m_inH);
		OnFreeDouble2D(newImageB, m_inH);
		OnFreeDouble2D(tmpImageR, m_inH);
		OnFreeDouble2D(tmpImageG, m_inH);
		OnFreeDouble2D(tmpImageB, m_inH);

	}
}

// 크로마키 보정 알고리즘(평균값 필터)
void CColorImageProcessingDoc::OnChromaKeyAvg()
{
	// 사용자에게 새로운 이미지를 선택하도록 함
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("이미지 파일 (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|모든 파일 (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) {

		// 기존 메모리 해제
		OnFreeOutImage();

		// 출력 이미지 크기 결정
		m_outH = m_inH;
		m_outW = m_inW;

		// 메모리 할당
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		//****************************************** 1. 배경 이미지 로드, 크기 조정 ****************************************** 
		// 배경 이미지 불러오기
		CString newImagePath = dlg.GetPathName();
		CImage newImage;
		newImage.Load(newImagePath);

		// 이미지 크기 알아내기
		int newWidth = newImage.GetWidth();
		int newHeight = newImage.GetHeight();

		// 배경 이미지 메모리 할당
		double** newImageR = OnMallocDouble2D(m_inH, m_inW);
		double** newImageG = OnMallocDouble2D(m_inH, m_inW);
		double** newImageB = OnMallocDouble2D(m_inH, m_inW);

		// 이미지 크기에 따른 확대 또는 축소 비율 계산
		double scaleX = (double)m_outW / newWidth;
		double scaleY = (double)m_outH / newHeight;

		// 양선형 보간법을 사용하여 출력 이미지 생성
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				// 보간을 통해 새로운 이미지에서 해당 위치의 픽셀 값을 가져옴
				double x = k / scaleX;
				double y = i / scaleY;

				// 양선형 보간법을 적용하여 새로운 이미지에서의 픽셀 값을 계산
				int x1 = (int)x;
				int y1 = (int)y;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				// 각 꼭지점의 색상 값 가져오기
				COLORREF p11 = newImage.GetPixel(x1, y1);
				COLORREF p12 = newImage.GetPixel(x1, y2);
				COLORREF p21 = newImage.GetPixel(x2, y1);
				COLORREF p22 = newImage.GetPixel(x2, y2);

				// 양선형 보간을 통해 새로운 픽셀 값 계산
				double weight1 = (x2 - x) * (y2 - y);
				double weight2 = (x - x1) * (y2 - y);
				double weight3 = (x2 - x) * (y - y1);
				double weight4 = (x - x1) * (y - y1);

				int r = (int)(GetRValue(p11) * weight1 + GetRValue(p21) * weight2 + GetRValue(p12) * weight3 + GetRValue(p22) * weight4);
				int g = (int)(GetGValue(p11) * weight1 + GetGValue(p21) * weight2 + GetGValue(p12) * weight3 + GetGValue(p22) * weight4);
				int b = (int)(GetBValue(p11) * weight1 + GetBValue(p21) * weight2 + GetBValue(p12) * weight3 + GetBValue(p22) * weight4);

				COLORREF px = RGB(r, g, b);

				newImageR[i][k] = GetRValue(px);
				newImageG[i][k] = GetGValue(px);
				newImageB[i][k] = GetBValue(px);
			}
		}

		//****************************************** 2. 크로마키 알고리즘 ****************************************** 

		// 임시 메모리 할당
		// tmpImage -> 에지 검출 연산을 위한 배열
		// tmpOutImage -> 최종 연산을 위한 배열
		double** tmpImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpImageB = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageR = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageG = OnMallocDouble2D(m_inH, m_inW);
		double** tmpOutImageB = OnMallocDouble2D(m_inH, m_inW);

		// 크로마키 범위 설정
		const int hue_green_min = 90; // 녹색 색상 최소값
		const int hue_green_max = 150; // 녹색 색상 최대값
		const double saturation_min = 0.3; // 채도 최소값

		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				double* hsi = RGB2HSI(m_inImageR[i][k], m_inImageG[i][k], m_inImageB[i][k]);
				double H = hsi[0];
				double S = hsi[1];
				double I = hsi[2];

				// 녹색 범위 내의 픽셀을 배경 이미지로 대입
				if (H >= hue_green_min && H <= hue_green_max &&
					S >= saturation_min) {
					// 이미지 범위를 벗어나는 경우 무시
					if (i < newHeight && k < newWidth) {
						// 배경 이미지의 RGB 값을 가져와서 대입
						// tmpImage -> 에지 검출을 위해 배경 픽셀 제거
						// tmpOutImage -> 배경 이미지 대입(노이즈 있음)
						tmpImageR[i][k] = 0;
						tmpImageG[i][k] = 0;
						tmpImageB[i][k] = 0;
						tmpOutImageR[i][k] = newImageR[i][k];
						tmpOutImageG[i][k] = newImageG[i][k];
						tmpOutImageB[i][k] = newImageB[i][k];
					}
				}
				else {
					// 녹색 범위 외의 픽셀은 원본 이미지를 유지
					tmpImageR[i][k] = m_inImageR[i][k];
					tmpImageG[i][k] = m_inImageG[i][k];
					tmpImageB[i][k] = m_inImageB[i][k];
					tmpOutImageR[i][k] = m_inImageR[i][k];
					tmpOutImageG[i][k] = m_inImageG[i][k];
					tmpOutImageB[i][k] = m_inImageB[i][k];
				}

				delete[] hsi; // 동적으로 할당된 메모리 해제
			}
		}

		//****************************************** 3. 이진화 ****************************************** 
		// 에지 검출을 위해 배경과 객체를 이진화
		// 배경이 제거된 tmpImage에 대해 적용
		double sum = 0;
		double avg;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				sum += (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
			}
		}
		avg = sum / (m_inH * m_inW);
		double avgRGB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				avgRGB = (tmpImageR[i][k] + tmpImageG[i][k] + tmpImageB[i][k]) / 3.0;
				if (avgRGB > avg)
					avgRGB = 255;
				else
					avgRGB = 0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avgRGB;
			}
		}

		////****************************************** 4. 에지 검출 ****************************************** 
		// 라플라시안 에지 기법 적용
		double mask[3][3] = { { 0.0,-1.0, 0.0},
						  {-1.0, 4.0,-1.0},
						  { 0.0,-1.0, 0.0} };

		// 회선 연산을 위한 메모리 할당
		double** edgeInImageR = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageG = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeInImageB = OnMallocDouble2D(m_inH + 2, m_inW + 2);
		double** edgeOutImageR = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageG = OnMallocDouble2D(m_outH, m_outW);
		double** edgeOutImageB = OnMallocDouble2D(m_outH, m_outW);

		// 임시 입력 메모리를 초기화(127) : 필요시 평균값
		for (int i = 0; i < m_inH + 2; i++) {
			for (int k = 0; k < m_inW + 2; k++) {
				edgeInImageR[i][k] = 127;
				edgeInImageG[i][k] = 127;
				edgeInImageB[i][k] = 127;
			}
		}

		// 입력 이미지 --> 임시 입력 이미지
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				edgeInImageR[i + 1][k + 1] = tmpImageR[i][k];
				edgeInImageG[i + 1][k + 1] = tmpImageG[i][k];
				edgeInImageB[i + 1][k + 1] = tmpImageB[i][k];
			}
		}

		// *** 회선 연산 ***
		double sumR, sumG, sumB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
				// Sum = 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
				sumR = sumG = sumB = 0.0;
				for (int m = 0; m < 3; m++) {
					for (int n = 0; n < 3; n++) {
						sumR += edgeInImageR[i + m][k + n] * mask[m][n];
						sumG += edgeInImageG[i + m][k + n] * mask[m][n];
						sumB += edgeInImageB[i + m][k + n] * mask[m][n];
					}
				}
				edgeOutImageR[i][k] = sumR;
				edgeOutImageG[i][k] = sumG;
				edgeOutImageB[i][k] = sumB;
			}
		}

		// 임시 출력 영상--> 출력 영상. 
		// tmpImage에 검출한 에지 정보를 저장
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				tmpImageR[i][k] = OnClamping(edgeOutImageR[i][k]);
				tmpImageG[i][k] = OnClamping(edgeOutImageG[i][k]);
				tmpImageB[i][k] = OnClamping(edgeOutImageB[i][k]);
			}
		}

		////****************************************** 5. 에지 라인 보정 ******************************************

		// 검출한 경계선 영역을 따라 배경 이미지 대입
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					tmpOutImageR[i][k] = newImageR[i][k];
					tmpOutImageG[i][k] = newImageG[i][k];
					tmpOutImageB[i][k] = newImageB[i][k];
				}
			}
		}

		// 평균값 필터링 알고리즘
		for (int i = 1; i < m_inH - 1; i++) {
			for (int k = 1; k < m_inW - 1; k++) {
				if (tmpImageR[i][k] == 255 && tmpImageG[i][k] == 255 && tmpImageB[i][k] == 255) {
					// 주변 3x3 픽셀 값의 합 계산
					int sumR, sumG, sumB;
					sumR = sumG = sumB = 0;
					for (int x = -1; x <= 1; x++) {
						for (int y = -1; y <= 1; y++) {
							sumR += tmpOutImageR[i + x][k + y];
							sumG += tmpOutImageG[i + x][k + y];
							sumB += tmpOutImageB[i + x][k + y];
						}
					}
					// 평균값 계산
					double avgR = sumR / 9.0;
					double avgG = sumG / 9.0;
					double avgB = sumB / 9.0;
					// 최종 보정 이미지에 평균값 적용
					tmpOutImageR[i][k] = avgR;
					tmpOutImageG[i][k] = avgG;
					tmpOutImageB[i][k] = avgB;
				}
			}
		}

		// 최종 보정 이후 클램핑 적용
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[i][k] = OnClamping(tmpOutImageR[i][k]);
				m_outImageG[i][k] = OnClamping(tmpOutImageG[i][k]);
				m_outImageB[i][k] = OnClamping(tmpOutImageB[i][k]);
			}
		}

		// 동적 메모리 할당 해제
		OnFreeDouble2D(edgeInImageR, m_inH + 2);
		OnFreeDouble2D(edgeInImageG, m_inH + 2);
		OnFreeDouble2D(edgeInImageB, m_inH + 2);
		OnFreeDouble2D(edgeOutImageR, m_outH);
		OnFreeDouble2D(edgeOutImageG, m_outH);
		OnFreeDouble2D(edgeOutImageB, m_outH);

		OnFreeDouble2D(newImageR, m_inH);
		OnFreeDouble2D(newImageG, m_inH);
		OnFreeDouble2D(newImageB, m_inH);
		OnFreeDouble2D(tmpImageR, m_inH);
		OnFreeDouble2D(tmpImageG, m_inH);
		OnFreeDouble2D(tmpImageB, m_inH);

	}
}
