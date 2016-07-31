/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

/*
 * gammatest.c
 *
 */

#include <math.h>
#include "allheaders.h"

#define  NPLOTS      5
#define  MINVAL      30
#define  MAXVAL      210

main(int    argc,
     char **argv)
{
char        *filein, *fileout;
char         bigbuf[512];
l_int32      iplot;
l_float32    gam;
l_float64    gamma[NPLOTS] = {.5, 1.0, 1.5, 2.0, 2.5};
GPLOT       *gplot;
NUMA        *na, *nax;
PIX         *pixs, *pixd;
static char  mainName[] = "gammatest";

    if (argc != 4)
	exit(ERROR_INT(" Syntax:  gammatest filein gam fileout", mainName, 1));

    filein = argv[1];
    gam = atof(argv[2]);
    fileout = argv[3];

    if ((pixs = pixRead(filein)) == NULL)
	exit(ERROR_INT("pixs not made", mainName, 1));

#if 1
    startTimer();
    pixGammaTRC(pixs, pixs, gam, MINVAL, MAXVAL);
    fprintf(stderr, "Time for gamma: %7.3f sec\n", stopTimer());
    pixWrite(fileout, pixs, IFF_JFIF_JPEG);
    pixDestroy(&pixs);
#endif

#if 0
    startTimer();
    pixd = pixGammaTRC(NULL, pixs, gam, MINVAL, MAXVAL);
    fprintf(stderr, "Time for gamma: %7.3f sec\n", stopTimer());
    pixWrite(fileout, pixd, IFF_JFIF_JPEG);
    pixDestroy(&pixs);
    pixDestroy(&pixd);
#endif

    na = numaGammaTRC(gam, MINVAL, MAXVAL);
    gplotSimple1(na, GPLOT_X11, "/tmp/junkroot", "gamma trc");
    numaDestroy(&na);

#if 1     /* plot gamma TRC maps */
    gplot = gplotCreate("/tmp/junkmap", GPLOT_X11,
                        "Mapping function for gamma correction",
		       	"value in", "value out");
    nax = numaMakeSequence(0.0, 1.0, 256);
    for (iplot = 0; iplot < NPLOTS; iplot++) {
        na = numaGammaTRC(gamma[iplot], 30, 215);
	sprintf(bigbuf, "gamma = %3.1f", gamma[iplot]);
        gplotAddPlot(gplot, nax, na, GPLOT_LINES, bigbuf);
	numaDestroy(&na);
    }
    gplotMakeOutput(gplot);
    gplotDestroy(&gplot);
    numaDestroy(&nax);
#endif

    return 0;
}


