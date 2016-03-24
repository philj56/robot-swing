#include "encoder.h"

Encoder::Encoder()
{
    cal = 0;
    libusb_device_handle * mypmd = pmd_find_first();
    handle = mypmd;
}
