
all: rtc12c887.c rtc-head.grc
	grc -f rtc-head.grc
	ca65 -t geos rtc-head.s
	cc65 -t geos -Osir rtc12c887.c
	ca65 -t geos rtc12c887.s
	ca65 -t geos rtclow.s
	ld65 -vm -m map.map -t geos -o rtc.cvt rtc-head.o geos.o rtc12c887.o rtclow.o geos.lib

clean:
	-rm *.o map.map rtc.cvt rtc12c887.s rtc-head.s

love:
	@echo Not war, eh?

remake: clean all
