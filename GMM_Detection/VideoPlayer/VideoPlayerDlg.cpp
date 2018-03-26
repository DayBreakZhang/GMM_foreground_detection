
// VideoPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "VideoPlayerDlg.h"
#include "GMM.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>  
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//CvVideoWriter *writer;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVideoPlayerDlg �Ի���




CVideoPlayerDlg::CVideoPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, Slider);
}

BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CVideoPlayerDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CVideoPlayerDlg::OnNMCustomdrawSlider1)
	ON_STN_CLICKED(IDC_VIEW, &CVideoPlayerDlg::OnStnClickedView)
	ON_STN_CLICKED(IDC_VIEW2, &CVideoPlayerDlg::OnStnClickedView2)
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoPlayerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CVideoPlayerDlg ��Ϣ�������

BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	pStc=(CStatic *)GetDlgItem(IDC_VIEW);//IDC_VIEWΪPicture�ؼ�ID
	pStc->GetClientRect(&rect);//��CWind��ͻ���������㴫������
	pDC=pStc->GetDC(); //�õ�Picture�ؼ��豸������
	hDC=pDC->GetSafeHdc(); //�õ��ؼ��豸�����ĵľ�� 

	pStc1 = (CStatic *)GetDlgItem(IDC_VIEW2);//IDC_VIEW2ΪPicture�ؼ�ID
	pStc1->GetClientRect(&rect1);//��CWind��ͻ���������㴫������
	pDC1 = pStc1->GetDC(); //�õ�Picture�ؼ��豸������
	hDC1 = pDC1->GetSafeHdc(); //�õ��ؼ��豸�����ĵľ�� 

    hbitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	pStc->ModifyStyle(1,SS_BITMAP);
	pStc->SetBitmap(hbitmap);
	hbitmap1 = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	pStc1->ModifyStyle(1, SS_BITMAP);
	pStc1->SetBitmap(hbitmap1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVideoPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int numframe = 0;
int fps = 0, frameHeight=0, frameWidth=0 ;
void CVideoPlayerDlg::OnBnClickedButton1()
{
	CString caption;
	GetDlgItemText(IDC_BUTTON1,caption);
	if (caption == _T("�ر���Ƶ"))    //�ر���Ƶ��ť
	{
		KillTimer(1);
		capture.release();
		SetDlgItemText(IDC_BUTTON1,_T("����Ƶ"));
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->EnableWindow(FALSE);  //������ʧЧ
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->SetPos(0); //���û�����λ��
		SetDlgItemText(IDC_BUTTON2, _T("��ͣ"));//�޸İ�ť��Caption
		pStc->SetBitmap(hbitmap); //�ָ�λͼ��Դ
		pStc1->SetBitmap(hbitmap1); //�ָ�λͼ��Դ
		numframe = 0;
		return;
	}
   

 //����Ƶ,����ͨ�öԻ���,ѡ��Ҫ���ŵ���Ƶ�ļ�
	String _Path;  
	CFileDialog Openfiledlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (Openfiledlg.DoModal() == IDOK)
	{
		//CString����ת��Ϊstring����
		CString  filepath;      
		filepath=Openfiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		filepath.ReleaseBuffer();
		_Path = temp.GetBuffer(0);
		temp.ReleaseBuffer();
	}
	else
		return ;
	

	capture.open(_Path);  //����Ƶ
	if (!capture.isOpened())
	{        
		MessageBox(_T("�޷�����Ƶ��"),_T("����"));     
		return;
	}
	else
	{
	
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->EnableWindow(TRUE);  //�������
		int sumframe = (int)capture.get(CV_CAP_PROP_FRAME_COUNT); //��ȡ��֡��
		fps = capture.get(CV_CAP_PROP_FPS);   //��ȡ��Ƶ֡��
		frameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //��ȡ��Ƶ�ߴ�
		frameWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);

		//int outCompressCodec = (int)capture.get(CV_CAP_PROP_FOURCC);  
		//���屣����Ƶ���ĸ�ʽ
		writer = cvCreateVideoWriter("Detection_result.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, Size(frameWidth, frameHeight));
		Slider.SetRange(0, sumframe);//���û������ķ�Χ,Ϊ��Ƶ����֡��
		SetTimer(1,(unsigned int)(1000*1.0/capture.get(CV_CAP_PROP_FPS)),NULL); //��ʱ������ʱʱ���֡��һ��
		SetDlgItemText(IDC_BUTTON1,_T("�ر���Ƶ"));//�޸İ�ť��Caption
		CString csumframe;
		csumframe.Format("%d", sumframe);
		SetDlgItemText(IDC_EDIT2, _T(csumframe));  //��ʾ��֡��
	
	}
}

void CVideoPlayerDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString caption;
	GetDlgItemText(IDC_BUTTON2, caption);
	if (caption == _T("��ͣ"))    //�ر���Ƶ��ť
	{
		KillTimer(1);
		SetDlgItemText(IDC_BUTTON2, _T("���²���"));
		return;
	}
	else
	{
		SetTimer(1, (unsigned int)(1000 * 1.0 / capture.get(CV_CAP_PROP_FPS)), NULL); //��ʱ������ʱʱ���֡��һ��
		SetDlgItemText(IDC_BUTTON2, _T("��ͣ"));//�޸İ�ť��Caption
	}
}

Mat frame, gray, mask;  //ÿһ֡��ԭͼ���Ҷ�ͼ����ֵͼ
MOG_BGS Mog_Bgs;
vector<Feather> featherList;
Mat dst;

void CVideoPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{

	if(1 == nIDEvent)
	{

       //ȡ��һ֡ͼ��
		capture.read(frame);
			
		if (frame.empty())
		{
			KillTimer(1);
			MessageBox(_T("��Ƶ����"),_T("��ʾ"));
			capture.release();
			//writer.release();
			cvReleaseVideoWriter(&writer);
		}
		else
		{
			numframe+=1;
			cvtColor(frame, gray, CV_RGB2GRAY);

			//����ȡ������ͨ����ͼ���ÿһ֡�б�ʾ����
			for (vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++)
			{
				rectangle(frame, it->boundingbox, 255);
			}

			CvvImage m_CvvImage;
			IplImage frame(frame);   //Mat תIplImage


			m_CvvImage.CopyOf(&frame, 1); //���Ƹ�֡ͼ��   
			m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���
		
			Slider.SetPos((int)capture.get(CV_CAP_PROP_POS_FRAMES));//������Ƶ��λ��

			CString cnumframe;
			cnumframe.Format("%d", numframe);
			SetDlgItemText(IDC_EDIT1, _T(cnumframe));

			if (numframe == 1)
			{
				Mog_Bgs.init(gray);  //��ʼ��
				Mog_Bgs.processFirstFrame(gray);
				//cout << " Using " << TRAIN_FRAMES << " frames to training GMM..." << endl;
			}
			else if (numframe < TRAIN_FRAMES)
			{
				Mog_Bgs.trainGMM(gray);  //��ʼѵ��
			}
			else if (numframe == TRAIN_FRAMES)
			{
				Mog_Bgs.getFitNum(gray);   //����ѵ������
				//cout << " Training GMM complete!" << endl;
			}
			else
			{
				Mog_Bgs.testGMM(gray);   //��ʼ����
				mask = Mog_Bgs.getMask();
				Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));   //ͼ�񿪲���
				morphologyEx(mask, mask, CV_MOP_OPEN, element);

				int contours = bwLabel(mask, dst, featherList);  //��ͨ����ȡ

				CvvImage m_CvvImage1;
				IplImage mask(mask);   //Mat תIplImage
				m_CvvImage1.CopyOf(&mask, 1); //���Ƹ�֡ͼ��   
				m_CvvImage1.DrawToHDC(hDC1, &rect1); //��ʾ���豸�ľ��ο���

			    //�����ͼ�񱣴�Ϊ��Ƶ��
				cvWriteFrame(writer, &frame);

			}


		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CVideoPlayerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
   capture.set(CV_CAP_PROP_POS_FRAMES,Slider.GetPos());  //������Ƶ����ʼ֡
	*pResult = 0;
}


void CVideoPlayerDlg::OnStnClickedView()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CVideoPlayerDlg::OnStnClickedView2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



