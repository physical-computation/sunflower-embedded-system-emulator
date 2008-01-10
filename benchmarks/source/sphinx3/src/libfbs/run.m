setquantum	1000000


--
--	Create a network link which node 0 and node 1 will be attached to
--
--netnewseg		0 8192 300000000 100000000 0 0 0 0 0 0 0 0 0
--netsegfailprob		0 0.0
--netsegfaildurmax	0 1000000

--
--	By default, node 0 already created, just do some addtl config
--
cacheoff
ff
sizemem			300000000
--netnodenewifc           0 0.250 0.250 0 0 0 0 0 1024 1024
--netsegnicattach         0 0
srecl s3decode.sr
run "s3decode command_simple.txt"


--
--	Create a new node and configure it
--
--newnode 0 0 0 0 0
--cacheoff
--ff
--sizemem			300000000
--netnodenewifc           0 0.250 0.250 0 0 0 0 0 1024 1024
--netsegnicattach         0 0
--srecl s3decode.sr
--run s3decode command_simple.txt

