// -*- C++ -*-
/*!
 * @file  RGBDCameraTester.cpp
 * @brief RGBDCameraImage Test Component
 * @date $Date$
 *
 * $Id$
 */

#include "RGBDCameraTester.h"

// Module specification
// <rtc-template block="module_spec">
static const char* rgbdcameratester_spec[] =
  {
    "implementation_id", "RGBDCameraTester",
    "type_name",         "RGBDCameraTester",
    "description",       "RGBDCameraImage Test Component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Senso",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RGBDCameraTester::RGBDCameraTester(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rgbdIn("rgbd", m_rgbd)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RGBDCameraTester::~RGBDCameraTester()
{
}



RTC::ReturnCode_t RGBDCameraTester::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("rgbd", m_rgbdIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RGBDCameraTester::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RGBDCameraTester::onActivated(RTC::UniqueId ec_id)
{
	cv::namedWindow("Source Image");
	cv::namedWindow("Depth Image");
  return RTC::RTC_OK;
}



RTC::ReturnCode_t RGBDCameraTester::onDeactivated(RTC::UniqueId ec_id)
{
	cv::destroyAllWindows();
  return RTC::RTC_OK;
}



RTC::ReturnCode_t RGBDCameraTester::onExecute(RTC::UniqueId ec_id)
{
	if (m_rgbdIn.isNew()) {
		m_rgbdIn.read();
		if (convertImgToCvMat(m_rgbd.data.cameraImage, m_srcImage)) {
			cv::imshow("Source Image", m_srcImage);

		}

		cv::Mat depthImage(m_rgbd.data.depthImage.height, m_rgbd.data.depthImage.width, CV_8UC1);
		double min_distance = 0.2;
		double max_distance = 2.0;
		for (int i = 0; i < m_rgbd.data.depthImage.height; i++) {
			for (int j = 0; j < m_rgbd.data.depthImage.width; j++) {
				int index = i * m_rgbd.data.depthImage.width + j;
				double distance = m_rgbd.data.depthImage.raw_data[index];
				if (distance < min_distance) { distance = min_distance; }
				else if (distance > max_distance) { distance = max_distance; }
				depthImage.data[index] = (m_rgbd.data.depthImage.raw_data[index] - min_distance) / max_distance * 255;
			}
		}
		cv::imshow("Depth Image", depthImage);
		cv::waitKey(1);
	}
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t RGBDCameraTester::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RGBDCameraTester::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RGBDCameraTesterInit(RTC::Manager* manager)
  {
    coil::Properties profile(rgbdcameratester_spec);
    manager->registerFactory(profile,
                             RTC::Create<RGBDCameraTester>,
                             RTC::Delete<RGBDCameraTester>);
  }
  
};


