// HighLow.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "HighLow.h"


// CHighLow 대화 상자

IMPLEMENT_DYNAMIC(CHighLow, CDialog)

CHighLow::CHighLow(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HIGH_LOW, pParent)
	, m_high(0)
	, m_low(0)
{

}

CHighLow::~CHighLow()
{
}

void CHighLow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HIGH, m_high);
	DDX_Text(pDX, IDC_EDIT_LOW, m_low);
}


BEGIN_MESSAGE_MAP(CHighLow, CDialog)
END_MESSAGE_MAP()


// CHighLow 메시지 처리기
