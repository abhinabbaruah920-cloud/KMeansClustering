// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KMeans.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
// including file handling header files
#include <fstream>
#include <sstream>
#include <string>
// including math module
#include <cmath>


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
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// GraphDlg message handlers

void GraphDlg::OnPaint()
{
    CPaintDC dc(this);
    // graph area
    CRect graph(10,10,900,680);
    dc.Rectangle(graph);

    const int SCALE=2;
	int cx=(graph.left+graph.right)/2;
    int cy=(graph.top+graph.bottom)/2;

	// x axis
    dc.MoveTo(graph.left,cy);
    dc.LineTo(graph.right,cy);

    // y axis
    dc.MoveTo(cx,graph.top);
    dc.LineTo(cx,graph.bottom);

    // drawing points
    for(int i=0;i<points.size();i++){
      
		int x=cx+(int)(points[i].x*SCALE);		
        int y=cy-(int)(points[i].y*SCALE);
        CBrush rb(RGB(255,0,0));
		CBrush gb(RGB(0,255,0));

		if(points[i].cluster==0){			// red colour for the first cluster
			dc.SelectObject(&rb);
		}else{
			dc.SelectObject(&gb);}			//green colour for the second cluster
		dc.Ellipse(x-5,y-5,x+5,y+5);

		int x1=cx+(int)(C1.x*SCALE);
		int y1=cy-(int)(C1.y*SCALE);
		int x2=cx+(int)(C2.x*SCALE);
		int y2=cy-(int)(C2.y*SCALE);
		dc.SelectObject(&rb);
		dc.Ellipse(x1-10,y1-10,x1+10,y1+10);		// cluster 1
		dc.SelectObject(&gb);
		dc.Ellipse(x2-10,y2-10,x2+10,y2+10);		// cluster 2

    }
}

void GraphDlg::Set(const vector<Data>& d){
    points=d;										// setting the points and initializing the KMEANS algo
	Init();
}

double GraphDlg::Dist(Data p1,Data p2){				// Euclidian distance function
    double dx=p1.x-p2.x;
    double dy=p1.y-p2.y;
    return sqrt(dx*dx+dy*dy);				
}

void GraphDlg::Init(){								// initialize KMEANS algo
	srand((unsigned)time(NULL));					// taking two random points as centroids
	int r1=rand()%points.size();
	int r2;
	do{
		r2=rand()%points.size();
	}while(r1== r2);

    C1=points[r1];
    C2=points[r2];
    TotErr=0;										// initializing total Error
    for(int i=0;i<points.size();i++){
        double d1=Dist(points[i],C1);
        double d2=Dist(points[i],C2);
        if(d1<d2){									// assigning the points to the centroids
            points[i].cluster=0;
            TotErr+=d1*d1;
        }else{
            points[i].cluster=1;
            TotErr+=d2*d2;
        }
    }
    Invalidate();
	UpdateWindow();
}



BOOL GraphDlg::PreTranslateMessage(MSG* pMsg)				// relay messages to tooltipCtrl
{
    tooltip.RelayEvent(pMsg);
    return CDialogEx::PreTranslateMessage(pMsg);
}

void GraphDlg::OnMouseMove(UINT nFlags, CPoint point)		// function to show points when hovering
{
    const int SCALE=2;
    CRect graph(10,10,900,680);
    int cx=(graph.left+graph.right)/2;
    int cy=(graph.top+graph.bottom)/2;
    CString tip =_T("");
    bool found=false;

    for(int i=0;i<points.size();i++){
        int x=cx+(int)(points[i].x*SCALE);
        int y=cy-(int)(points[i].y*SCALE);
        int dx=point.x-x;
        int dy=point.y-y;

        if(dx*dx+dy*dy<=50){
            tip.Format(_T("(%.1f,%.1f)"),points[i].x,points[i].y);		// showing the points till first decimal places
            found=true;
            break;
        }
    }
    if(found){
        tooltip.UpdateTipText(tip,this);
	}else{
		tooltip.UpdateTipText(_T(""),this);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL GraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	tooltip.Create(this);
    tooltip.AddTool(this,_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
