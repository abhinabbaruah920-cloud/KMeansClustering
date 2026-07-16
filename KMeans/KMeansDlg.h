
// KMeansDlg.h : header file
//

#pragma once
#include "GraphDlg.h"
// CKMeansDlg dialog
class CKMeansDlg : public CDialogEx
{
// Construction
public:
	CKMeansDlg(CWnd* pParent = NULL);	// standard constructor
	GraphDlg graph;						// Creating obj of GraphDlg box


// Dialog Data
	enum { IDD = IDD_KMEANS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedButton1();
};
