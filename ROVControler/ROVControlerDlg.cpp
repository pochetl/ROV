
// ROVControlerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ROVControler.h"
#include "ROVControlerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CROVControlerDlg dialog



CROVControlerDlg::CROVControlerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROVCONTROLER_DIALOG, pParent)
	, leftSides(_T(""))
	, leftUp(_T(""))
{
	this->insideTimer = false;  
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	count = 0;
	Player1 = new CXBOXController(1);

}

void CROVControlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEST, testString);
	DDX_Text(pDX, IDC_LEFT_SIDES, leftSides);
	DDX_Text(pDX, IDC_LEFT_UP, leftUp);
	DDX_Control(pDX, IDC_RECONNECT, ReconnectButtonCtrl);
	DDX_Control(pDX, IDC_JOYSTICK_DISPLAY, joyStickDisplayCTRL);
}

BEGIN_MESSAGE_MAP(CROVControlerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDABORT, &CROVControlerDlg::OnStnClickedAbort)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RECONNECT, &CROVControlerDlg::OnBnClickedReconnect)
	ON_STN_CLICKED(IDC_LEFT_UP, &CROVControlerDlg::OnStnClickedLeftUp)
END_MESSAGE_MAP()


// CROVControlerDlg message handlers

BOOL CROVControlerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	testString.Format("Chuck345");
	UpdateData(0);
	UINT_PTR myTimer = SetTimer(1, 10, NULL); // one event every 1000 ms = 1 s
	 


	CStatic * pWnd = (CStatic *)GetDlgItem(IDC_JOYSTICK_DISPLAY);
	CRect rect;
	pWnd->GetClientRect(&rect);
	scaleX = rect.Width() /( SHORT_MAX*2.0);
	scaleY = rect.Height() / (SHORT_MAX*2.0);

	this->ReconnectButtonCtrl.EnableWindowsTheming(FALSE);
	ReconnectButtonCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	ReconnectButtonCtrl.m_bTransparent = false;
	//ReconnectButtonCtrl.SetTextColor(RGB(0, 0, 255));
	ReconnectButtonCtrl.SetTextColor(RGB(255, 255, 255));

	checkControllerConnection();
	allowRedraw=true;


	
	serial.start();



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void DrawCrossXOR(CDC * pDC, CPoint pt)
{
	
//	int r2 = pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(pt + CSize(0, 5));
	pDC->LineTo(pt + CSize(0, -5));
	pDC->MoveTo(pt + CSize(5, 0));
	pDC->LineTo(pt + CSize(-5, 0));
//	pDC->SetROP2(r2);
}

void CROVControlerDlg::DrawScene(void)
{
	redrawCount++;

	//if (redrawCount > 100)
	//{

	//	CRect rect;
	//	joyStickDisplayCTRL.GetClientRect(rect);
	//	CBrush brush;
	//	// repaint the window with solid black background
	//	brush.CreateSolidBrush(RGB(255, 255, 255));
	//	joyStickDisplayCTRL.GetDC()->FillRect(rect, &brush);
	//	redrawCount = 0;
	//}


	DrawCrossXOR(joyStickDisplayCTRL.GetDC(), m_ptXY);
	/*
	pWnd = (CStatic *)GetDlgItem(IDC_STATIC_Z);
	DrawCrossXOR(pWnd->GetDC(), m_ptZ);*/
}

void CROVControlerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CROVControlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawScene();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CROVControlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CROVControlerDlg::OnStnClickedAbort()
{
	// TODO: Add your control notification handler code here
}


void CROVControlerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1 &&   ! insideTimer)
	{
		insideTimer = true; 
		count = count + 1;
		testString.Format("Chuck345   %d",count);
		UpdateData(0);
		
		



		if (count > 100)
		{
			checkControllerConnection();
			count = 0;
		}

		char testArray[10];
		int size = serial.buildMotorPacket(Player1->GetState().Gamepad.sThumbLX, Player1->GetState().Gamepad.sThumbLY, 0x6700, 0x8500, testArray);
		serial.writePacket(testArray, size);
		if (allowRedraw)
		{

			XINPUT_STATE state = Player1->GetState();
			this->leftSides.Format("%d", Player1->GetState().Gamepad.sThumbLX);
			this->leftUp.Format("%d", Player1->GetState().Gamepad.sThumbLY);
			m_ptXY.x = state.Gamepad.sThumbLX* scaleX + (SHORT_MAX* scaleX)*.9;
			m_ptXY.y = (-1*state.Gamepad.sThumbLY*scaleY) + (SHORT_MAX* scaleY)*.9;

			if (m_ptXY.y < 1) {
				m_ptXY.y = 0;
			}
			if (m_ptXY.x < 1) {
				m_ptXY.x = 0;
			}

			insideTimer = false;
		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CROVControlerDlg::checkControllerConnection()
{
	if (this->Player1->IsConnected())
	{
		ReconnectButtonCtrl.SetFaceColor(RGB(0, 255, 0), true);
	}
	else
	{
		ReconnectButtonCtrl.SetFaceColor(RGB(255, 0, 0), true);
	}
}


void CROVControlerDlg::OnBnClickedReconnect()
{
	allowRedraw = false;
	delete Player1; 

		Player1 = new CXBOXController(1);

		checkControllerConnection();
		allowRedraw = true;
	// TODO: Add your control notification handler code here
}


void CROVControlerDlg::OnStnClickedLeftUp()
{
	// TODO: Add your control notification handler code here
}
