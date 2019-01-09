netnewseg		0 8192 300000000 100000000 0 0 0 0 0 0 0 0 0
netnewseg		1 8192 300000000 100000000 0 0 0 0 0 0 0 0 0
netnewseg		2 8192 300000000 100000000 0 0 0 0 0 0 0 0 0
netnewseg		3 8192 300000000 100000000 0 0 0 0 0 0 0 0 0





clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 0
sizemem			3000000
srecl			swradiosource.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 0
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 1
sizemem			3000000
srecl 			swradiolpf.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 1
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 2
sizemem			3000000
srecl 			swradiodemod.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 2
netnodenewifc		1 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		1 3
sizemem			3000000
srecl 			swradioeq.sr
run


newnode			superH 0 0 0
clockintr		1
cacheoff
ff
netnodenewifc		0 0.250 0.250 0 0.250 0 0 0 0 1024 1024
netsegnicattach		0 3
sizemem			3000000
srecl 			swradiosink.sr
run
