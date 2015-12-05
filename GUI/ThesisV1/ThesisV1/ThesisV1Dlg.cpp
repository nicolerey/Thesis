
// ThesisV1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThesisV1.h"
#include "ThesisV1Dlg.h"
#include "afxdialogex.h"
#include "Thesis.h"

#include "MainMenuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CThesisV1Dlg dialog




CThesisV1Dlg::CThesisV1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThesisV1Dlg::IDD, pParent)
	, tf_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThesisV1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, tf_password);
}

BEGIN_MESSAGE_MAP(CThesisV1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CThesisV1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CThesisV1Dlg message handlers

BOOL CThesisV1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThesisV1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThesisV1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThesisV1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CThesisV1Dlg::OnBnClickedButton1()
{
	UpdateData();

	CString catcher;
	CString toDisplay;
	int request;

	catcher.Format(_T("%sfunction=get_auth&password=%s"), url, tf_password);
	LPCTSTR finalUrl = catcher;

	request = thesis_request(finalUrl);

	if(request){
		toDisplay.Format(_T("%s"),response["message"].GetString());

		if(response["status"].GetInt() == 1){
			MainMenuDlg main;
			CDialog dlg;

			this->OnInitDialog();
			//this->ShowWindow(SW_HIDE);
			this->EndDialog(0);
			main.DoModal();

			//MessageBox(toDisplay, 0, MB_ICONINFORMATION);
		}else{
			MessageBox(toDisplay, "Authentication Error!", MB_ICONERROR);
			CWnd *cedit = GetDlgItem(IDC_EDIT1);
			cedit->SetWindowTextA(_T(""));
		}
	}else{
		CString errorDisplay(errorMsg);
		MessageBox(errorDisplay, "Server Error!", MB_ICONERROR);
	}
}
