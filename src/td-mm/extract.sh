#!/bin/bash
./mm_binarize 220 255 gear.ppm gear_a.ppm
./mm_open 2 15 gear_a.ppm gear_b.ppm
./mm_subtract gear_b.ppm gear_a.ppm gear_c.ppm
./mm_label gear_c.ppm gear_d.ppm
rm -f gear_a.ppm
rm -f gear_b.ppm
pvisu gear_c.ppm
pvisu gear_d.ppm
exit 0
