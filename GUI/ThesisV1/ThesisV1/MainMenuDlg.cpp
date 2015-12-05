// MainMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThesisV1.h"
#include "MainMenuDlg.h"
#include "afxdialogex.h"

#include "ThesisV1Dlg.h"
#include "ViewDlg.h"


// MainMenuDlg dialog

IMPLEMENT_DYNAMIC(MainMenuDlg, CDialogEx)

MainMenuDlg::MainMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MainMenuDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

MainMenuDlg::~MainMenuDlg()
{
}

void MainMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainMenuDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &MainMenuDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &MainMenuDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// MainMenuDlg message handlers

BOOL MainMenuDlg::OnInitDialog()
{
	CMenu *pMenu = GetSystemMenu(FALSE);
	pMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Add Extra Initializations here!

	return TRUE;
}

void MainMenuDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CThesisV1Dlg login;
	CDialog dlg;

	this->OnInitDialog();
	this->EndDialog(0);
	login.DoModal();
}


void MainMenuDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialog dlg;
	ViewDlg view;

	this->OnInitDialog();
	this->EndDialog(0);
	view.DoModal();
}
