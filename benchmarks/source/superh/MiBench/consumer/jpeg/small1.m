sizemem 20000000
srecl "jpeg-6a/cjpeg.sr"
ff
setquantum 1000000000
registerstabs "jpeg-6a/cjpeg.stabs"
run "cjpeg -dct int -progressive -opt -outfile output_small_encode.jpeg input_small.ppm"
on
