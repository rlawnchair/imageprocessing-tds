#!/bin/bash
./color2mean im-1.ppm im-1-g.ppm
./mm_binarize 220 255 im-1-g.ppm im-1-gb.ppm
./mm_close 2 4 im-1-gb.ppm im-1-target1.ppm
./mm_label im-1-target1.ppm im-1-target2.ppm
pvisu im-1-target2.ppm
rm im-1-g.ppm
rm im-1-gb.ppm
rm im-1-target1.ppm
exit 0
