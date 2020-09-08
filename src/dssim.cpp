/*
	Copyright 2005, 2009 The Developers
	This program is free software; see LICENSE.txt for details.

	This DSSIM program has been created by Philipp Klaus Krause based on
	Rabah Mehdi's C++ implementation of SSIM <http://mehdi.rabah.free.fr/SSIM>.
	Originally it has been created for the VMV '09 paper
	"ftc - floating precision texture compression" by Philipp Klaus Krause.

	The last version of this program was found at <http://www.colecovision.eu>.

	DSSIM is described in
	"Structural Similarity-Based Object Tracking in Video Sequences" by Loza et al.
	however setting all Ci to 0 as proposed there results in numerical instabilities.
	Thus this implementation used the Ci from the SSIM implementation.
	SSIM is described in
	"Image quality assessment: from error visibility to structural similarity" by Wang et al.
*/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: dssim image0 image1\n";
		return 0;
	}

	// Default settings
	const double C1 = 6.5025, C2 = 58.5225;

	/***************************** INITS **********************************/
	cv::Mat img1_temp = cv::imread(argv[1]);
	cv::Mat img2_temp = cv::imread(argv[2]);

	if (!img1_temp.data)
	{
		std::cerr << "Could not read image file " << argv[1] << "\n";
		return -1;
	}

	if (!img2_temp.data)
	{
		std::cerr << "Could not read image file " << argv[2] << "\n";
		return -1;
	}

	cv::Mat img1; img1_temp.convertTo(img1, CV_32F);
	cv::Mat img2; img2_temp.convertTo(img2, CV_32F);
	if (img1.rows * img1.cols > img2.rows * img2.cols)
		cv::resize(img2, img2, img1.size());
	else
		cv::resize(img1, img1, img2.size());	

	cv::Mat img1_sq; cv::pow(img1, 2, img1_sq);
	cv::Mat img2_sq; cv::pow(img2, 2, img2_sq);
	cv::Mat img1_img2 = img1.mul(img2);

	/*************************** END INITS **********************************/

	//////////////////////////////////////////////////////////////////////////
	// PRELIMINARY COMPUTING
	cv::Mat mu1; cv::GaussianBlur(img1, mu1, cv::Size(11, 11), 1.5);
	cv::Mat mu2; cv::GaussianBlur(img2, mu2, cv::Size(11, 11), 1.5);

	cv::Mat mu1_sq; cv::pow(mu1, 2, mu1_sq);
	cv::Mat mu2_sq; cv::pow(mu2, 2, mu2_sq);
	cv::Mat mu1_mu2 = mu1.mul(mu2);

	cv::Mat sigma1_sq; cv::GaussianBlur(img1_sq, sigma1_sq, cv::Size(11, 11), 1.5);
	cv::addWeighted(sigma1_sq, 1, mu1_sq, -1, 0, sigma1_sq);

	cv::Mat sigma2_sq; cv::GaussianBlur(img2_sq, sigma2_sq, cv::Size(11, 11), 1.5);
	cv::addWeighted(sigma2_sq, 1, mu2_sq, -1, 0, sigma2_sq);

	cv::Mat sigma12; cv::GaussianBlur(img1_img2, sigma12, cv::Size(11, 11), 1.5);
	cv::addWeighted(sigma12, 1, mu1_mu2, -1, 0, sigma12);

	//////////////////////////////////////////////////////////////////////////
	// FORMULA

	// (2*mu1_mu2 + C1)
	cv::Mat temp1; mu1_mu2.convertTo(temp1, -1, 2.0);
	cv::add(temp1, cv::Scalar::all(C1), temp1 );

	// (2*sigma12 + C2)
	cv::Mat temp2; sigma12.convertTo(temp2, -1, 2.0);
	cv::add(temp2, cv::Scalar::all(C2), temp2);

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
	cv::Mat temp3 = temp1.mul(temp2);

	// (mu1_sq + mu2_sq + C1)
	cv::add(mu1_sq, mu2_sq, temp1);
	cv::add(temp1, cv::Scalar::all(C1), temp1);

	// (sigma1_sq + sigma2_sq + C2)
	cv::add(sigma1_sq, sigma2_sq, temp2);
	cv::add(temp2, cv::Scalar::all(C2), temp2);

	// ((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	temp1 = temp1.mul(temp2);

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	cv::Mat ssim_map = temp3.mul(1.0 / temp1);

	cv::Scalar index_scalar = cv::mean(ssim_map);

	double dssim = index_scalar.val[0];
	for (unsigned int i = 1; i < ssim_map.channels(); i++)
		dssim = std::min(dssim, index_scalar.val[i]);
	dssim = 1.0 / dssim - 1;

	std::cout.precision(3);
	std::cout << argv[1] << " " << argv[2] << " : " << fixed << dssim << "\n";

	return 0;
}

