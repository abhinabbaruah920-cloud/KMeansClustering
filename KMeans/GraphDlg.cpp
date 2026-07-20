// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KMeans.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
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


void GraphDlg::processmsg(){
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){		// Removing messages from Window Message Queue
	DispatchMessage(&msg);
	}
}

// GraphDlg message handlers
const int SCALE=2;		// defining the scale globally

void GraphDlg::OnPaint()
{
    CPaintDC dc(this);
    // graph area
    CRect graph(10,10,900,680);
    dc.Rectangle(graph);
	int cx=(graph.left+graph.right)/2;
    int cy=(graph.top+graph.bottom)/2;

	// x axis
    dc.MoveTo(graph.left,cy);
    dc.LineTo(graph.right,cy);

    // y axis
    dc.MoveTo(cx,graph.top);
    dc.LineTo(cx,graph.bottom);

    // drawing points
	CBrush rb(RGB(255,0,0));
	CBrush gb(RGB(0,255,0));
	CBrush nb(RGB(255,255,255));			// white color for the initial points
    for(int i=0;i<points.size();i++){
		int x=cx+(int)(points[i].x*SCALE);		
        int y=cy-(int)(points[i].y*SCALE);

		if(points[i].cluster==0){			// red colour for the first cluster
			dc.SelectObject(&rb);
		}else if(points[i].cluster==1){		//green colour for the second cluster
			dc.SelectObject(&gb);
		}else{
			dc.SelectObject(&nb);			// white colour before starting
		}			
		dc.Ellipse(x-5,y-5,x+5,y+5);
    }

		int x1=cx+(int)(C1.x*SCALE);
		int y1=cy-(int)(C1.y*SCALE);
		int x2=cx+(int)(C2.x*SCALE);
		int y2=cy-(int)(C2.y*SCALE);
		
		if(C1.x>0 || C2.y>0){						// initial white colour for the centroids before updating it
			dc.SelectObject(&nb);
			dc.Ellipse(x1-10,y1-10,x1+10,y1+10);
			dc.Ellipse(x2-10,y2-10,x2+10,y2+10);
		}
		if(points[0].cluster>=0){
		dc.SelectObject(&rb);
		dc.Ellipse(x1-10,y1-10,x1+10,y1+10);		// red colour for centroid of cluster 1
		dc.SelectObject(&gb);
		dc.Ellipse(x2-10,y2-10,x2+10,y2+10);		// green colour for centroid of cluster 2
	}
}


void GraphDlg::Set(const vector<Data>& d){			// temporary reference vector d
    points=d;										// setting the points
}

double GraphDlg::Dist(Data p1,Data p2){				// Euclidian distance function
    double dx=p1.x-p2.x;
    double dy=p1.y-p2.y;
    return sqrt(dx*dx+dy*dy);				
}

void GraphDlg::Init(){								// initialize KMEANS algo
	//choosing random variables r1 and r2 (distinct)
	int r1=rand()%points.size();
	int r2;
	do{
		r2=rand()%points.size();
	}while(r1== r2);
	// assigning random points to C1 and C2
    C1=points[r1];
    C2=points[r2];
	Invalidate();
    UpdateWindow();									// drawing initial centroids
    processmsg();
    Sleep(500);

    TotErr=0;										// initializing total Error
    for(int i=0;i<points.size();i++){
        double d1=Dist(points[i],C1);
        double d2=Dist(points[i],C2);
        if(d1<d2){									// assigning the points to the clusters
            points[i].cluster=0;
            TotErr+=d1*d1;
        }else{
            points[i].cluster=1;
            TotErr+=d2*d2;
        }
    }
    Invalidate();
	UpdateWindow();						// drawing the initial cluster colours for initial centroids
	Sleep(500);
}

void GraphDlg::Update(){						// updating the centroids
    double sumx1=0,sumy1=0,sumx2=0,sumy2=0;
    int count1=0,count2= 0;
    for(int i=0;i<points.size();i++){
        if(points[i].cluster==0){		// summation of all x and y points of cluster 0
            sumx1+=points[i].x;
            sumy1+=points[i].y;
            count1++;
        }else{							// summation of all x and y points of cluster 1
            sumx2+=points[i].x;
            sumy2+=points[i].y;
            count2++;
        }
    }

    if(count1> 0){								// Mean Calculation for centroid 1
        C1.x=sumx1/count1;
        C1.y=sumy1/count1;
    }
    if(count2>0){								// Mean calculation for centroid 2
        C2.x=sumx2/count2;
        C2.y=sumy2/count2;
    }
}

void GraphDlg::Assign(){						// reassigning points to clusters

    for(int i=0;i<points.size();i++){
        double d1=Dist(points[i],C1);
        double d2=Dist(points[i],C2);
        int newCluster;
        if(d1<d2){
            newCluster=0;
		}else{
            newCluster=1;
		}
        if(newCluster!=points[i].cluster){
            points[i].cluster=newCluster;

        }
    }

}
double GraphDlg::CalErr(){						// error calculation for convergence
    double err=0;
    for(int i=0;i< points.size();i++){
        if(points[i].cluster==0){
            double d=Dist(points[i],C1);
            err+=d*d;
        }else{
            double d=Dist(points[i],C2);
            err+=d*d;
        }
    }
	return err;
}
void GraphDlg::run(){						// run function to start K means Clustering
	Init();
	Invalidate();
	UpdateWindow();
    while(true){
        double prevErr=TotErr;				// storing previous error
        Update();							// Updating the centroids 
		Invalidate();
        UpdateWindow();
		processmsg();
		Sleep(500);
		Assign();							// assigning the cluster to the updated centroids
		Invalidate();
        UpdateWindow();
		processmsg();
		Sleep(500);
        TotErr=CalErr();					// calculating the new error and storing to the total error

        if(fabs(prevErr-TotErr)<0.0001)		// comparing the absolute error for convergence
            break;
    }
}

BOOL GraphDlg::PreTranslateMessage(MSG* pMsg)				// relay messages to tooltipCtrl
{
    tooltip.RelayEvent(pMsg);
    return CDialogEx::PreTranslateMessage(pMsg);
}

void GraphDlg::OnMouseMove(UINT nFlags, CPoint point)		// function to show points when hovering
{
    CRect graph(10,10,900,680);
    int cx=(graph.left+graph.right)/2;
    int cy=(graph.top+graph.bottom)/2;
    CString tip =_T("");
    bool found=false;
	// POINTS
    for(int i=0;i<points.size();i++){
        int x=cx+(int)(points[i].x*SCALE);
        int y=cy-(int)(points[i].y*SCALE);
        int dx=point.x-x;
        int dy=point.y-y;

        if(dx*dx+dy*dy<=50){
            tip.Format(_T("(%.1f,%.1f)"),points[i].x,points[i].y);		// showing the points till first decimal place
            found=true;
            break;
        }
    }
	// CENTROIDS
	if(!found){															
    int x1=cx+(int)(C1.x*SCALE);
    int y1=cy-(int)(C1.y*SCALE);
	int x2=cx+(int)(C2.x*SCALE);
    int y2=cy-(int)(C2.y*SCALE);
	int dx1=point.x-x1;
    int dy1=point.y-y1;
    int dx2=point.x-x2;
    int dy2=point.y-y2;
	
	// showing the centroids till first decimal place
    if(dx1*dx1+dy1*dy1<= 50){
        tip.Format(_T("Centroid:(%.1f, %.1f)"),C1.x,C1.y);
        found=true;
    }
    if(dx2*dx2+dy2*dy2<=50){
        tip.Format(_T("Centroid: (%.1f, %.1f)"),C2.x,C2.y);
        found=true;
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
	srand((unsigned)time(NULL));					// taking two random points as centroids
	tooltip.Create(this);
    tooltip.AddTool(this,_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


