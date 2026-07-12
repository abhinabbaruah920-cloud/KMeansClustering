#pragma once

// GraphDlg dialog

#include <vector>
using namespace std;

struct Data{				// declaring structure to hold values of x and y
    double x;
    double y;
	};

class GraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GraphDlg)

public:
	GraphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GraphDlg();

	vector<Data> points;						// declaring a vector for Data objects
    void SetPoints(const vector<Data>& p);		// function to set the Points
// Dialog Data
	enum { IDD = IDD_GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
