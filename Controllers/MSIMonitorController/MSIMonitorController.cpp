/*---------------------------------------------------------*\
| RGBController_MSIMonitor.cpp                              |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 May 16   |
|   Based off the LG monitor controller                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "MSIMonitorController.h"
#include "StringUtils.h"

MSIMonitorController::MSIMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

MSIMonitorController::~MSIMonitorController()
{
    hid_close(dev);
}

std::string MSIMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMonitorController::GetNameString()
{
    return(name);
}

std::string MSIMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MSIMonitorController::Set(uint8_t mode_value, const std::vector<RGBColor> colors)
{

    /*---------------------------------------------------------*\
    | Prepare the colors data                                   |
    \*---------------------------------------------------------*/
    uint8_t data[MSI_MONITOR_PACKET_SIZE];
    memset(data, 0x00, MSI_MONITOR_PACKET_SIZE);

    unsigned int offset = 0;

    data[offset] = 0x71'01'00'00'00'01'64'00'00'00'00'00
    offset += 12

    /*---------------------------------------------------------*\
    | put mode_value                                            |
    \*---------------------------------------------------------*/
    data[offset++] = mode_value

    //this data might be placeholder for additional LEDs in other monitors, idk
    data[offset] = 0x00'00'00'01'64'00'00'00'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00'ff'00'00
    offset += 37

    for(const RGBColor color: colors)
    {
        data[offset++] = RGBGetRValue(color);
        data[offset++] = RGBGetGValue(color);
        data[offset++] = RGBGetBValue(color);
    }

    //not sure what the last byte is, data packets show either 0x00 or 0x01
    data[offset++] = 0x01

    /*---------------------------------------------------------*\
    | Send the data (1 packet)                                  |
    \*---------------------------------------------------------*/
   
    hid_write(dev, buf, MSI_MONITOR_PACKET_SIZE);
}
