/**
 * @file pmd1208fs.h
 *
 * @brief Header for libpmd1208fs library
 *
 * @author Mark Colclough
 * @date 2005-2016
 * @copyright Dr. Mark Colclough, University of Birmingham.
 */

#ifndef __PMD1208FS_H__
#define __PMD1208FS_H__

#include <libusb-1.0/libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Find a PMD1208FS device. Return a device handle or NULL.
	 *
	 * @return Handle to device, or NULL if none found.
	 */
	libusb_device_handle* pmd_find_first(void);

	/**
	 * @brief Close the PMD1208FS device.
	 *
	 * @param pmdhandle Handle to PMD1208FS device to close.
	 */
	void pmd_close(libusb_device_handle* pmdhandle);

	/* Flash the device LED.  Return 2 (bytes sent) or negative libusb error */

	/**
	 * @brief Flash the device LED. Return 2 (bytes sent) or negative libusb error.
	 *
	 * @param pmdhandle Handle to PMD1208FS device to flash.
	 * @return 2 if bytes sent or negative libusb error.
	 */
	int pmd_flash(libusb_device_handle* pmdhandle);

	/* Get the hardware serial number.  Returns a static string containing 8 chars */

	/**
	 * @brief Gets the hardware serial number. Returns a static string containing 8 chars.
	 *
	 * @param pmdhandle Handle to PMD1028FS device.
	 * @return char array containing 8 chars.
	 */
	char *pmd_serial(libusb_device_handle* pmdhandle);

	/* Set the range for an analogue input channel and take one reading.
	* Succesful return values are is in the range -2048..+2047.
	* Errors return -2049+(negative usb error code) */

	/**
	 * @brief Set the range for an analogue input channel and take one reading.
	 *
	 * @remark Successful return values are in the range -2048 <-> +2047. Errors return
	 *		   -2049+ (negative usb error code).
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param channel Analogue input channel 
	 * @param range Range to set for channel
	 * @return Value code, see remark above for details.
	 */
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

	/**
	 * @brief Perform an analogue input scan.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param lowch Low of channel
	 * @param highch High of channel
	 * @param npts Number of samples per channel
	 * @param interval Time betwwen successive samples of the same channel (micro-seconds)
	 * @param trigger Triggere = 0 for internal trigger, -1 for external falling edge or +1 for external rising edge.
	 * @param data Array of int16_t that the caller must provide for receiving the data
	 * @param datsize Size of data array
	 * @param blocking Determins whether this function blocks until all the data have been received.
	 * @return Return value is 0 or a negative error code: either -EINVAL, -ENOMEM, -EIO, EAGAIN or a libusberror.
	 */
	int pmd_ainscan(libusb_device_handle* pmdhandle,
		int lowch, int highch, int npts, int interval, int trigger,
		int16_t *data, int datsize, int blocking);

	/* Return the number of bytes outstanding in the current ainscan.  Zero
	* signifies proper completion, so the output data are now ready.  Negative is
	* either a detected error (-EIO), or some equally erroneous bad counting */

	/**
	 * @brief Return the number of bytes outstanding in the current ainscan. Zero signifies
	 *		  proper completion, so the output data are now ready. Negative is either a detected
	 *		  error (-EIO), or some equally erroneous bad counting.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 */
	int pmd_ainawaited(libusb_device_handle* pmdhandle);

	/* Return the number of active USB transfers.  If it is not zero, ainscan
	* will return -EAGAIN.  This goes to zero a short while after ainawaited
	* goes to zero -- up to a few seconds if an ainkill was issued */

	/**
	 * @brief Return the number of active USB transfers. If it is not zero, ainscan
	 *		  will return -EAGAIN. This goes to zero a short while after ainawaited
	 *		  goes to zero -- up to a few seconds if an ainkill was issued.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @return Number of active USB transfers.
	 */
	int pmd_ainactive(libusb_device_handle* pmdhandle);

	/**
	 * @brief Force a non-blocking ainscan to finish. Check pmd_ainactive to
	 *		  see when it has. Automatically issues ainstop first.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 */
	void pmd_ainkill(libusb_device_handle* pmdhandle);

	/**
	 * @brief Tell analogue input to stop sending. Returns 2 (bytes sent) or negative libusb error.
	 *		  Should not be needed in user code.
	 *
	 * @return 2 if bytes sent or negative libusberror.
	 */
	int pmd_ainstop(libusb_device_handle* pmdhandle);

	int pmd_ainactive(libusb_device_handle* pmdhandle);

	/**
	 * @brief Send a value to an analogue output channel (0 or 1).
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param channel Channel number to sent value to
	 * @param value Value to send, 0 ... 4095.
	 * @return 4 (bytes sent) or a negative libusb error.
	 */
	int pmd_aout(libusb_device_handle* pmdhandle, int channel, int value);

	/* Configure digital port A(0) or B(1) for input(1) or output(0) (bytewise only)
	* Returns 3 (bytes sent) or a negative libusb error */

	/**
	 * @brief Configure digital port A(0) or B(1) for input(1) or output(0) (bytewise only)
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param port Port number
	 * @param direction Direction (input = 1, output = 0)
	 * @return 3 (bytes sent) or a negative libusb error.
	 */
	int pmd_digconf(libusb_device_handle* pmdhandle, int port, int direction);

	/**
	 * @brief Set an output byte on digital port A(0) or B(1).
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param port Port number
	 * @param data Data to set
	 * @return 3 (bytes sent) or a negative libusb error.
	 */
	int pmd_digout(libusb_device_handle* pmdhandle, int port, int data);

	/**
	 * @brief Read a byte from digital port A(0) or B(1).
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @param port Port number.
	 * @return 0-255 (or junk if the read failed).
	 */
	int pmd_digin(libusb_device_handle* pmdhandle, int port);

	/**
	 * @brief Read 16-bits from both digital ports at once.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @return 0-65535 (or junk if the read failed).
	 */
	int pmd_digin16(libusb_device_handle* pmdhandle);

	/**
	 * @brief Zero the counter.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @return 2 (bytes sent) or a negative libusb error.
	 */
	int pmd_crst(libusb_device_handle* pmdhandle);

	/**
	 * @brief Read the counter.
	 *
	 * @param pmdhandle Handle to PMD1208FS device.
	 * @return Returns the count or junk in case of error.
	 */
	unsigned long pmd_cin(libusb_device_handle* pmdhandle);

	/* Translate a libusb error into a static string */

	/**
	 * @brief Translate a libusb error into a static string.
	 *
	 * @param errcode Error code to translate
	 * @return char array of error code translation.
	 */
	char* usb_get_errmsg(int errcode);

	/**
	 * @brief Return the version of this library.
	 *
	 * @return char array giving version of library.
	 */
	char* pmd_version(void);

#ifdef __cplusplus
}
#endif

#endif /* __PMD1208FS_H__ */
