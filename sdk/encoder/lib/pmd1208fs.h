#ifndef __PMD1208FS_H__
#define __PMD1208FS_H__
/* pmd1208fs.h  --  Header for libpmd1208fs library 
 * Copyright 2005 - 2015   Mark Colclough  */

#include <libusb-1.0/libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find a PMD1208FS device.  Return a device handle, or NULL. */
libusb_device_handle* pmd_find_first(void);

/* Close the PMD1208FS device. */
void pmd_close(libusb_device_handle* pmdhandle);

/* Flash the device LED.  Return 2 (bytes sent) or negative libusb error */
int pmd_flash(libusb_device_handle* pmdhandle);

/* Get the hardware serial number.  Returns a static string containing 8 chars */
char *pmd_serial(libusb_device_handle* pmdhandle);

/* Set the range for an analogue input channel and take one reading.
 * Succesful return values are is in the range -2048..+2047.
 * Errors return -2049+(negative usb error code) */
int pmd_ain(libusb_device_handle* pmdhandle, int channel, int range);

/* Perform an analogue input scan.  The last parameter, 'blocking',  determines
 * whether this function blocks until all the data have been received.
 * 
 * npts is the number of samples per channel,
 * interval is the time between successive samples of the same channel (us)
 * (interval = 0 means external clock), trigger =0 for internal trigger,
 * -1 for external, falling edge, +1 for external, rising edge.
 * data is an array of int16_t that the caller must provide for receiving the
 * data, and datsize is the size of that array.
 * Return value is 0 or a negative error code: either -EINVAL, -ENOMEM, -EIO,
 * -EAGAIN or a libusb error.
 */
int pmd_ainscan(libusb_device_handle* pmdhandle,
          int lowch, int highch, int npts, int interval, int trigger,
          int16_t *data, int datsize, int blocking);

/* Return the number of bytes outstanding in the current ainscan.  Zero
 * signifies proper completion, so the output data are now ready.  Negative is
 * either a detected error (-EIO), or some equally erroneous bad counting */
int pmd_ainawaited(libusb_device_handle* pmdhandle);

/* Return the number of active USB transfers.  If it is not zero, ainscan
 * will return -EAGAIN.  This goes to zero a short while after ainawaited
 * goes to zero -- up to a few seconds if an ainkill was issued */
int pmd_ainactive(libusb_device_handle* pmdhandle);

/* Force a non-blocking ainscan to finish.  Check pmd_ainactive to see
 * when it has.   Automatically issues ainstop first */
void pmd_ainkill(libusb_device_handle* pmdhandle);

/* Tell analogue input to stop sending. Returns 2 (bytes sent) or negative libusb error.
 * Should not be needed in user code */
int pmd_ainstop(libusb_device_handle* pmdhandle);

int pmd_ainactive(libusb_device_handle* pmdhandle);
    
/* Send a value to an analogue output channel (0 or 1).  Value 0...4095.
 * Returns 4 (bytes sent) or a negative libusb error */
int pmd_aout(libusb_device_handle* pmdhandle, int channel, int value);

/* Configure digital port A(0) or B(1) for input(1) or output(0) (bytewise only)
 * Returns 3 (bytes sent) or a negative libusb error */
int pmd_digconf(libusb_device_handle* pmdhandle, int port, int direction);

/* Set an output byte on digital port A(0) or B(1).
 * Returns 3 (bytes sent) or a negative libusb error */
int pmd_digout(libusb_device_handle* pmdhandle, int port, int data);

/* Read a byte from digital port A(0) or B(1).
 * Returns 0-255 (or junk if the read failed) */
int pmd_digin(libusb_device_handle* pmdhandle, int port);

/* Read 16 bits from both digital ports at once.
 * Returns 0-65535 (or junk if the read failed) */
int pmd_digin16(libusb_device_handle* pmdhandle);
    
/* Zero the counter. Returns 2 (bytes sent) or a negative libusb error */
int pmd_crst(libusb_device_handle* pmdhandle);

/* Read the counter. Returns the count or junk in case of error */
unsigned long pmd_cin(libusb_device_handle* pmdhandle);

/* Translate a libusb error into a static string */
char* usb_get_errmsg(int errcode);

/* Return the version of this library */
char* pmd_version(void);
    
#ifdef __cplusplus
}
#endif

#endif /* __PMD1208FS_H__ */
