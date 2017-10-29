sizemem 20000000
srecl crc.sr
ca
--setquantum 1000000000
registerstabs crc.stabs
run "crc ../adpcm/data/large.pcm"
newbatt 0 1.0
battnodeattach 0
on
