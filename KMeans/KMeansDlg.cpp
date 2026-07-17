
// KMeansDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KMeans.h"
#include "KMeansDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


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


// CKMeansDlg dialog

CKMeansDlg::CKMeansDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKMeansDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKMeansDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKMeansDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMPORT, &CKMeansDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_BUTTON1, &CKMeansDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CKMeansDlg message handlers

BOOL CKMeansDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	graph.Create(IDD_GRAPH,this);		// Creating the Dlg box

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

void CKMeansDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKMeansDlg::OnPaint()
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
HCURSOR CKMeansDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKMeansDlg::OnBnClickedImport()
{
    CFileDialog dlg(TRUE,_T("csv"),NULL,OFN_FILEMUSTEXIST,_T("CSV Files (*.csv)|*.csv||"));			//importing only csv files
    if (dlg.DoModal()!=IDOK)
        return;

    CString filename=dlg.GetPathName();				// getting the entire path of the file
    // Convert CString to ANSI string for ifstream
    CStringA filenameA(filename);
    ifstream fin(filenameA);
    if (!fin.is_open()){
        AfxMessageBox(_T("Select CSV file only"));
        return;
    }

    vector<Data> points;
    string line;
    while(getline(fin,line)){			
        if (line.empty())					// skipping blanks
            continue;
        stringstream ss(line);
        string xs;
        string ys;
        if(!std::getline(ss,xs,','))		// taking x and assigning it to xs
            continue;
        if(!std::getline(ss,ys))			// taking y and assigning it to xy
            continue;
        Data d;
        d.x=stod(xs);						// stod: string to double for x
        d.y=stod(ys);						// stod for y
        points.push_back(d);				// appending to the vector
    }
	fin.close();
	
	graph.Set(points);						//setting points to the Graph
	graph.ShowWindow(SW_SHOW);				// showing the graph dialog window
	graph.Invalidate();
	graph.UpdateWindow();			
}


void CKMeansDlg::OnBnClickedButton1()		// start button for execution of K means clustering Algo
{
    if(graph.points.empty()){
        AfxMessageBox(_T("Select CSV file first"));
        return;
    }
    graph.run();
}
