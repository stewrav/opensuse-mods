/*

 * http://www.samag.com/documents/s=9338/sam0410i/0410i.htm

 * Listing 3: keycode:
 * When a key is pressed, this program displays the key and its value
 * in decimal, octal, and hexadecimal values.
 */
#include <stdio.h>
#include <sys/types.h>
#include <termio.h>

#define EOT 004
#define BEL 007
#define BS  010
#define LF  012
#define ESC 033
#define DEL 0177

int main(argc, argv)
int  argc;
char *argv[];
{
char    ch;
struct  termio tty, oldtty;

if (ioctl(0, TCGETA, &tty)) 
    {
    perror("ioctl");
    exit(1);
    }
/* save current terminal settings */ 
oldtty = tty; 
/* turn off canonical mode, echo, etc */
oldtty.c_iflag &= ~IXON;
oldtty.c_iflag &= ~IXOFF;
oldtty.c_lflag &= ~ISIG;
oldtty.c_lflag &= ~ICANON;
oldtty.c_lflag &= ~ECHO;
oldtty.c_oflag &= ~OPOST;
oldtty.c_cc[VTIME] = 30;
oldtty.c_cc[VMIN] = 1;
ioctl(0, TCSETA, &oldtty);

printf("Enter keys to display in dec, octal, and hex.  press control-d to \
        quit\n\r");
do {
   printf("keycode> ");
   ch=getc(stdin);
   switch (ch) {
   case ESC : 
      printf("<ESC>");
      break;
   case EOT : 
      printf("<EOT>");
      break;
   case DEL : 
      printf("<DEL>");
      break;
   case LF : 
      printf("<LF> ");
      break;
   case BS : 
      printf("<BS> ");
      break;
   default :
      printf("    ");
      break;
   }
   printf("  %c    %4d 0%-4o 0x%-4x\n\r",ch, ch, ch, ch);

} while ((ch != EOT));

ioctl(0, TCSETA, &tty);
exit(0);
}
