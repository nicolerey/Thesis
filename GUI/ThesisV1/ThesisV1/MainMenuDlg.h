#pragma once


// MainMenuDlg dialog

class MainMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MainMenuDlg)

public:
	MainMenuDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MainMenuDlg();

// Dialog Data
	enum { IDD = IDD_MAIN_MENU_DLG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};
