
;    RTC-Setup for GEOS 2.0 64/128
;    (DS12C887 RTC chip)
;
;    Maciej 'YTM/Elysium' Witkowiak <ytm@elysium.pl>
;    17.07.2002
;
;    This program is freeware (look at README for more)

; low-level RTC DS12C887 functions
; should be applicable also for DS1687 (possibly except century and/or control registers)
; should be applicable also for other environments than GEOS, InitForIO and DoneForIO functions
; just enable and disable I/O space at $d000-$dfff

; void PutTimeDateToRTC(void)
; unsigned char RTCPeek(char reg)
;          void RTCPoke(char reg, char val)
; unsigned char fromBCD(unsigned char)

    .import _m_second, _m_minute, _m_hour
    .import _m_dayofweek, _m_day, _m_month, _m_year, _m_century
    .import _base_addy
    .import _InitForIO, _DoneWithIO
    .import popa

    .export _PutTimeDateToRTC
    .export _RTC_Peek, _RTC_Poke
    .export _fromBCD

.proc _PutTimeDateToRTC

		jsr _InitForIO
		lda _base_addy
		sta mybase1+1
		sta mybase2+1
		inc mybase2+1
		lda _base_addy+1
		sta mybase1+2
		sta mybase2+2

		ldx #$0b
		lda #$82		; BCD, 24hr, no updates
		jsr MyRTCPoke

		ldx #0
		lda _m_second
		jsr MyRTCPoke
		ldx #2
		lda _m_minute
		jsr MyRTCPoke
		ldx #4
		lda _m_hour
		jsr MyRTCPoke
		ldx #6
		lda _m_dayofweek
		jsr MyRTCPoke
		ldx #7
		lda _m_day
		jsr MyRTCPoke
		ldx #8
		lda _m_month
		jsr MyRTCPoke
		ldx #9
		lda _m_year
		jsr MyRTCPoke
		ldx #50
		lda _m_century
		jsr MyRTCPoke

		ldx #$0b
		lda #$02		; BCD, 24hr, enable updates
		jsr MyRTCPoke
		ldx #$0a
		lda #$22		; enable oscillator
		jsr MyRTCPoke

		jmp _DoneWithIO

MyRTCPoke:
mybase1:	stx $d000
mybase2:	sta $d001
		rts

.endproc

.proc _RTC_Peek
		tax
		lda _base_addy
		sta $02
		lda _base_addy+1
		sta $03
		ldy #0
		txa
		sta ($02),y
		iny
		lda ($02),y
		rts
.endproc

.proc _RTC_Poke
		pha
		jsr popa
		tax
		lda _base_addy
		sta $02
		lda _base_addy+1
		sta $03
		ldy #0
		txa
		sta ($02),y
		iny
		pla
		sta ($02),y
		rts
.endproc

.proc _fromBCD
		php
		cld
		sei
		pha
		and  #$0f
		sta  $02
		pla
		and  #$f0
		lsr  a
		sta  $02+1
		lsr  a
		lsr  a
		adc  $02
		adc  $02+1
		plp
		rts
.endproc
