// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KMeans.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include <string>


// GraphDlg dialog

IMPLEMENT_DYNAMIC(GraphDlg, CDialogEx)

GraphDlg::GraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GraphDlg::IDD, pParent)
{

}

GraphDlg::~GraphDlg()
{
}

void GraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GraphDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// GraphDlg message handlers

void GraphDlg::OnPaint()
{
    CPaintDC dc(this);
    // graph area
    CRect graph(10,10,900,680);
    dc.Rectangle(graph);

    const int SCALE = 2;
	int cx=(graph.left+graph.right)/2;
    int cy=(graph.top+graph.bottom)/2;

	// x axis
    dc.MoveTo(graph.left,cy);
    dc.LineTo(graph.right,cy);

    // y axis
    dc.MoveTo(cx,graph.top);
    dc.LineTo(cx,graph.bottom);

    // drawing points
    for (int i=0;i<points.size();i++){
        int x=cx+(int)(points[i].x*SCALE);		
        int y=cy-(int)(points[i].y*SCALE);
        dc.Ellipse(x-2,y-2,x+2,y+2);
    }
}

void GraphDlg::SetPoints(const vector<Data>& d)
{
    points=d;
}