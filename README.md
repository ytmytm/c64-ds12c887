DS12C887 - Real Time Clock for C64/128
======================================

Maciej 'YTM/Elysium' Witkowiak, 2005-02-02


#What is it?
A friend of mine got some amount of these chips (DS12C887) and we were wondering if it was possible
to interface them to C64/128 computers. Of course it was :).

DS12C887 is more than just a Y2K-compliant RTC. It has internal lithium battery which backs up
the clock and internal RAM - yes it has 113 bytes of non-volatile RAM free for your usage.

#Hardware
This is quite simple. You just need to have DS12C887 mentioned before, 74HCT138 and 74HCT00 (or '02,
'04 - it's just an inverter). Apart from that you need to have additional address decoder installed
which will generate /RTC signal when proper I/O memory page is accessed. For installing and testing
you might use /IO1 or /IO2 signals from Expansion Port, but I don't recommend it as permanent as it
may cause problems with cartridges and REU.

Here is Chris Ward's interface for DS1687 chip (similar to DS12C887), it was taken
![from his page](http://home.freeuk.net/c.ward/6502/):

![Ward's interface](/ds1687.png?raw=true)

DS12C887 application is quite similar, but ignore Vbaux and /RCLR signals, they are not present here.

Below is the schematic of the interface. I used one gate from 74'00 as inverter but you might want to use
74'02 in the same way or 1/6th of 74'04 (a real inverter). There is also a minor change - /RTC signal (/SELECT)
on the picture above) is connected to both /E1 and /E2 (G2A, G2B) inputs of '138.

Input lines (data, R/W, phi2) are easy accessible on Expansion Port or processor itself.

![DS12C887 interface to C64/128](/ds12c887.png?raw=true)

The PDF and gEDA version of this schematic are in schematic/ directory.

Leave the rest of pins unconnected. SQW pin is an output from RTC, it can generate a square wave
with given frequency there - this is only an option. The same applies to /RESET and /IRQ lines -
connect them if you need them.

Here is a general photo of the RTC chip installed in my C128DCR. I used 1571 part of the board to
mount the chip on top of another one to get the power. The rest of lines is soldered to C128 parts.
Click to enlarge.


![RTC - top view](/ds12c887.jpg?raw=true)

#Software

Although the address decoder might reserve a whole page of I/O space for RTC only addresses
BASE+0 and BASE+1 matter. To read something from RTC you first need to write RTC register address to
BASE+0 and then read the data from BASE+1. To write something to RTC you first write register number
to BASE+0 and then data to BASE+1. This chip has address space of 128 locations and you can put
these values into BASE+0.

I don't want to copy whole ![datasheet](http://pdfserv.maxim-ic.com/arpdf/DS12C887.pdf) here.
Local copy is included in this repo, better grab it and read it carefully.

Here are some snippets of code. All of them assume that RTC base address is at $DE00 (line
/IO1 used as /RTC).
```
; This one reads the current second
   LDA #$00	; register 0 - seconds
   STA $DE00	; address register
   LDA $DE01	; data register
```

Replace 'LDA $DE01' with 'STA $DE01' to have an example for write.

Here is example for reading whole RTC RAM into C64 screen memory:
```
     LDX #0
loop STX $DE00
     LDA $DE01
     STA $0400,X
     INX
     CPX #$80
     BNE loop
     RTS
```

Don't get confused if the clock doesn't advance. It took me some time to figure it
out, but careful reading of datasheet was enough :). These chips leave the
factory with clock oscillator turned off to save battery power. It needs to be
enabled with following code:

```
     LDX #$0a    ; control register A
     LDA #$20    ; %x010xxxx
     STX $DE00
     STA $DE01
     RTS
```

Everything else concerning programming the RTC chip is included in data sheet mentioned before.

There are two examples included:
- rtc-basic - a C64 BASIC program to set CIA1 TOD clock from RTC

- rtc-geos - GEOS Auto-Exec to set and read time from RTC. It will set the time each time you boot GEOS. You can also set the RTC time with it. Use it if you want 6502 assembly example.

#Conclusion

DS12C887 is a very nice chip, especially because it has everything it needs inside - oscillator,
battery and non-volatile RAM can be used to store some of your preferences.

This circuit has been tested on C64 and finally it is installed in my C128DCR.
