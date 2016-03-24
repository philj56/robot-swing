/* demo.c  --  test client for pmd1208fs library
 * 
 * compile:  gcc demo.c -o demo -lpmd1208fs
 * 
 *                                        Copyright 2005 - 2009 Mark Colclough
 */

// Note -- The PMD device will not work through a usb(2?) hub, probably because
// of the limited bandwidth of the transaction translator, and the 1 ms
// default interval that libusb obeys (too fast for the TT).  The old
// kernel-based driver has an option to set interval=2 to work around this.
// I don't see a way to tell libusb1 to use a non-default interval, so it would
// have to be modified if this mattered.  Not sure that it does, because high
// speed operation would, of course, be compromised.

// This implementation works OK at 20us (that's the hardware spec; the Windows
// driver never worked this well).  19us is a bit imperfect, but does not fail,
// 18us is useless, and exposes problems in cancellation handling - cancelled
// xfers do not enter completion handler, and handle_events repeatedly times out.
// By default, the library complains if you go below 20 us.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pmd1208fs.h"

void showmenu(void) {
    printf("1: serial      9: crst       g: timeout\n");
    printf("2: ain         a: cin        h: aoutscan\n");
    printf("3: aout        b: ainstop    i: digin16\n");
    printf("4: ainscan     c: aoutstop\n");
    printf("5: ainscan_nb  d: reset\n");
    printf("6: digconf     e: errcode\n");
    printf("7: digin       f: flash\n");
    printf("8: digout                    0: quit\n");
}

int main(void) {
    libusb_device_handle* mypmd;
    int i, ret;
    
    int ch, rng, val;
    char opt;
    int lowch, highch, n, t, trig;
    //aoutscan int single, conttime;
    int port, dat;
    FILE *outfil;
    FILE *gp;
    int16_t data[10000];
    
    printf("libpmd1208fs version: %s\n", pmd_version());
    
    mypmd = pmd_find_first();
    if (!mypmd) {
        printf("failed to find PMD device\n");
        exit(EXIT_FAILURE);
    }
    printf("serial number is >%s<\n", pmd_serial(mypmd));
    printf("\n");
    
    showmenu();
    printf("option ? ");
    while (scanf("%c", &opt), opt !='0' ) {
        switch (opt) {
        case 10: // ignore \n
            continue;
        case '1': // serial
            printf("Serial number=%s.\n", pmd_serial(mypmd));
            break;
        case '2': // ain
            printf("channel range? ");
            scanf("%d %d", &ch, &rng);
            printf("ain=%d for channel %d on range %d\n", pmd_ain(mypmd, ch, rng), ch, rng);
            break;
        case '3': // aout
            printf("channel value? ");
            scanf("%d %d", &ch, &val);
            pmd_aout(mypmd, ch, val);
            printf("value %d sent to aout %d\n", val, ch);
            break;
        case '4': // ainscan
            printf("lowch highch nperch time(us) trig?\n");
            scanf("%d %d %d %d %d", &lowch, &highch, &n, &t, &trig);
            if ((unsigned)((highch-lowch+1)*n) > sizeof(data)) {
                printf("max %lu readings in this demo\n", sizeof(data));
                break;
            }
            printf("starting scan from %d to %d npts=%d time=%d us trig=%d\n",
                       lowch, highch, n, t, trig);
            ret = pmd_ainscan(mypmd, lowch, highch, n, t, trig, data, sizeof(data), 1);
            printf("return code = %d\n", ret);
            if (ret) break;
            printf("how many to plot? ");
            scanf("%d", &n);
            if (n < 1 || (unsigned)n > sizeof(data)) break;
            printf("writing to tdat and running gnuplot\n");
            outfil = fopen("tdat", "w");
            for (i=0; i<n; i++) {
                fprintf(outfil, "%d\n", data[i]);
            }
            fclose(outfil);
            gp = popen("gnuplot", "w");
            fprintf(gp, "plot 'tdat' with linespoints\n");
            fflush(gp);
            break;
        case '5': // ainscan, non blocking
            printf("lowch highch nperch time(us) trig?\n");
            scanf("%d %d %d %d %d", &lowch, &highch, &n, &t, &trig);
            if ((unsigned)((highch-lowch+1)*n) > sizeof(data)) {
                printf("max %lu readings in this demo\n", sizeof(data));
                break;
            }
            printf("starting scan from %d to %d npts=%d time=%d us trig=%d\n",
                       lowch, highch, n, t, trig);
            ret = pmd_ainscan(mypmd, lowch, highch, n, t, trig, data, sizeof(data), 0);
            printf("return code = %d\n", ret);
            if (ret) break;
            for (i=0; i<20; i++) {
                ret = pmd_ainawaited(mypmd);
                printf("ainawaited=%d, ", ret);
                if (ret <= 0) break;
                printf("sleeping\n");
                usleep(500000);
            }
            if (ret > 0) {
                printf("got bored waiting, so killed transfer\n");
                pmd_ainkill(mypmd);
            }
            
            printf("writing to tdat and running gnuplot\n");
            outfil = fopen("tdat", "w");
            for (i=0; i<n; i++) {
                fprintf(outfil, "%d\n", data[i]);
            }
            fclose(outfil);
            gp = popen("gnuplot", "w");
            fprintf(gp, "plot 'tdat' with linespoints\n");
            fflush(gp);
            break;
        case '6': // digconf
            printf("conf port value ?");
            scanf("%d %d", &port, &dat);
            pmd_digconf(mypmd, port, dat);
            break;
        case '7': // digin
            printf("dports are %d %d\n", pmd_digin(mypmd, 0), pmd_digin(mypmd, 1));
            break;
        case '8': // digout
            printf("output port value ?");
            scanf("%d %d", &port, &dat);
            pmd_digout(mypmd, port, dat);
            break;
        case '9': // crst
            printf("counter reset\n");
            pmd_crst(mypmd);
            break;
        case 'a': // cin
            printf("count is %ld\n", pmd_cin(mypmd));
            break;
        case 'b': // ainstop
            printf("ainstop sent\n");
            pmd_ainstop(mypmd);
            break;
        case 'c': // aoutstop
            printf("aoutstop not implemented\n");
            break;
        case 'd': // reset
            printf("reset not implemented\n");
            break;
        case 'e': // errcode
            printf("code to translate? ");
            scanf("%d", &n);
            printf("%d = %s\n", n, usb_get_errmsg(n));
            break;
        case 'f': // flash
            pmd_flash(mypmd);
            break;
        case 'g': // timeout
            printf("timeout not implemented\n");
            break;
        case 'h':  //aoutscan
            printf("aoutscan not implemented\n");
            //printf("lowch highch nperch time(us) trig(1=ext) single conttime\n");
            //scanf("%d %d %d %d %d %d %d", &lowch, &highch, &n, &t,
            //                        &trig, &single, &conttime);
            //pmd_aoutscan(mypmd, lowch, highch, n, t, 
            //                      trig, single, conttime);
            //printf("started outscan\n");
            break;
        case 'i':  //digin16
            printf("dports are 0x%4x\n", pmd_digin16(mypmd));
            break;
        case '0':
        case 'q':
            pmd_close(mypmd);
            exit(EXIT_SUCCESS);
            break;
        default:
            showmenu();
            break;
        }
        printf("option ? ");
    }
    exit(EXIT_SUCCESS); /* not reached */
}
 
