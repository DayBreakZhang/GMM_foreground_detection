#pragma once
#pragma once
#include "StdAfx.h"
#include <vector>  
#include <stack>  
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

//����gmmģ���õ��ı���
#define GMM_MAX_COMPONT 6			//ÿ��GMM���ĸ�˹ģ�͸���
#define GMM_LEARN_ALPHA 0.005      //ѧϰ����
#define GMM_THRESHOD_SUMW 0.8      //ǰ�����ٽ���ֵ
#define TRAIN_FRAMES 60	        //��ǰ TRAIN_FRAMES ֡��ģ

class MOG_BGS
{
public:
	MOG_BGS(void);
	~MOG_BGS(void);

	void init(const Mat _image);
	void processFirstFrame(const Mat _image);
	void trainGMM(const Mat _image);
	void getFitNum(const Mat _image);
	void testGMM(const Mat _image);
	Mat getMask(void) { return m_mask; };

private:
	Mat m_weight[GMM_MAX_COMPONT];  //Ȩֵ
	Mat m_mean[GMM_MAX_COMPONT];    //��ֵ
	Mat m_sigma[GMM_MAX_COMPONT];   //����

	Mat m_mask;
	Mat m_fit_num;
};
// ��ͨ����ȡ����
typedef struct _Feather
{
	int label;              // ��ͨ���labelֵ  
	int area;               // ��ͨ������  
	Rect boundingbox;       // ��ͨ�����Ӿ��ο�  
} Feather;

int bwLabel(Mat & src, Mat & dst, vector<Feather> & featherList);