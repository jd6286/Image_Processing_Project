// HeightWidth.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "HeightWidth.h"


// CHeightWidth 대화 상자

IMPLEMENT_DYNAMIC(CHeightWidth, CDialog)

CHeightWidth::CHeightWidth(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HEIGHT_WIDTH, pParent)
	, m_height(0)
	, m_width(0)
{

}

CHeightWidth::~CHeightWidth()
{
}

void CHeightWidth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
}


BEGIN_MESSAGE_MAP(CHeightWidth, CDialog)
END_MESSAGE_MAP()


// CHeightWidth 메시지 처리기
