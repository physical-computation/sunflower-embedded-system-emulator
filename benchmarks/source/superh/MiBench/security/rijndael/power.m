sizemem 20000000
srecl rijndael.sr
ca
--setquantum 1000000000
registerstabs rijndael.stabs
run "rijndael input_large.asc output_large.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321"
newbatt 0 1.0
battnodeattach 0
on
