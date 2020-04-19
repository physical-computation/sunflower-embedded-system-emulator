sizemem 20000000
srecl "lout-3.24/lout.sr"
ff
setquantum 1000000000
registerstabs "lout-3.24/lout.stabs"
run "lout -I lout-3.24/include -D lout-3.24/data -F lout-3.24/font -C lout-3.24/maps -H lout-3.24/hyph small.lout"
on
