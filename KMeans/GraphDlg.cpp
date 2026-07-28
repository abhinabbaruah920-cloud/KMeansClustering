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


void GraphDlg::OnPaint()
{
    CPaintDC dc(this);
    //main graph border
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

    // maximum absolute distance from (0,0) among all points and centroids
    double mAbx=0;
    double mAby=0;
    // Check points
    for(int i=0;i<points.size();i++){
        if(fabs(points[i].x)>mAbx){
			mAbx=fabs(points[i].x);
		}
        if(fabs(points[i].y)>mAby){
			mAby=fabs(points[i].y);
		}
    }

    // checking centroids to keep them on screen
    for(int i=0;i< centroids.size();i++){
        if(fabs(centroids[i].x)>mAbx){
			mAbx= fabs(centroids[i].x);
		}
        if(fabs(centroids[i].y)>mAby){
			mAby=fabs(centroids[i].y);
		}
    }

    // preventing division by zero
    if(mAbx== 0){
		mAbx=1;
	}
    if(mAby==0){
		mAby= 1;
	}
	//calculating the scale
    int pad=30;
    double scaleX=((graph.Width()/2.0) - pad)/ mAbx;
    double scaleY=((graph.Height()/2.0) - pad)/ mAby;
    SCALE =min(scaleX,scaleY);

    // brush initialization
    CBrush b[5];
    b[0].CreateSolidBrush(RGB(255,0,0));      // Red color
    b[1].CreateSolidBrush(RGB(0,255,0));      // Green colour
    b[2].CreateSolidBrush(RGB(0,0,255));      // Blue colour
    b[3].CreateSolidBrush(RGB(255,255,0));    // Yellow colour
    b[4].CreateSolidBrush(RGB(0,255,255));    // Cyan colour
    CBrush nb(RGB(255,255,255));              // White colour when importing

    for(int i=0;i < points.size();i++){		// drawing the points
        int x=cx+ (int)(points[i].x* SCALE);
        int y=cy- (int)(points[i].y* SCALE);
        
        if(points[i].cluster>=0 && points[i].cluster<k){
            dc.SelectObject(&b[points[i].cluster]);				// colouring the points based on clusters
        }else{
            dc.SelectObject(&nb); //initital color of points
        }
        dc.Ellipse(x-5,y-5,x+5,y+5);
    }

    if(!points.empty()){						// drawing the centroids
        if(points[0].cluster<0){
            dc.SelectObject(&nb);						//initial colour
            for(int i=0;i < centroids.size();i++){
                int x=cx+ (int)(centroids[i].x* SCALE);
                int y=cy- (int)(centroids[i].y* SCALE);
                dc.Ellipse(x-10,y-10,x+10,y+10);
            }
        }else{
            for(int i=0;i<k && i<(int)centroids.size();i++){
                dc.SelectObject(&b[i]);							//colouring the centroids 
                int x=cx+ (int)(centroids[i].x* SCALE);
                int y=cy- (int)(centroids[i].y* SCALE);
                dc.Ellipse(x-10,y-10,x+10,y+10);
            }
        }
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

    newErr=0;										// initializing total Error
    for(int i=0;i<points.size();i++){
        double d1=Dist(points[i],C1);
        double d2=Dist(points[i],C2);
        if(d1<d2){									// assigning the points to the clusters
            points[i].cluster=0;
            newErr+=d1*d1;
        }else{
            points[i].cluster=1;
            newErr+=d2*d2;
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
        if(d1<d2){								// checking which points are closer to which centroids
            newCluster=0;
		}else{
            newCluster=1;
		}
        if(newCluster!=points[i].cluster){			// updating the points
            points[i].cluster=newCluster;

        }
    }

}
double GraphDlg::CalErr(){						// error calculation for convergence
    double err=0;
    for(int i=0;i< points.size();i++){
        if(points[i].cluster==0){
            double d=Dist(points[i],C1);
            err+=d*d;							// measuring the dist. between the point and C1 and adding it to err
        }else{
            double d=Dist(points[i],C2);		// measuring the dist. between the point and C2 and adding it to err
            err+=d*d;
        }
    }
	return err;
}

void GraphDlg::run(){						// run function to start K means Clustering
	Init();
    while(true){
        double prevErr=newErr;				// storing previous error
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
        newErr=CalErr();					// calculating the new error and storing to the total error

        if(fabs(prevErr-newErr)<0.001)		// comparing the absolute error for convergence
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


