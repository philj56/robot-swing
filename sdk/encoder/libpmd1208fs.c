/* libpmd1208fs.c  --  Library for talking to a Measurement Computing
 *                     USB data acquisition module, model USB1208FS
 *                     (formerly pmd1208fs)
 * 
 * Copyright 2005 - 2009 Mark Colclough
 */

/* TODO: ainkill works, but requires up to 2s (for libusb_handle_events to
 * return and find the artificial error) before ainactive will return zero. It 
 * would be better to cancel a transfer (/all transfers?) so libusb will see it
 * immediately, then the completion hander could set the error flag.  This would
 * mean modifying ainscan so the transfers always exist: for no good reason at
 * the moment, they are created and deleted a lot.  This would also reduce the
 * workload in ainscan.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "pmd1208fs.h"
//#include "version.h"

#define PMD_VID 0x09db
#define PMD_PID 0x0082
#define IN_PKT_SIZE 64
// multiple of 3 xfers. 6 is better than 3 when we have large no of pts (10000)
#define NUM_XFERS 6

#undef dbg
#ifdef DEBUG
#define dbg(arg...) printf(arg)
#else
#define dbg(arg...) do { } while (0)
#endif

#undef err
#define err(arg...) fprintf(stderr, arg)

#define UNUSED __attribute__((unused))

/* Translate a libusb error into a static string */
char* usb_get_errmsg(int errcode) {
    switch (errcode) {
        case LIBUSB_SUCCESS: return "usb success";
        case LIBUSB_ERROR_IO: return "usb io error";
        case LIBUSB_ERROR_INVALID_PARAM: return "usb param error";
        case LIBUSB_ERROR_ACCESS: return "usb access error";
        case LIBUSB_ERROR_NO_DEVICE: return "usb nodevice error";
        case LIBUSB_ERROR_NOT_FOUND: return "usb notfound error";
        case LIBUSB_ERROR_BUSY: return "usb busy error";
        case LIBUSB_ERROR_TIMEOUT: return "usb timeout error";
        case LIBUSB_ERROR_OVERFLOW: return "usb overflow error";
        case LIBUSB_ERROR_PIPE: return "usb pipe error";
        case LIBUSB_ERROR_INTERRUPTED: return "usb interrupted error";
        case LIBUSB_ERROR_NO_MEM: return "usb nomem error";
        case LIBUSB_ERROR_NOT_SUPPORTED: return "usb not supported error";
        case LIBUSB_ERROR_OTHER: return "usb mystery error";
        default: return "usb unknown error";
    }
}

/* Send to control endpoint.
 * Returns number of bytes sent or negative LIBUSB_ERROR_* */
static int send_control(libusb_device_handle* pmdhandle,
                        unsigned char msg[],
                        uint16_t msgsize,
                        unsigned int timeout) {
    int ret;
    ret = libusb_control_transfer(pmdhandle, 
        0x21, //bmRequestType,
        0x09, //bRequest,
        0x200+msg[0], //wValue,
        0, //wIndex,
        msg, //*data
        msgsize, //wLength
        timeout );//int timeout
    //dbg("send_control sent %d of %d: 0x%x 0x%x 0x%x 0x%x 0x%x ...\n",
    //        ret, msgsize, msg[0], msg[1], msg[2], msg[3], msg[4]);
    if (ret < 0) {
        err("failed to send control %x: %s code %d\n", msg[0], usb_get_errmsg(ret), ret);
    }
    return ret;
}

/* Read from endpoint 0x81 (ep 1 in) into a buffer with timeout in ms.
 * Returns zero for success or negative LIBUSB_ERROR_* */
static int get_data(libusb_device_handle* pmdhandle,
                    unsigned char* data,
                    int maxlength,
                    int* receivedlength,
                    unsigned int timeout) {
    int ret;
    ret = libusb_bulk_transfer(pmdhandle, 0x81, data, maxlength,
                                receivedlength, timeout);
    //dbg("get_data received %d (max %d): 0x%x 0x%x 0x%x 0x%x 0x%x ...\n",
    //    *receivedlength, maxlength, data[0], data[1], data[2], data[3], data[4]);
    if (ret) {
        err("failed to get from ep 0x81: %s\n", usb_get_errmsg(ret));
    }
    return ret;
}


/* Find a PMD1208FS device.  Return a device handle, or NULL. */
libusb_device_handle* pmd_find_first(void) {
    int i, ret;
    libusb_device_handle* pmd;
    
    ret = libusb_init(NULL);
    if (ret) {
        err("failed to init libusb: %s\n", usb_get_errmsg(ret));
        return NULL;
    }
    
    pmd = libusb_open_device_with_vid_pid(NULL, PMD_VID, PMD_PID);
    if (!pmd) {
        err("device not found\n");
        libusb_exit(NULL);
        return NULL;
    }
    
    for(i = 0; i < 4; i++) {
        libusb_detach_kernel_driver(pmd, i);
    }
    libusb_reset_device(pmd);  /* fixes odd-number-of-operations timeout */
    libusb_set_configuration(pmd, 1);
    for(i = 0; i < 4; i++) {
        ret = libusb_claim_interface(pmd, i);
        if (ret) {
            err("failed to claim interface: %s\n", usb_get_errmsg(ret));
            libusb_close(pmd);
            libusb_exit(NULL);
            return NULL;
        }
    }
    return pmd;
}

/* Close the PMD1208FS device. */
void pmd_close(libusb_device_handle* pmdhandle) {
    int i, ret;
    for(i = 0; i < 4; i++) {
        ret = libusb_release_interface(pmdhandle, i);
        if (ret) {
            err("pmd_close: %s releasing if %d\n", usb_get_errmsg(ret), i);
        }
    }
    libusb_close(pmdhandle);
    libusb_exit(NULL);
}

/* Flash the device LED.  Return 2 (bytes sent) or negative libusb error */
int pmd_flash(libusb_device_handle* pmdhandle) {
    unsigned char msgbuf[64] = {0x40, 0x00};
    uint16_t msgsize = 2;
    return send_control(pmdhandle, msgbuf, msgsize, 3000);
}

/* Get the hardware serial number.  Returns a static string containing 8 chars */
char *pmd_serial(libusb_device_handle* pmdhandle) {
    int ret, rxlength;
    static char serialno[9];
    unsigned char msgbuf[64] = {0x55, 0x00, 0x00, 0x20, 0x08};
    uint16_t msgsize = 5;
    ret = send_control(pmdhandle, msgbuf, msgsize, 3000);
    if (ret < 0) {
        return "????????";
    }
    ret = get_data(pmdhandle, msgbuf, 64, &rxlength, 5000);
    if (ret) {
        return "????????";
    }
    strncpy(serialno, (char*)msgbuf+1, 8);
    serialno[8] = '\0';
    return serialno;
}

/* Set the range for an analogue input channel and take one reading.
 * Succesful return values are is in the range -2048..+2047.
 * Errors return -2049+(negative usb error code) */
int pmd_ain(libusb_device_handle* pmdhandle, int channel, int range) {
    unsigned char msgbuf[64];
    int ret, rxlength;
    if (channel < 0 || channel > 3 || range < 0 || range > 7) {
        err("invalid ain argument\n");
        return -2049 + LIBUSB_ERROR_INVALID_PARAM;
    }
    msgbuf[0] = 0x10;
    msgbuf[1] = (unsigned char) channel;
    msgbuf[2] = (unsigned char) range;
    ret = send_control(pmdhandle, msgbuf, 3, 3000);
    if (ret < 0) {
        return -2049 + ret;
    }
    
    ret = get_data(pmdhandle, msgbuf, 64, &rxlength, 5000);
    if (ret < 0) {
        return -2049 + ret;
    }
    return (*(int16_t*)(msgbuf+1))/16;
}

/* Tell analogue input to stop sending. Returns 2 (bytes sent) or negative libusb error
 * Should not be needed in user code */
int pmd_ainstop(libusb_device_handle* pmdhandle) {
    unsigned char msgbuf[64] = {0x12, 0x00};
    uint16_t msgsize = 2;
    return(send_control(pmdhandle, msgbuf, msgsize, 3000));
}

/* Globals  -- there can only ever be one scan in progress, and one set of xfers for it */
struct ainscan_control {
    int bytes_awaited;
    uint16_t last_pkt_seqno;
    int last_pkt_bytes;
    int error;
    int active_transfers;
    int16_t *data;
    int datsize;
} scancontrol;

struct libusb_transfer *xfers[NUM_XFERS];

/* prototypes for transfers */
static void xfer_cb(struct libusb_transfer *xfer);
void* ainscan_worker(void* arg);

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
          int16_t *data, int datsize, int blocking) {
    int nchannels;
    int totalpts;   /* overall number of conversions */
    int adctime;    /* us per conversion */
    long int timer; /* adc timer fine setting */
    int pre;        /* adc timer prescaler */
    unsigned char syncopts; /* sync options */
    unsigned char scanopts; /* scan options */
    unsigned char trigopts; /* trigger options */
    unsigned char msgbuf[64];
    int ret, i, returncode;
    /* scancontrol is global, to allow for nonblocking case */
    
    /* Segfault city awaits if we have outstanding xfers (including cancellations) */
    ret = scancontrol.active_transfers;
    if (ret > 0) {
        err("declined to ainscan while %d transfers active\n", ret);
        return -EAGAIN;
    }
    
    returncode = 0; /* used when we have transfers to unwind */
              
    if (lowch < 0 || lowch > 3 || highch < 0 || highch > 3 || highch < lowch) {
        err("invalid channel\n");
        return -EINVAL;
    }
    
    nchannels = highch - lowch + 1;
    totalpts = npts * nchannels;
    if (totalpts > datsize || totalpts < 1) {
        err("invalid data size\n");
        return -EINVAL;
    }
    
    /* Scan control struct to help completion handler sort the data */
    scancontrol.bytes_awaited = totalpts * 2;
    scancontrol.last_pkt_seqno = (uint16_t)((totalpts * 2 - 1) / (IN_PKT_SIZE-2));
    scancontrol.last_pkt_bytes = 1+(totalpts * 2 - 1) % (IN_PKT_SIZE-2);
    scancontrol.error = 0;
    scancontrol.active_transfers = 0;
    scancontrol.data = data;
    scancontrol.datsize = datsize;
    
    /* calculate timer settings */
    if (interval == 0) {
        adctime = 1000000;  /* fake timer settings (10 sps) for external clock */
        syncopts = 1;     /* external adc clock, skip first pulse (not gated) */
    } else if (interval/nchannels < 20) {
        /* set to allow < 20 in order to force errors */
        err("conversion time below 20us\n");
        return -EINVAL;
    } else {
        adctime = 10 * interval / nchannels; /* 10ths of us */
        syncopts = 0; /* internal adc clock */
    }
    pre = -1;
    do {
        pre++;
        timer = adctime / (1<<pre);
    } while (timer > 0xffff);
    
    /* Stop any existing conversions, just in case */
    if ((ret = pmd_ainstop(pmdhandle)) < 0) {
        err("ainstop failed: %s\n", usb_get_errmsg(ret));
        return ret;
    }
    
    /* Set the sync options */
    msgbuf[0] = 0x43;
    msgbuf[1] = syncopts;
    if ((ret = send_control(pmdhandle, msgbuf, 2, 3000)) < 0) {
        err("set sync failed: %s\n", usb_get_errmsg(ret));
        return ret;
    }
    
    /* Set trigger options in scanopts; set ext trig polarity if necessary */
    if (trigger == 0) {
        scanopts = 1; /* internal trigger, one scan */
    } else {
        scanopts = 5; /* external trigger, one scan */
        trigopts = (trigger > 0) ? 1 : 0;
        msgbuf[0] = 0x42;
        msgbuf[1] = trigopts;
        if ((ret = send_control(pmdhandle, msgbuf, 2, 3000)) < 0) {
            err("set trig failed: %s\n", usb_get_errmsg(ret));
            return ret;
        }
    }
    
    /* Create the transfers */
    for (i = 0; i < NUM_XFERS; i++) {
        xfers[i] = NULL; /* not yet allocated */
    }
    for (i = 0; i < NUM_XFERS; i++) {
        unsigned char *databuf = (unsigned char *)(malloc(IN_PKT_SIZE));
        if (!databuf) {
            err("no memory for data buffer\n");
            returncode = -ENOMEM;
            goto out_free_transfers;
        }
        xfers[i] = libusb_alloc_transfer(0);
        if (!xfers[i]) {
            err("alloc transfer %d failed\n", i);
            free (databuf); /* was malloced but will not be used */
            returncode = -ENOMEM;
            goto out_free_transfers;
        }
        libusb_fill_interrupt_transfer(xfers[i], pmdhandle,
                                0x83+i%3, // ep
                                databuf,
                                IN_PKT_SIZE,
                                xfer_cb, 
                                &scancontrol, // user data
                                0); // no timeout
        xfers[i]->flags = LIBUSB_TRANSFER_FREE_BUFFER;
    }
    /* Start the scan */
    msgbuf[0] = 0x11;
    msgbuf[1] = (unsigned char)lowch;
    msgbuf[2] = (unsigned char)highch;
    msgbuf[3] = totalpts & 0xff;
    msgbuf[4] = (totalpts & 0xff00)>>8;
    msgbuf[5] = (totalpts & 0xff0000)>>16;
    msgbuf[6] = (totalpts & 0xff000000)>>24;
    msgbuf[7] = (unsigned char)pre;
    msgbuf[8] = timer & 0xff; 
    msgbuf[9] = (timer & 0xff00)>>8;
    msgbuf[10]= scanopts;
    
    dbg("requesting %d samples\n", totalpts);
    if ((ret = send_control(pmdhandle, msgbuf, 11, 3000)) < 0) {
        err("failed sending ain start: %s\n", usb_get_errmsg(ret));
        returncode = ret;
        goto out_free_transfers;
    }
    
    /* Submit the transfers */
    for (i = 0; i < NUM_XFERS; i++) {
        if ((ret = libusb_submit_transfer(xfers[i])) < 0) {
            err("submit transfer %d failed: %s\n", i, usb_get_errmsg(ret));
            returncode = ret;
            goto out_cancel_transfers;  /// should also cancel scan??
        } else {
            scancontrol.active_transfers++;
        }
    }
    
    /* This is the non-blocking case ...*/
    if (! blocking) {
        pthread_t worker;
    
        ret = pthread_create(&worker, NULL, ainscan_worker, NULL);
        if (ret) {
            err("failed to create ain worker thread: err %d\n", ret);
            /* make positive errors negative! +ENOMEM was seen 
             * when threads were mistakenly not detached. */
            returncode = -abs(ret);
            goto out_cancel_transfers;
        }
        /* drop resources when thread ends. 8 meg leak per shot otherwise! */
        pthread_detach(worker);
        /* now the worker has to handle everything.  We're off. */
        dbg("nb ainscan handing over to worker thread\n");
        return 0;
    } /* end of non-blocking case */
    
    /* This is the blocking case ...*/
    
    /* Handle the returning transfers */
    while (scancontrol.bytes_awaited > 0 && !scancontrol.error) {
        ret = libusb_handle_events(NULL); /* block 2s, handle 1 event */
        /* Any error means the data are trash, probably even from those
         * packets that arrived without error.  Flag this to the caller */
        if (scancontrol.error) {
            returncode = -EIO; 
        }
        dbg("handle_events returned %d, bytes_awaited = %d, error = %d\n", ret, scancontrol.bytes_awaited, scancontrol.error);
    }
    
    /* Either we are in blocking mode, and data arrived OK, or some error
     * occurred during the setup of the xfers or worker thread (so we never
     * got as far as caring whether we should block or not).  Either way, cancel
     * all xfers (some cancels may fail if their xfers were not (re)submitted
     * because of an error) */
out_cancel_transfers:
    for (i = 0; i < NUM_XFERS; i++) {
        dbg("cancelling xfer %d\n", i);
        libusb_cancel_transfer(xfers[i]);
    }

    // Handle the cancellations
    while(scancontrol.active_transfers > 0) {
        ret = libusb_handle_events(NULL);
        dbg("cancellation handle_events returned %d, active_transfers=%d\n", ret, scancontrol.active_transfers);
    }

out_free_transfers:
    for (i = 0; i < NUM_XFERS; i++) {
        libusb_free_transfer(xfers[i]);
    }
    
    return returncode;
}

/* Data collection thread for pmd_ainscan_nb */
void* ainscan_worker(UNUSED void* arg) {
    /* scancontrol is the global struct */
    int i;
    int ret __attribute__ ((unused));  /* only used when DEBUG is defined */
    
    dbg("nb worker starting\n");
    /* Poll for and handle the returning transfers */
    while (scancontrol.bytes_awaited > 0 && !scancontrol.error) {
        ret = libusb_handle_events(NULL); /* block 2s, handle 1 event */
        dbg("nb handle_events returned %d, bytes_awaited = %d, error = %d\n", ret, scancontrol.bytes_awaited, scancontrol.error);
    }
    
    /* All bytes in, or error found in completion handler. Either way,
     * cancel and free all xfers */   /// cancel scan as well?
    for (i = 0; i < NUM_XFERS; i++) {
        dbg("nb cancelling xfer %d\n", i);
        libusb_cancel_transfer(xfers[i]);
    }

    /* Handle the cancellations */
    while(scancontrol.active_transfers > 0) {
        ret = libusb_handle_events(NULL);
        dbg("nb cancellation handle_events returned %d, active_transfers=%d\n", ret, scancontrol.active_transfers);
    }
    /* free the transfers - their buffers get done automatically */
    for (i = 0; i < NUM_XFERS; i++) {
        libusb_free_transfer(xfers[i]);
    }
    dbg("nb handler finishing\n");
    return 0; /* end of collecton thread*/
}

/* Return the number of bytes outstanding in the current ainscan.  Zero
 * signifies proper completion, so the output data are now ready.  Negative is
 * either a detected error (-EIO), or some equally erroneous bad counting */
int pmd_ainawaited(UNUSED libusb_device_handle* pmdhandle) {
    if (scancontrol.error) {
        return -EIO;
    } else {
        return scancontrol.bytes_awaited;
    }
}

/* Return the number of active USB transfers.  If it is not zero, ainscan
 * will return -EAGAIN.  This goes to zero a short while after ainawaited
 * goes to zero -- up to a few seconds if an ainkill was issued */
int pmd_ainactive(UNUSED libusb_device_handle* pmdhandle) {
    return scancontrol.active_transfers;
}

/* Force a non-blocking ainscan to finish.  Check pmd_ainactive to see
 * when it has.   Automatically issues ainstop first */
void pmd_ainkill(libusb_device_handle* pmdhandle) {
    pmd_ainstop(pmdhandle);
    scancontrol.error = 1;
}

/* ain completion handler */
static void xfer_cb(struct libusb_transfer *xfer) {
    unsigned int seqno;
    int count, i, ret;
    struct ainscan_control* scancontrol = (struct ainscan_control*)(xfer->user_data);
    dbg("ep %x in completion handler status %d\n", xfer->endpoint, xfer->status);
    
    switch(xfer->status) {
    case LIBUSB_TRANSFER_COMPLETED:
        seqno = *(uint16_t*)(xfer->buffer+IN_PKT_SIZE-2);
        if (seqno > scancontrol->last_pkt_seqno) {
            err("bad seqno %d\n", seqno);
            scancontrol->error = 1;
            scancontrol->active_transfers--;  // we will not resubmit
            return;
        }
        /* Get packet data size from its seqno */
        count = (seqno == scancontrol->last_pkt_seqno) ? scancontrol->last_pkt_bytes : IN_PKT_SIZE-2;
        
        /* Scale (int16) data elements and copy to output array */
        for (i=0; i<count/2; i++) {
            *(scancontrol->data + seqno*(IN_PKT_SIZE-2)/2 + i) = *(((int16_t*)xfer->buffer)+i)/16;
        }
        
        dbg("took %d bytes from seqno %d\n", count, seqno);
        scancontrol->bytes_awaited -= count;
        
        /* Resubmit. Leave it to higer level to cancel if all data have arrived. */
        if ((ret = libusb_submit_transfer(xfer)) < 0 ) {
            err("resubmit transfer failed: %s\n", usb_get_errmsg(ret));
            scancontrol->error = 1;
        }
        return;
        
    case LIBUSB_TRANSFER_CANCELLED:
        dbg("xfer cancelled ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        return;
    
    /* misc errors that we hope don't happen */
    case LIBUSB_TRANSFER_ERROR:
        err("xfer error ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    case LIBUSB_TRANSFER_TIMED_OUT:
        err("xfer timo ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    case LIBUSB_TRANSFER_STALL:
        err("xfer stall ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    case LIBUSB_TRANSFER_NO_DEVICE:
        err("xfer nodev ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    case LIBUSB_TRANSFER_OVERFLOW:
        err("xfer overflow ep %x\n", xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    default:
        err("xfer unknown status %d, ep %d\n", xfer->status, xfer->endpoint);
        scancontrol->active_transfers--;
        scancontrol->error = 1;
        return;
    } // end switch
}


/* Send a value to an analogue output channel (0 or 1).  Value 0...4095.
 * Returns 4 (bytes sent) or a negative libusb error */
int pmd_aout(libusb_device_handle* pmdhandle, int channel, int value) {
    if (value < 0) { 
        value = 0;
    } else if (value > 4095) {
        value = 4095;
    }
    unsigned char msgbuf[64];
    msgbuf[0] = 0x14;
    msgbuf[1] = (unsigned char)(channel & 0x01);
    msgbuf[2] = (unsigned char)((value & 0xf) << 4);
    msgbuf[3] = (unsigned char)(value >> 4);
    return (send_control(pmdhandle, msgbuf, 4, 3000));
}

/* Configure digital port A(0) or B(1) for input(1) or output(0) (bytewise only)
 * Returns 3 (bytes sent) or a negative libusb error */
int pmd_digconf(libusb_device_handle* pmdhandle, int port, int direction) {
    unsigned char msgbuf[64];
    msgbuf[0] = 0x01;
    msgbuf[1] = (unsigned char)(port & 0x01);
    msgbuf[2] = (unsigned char)(direction & 0x01);
    return (send_control(pmdhandle, msgbuf, 3, 3000));
}

/* Set an output byte on digital port A(0) or B(1).
 * Returns 3 (bytes sent) or a negative libusb error */
int pmd_digout(libusb_device_handle* pmdhandle, int port, int data) {
    unsigned char msgbuf[64];
    msgbuf[0] = 0x04;
    msgbuf[1] = (unsigned char)(port & 0x01);
    msgbuf[2] = (unsigned char)(data & 0xff);
    return (send_control(pmdhandle, msgbuf, 3, 3000));
}

/* Read a byte from digital port A(0) or B(1).
 * Returns 0-255 (or junk if the read failed) */
int pmd_digin(libusb_device_handle* pmdhandle, int port) {
    unsigned char msgbuf[64];
    int rxlength;
    msgbuf[0] = 0x03;
    msgbuf[1] = 0x00;
    send_control(pmdhandle, msgbuf, 2, 3000);
    get_data(pmdhandle, msgbuf, 64, &rxlength, 3000);
    if (port == 0) {
        return msgbuf[1];
    } else {
        return msgbuf[2];
    }
}

/* Read 16 bits from both digital ports at once.
 * Returns 0-65535 (or junk if the read failed) */
int pmd_digin16(libusb_device_handle* pmdhandle) {
    unsigned char msgbuf[64];
    int rxlength;
    msgbuf[0] = 0x03;
    msgbuf[1] = 0x00;
    send_control(pmdhandle, msgbuf, 2, 3000);
    get_data(pmdhandle, msgbuf, 64, &rxlength, 3000);
    return msgbuf[1] + (msgbuf[2] << 8);
}

/* Zero the counter. Returns 2 (bytes sent) or a negative libusb error */
int pmd_crst(libusb_device_handle* pmdhandle) {
    unsigned char msgbuf[64];
    msgbuf[0] = 0x20;
    msgbuf[1] = 0x00;
    return (send_control(pmdhandle, msgbuf, 2, 3000));
}
    
/* Read the counter. Returns the count or junk in case of error */
unsigned long pmd_cin(libusb_device_handle* pmdhandle) {
    unsigned char msgbuf[64];
    unsigned long count;
    int rxlength;
    msgbuf[0] = 0x21;
    msgbuf[1] = 0x00;
    send_control(pmdhandle, msgbuf, 2, 3000);
    get_data(pmdhandle, msgbuf, 64, &rxlength, 3000);
    memcpy(&count, &msgbuf[1], 4);  /* NOTE little-endian */
    return count;
}


/* Return the version of this library 
char* pmd_version(void) {

    return VERSION;
}*/
