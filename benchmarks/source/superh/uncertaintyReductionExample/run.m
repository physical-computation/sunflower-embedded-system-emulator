v

--
--
--
setquantum		1
setfreq 40
sigsrc 			0 "Temperature readings from BME680"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "	0 0.0 0.0 0.0 0	"Temperature.txt" 1800 1 0 0
sigsrc 			1 "Humidity readings from BME680"	0.0 0.0 	1.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0		" "	0 0.0 0.0 0.0 0	"Humidity.txt" 1800 1 0 0
--
--			Node 0
--
cacheoff
sigsubscribe		0 0
sigsubscribe		1 1
sizemem		96000000
srecl		virtualSensorExample.sr
run
setrandomseed		936977
v
on
