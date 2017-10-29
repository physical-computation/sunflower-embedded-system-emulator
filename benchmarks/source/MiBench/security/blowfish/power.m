sizemem 20000000
srecl bf.sr
ca
--setquantum 1000000000
registerstabs bf.stabs
run "bf e input_large.asc output_large.enc 1234567890abcdeffedcba0987654321"
newbatt 0 1.0
battnodeattach 0
on
