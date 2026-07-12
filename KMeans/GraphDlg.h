#pragma once

// GraphDlg dialog

#include <vector>
using namespace std;

struct Data{
    double x;
    double y;
	};

class GraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GraphDlg)

public:
	GraphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GraphDlg();
	vector<Data> points;
    void SetPoints(const vector<Data>& p);
// Dialog Data
	enum { IDD = IDD_GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
