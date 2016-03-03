#include <stdio.h>
#include <stdlib.h>

#include <libusb/libusb.h>

#define PMD_VID 0x09db
#define PMD_PID 0x0082

int main(void) {
    int i, ret;
    libusb_device_handle* pmd;
    
    
    ret = libusb_init(NULL);
    if (ret < 0) {
        return ret;
    }
    
    pmd = libusb_open_device_with_vid_pid(NULL, PMD_VID, PMD_PID);
    
    if (!pmd) {
        printf("device not found\n");
        exit(EXIT_FAILURE);
    }
    
    printf("found!\n");
    
    /* There is only one config, but we must prise the interfaces from
     * the kernel HID driver */
    for(i = 0; i < 4; i++) {
        libusb_detach_kernel_driver(pmd, i);
        ret = libusb_claim_interface(pmd, i);
        if (ret) {
            printf("failed to claim interface %d : error %d\n", i, ret);
            exit(EXIT_FAILURE);
        }
    }
    
    /* this is a flash, which works */
    unsigned char msgbuf[64];
    msgbuf[0] = 0x40;
    msgbuf[1] = 0x00;
    uint16_t msgsize = 2;


ret = libusb_control_transfer(pmd, 
0x21, //bmRequestType,
0x09, //bRequest,
0x200+msgbuf[0], //wValue,
0, //wIndex,
msgbuf, //*data
msgsize, //wLength
5000//int timeout
);

printf("flash returned %d\n", ret);




    libusb_close(pmd);

	libusb_exit(NULL);
	return 0;
}

#if 0
1 config
4 interfaces:
0 0x81  EP 1 IN  Interrupt  64 bytes
1 0x02  EP 2 OUT Interrupt 64 bytes; 0x83  EP 3 IN Interrupt 64 bytes
2 0x84  EP 4 IN Interrupt  64 bytes 
3 0x85  EP 5 IN Interrupt 64 bytes
all HID
#endif
