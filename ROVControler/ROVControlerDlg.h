
// ROVControlerDlg.h : header file
//

#pragma once
#include "CXBOXController.h"

#include "SerialControler.h"
// CROVControlerDlg dialog
class CROVControlerDlg : public CDialogEx
{
// Construction
public:
	CROVControlerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROVCONTROLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	bool insideTimer;
	SerialControler serial;
	bool allowRedraw;
	CPoint m_ptXY;
	float scaleX, scaleY;
	int redrawCount;
	void DrawScene(void);
	HICON m_hIcon;
	int count;
	CXBOXController* Player1;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedAbort();
	CString testString;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void checkControllerConnection();
	CString leftSides;
	CString leftUp;
	CMFCButton ReconnectButtonCtrl;
	afx_msg void OnBnClickedReconnect();
	CStatic joyStickDisplayCTRL;
	afx_msg void OnStnClickedLeftUp();
};
