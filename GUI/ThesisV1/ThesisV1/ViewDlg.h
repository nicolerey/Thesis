#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// ViewDlg dialog

class ViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ViewDlg)

public:
	ViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ViewDlg();

// Dialog Data
	enum { IDD = IDD_VIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl roomSchedules;
	afx_msg void OnBnClickedBackBtn();
	afx_msg void OnBnClickedDisplayRooms();
	CButton displayRoomBtn;
	CButton displaySchedulesBtn;
	afx_msg void OnBnClickedDisplaySchedules();
};
