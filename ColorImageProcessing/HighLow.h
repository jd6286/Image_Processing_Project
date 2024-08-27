#pragma once
#include "afxdialogex.h"


// CHighLow 대화 상자

class CHighLow : public CDialog
{
	DECLARE_DYNAMIC(CHighLow)

public:
	CHighLow(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHighLow();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIGH_LOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_high;
	int m_low;
};
