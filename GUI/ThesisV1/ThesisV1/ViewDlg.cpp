// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThesisV1.h"
#include "ViewDlg.h"
#include "afxdialogex.h"
#include "afxcmn.h"

#include "ThesisV1Dlg.h"
#include "MainMenuDlg.h"


// ViewDlg dialog

IMPLEMENT_DYNAMIC(ViewDlg, CDialogEx)

ViewDlg::ViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewDlg::IDD, pParent)
{

}

ViewDlg::~ViewDlg()
{
}

void ViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, roomSchedules);
	DDX_Control(pDX, IDC_DISPLAY_ROOMS, displayRoomBtn);
	DDX_Control(pDX, IDC_DISPLAY_SCHEDULES, displaySchedulesBtn);
}


BEGIN_MESSAGE_MAP(ViewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ViewDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BACK_BTN, &ViewDlg::OnBnClickedBackBtn)
	ON_BN_CLICKED(IDC_DISPLAY_ROOMS, &ViewDlg::OnBnClickedDisplayRooms)
	ON_BN_CLICKED(IDC_DISPLAY_SCHEDULES, &ViewDlg::OnBnClickedDisplaySchedules)
END_MESSAGE_MAP()


// ViewDlg message handlers


BOOL ViewDlg::OnInitDialog()
{
	CMenu *pMenu = GetSystemMenu(FALSE);
	pMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);

	// Add Extra Initializations here!

	return TRUE;
}

void ViewDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CThesisV1Dlg login;
	CDialog dlg;

	this->OnInitDialog();
	this->EndDialog(0);
	login.DoModal();
}


void ViewDlg::OnBnClickedBackBtn()
{
	// TODO: Add your control notification handler code here
	MainMenuDlg menu;
	CDialog dlg;

	this->OnInitDialog();
	this->EndDialog(0);
	menu.DoModal();
}


void ViewDlg::OnBnClickedDisplayRooms()
{
	// TODO: Add your control notification handler code here
	CDialog::OnInitDialog();
	
	//roomSchedules.DeleteAllItems();
	//roomSchedules.DeleteColumn(0);
	//roomSchedules.DeleteColumn(1);
	//roomSchedules.DeleteColumn(2);
	
	roomSchedules.InsertColumn(0, _T("Schedule Name"), LVCFMT_LEFT, 90);
	roomSchedules.InsertColumn(1, _T("Schedule"), LVCFMT_LEFT, 90);
	roomSchedules.InsertColumn(2, _T("Rooms"), LVCFMT_LEFT, 90);
	
	displayRoomBtn.EnableWindow(FALSE);
	displaySchedulesBtn.EnableWindow(TRUE);
}


void ViewDlg::OnBnClickedDisplaySchedules()
{
	// TODO: Add your control notification handler code here
	CDialog::OnInitDialog();

	//roomSchedules.DeleteAllItems();
	for(int x=0; x<3; x++){
		TRACE("\n%ddelete = %d",x,roomSchedules.DeleteColumn(x));
	}
	TRACE("\n%ddelete = %d",2,roomSchedules.DeleteColumn(2));
	//roomSchedules.InsertColumn(0, _T("HARRIS"), LVCFMT_LEFT, 90);
	//roomSchedules.InsertColumn(1, _T("BALDON"), LVCFMT_LEFT, 90);
	//roomSchedules.InsertColumn(2, _T("OH YEAH"), LVCFMT_LEFT, 90);

	displayRoomBtn.EnableWindow(TRUE);
	displaySchedulesBtn.EnableWindow(FALSE);
}
