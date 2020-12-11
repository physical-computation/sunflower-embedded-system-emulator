sizemem 20000000
srecl src/gs.sr
ff
setquantum 1000000000
registerstabs src/gs.stabs
run "gs -q -sDEVICE=ppm -dNOPAUSE -sOutputFile=data/output_small.ppm -- data/small.ps"
on
