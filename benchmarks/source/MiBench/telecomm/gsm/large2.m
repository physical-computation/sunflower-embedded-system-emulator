sizemem 20000000
srecl bin/toast.sr
ff
setquantum 1000000000
registerstabs bin/toast.stabs
run "untoast -fps -c data/large.au.run.gsm"
on
