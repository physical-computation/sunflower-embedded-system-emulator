v

--
--
--
setquantum		1



--
--	Signal source 0 and 1.
--
--	Gaussian (Normal) distribution (with distance from center of peak)
--		P(x) = (1/sigma*sqrt(2*pi))* e ^ -(x - mu)^2/(2*sigma^2)
--	Standard normal distribution w/ mu = 0 and sigma^2 = 1 expands to
--		P(x) 	= 1/sqrt(2*pi) e ^ -x^2/2
--			= 0.398942 * 2.71828 ^ (-0.5 x^2)
--
--	Normalizing so that P(x) is 1.0 at x==0 gives
--		P(x) 	= 0.997356 * 2.71828 ^ (-0.5 x^2)
--
--	Some points about Light (http://www.intl-light.com/handbook/ch13.html):
--		Sunny Day	~ 100,000 Lux
--		Office Lights	~ 1000 Lux
--		Full Moon	~ 0.1 Lux
--		Overcast Night	~ 0.0001 Lux
--
--	The "fixedsampleval" field below assumes an intensity at peak of 1000 Lux
--
--	Signal source trajectory speed = (trajectoryrate/ntrajectories)*(total distance) units per second
--	With 400 samples in locdata, total distance of 40in, and trajectoryrate of 100,
--	trajectory speed is 0.254 m/s (0.5681 mph = 0.8333 ft/s)
--
sigsrc 			0 "Gaussian lightsource 1"	0.0 0.0 	0.0 0.0 0.0 0.0 0.997356 -0.5 0.0 0.0 0.0 2.71828		0.0 0.0 0.0 0.0 2.0 0.0 0.0 0.0		"dam.locdata.1"	100 0.0 0.0 0.0 1	" " 0 1000.0 1 1

sigsrc 			0 "Gaussian lightsource 2"	0.0 0.0 	0.0 0.0 0.0 0.0 0.997356 -0.5 0.0 0.0 0.0 2.71828		0.0 0.0 0.0 0.0 2.0 0.0 0.0 0.0		"dam.locdata.2"	100 0.0 0.0 0.0 1	" " 0 1000.0 1 1

--
--	Signal source 2. Ambient light at 0.1 Lux
--
sigsrc 			0 "Ambient light"		0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "		0 0.0 0.0 0.0 1	" " 0 0.1 1 1






--
--	Signal source 3
--	Due to the proximity of the sensors in the original experiment @ PARC,
--	we use a Ricean model for the RF propagation, w/ received Pr = K/d^n, n = 2
--	We set ambient RF noise at 1/100 of the Peak radio power.
--
sigsrc 			1 "Radio propagation model"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		-2.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "		0 0.0 0.0 0.0 1	" " 0 0.0 1 1

--	Signal source 4
--	We set ambient RF sig strength to equal 1/10 strength of 89.1mW
--	radio at sqrt(10^2+10^2)=14.14 units, and set minsnr to 9 (9 < 10)
sigsrc 			1 "Ambient RF noise"		0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "		0 0.0 0.0 0.0 1	" " 0 0.00004455 1 1


--
--	For the more realist wireless MAC layer with software-implemented
--	MAC, we use an effectively infinite medium width (currently set
--	to 999, but we should modofy sim to permit specification of
--	truly inf. medium width
--
netnewseg		0 1024 300000000 38400 999 0 0 0 0 0 0 0 0
netsegfailprob		0 0.0
netsegfaildurmax	0 1000000
-- netseg2file		0 netseg0log

--
--	The SNR is tuned for the spatial layout (below), so that only
--	immediate neighbors in the topology get valid transmissions
--
netsegpropmodel		0 3 9.0 







newbatt 		0 0.1
newbatt 		1 0.1
newbatt 		2 0.1
newbatt 		3 0.1
newbatt 		4 0.1
newbatt 		5 0.1
newbatt 		6 0.1
newbatt 		7 0.1
newbatt 		8 0.1
newbatt 		9 0.1
newbatt			10 0.1
newbatt 		11 0.1
newbatt 		12 0.1
newbatt 		13 0.1
newbatt 		14 0.1
newbatt 		15 0.1
newbatt 		16 0.1
newbatt 		17 0.1
newbatt 		18 0.1
newbatt			19 0.1
newbatt 		20 0.1
newbatt 		21 0.1
newbatt 		22 0.1
newbatt 		23 0.1
newbatt 		24 0.1
newbatt 		25 0.1


--
--		Topological layout of the nodes is as follows:
--		
--		 		0.0	10.0	20.0	30.0	40.0   45.0
--		     	--------------------------------------------------------->   x
--			|
--			|
--		    	|
--		0.0 	|	0	5	10	15	20
--		    	|
--		    	|
--		10.0 	|	1	6	11	16	21
--		    	|
--		    	|
--		20.0 	|	2	7	12	17	22
--		    	|
--		    	|
--		30.0 	|	3	8	13	18	23
--		    	|
--		    	|
--		40.0 	|	4	9	14	19	24
--		    	|
--		45.0   	|					      >>25<<	<-- sink
--			|
--		    	V
--		
--		    	y
--		
--		The spacing is 10 "units", corresponding to the 10" spacing as
--		in the original PARC experiment. All Space "units" should therefore
--		be interpreted, in this experiment, as inches.
--

--
--	NIC TX/RX/Sleep pwr, CPU active and sleep pwr based on current draw for
--	MICA2DOT motes, from entries for MPR500CA device in data sheet, for an
--	operating voltage of 3.3V (6020-0043-04_A_MICA2DOT.pdf from xbow.com)
--


--
--			Node 0
--
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		0
battalertfrac		0.0
clockintr		1
ebattintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 1
--
newnode			superH 0.0 10.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		1
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 2
--
newnode			superH 0.0 20 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		2
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 3
--
newnode			superH 0.0 30.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		3
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 4
--
newnode			superH 0.0 40.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		4
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 5
--
newnode			superH 10.0 0.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		5
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 6
--
newnode			superH 10.0 10.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		6
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 7
--
newnode			superH 10.0 20.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		7
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 8
--
newnode			superH 10.0 30.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		8
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 9
--
newnode			superH 10.0 40.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		9
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 10
--
newnode			superH 20.0 0.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		10
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 11
--
newnode			superH 20.0 10.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		11
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 12
--
newnode			superH 20.0 20.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		12
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 13
--
newnode			superH 20.0 30.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		13
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 14
--
newnode			superH 20.0 40.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		14
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 15
--
newnode			superH 30.0 0.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		15
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 16
--
newnode			superH 30.0 10.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		16
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 17
--
newnode			superH 30.0 20.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		17
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 18
--
newnode			superH 30.0 30.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		18
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 19
--
newnode			superH 30.0 40.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		19
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 20
--
newnode			superH 40.0 0.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		20
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 21
--
newnode			superH 40.0 10.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		21
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 22
--
newnode			superH 40.0 20.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		22
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 23
--
newnode			superH 40.0 30.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		23
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"



--
--			Node 24
--
newnode			superH 40.0 40.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		24
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			damv2.sr
run			"100000 1 50 1 100 4"






--
--			Node 25 Sink
--
newnode			superH 45.0 45.0 0
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		25
battalertfrac		0.0
ebattintr		1
clockintr		1
cacheoff
ff
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 2048 2048
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
sigsubscribe		0 1
sigsubscribe		0 2
srecl 			sink.sr

--
--	Unlike DAM nodes, the argv here is the period
--	for querying node in network
--
run			"1000000 1 50 1 100 4"



v
setnode 25
setrandomseed 134618
-- setrandomseed 641478
-- setrandomseed 238578
-- setrandomseed 735957
-- setrandomseed 948504
on
