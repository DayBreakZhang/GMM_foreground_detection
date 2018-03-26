
// VideoPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "CvvImage.h"
#include "afxcmn.h"
using namespace cv;

// CVideoPlayerDlg �Ի���
class CVideoPlayerDlg : public CDialog
{
// ����
public:
	CVideoPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VIDEOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CSliderCtrl Slider;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	VideoCapture capture;
	CvVideoWriter* writer;
	HBITMAP hbitmap,hbitmap1;
	CRect rect,rect1;
	CStatic* pStc, *pStc1; //��ʶͼ����ʾ��Picture�ؼ�
	CDC* pDC, *pDC1; //��Ƶ��ʾ�ؼ��豸������
	HDC hDC, hDC1; //��Ƶ��ʾ�ؼ��豸���

	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedView();
	afx_msg void OnStnClickedView2();
	afx_msg void OnBnClickedButton2();
};
