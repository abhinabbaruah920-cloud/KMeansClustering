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
	running=false;
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
    // Main graph border
    CRect graph(10,10,900,680);
    dc.Rectangle(graph);

    // Maximum absolute distance from (0,0) among centroids
    double mAbx=0;
    double mAby=0;

	double minx= points[0].x;
    double maxx= points[0].x;
    double miny= points[0].y;
    double maxy= points[0].y;

	for(int i=0;i<points.size();i++){
		if(fabs(points[i].x)>mAbx){
			mAbx=fabs(points[i].x);
		}
		if(fabs(points[i].y)>mAby){
			mAby=fabs(points[i].y);
		}
		if(points[i].x<minx){
			minx=points[i].x;
		}
		if(points[i].x>maxx){
			maxx=points[i].x;
		}
		if(points[i].y<miny){
			miny=points[i].y;
		}
		if(points[i].y>maxy){
			maxy=points[i].y;
		}
	}

    // Checking centroids to keep them on screen
    for(int i=0;i< centroids.size();i++){
        if(fabs(centroids[i].x)>mAbx){
			mAbx= fabs(centroids[i].x);
		}
        if(fabs(centroids[i].y)>mAby){
			mAby=fabs(centroids[i].y);
		}
    }

    // Preventing division by zero
    if(mAbx== 0){
		mAbx=1;
	}
    if(mAby==0){
		mAby= 1;
	}
	// Calculating scale
    int pad=10;
    double scalx=((graph.Width()/2.0)-pad)/mAbx;
    double scaly=((graph.Height()/2.0)-pad)/mAby;
    SCALE =min(scalx,scaly);

	double cenx=(minx + maxx)/2.0;
	double ceny=(miny + maxy)/2.0;

	int cx=graph.CenterPoint().x- (int)(cenx* SCALE);
	int cy= graph.CenterPoint().y+ (int)(ceny* SCALE);

    // Drawing x axis
    dc.MoveTo(graph.left,cy);
    dc.LineTo(graph.right,cy);
    // Drawing y axis
    dc.MoveTo(cx,graph.top);
    dc.LineTo(cx,graph.bottom);

    // Brush initialization
    CBrush b[5];
    b[0].CreateSolidBrush(RGB(255,0,0));			// Red color
    b[1].CreateSolidBrush(RGB(0,255,0));			// Green colour
    b[2].CreateSolidBrush(RGB(0,0,255));			// Blue colour
    b[3].CreateSolidBrush(RGB(255,255,0));			// Yellow colour
    b[4].CreateSolidBrush(RGB(0,255,255));			// Cyan colour
    CBrush nb(RGB(255,255,255));					// White colour when importing

    for(int i=0;i < points.size();i++){									// Drawing the points
        int x=cx+ (int)(points[i].x* SCALE);
        int y=cy- (int)(points[i].y* SCALE);
        
        if(points[i].cluster>=0){
            dc.SelectObject(&b[points[i].cluster]);						// Colouring the points based on clusters
        }else{
            dc.SelectObject(&nb);										// Initial color of points
        }
        dc.Ellipse(x-4,y-4,x+4,y+4);
    }

    if(!points.empty()){
		if(points[0].cluster<0){										// Drawing the centroids
			dc.SelectObject(&nb);										// Initial colour
			 for(int i=0;i < centroids.size();i++){
				int x=cx+ (int)(centroids[i].x* SCALE);
				int y=cy- (int)(centroids[i].y* SCALE);
				dc.Ellipse(x-8,y-8,x+8,y+8);
			 }
		}else{
			for(int i=0;i<k && i<(int)centroids.size();i++){
				dc.SelectObject(&b[i]);									// Colouring the centroids 
				int x=cx+ (int)(centroids[i].x* SCALE);
				int y=cy- (int)(centroids[i].y* SCALE);
				dc.Ellipse(x-10,y-10,x+10,y+10);
			   }
		 }
	}
}


void GraphDlg::Set(const vector<Data>& d){				// Temporary reference vector d
    points=d;
}

double GraphDlg::Dist(Data p1,Data p2){				// Euclidian distance function
    double dx=p1.x-p2.x;
    double dy=p1.y-p2.y;
    return sqrt(dx*dx+dy*dy);
}

void GraphDlg::Init(){								// Initialize KMEANS algo

	// choosing random centroids(distinct)
    centroids.clear();
    while(centroids.size() < k){
        int r=rand()%points.size();									// Random number picking
        bool exist=false;
        for(int i=0;i < centroids.size();i++){										// Unique random number
            if(centroids[i].x==points[r].x && centroids[i].y==points[r].y){
                exist=true;															// Bool flag if exists
                break;
            }
        }
        if(!exist){
            centroids.push_back(points[r]);					// Adding unique rand number
		}
    }
    Invalidate();
    UpdateWindow();
    processmsg();
    Sleep(500);

    // Assign every point to the nearest centroid
    for(int i=0;i < points.size();i++){
        double nearest= Dist(points[i],centroids[0]);			// Shortest distance from point to centroid
        int cluster=0;											// Index of closest centroid

		// calculating closest centroids
        for(int j=1;j < k;j++){
            double d =Dist(points[i],centroids[j]);		
            if(d<nearest){
                nearest= d;									// Updating closest distance
                cluster=j;									// Updating the index to this closer centroid
            }
        }
        points[i].cluster =cluster;					// Saving the final cluster assignment
    }
    Invalidate();
    UpdateWindow();
    processmsg();
    Sleep(500);
}

void GraphDlg::Update(){								// Updating the centroids
	vector<double> sumx(k,0);
	vector<double> sumy(k,0);
	vector<int> count(k,0);
	for(int i=0;i<points.size();i++){					// Summation of all the x and y points
		int c=points[i].cluster;
		sumx[c]+=points[i].x;				
		sumy[c]+=points[i].y;
		count[c]++;										// Adding counter for division by zero
	}
	for(int i=0;i<k;i++){
		if(count[i]> 0){
			centroids[i].x= sumx[i]/count[i];			// Mean value of x for centroids
			centroids[i].y= sumy[i]/count[i];			// Mean value of y for centroids
		}
	}
}

void GraphDlg::Assign(){								// Reassigning points to clusters
	for(int i=0;i<points.size();i++){
		double nearest= Dist(points[i],centroids[0]);
		int cluster=0;
		
		// assigning points to the nearest centroids
		for(int j=1;j < k;j++){
			double d=Dist(points[i],centroids[j]);
			if(d<nearest){								// Update the assigned cluster if closer centroid is found
				nearest= d;
				cluster=j;
			}
		}
		points[i].cluster =cluster;				// Storing the closest clusters
	}

}


double GraphDlg::CalErr(){								// Error calculation for convergence
	double err=0;
	for(int i=0;i<points.size();i++){
		double d= Dist(points[i],centroids[points[i].cluster]);
		err+=d*d;													// Calculation of sum of squared errors
	}
	return err;
}

void GraphDlg::run(){								// Run function to start K means Clustering
	running=true;
	Init();
	newErr=CalErr();
    while(true){
        double prevErr=newErr;						// Previous error
        
		Update();									// Updating the centroids 
		Invalidate();
        UpdateWindow();
		processmsg();
		Sleep(500);

		Assign();									// Assigning the cluster to the updated centroids
		Invalidate();
        UpdateWindow();
		processmsg();
		Sleep(500);

        newErr=CalErr();							// New error 

        if(fabs(prevErr-newErr)<0.001)				// Convergence
            break;
    }
	running=false;
}

BOOL GraphDlg::PreTranslateMessage(MSG* pMsg)						// Relay messages to tooltipCtrl
{
    tooltip.RelayEvent(pMsg);
    return CDialogEx::PreTranslateMessage(pMsg);
}

void GraphDlg::OnMouseMove(UINT nFlags, CPoint point)				// Function to show points when hovering
{
	if(points.empty()){
        CDialogEx::OnMouseMove(nFlags, point);
        return;
    }
	    
	// Onpaint function part
    CRect graph(10,10,900,680);
    double mAbx=0;
    double mAby=0;
	double minx= points[0].x;
    double maxx= points[0].x;
    double miny= points[0].y;
    double maxy= points[0].y;

	for(int i=0;i<points.size();i++){
		if(fabs(points[i].x)>mAbx){
			mAbx=fabs(points[i].x);
		}
		if(fabs(points[i].y)>mAby){
			mAby=fabs(points[i].y);
		}
		if(points[i].x<minx){
			minx=points[i].x;
		}
		if(points[i].x>maxx){
			maxx=points[i].x;
		}
		if(points[i].y<miny){
			miny=points[i].y;
		}
		if(points[i].y>maxy){
			maxy=points[i].y;
		}
	}

    // Checking centroids to keep them on screen
    for(int i=0;i< centroids.size();i++){
        if(fabs(centroids[i].x)>mAbx){
			mAbx= fabs(centroids[i].x);
		}
        if(fabs(centroids[i].y)>mAby){
			mAby=fabs(centroids[i].y);
		}
    }

    // Preventing division by zero
    if(mAbx== 0){
		mAbx=1;
	}
    if(mAby==0){
		mAby= 1;
	}

	// Calculating scale
    int pad=10;
    double scalx=((graph.Width()/2.0)-pad)/mAbx;
    double scaly=((graph.Height()/2.0)-pad)/mAby;
    SCALE =min(scalx,scaly);

	double cenx=(minx + maxx)/2.0;
	double ceny=(miny + maxy)/2.0;

	int cx=graph.CenterPoint().x- (int)(cenx* SCALE);
	int cy= graph.CenterPoint().y+ (int)(ceny* SCALE);

    CString tip;
    bool found=false;

	// POINTS
    for(int i=0;i<points.size();i++){
        int x=cx+(int)(points[i].x*SCALE);
        int y=cy-(int)(points[i].y*SCALE);
        int dx=point.x-x;
        int dy=point.y-y;

        if(dx*dx+dy*dy<=50){
            tip.Format(_T("(%.1f,%.1f)"),points[i].x,points[i].y);								// Showing the points
            found=true;
            break;
        }
    }
	
	// CENTROIDS
	for(int i=0;i<(int)centroids.size();i++){
		int x=cx+ centroids[i].x* SCALE;
		int y=cy- centroids[i].y* SCALE;
		int dx=point.x-x;
		int dy=point.y-y;
		if(dx*dx+dy*dy<= 50){
			tip.Format(_T("Centroid %d: (%.2f, %.2f)"),i+1,centroids[i].x,centroids[i].y);		// Showing the centroids
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
	srand((unsigned)time(NULL));					// Taking random points as centroids
	tooltip.Create(this);
    tooltip.AddTool(this,_T(""));
	tooltip.Activate(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


