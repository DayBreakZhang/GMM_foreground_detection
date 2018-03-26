#include "StdAfx.h"
#include "GMM.h"

MOG_BGS::MOG_BGS(void)
{

}

MOG_BGS::~MOG_BGS(void)
{

}

// ȫ����ʼ��Ϊ0
void MOG_BGS::init(const Mat _image)
{
	//������ʼ��
	for (int i = 0; i < GMM_MAX_COMPONT; i++)
	{
		m_weight[i] = Mat::zeros(_image.size(), CV_32FC1);
		m_mean[i] = Mat::zeros(_image.size(), CV_8UC1);
		m_sigma[i] = Mat::zeros(_image.size(), CV_32FC1);
	}
	m_mask = Mat::zeros(_image.size(), CV_8UC1);
	m_fit_num = Mat::ones(_image.size(), CV_8UC1);
}

//gmm��һ֡��ʼ������ʵ��
//���񵽵�һ֡ʱ�Ը�˹�ֲ����г�ʼ������Ҫ������ÿ����˹�ֲ���Ȩֵ�������ͷ����ֵ��
//���е�һ����˹�ֲ���ȨֵΪ1������Ϊ��һ���������ݣ������˹�ֲ�ȨֵΪ0������Ϊ0��
//ÿ����˹�ֲ����������ʵ�����ȵĳ�ʼ���� 15
void MOG_BGS::processFirstFrame(const Mat _image)
{
	for (int i = 0; i < GMM_MAX_COMPONT; i++)
	{
		if (i == 0)
		{
			m_weight[i].setTo(1.0);
			_image.copyTo(m_mean[i]);
			m_sigma[i].setTo(15.0);
		}
		else
		{
			m_weight[i].setTo(0.0);
			m_mean[i].setTo(0);
			m_sigma[i].setTo(15.0);
		}
	}
}

// ͨ���µ�֡��ѵ��GMM
void MOG_BGS::trainGMM(const Mat _image)
{
	for (int i = 0; i < _image.rows; i++)
	{
		for (int j = 0; j < _image.cols; j++)
		{
			int num_fit = 0;

			for (int k = 0; k < GMM_MAX_COMPONT; k++)
			{
				int delm = abs(_image.at<uchar>(i, j) - m_mean[k].at<uchar>(i, j));
				long dist = delm * delm;
				// �ж��Ƿ�ƥ�䣺����ֵ���˹�ֲ��ľ�ֵ�ľ���С��3�������ʾƥ�䣩
				if (dist < 3.0 * m_sigma[k].at<float>(i, j))
				{
					// ���ƥ��
					/****����Ȩ��****/
					m_weight[k].at<float>(i, j) += GMM_LEARN_ALPHA * (1 - m_weight[k].at<float>(i, j));

					/****���¾�ֵ****/
					//m_mean[k].at<uchar>(i, j) += (GMM_LEARN_ALPHA /m_weight[k].at<uchar>(i, j)) * delm;
					m_mean[k].at<uchar>(i, j) += (uchar)(GMM_LEARN_ALPHA / m_weight[k].at<float>(i, j)) * delm;

					/****���·���****/
					m_sigma[k].at<float>(i, j) += (GMM_LEARN_ALPHA / m_weight[k].at<float>(i, j)) * (dist - m_sigma[k].at<float>(i, j));
				}
				else
				{
					// �����ƥ�䡣��øø�˹ģ�͵�Ȩֵ��С
					m_weight[k].at<float>(i, j) += GMM_LEARN_ALPHA * (0 - m_weight[k].at<float>(i, j));
					num_fit++; // ��ƥ���ģ�͸���
				}
			}

			//��gmm������˹��������,�Ӵ�С����,��������Ϊ weight / sigma
			for (int kk = 0; kk < GMM_MAX_COMPONT; kk++)
			{
				for (int rr = kk; rr< GMM_MAX_COMPONT; rr++)
				{
					if (m_weight[rr].at<float>(i, j) / m_sigma[rr].at<float>(i, j) > m_weight[kk].at<float>(i, j) / m_sigma[kk].at<float>(i, j))
					{
						//Ȩֵ����
						float temp_weight = m_weight[rr].at<float>(i, j);
						m_weight[rr].at<float>(i, j) = m_weight[kk].at<float>(i, j);
						m_weight[kk].at<float>(i, j) = temp_weight;

						//��ֵ����
						uchar temp_mean = m_mean[rr].at<uchar>(i, j);
						m_mean[rr].at<uchar>(i, j) = m_mean[kk].at<uchar>(i, j);
						m_mean[kk].at<uchar>(i, j) = temp_mean;

						//�����
						float temp_sigma = m_sigma[rr].at<float>(i, j);
						m_sigma[rr].at<float>(i, j) = m_sigma[kk].at<float>(i, j);
						m_sigma[kk].at<float>(i, j) = temp_sigma;
					}
				}
			}

			if (num_fit == GMM_MAX_COMPONT && 0 == m_weight[GMM_MAX_COMPONT - 1].at<float>(i, j))
			{
				//������ֵ���ֵ�ʱ����Ŀǰ�ֲ�����С��M������һ���ֲ������²���ֵ��Ϊ��ֵ��������ϴ󷽲�ͽ�СȨֵ
				for (int k = 0; k < GMM_MAX_COMPONT; k++)
				{
					if (0 == m_weight[k].at<float>(i, j))
					{
						m_weight[k].at<float>(i, j) = GMM_LEARN_ALPHA;
						m_mean[k].at<uchar>(i, j) = _image.at<uchar>(i, j);
						m_sigma[k].at<float>(i, j) = 15.0;

						for (int q = 0; q < GMM_MAX_COMPONT && q != k; q++)
						{
							//�������ĸ�˹ģ�͵�Ȩֵ���и��£�����Ȩֵ��Ϊ1
							m_weight[q].at<float>(i, j) *= (1 - GMM_LEARN_ALPHA);
						}
						break; //�ҵ���һ��Ȩֵ��Ϊ0�ļ���
					}
				}
			}
			else if (num_fit == GMM_MAX_COMPONT && m_weight[GMM_MAX_COMPONT - 1].at<float>(i, j) != 0)
			{
				//���GMM_MAX_COMPONT��������ֵ�������������ĸ�˹����Ȩֵ�����ĸ�˹��Ȩֵ���䣬ֻ���¾�ֵ�ͷ���
				m_mean[GMM_MAX_COMPONT - 1].at<uchar>(i, j) = _image.at<uchar>(i, j);
				m_sigma[GMM_MAX_COMPONT - 1].at<float>(i, j) = 15.0;
			}
		}
	}
}

//������ͼ��ÿ������gmmѡ����ʵĸ�˹��������
//��������п����Ǳ����ֲ���������ǰ�棬��С���ܵĶ��ݵķֲ�������ĩ�ˣ����ǽ�������ǰfit_num���ֲ�ѡΪ����ģ��;
//���Ź���ķֲ��У��ۻ����ʳ���GMM_THRESHOD_SUMW��ǰfit_num���ֲ�����������ģ�ͣ�ʣ��������ֲ�������ǰ��ģ�ͣ�
void MOG_BGS::getFitNum(const Mat _image)
{
	for (int i = 0; i < _image.rows; i++)
	{
		for (int j = 0; j < _image.cols; j++)
		{
			float sum_w = 0.0;	//���¸�ֵΪ0������һ���������ۻ�
			for (uchar k = 0; k < GMM_MAX_COMPONT; k++)
			{
				sum_w += m_weight[k].at<float>(i, j);
				if (sum_w >= GMM_THRESHOD_SUMW)	//�������THRESHOD_SUMW=0.6�Ļ�����ô�õ��ĸ�˹��Ŀ��Ϊ1����Ϊÿ�����ض���һ��Ȩֵ�ӽ�1
				{
					m_fit_num.at<uchar>(i, j) = k + 1;
					break;
				}
			}
		}
	}
}

//gmm���Ժ�����ʵ��
void MOG_BGS::testGMM(const Mat _image)
{
	for (int i = 0; i < _image.rows; i++)
	{
		for (int j = 0; j < _image.cols; j++)
		{
			int k = 0;
			for (; k < m_fit_num.at<uchar>(i, j); k++)
			{
				if (abs(_image.at<uchar>(i, j) - m_mean[k].at<uchar>(i, j)) < (uchar)(2.5 * m_sigma[k].at<float>(i, j)))
				{
					m_mask.at<uchar>(i, j) = 0;
					break;
				}
			}
			if (k == m_fit_num.at<uchar>(i, j))
			{
				m_mask.at<uchar>(i, j) = 255;
			}
		}
	}
}

/*
Input:
src: �������ͨ��Ķ�ֵ��ͼ��
Output:
dst: ��Ǻ��ͼ��
featherList: ��ͨ���������嵥
return��
��ͨ��������
*/
int bwLabel(Mat & src, Mat & dst, vector<Feather> & featherList)
{
	int rows = src.rows;
	int cols = src.cols;

	int labelValue = 0;
	Point seed, neighbor;
	stack<Point> pointStack;    // ��ջ  

	int area = 0;               // ���ڼ�����ͨ������  
	int leftBoundary = 0;       // ��ͨ�����߽磬�������С���ε���߿򣬺�����ֵ����������  
	int rightBoundary = 0;
	int topBoundary = 0;
	int bottomBoundary = 0;
	Rect box;                   // ��Ӿ��ο�  
	Feather feather;

	featherList.clear();    // �������  

	dst.release();
	dst = src.clone();
	for (int i = 0; i < rows; i++)
	{
		uchar *pRow = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			if (pRow[j] == 255)
			{
				area = 0;
				labelValue++;           // labelValue���Ϊ254����СΪ1.  
				seed = Point(j, i);     // Point�������꣬�����꣩  
				dst.at<uchar>(seed) = labelValue;
				pointStack.push(seed);

				area++;
				leftBoundary = seed.x;
				rightBoundary = seed.x;
				topBoundary = seed.y;
				bottomBoundary = seed.y;

				while (!pointStack.empty())
				{
					neighbor = Point(seed.x + 1, seed.y);
					if ((seed.x != (cols - 1)) && (dst.at<uchar>(neighbor) == 255))
					{
						dst.at<uchar>(neighbor) = labelValue;
						pointStack.push(neighbor);

						area++;
						if (rightBoundary < neighbor.x)
							rightBoundary = neighbor.x;
					}

					neighbor = Point(seed.x, seed.y + 1);
					if ((seed.y != (rows - 1)) && (dst.at<uchar>(neighbor) == 255))
					{
						dst.at<uchar>(neighbor) = labelValue;
						pointStack.push(neighbor);

						area++;
						if (bottomBoundary < neighbor.y)
							bottomBoundary = neighbor.y;

					}

					neighbor = Point(seed.x - 1, seed.y);
					if ((seed.x != 0) && (dst.at<uchar>(neighbor) == 255))
					{
						dst.at<uchar>(neighbor) = labelValue;
						pointStack.push(neighbor);

						area++;
						if (leftBoundary > neighbor.x)
							leftBoundary = neighbor.x;
					}

					neighbor = Point(seed.x, seed.y - 1);
					if ((seed.y != 0) && (dst.at<uchar>(neighbor) == 255))
					{
						dst.at<uchar>(neighbor) = labelValue;
						pointStack.push(neighbor);

						area++;
						if (topBoundary > neighbor.y)
							topBoundary = neighbor.y;
					}

					seed = pointStack.top();
					pointStack.pop();
				}
				box = Rect(leftBoundary, topBoundary, rightBoundary - leftBoundary, bottomBoundary - topBoundary);
				//rectangle(src, box, 255);
				feather.area = area;
				feather.boundingbox = box;
				feather.label = labelValue;
				// ɾ������С����ͨ��
				if (feather.area >= 50)
				{
					featherList.push_back(feather);
				}

			}
		}
	}
	return labelValue;
}