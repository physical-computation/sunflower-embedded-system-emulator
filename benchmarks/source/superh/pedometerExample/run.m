v

--
--
--
setquantum		1
setfreq 40
--   	All signals have sample rate 400 to simulate the 400 Hz acceleromter used to collect them, interpolation and we aren't using location data
--	Tell sunflower where to find x-axis acceleration readings
sigsrc 			0 "X-Axis Acceleration"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "	0 0.0 0.0 0.0 0	"xAcceleration.txt" 400 1 0 0
--	Tell sunflower where to find y-axis acceleration readings
sigsrc 			0 "Y-Axis Acceleration"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "	0 0.0 0.0 0.0 0	"yAcceleration.txt" 400 1 0 0
-- 	Tell sunflower where to find z-axis acceleration readings
sigsrc 			0 "Z-Axis Acceleration"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "	0 0.0 0.0 0.0 0	"zAcceleration.txt" 400 1 0 0
--
--			Node 0
--
cacheoff
-- subscribe to all three signals with a seperate virtual sensor for each
sigsubscribe		0 0
sigsubscribe            1 1
sigsubscribe		2 2
sizemem		96000000
srecl		virtualSensorExample.sr
run
setrandomseed		936977
setnode			0
v
on
