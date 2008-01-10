sizemem 20000000
srecl rijndael.sr
ff
setquantum 1000000000
registerstabs rijndael.stabs
run "rijndael output_large.enc output_large.dec d 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321"
on
