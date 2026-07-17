#pragma once

// GraphDlg dialog

#include <vector>
using namespace std;

struct Data{				// declaring structure to hold values of x and y
    double x;
    double y;
	int cluster;
	};

class GraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GraphDlg)

public:
	GraphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GraphDlg();

	vector<Data> points;							// Declaring a vector for Data objects
	Data C1;										// Cluster 1
	Data C2;										// Cluster 2
	double TotErr;									// Total Error
    void Set(const vector<Data>& p);				// function to set the Points
	void Init();									// initialize K Means
	double Dist(Data p1,Data p2);					// Distance from clusters
	virtual BOOL PreTranslateMessage(MSG* pMsg);	// Relay message for tooltip
	CToolTipCtrl tooltip;							// Tooltip for hovering over points
	void run();										// run K means algo
    void Update();									// update centroids
    void Assign();									// assign clusters after updating centroids
    double CalErr();								// calculate error for convergence

// Dialog Data
	enum { IDD = IDD_GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
