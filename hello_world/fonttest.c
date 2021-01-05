#include <pitrex/pitrexio-gpio.h>
#include <vectrex/vectrexInterface.h>
#include <string.h>
extern int bufferType; // 0 = none, 1 = double buffer, 2 = auto buffer (if pipeline is empty -> use previous

//        !     "     #     $     %     &     '     (     )     *     +     ,     -     .     /   
//  0     1     2     3     4     5     6     7     8     9     :     ;     <     =     >     ?   
//  @     A     B     C     D     E     F     G     H     I     J     K     L     M     N     O   
//  P     Q     R     S     T     U     V     W     X     Y     Z     [     \     ]     ^     _   
//  `     a     b     c     d     e     f     g     h     i     j     k     l     m     n     o   
//  p     q     r     s     t     u     v     w     x     y     z     {     |     }     ~      
unsigned char lcrasterline1[] = {
  0x00, 0x18, 0x6c, 0x6c, 0x30, 0x00, 0x38, 0x60, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x7c, 0x30, 0x78, 0x78, 0x1c, 0xfc, 0x38, 0xfc, 0x78, 0x78, 0x00, 0x00, 0x18, 0x00, 0x60, 0x78, 
  0x7c, 0x30, 0xfc, 0x3c, 0xf8, 0xfe, 0xfe, 0x3c, 0xcc, 0x78, 0x1e, 0xe6, 0xf0, 0xc6, 0xc6, 0x38, 
  0xfc, 0x78, 0xfc, 0x78, 0xfc, 0xcc, 0xcc, 0xc6, 0xc6, 0xcc, 0xfe, 0x78, 0xc0, 0x78, 0x10, 0x00, 
  0x30, 0x00, 0xe0, 0x00, 0x1c, 0x00, 0x38, 0x00, 0xe0, 0x30, 0x0c, 0xe0, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x18, 0xe0, 0x76, 0x00, 
};
unsigned char lcrasterline2[] = {
  0x00, 0x3c, 0x6c, 0x6c, 0x7c, 0xc6, 0x6c, 0x60, 0x30, 0x30, 0x66, 0x30, 0x00, 0x00, 0x00, 0x0c, 
  0xc6, 0x70, 0xcc, 0xcc, 0x3c, 0xc0, 0x60, 0xcc, 0xcc, 0xcc, 0x30, 0x30, 0x30, 0x00, 0x30, 0xcc, 
  0xc6, 0x78, 0x66, 0x66, 0x6c, 0x62, 0x62, 0x66, 0xcc, 0x30, 0x0c, 0x66, 0x60, 0xee, 0xe6, 0x6c, 
  0x66, 0xcc, 0x66, 0xcc, 0xb4, 0xcc, 0xcc, 0xc6, 0xc6, 0xcc, 0xc6, 0x60, 0x60, 0x18, 0x38, 0x00, 
  0x30, 0x78, 0x60, 0x78, 0x0c, 0x78, 0x60, 0x76, 0x6c, 0x00, 0x00, 0x66, 0x30, 0xcc, 0xf8, 0x78, 
  0xdc, 0x76, 0xdc, 0x7c, 0x7c, 0xcc, 0xcc, 0xc6, 0xc6, 0xcc, 0xfc, 0x30, 0x18, 0x30, 0xdc, 0x00, 
};
unsigned char lcrasterline3[] = {
  0x00, 0x3c, 0x00, 0xfe, 0xc0, 0xcc, 0x38, 0xc0, 0x60, 0x18, 0x3c, 0x30, 0x00, 0x00, 0x00, 0x18, 
  0xce, 0x30, 0x0c, 0x0c, 0x6c, 0xf8, 0xc0, 0x0c, 0xcc, 0xcc, 0x30, 0x30, 0x60, 0xfc, 0x18, 0x0c, 
  0xde, 0xcc, 0x66, 0xc0, 0x66, 0x68, 0x68, 0xc0, 0xcc, 0x30, 0x0c, 0x6c, 0x60, 0xfe, 0xf6, 0xc6, 
  0x66, 0xcc, 0x66, 0xe0, 0x30, 0xcc, 0xcc, 0xc6, 0x6c, 0xcc, 0x8c, 0x60, 0x30, 0x18, 0x6c, 0x00, 
  0x18, 0x0c, 0x7c, 0xcc, 0x7c, 0xcc, 0xf0, 0xcc, 0x76, 0x70, 0x0c, 0x6c, 0x30, 0xfe, 0xcc, 0xcc, 
  0x66, 0xcc, 0x76, 0xc0, 0x30, 0xcc, 0xcc, 0xd6, 0x6c, 0xcc, 0x98, 0x30, 0x18, 0x30, 0x00, 0x00, 
};
unsigned char lcrasterline4[] = {
  0x00, 0x18, 0x00, 0x6c, 0x78, 0x18, 0x76, 0x00, 0x60, 0x18, 0xfe, 0xfc, 0x00, 0xfc, 0x00, 0x30, 
  0xde, 0x30, 0x38, 0x38, 0xcc, 0x0c, 0xf8, 0x18, 0x78, 0x7c, 0x00, 0x00, 0xc0, 0x00, 0x0c, 0x18, 
  0xde, 0xcc, 0x7c, 0xc0, 0x66, 0x78, 0x78, 0xce, 0xfc, 0x30, 0x0c, 0x78, 0x60, 0xfe, 0xde, 0xc6, 
  0x7c, 0xcc, 0x7c, 0x70, 0x30, 0xcc, 0xcc, 0xd6, 0x38, 0x78, 0x18, 0x60, 0x18, 0x18, 0xc6, 0x00, 
  0x00, 0x7c, 0x66, 0xc0, 0xcc, 0xfc, 0x60, 0xcc, 0x66, 0x30, 0x0c, 0x78, 0x30, 0xfe, 0xcc, 0xcc, 
  0x66, 0xcc, 0x66, 0x78, 0x30, 0xcc, 0xcc, 0xfe, 0x38, 0xcc, 0x30, 0xe0, 0x00, 0x1c, 0x00, 0x00, 
};
unsigned char lcrasterline5[] = {
  0x00, 0x18, 0x00, 0xfe, 0x0c, 0x30, 0xdc, 0x00, 0x60, 0x18, 0x3c, 0x30, 0x00, 0x00, 0x00, 0x60, 
  0xf6, 0x30, 0x60, 0x0c, 0xfe, 0x0c, 0xcc, 0x30, 0xcc, 0x0c, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 
  0xde, 0xfc, 0x66, 0xc0, 0x66, 0x68, 0x68, 0xce, 0xcc, 0x30, 0xcc, 0x6c, 0x62, 0xd6, 0xce, 0xc6, 
  0x60, 0xdc, 0x6c, 0x1c, 0x30, 0xcc, 0xcc, 0xfe, 0x38, 0x30, 0x32, 0x60, 0x0c, 0x18, 0x00, 0x00, 
  0x00, 0xcc, 0x66, 0xcc, 0xcc, 0xc0, 0x60, 0x7c, 0x66, 0x30, 0xcc, 0x6c, 0x30, 0xd6, 0xcc, 0xcc, 
  0x7c, 0x7c, 0x60, 0x0c, 0x34, 0xcc, 0x78, 0xfe, 0x6c, 0x7c, 0x64, 0x30, 0x18, 0x30, 0x00, 0x00, 
};
unsigned char lcrasterline6[] = {
  0x00, 0x00, 0x00, 0x6c, 0xf8, 0x66, 0xcc, 0x00, 0x30, 0x30, 0x66, 0x30, 0x30, 0x00, 0x30, 0xc0, 
  0xe6, 0x30, 0xcc, 0xcc, 0x0c, 0xcc, 0xcc, 0x30, 0xcc, 0x18, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x00, 
  0xc0, 0xcc, 0x66, 0x66, 0x6c, 0x62, 0x60, 0x66, 0xcc, 0x30, 0xcc, 0x66, 0x66, 0xc6, 0xc6, 0x6c, 
  0x60, 0x78, 0x66, 0xcc, 0x30, 0xcc, 0x78, 0xee, 0x6c, 0x30, 0x66, 0x60, 0x06, 0x18, 0x00, 0x00, 
  0x00, 0x76, 0xdc, 0x78, 0x76, 0x78, 0xf0, 0x0c, 0xe6, 0x78, 0xcc, 0xe6, 0x78, 0xc6, 0xcc, 0x78, 
  0x60, 0x0c, 0xf0, 0xf8, 0x18, 0x76, 0x30, 0x6c, 0xc6, 0x0c, 0xfc, 0x30, 0x18, 0x30, 0x00, 0x00, 
};
unsigned char lcrasterline7[] = {
  0x00, 0x18, 0x00, 0x6c, 0x30, 0xc6, 0x76, 0x00, 0x18, 0x60, 0x00, 0x00, 0x70, 0x00, 0x30, 0x80, 
  0x7c, 0xfc, 0xfc, 0x78, 0x1e, 0x78, 0x78, 0x30, 0x78, 0x70, 0x30, 0x70, 0x18, 0x00, 0x60, 0x30, 
  0x78, 0xcc, 0xfc, 0x3c, 0xf8, 0xfe, 0xf0, 0x3e, 0xcc, 0x78, 0x78, 0xe6, 0xfe, 0xc6, 0xc6, 0x38, 
  0xf0, 0x1c, 0xe6, 0x78, 0x78, 0xfc, 0x30, 0xc6, 0xc6, 0x78, 0xfe, 0x78, 0x02, 0x78, 0x00, 0xfe, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xf0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x1c, 0x18, 0xe0, 0x00, 0x00, 
};

unsigned char *lcrasterlines[7] = {
  lcrasterline1, lcrasterline2, lcrasterline3, lcrasterline4, lcrasterline5, lcrasterline6, lcrasterline7
};


void startFrame()
{
  v_WaitRecal();
  v_readButtons();
  v_readJoystick1Analog();
}

int main(int argc, char **argv) {
  static char line[21];
  int i,x,y;
  char *prognamep;
  char *progname = argv[0];

  vectrexinit(1);
  if ((prognamep=strrchr(progname,  '/')) != NULL) progname = prognamep;
  v_setName(progname);
  v_init();
  v_set_font(lcrasterlines);
  // bufferType = 2; usePipeline = 1; 
  v_setRefresh(32);
  for (i = 0; i < 20; i++) line[i] = ' '; line[20] = '\0';
  for (;;) {
    startFrame();
    v_setBrightness(80);
    for (y = 32; y < 128 /* 32+64+16 */; y+=16 /*8*/) {
      line[0] = ' ';
      for (x = 0; x < 16; x++) {
	int c = x+y;
	//if (c == 32+9*8-1) c = ' '; // @
	//if (c == 32+10*8-1) c = ' '; // wide block
        //line[x*2+1] = c; line[x*2+1+1] = ' ';
        line[x] = c;
      }
      line[16] = ' '; line[17] = '\0';
      //line[8] = '\0';
      v_printStringRaster(-110, -((y-32)* 2 /*3*/ -88), line, 70, -14, '\0');
      //v_printStringRaster(-128, -((y-32)*3-88), line, 110, -14, '\0');
    }
  }
  return 0;
}
