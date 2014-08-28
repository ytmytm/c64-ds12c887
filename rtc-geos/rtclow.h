
/* functions in rtclow.s */

void __fastcall__ PutTimeDateToRTC (void);
unsigned char __fastcall__ RTC_Peek(unsigned char reg);
void __fastcall__ RTC_Poke(unsigned char reg, unsigned char val);
unsigned char __fastcall__ fromBCD(unsigned char val);
