/*---------------------------------------------------------*\
| MSIKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for MSI  monitor (MSI Gaming Controller)       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "MSIMonitorController.h"
#include "RGBController_MSIMonitor.h"

#define MSI_USB_VID         0x1462
#define MSI_USB_PID         0x3FA4

/*----------------------------------------------------------*\
|                                                            |
|   DetectMSIMonitorController                               |
|                                                            |
|       Detect MSI monitor and maybe others                  |
|                                                            |
\*----------------------------------------------------------*/

void DetectMSIMonitorController
    (
    hid_device_info*    info,
    const std::string&  /*name*/
    )
{
    hid_device* dev = hid_open_path(info->path);
    if(dev != nullptr)
    {
        MSIMonitorController*     controller     = new MSIMonitorController(dev, info->path);
        RGBController_MSIMonitor* rgb_controller = new RGBController_MSIMonitor(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("MSI Gaming Controller", DetectMSIMonitorController, MSI_USB_VID, MSI_USB_PID, 0, 0x01, 0);