/**********************************************/
/* thunder_game was coded originaly in pascal */
/* converted to C code in JULY.1993 by kaswan */
/**********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <math.h>

#define      wd 15        /* range of hiting */
#define      bld 20       /* displacement of bullet */
#define      enbld 10     /* displacement of enemy bullet */
#define      en1yd 6      /* enemy1 y-displacement */
#define      width 160    /* of x */
#define      blno 100      /* maximam numbers of bullet */
#define      enblno 50    /* maximam numbers of enemy bullet */
#define      bmno 5       /* power of bomb */
#define      en1no 5      /* numbers of enemy1 */
#define      fullhit 15    /* fullhit of enemy2 */
#define      movethunder 8    /* displacement of thunder */
#define      ESC 0x01
#define      UP 0x48
#define      DOWN 0x50
#define      LEFT 0x4B
#define      RIGHT 0x4D
#define      Lctrl 0x1D
#define      Lalt 0x38
#define      true  1
#define      false 0
#define      INTR 0x09

char far *k_e=(char far *)MK_FP(0x40,0x1c);  /* key ring buffer pointers */
char far *k_s=(char far *)MK_FP(0x40,0x1a);  /* these pointer point ring buffer's head and tail */

int txd,tyd;
int key;
unsigned char scancode[129];
int blpresent[101],enblpresent[51],enemy1present[16];
int enemy2present,enemy3present,enemy4present,poweruppresent;
int thunderpresent,flamepresent ;
int vs,ac,thndac;
unsigned char powerupyes,calsvthndyes,bombpresent;
unsigned char fireyes,bombyes,enemy2fireyes,enemy3fireyes,enemy4fireyes,bonus;
int i,ii,j,k,l,m,n;
int svthnd,obmpnt,nbmpnt;
int rnx,rny;
int en1nb,pwno;
int hit2no,hit3no,hit4no;
int oldpnt,newpnt,pntup;
int opnt0000,opnt000,opnt00,opnt0;
int npnt0000,npnt000,npnt00,npnt0;
int a,b,bb;
int en1x[16][2], en1y[16][2];
int en1xd[16];
float en1yr[16][2];
int en2x[2],en2y[2];
int en2xd,en2yd;
int en3x[2],en3y[2];
int en3xd,en3yd;
int en4x[2],en4y[2];
int en4xd;
float en4yr[2];
float en4yd;
int thunderx[2],thundery[2];
int thunderxd,thunderyd,fx,fy;
int pwx[2],pwy[2];
int pwyd;
int flagx,flagy;
int blx[101][2],bly[101][2];
float blxr[101][2],blyr[101][2];
float blxd[101],blyd[101];
float enblx[51][2],enbly[51][2];
float enblxd[51],enblyd[51];
unsigned char ch;
int maxx,maxy;
int hiscore;

char thunder[2][516]; /* -------- buffer for font -------- */
char enemy1[2][516];  /*                                   */
char enemy2[2][516];  /*                                   */
char enemy3[2][516];  /*                                   */
char enemy4[4][756];  /* --------------------------------- */

char thndbk[2][516];    /* ------ buffer for backgraund  */
char en1bk[16][2][516];
char en2bk[2][516];
char en3bk[2][516];
char en4bk[2][1524];     /* ----------------------------- */

char powerup[28];
char bulletf[28];
char bullete[28];
char woman[5][516];
char flame[804];   /* --------------------------------- */

void readfont(void);
void firesound(void);
void hitingsound(void);
void shitingsound(void);
void openscen(void);
void start(void);
void grpagedetect(void);
void grpagechange(void);
void thunderdraw(int x, int y);
void thundererase(int x,int y);
void enemy1draw(int x,int y);
void enemy1erase(int x,int y);
void enemy2draw(int x,int y);
void enemy2erase(int x,int y);
void enemy3draw(int x,int y);
void enemy3erase(int x,int y);
void enemy4draw(int x,int y);
void enemy4erase(int x,int y);
void nprintxy(int x, int y, int number);
void erase(void);
void draw(void);
void bombpoint(int bn );
void bonusup(void);
void thunderappear(void);
void flameappear(void);
void enemy1appear(void);
void enemy2appear(void);
void enemy3appear(void);
void enemy4appear(void);
void powerupappear(void);
void bulletmove(void);
void enemybulletmove(void);
void thundermove(void);
void enemy1move(void);
void enemy2move(void);
void enemy3move(void);
void enemy4move(void);
void powerupmove(void);
void calsvthnd(void);    /*  calculate survived thunder  */
void fire(void);
void bomb(void);
void enemy1fire(void);
void enemy2fire(void);
void enemy3fire(void);
void enemy4fire(void);
void enemy1hiting(void);
void enemy2hiting(void);
void enemy3hiting(void);
void enemy4hiting(void);
void interrupt (*oldkeyboard) (void); /* original keyboard routine(interrupt 0x9) */
void interrupt keyboard (void);       /* new keyboard routine */
void gameover(void);
void endscene(void);

void main(void) /* ------------------ main ------------------------------------ */
 {
   openscen();
restart:
   start();

   oldkeyboard = getvect(INTR);

   setvect(INTR, keyboard);


   thunderappear();
   do{
    grpagedetect();
    erase();
    /* ------------------------------key input-------------------------------------- */
    if( scancode[UP] ) { tyd=-movethunder; thundermove(); };
    if( scancode[LEFT] ) { txd=-movethunder; thundermove(); };
    if( scancode[RIGHT] ) { txd=movethunder; thundermove(); };
    if( scancode[DOWN] ) { tyd=movethunder; thundermove(); };
    if(!(scancode[UP] || scancode[LEFT] || scancode[RIGHT] || scancode[DOWN]))
      { thunderx[ac]=thunderx[vs]; thundery[ac]=thundery[vs]; };
    if( scancode[Lctrl] ) fireyes=true;
    if( scancode[Lalt] ) bombyes=true;
    if( scancode[ESC] ) { setvect(INTR,oldkeyboard); closegraph(); exit(0);}
    /* ----------------------------------------------------------------------------- */

   enemy2move();
   enemy3move();
   enemy4move();
   powerupmove();

   for (n=1; n<=en1no; n++)
    enemy1move();

   for (i=1;i<=blno;i++) {
   if( blpresent[i]>1 )
    bulletmove();
   }

   for (l=1;l<=enblno;l++) {
    if( enblpresent[l]>1 )
     enemybulletmove();
   }
   /* --------------check for bullet hiting of enemy--------------- */
   for (i=1;i<=blno;i++)
    if( blpresent[i]>1 ) {
     for (n=1;n<=en1no;n++){
      if( enemy1present[n]>1 ) {
       flagy=abs(en1y[n][ac]-bly[i][ac]);
       flagx=abs(en1x[n][ac]-blx[i][ac]);
       if((flagy<=wd) && (flagx<=wd))
	enemy1hiting();
      }
     }

     if( enemy2present>1 ) {
      flagx=abs(en2x[ac]-blx[i][ac]);
      flagy=abs(en2y[ac]-bly[i][ac]);
      if((flagx<20) && (flagy<20)) enemy2hiting();
     }

     if( enemy3present>1 ) {
      flagx=abs(en3x[ac]-blx[i][ac]);
      flagy=abs(en3y[ac]-bly[i][ac]);
      if((flagx<20) && (flagy<20)  )
       enemy3hiting();
     }

     if( enemy4present>1 ) {
      flagx=abs(en4x[ac]-blx[i][ac]);
      flagy=abs(en4y[ac]-bly[i][ac]);
      if((flagx<35) && (flagy<30)  )
       enemy4hiting();
     }
   }

   if(thunderpresent==-1) flameappear();
   if(flamepresent==-1)
    {
     calsvthnd();
     if( svthnd>0 ) thunderappear();
    }
   enemy1appear();
   enemy2appear();
   enemy3appear();
   enemy4appear();
   powerupappear();
   bonusup();
   fire();
   if((obmpnt > 0) && (bombyes) )  {
     bomb(); bombpoint(-1);
   }
   draw();

   grpagechange();

   } while (svthnd > 0);

   setactivepage(vs);
   setvisualpage(vs);
   setvect(INTR,oldkeyboard);
   gameover();

   switch (ch) {
	case 'Y','y' : goto restart;
	case 'N','n' : endscene();
   }
}


void readfont(void)
  {
    FILE *thnd;
    FILE *fpw,*fe1,*fe2,*fe3,*fe4;
    FILE *ftbl;
    FILE *febl;
    FILE *fwm[6];
    FILE *ffm;
    char *thndfilename;
    char *pwfilename;
    char *e1filename;
    char *e2filename;
    char *e3filename;
    char *e4filename;
    char *tblfilename;
    char *eblfilename;
    char *wmfilename[6];
    char *fmfilename;
    thndfilename="THUNDER.plf";
    pwfilename="powerup.fnt";
    e1filename="enemy1.plf";
    e2filename="enemy2.plf";
    e3filename="enemy3.plf";
    e4filename="enemy4.plf";
    tblfilename="bulletf.fnt";
    eblfilename="bullete.fnt";
    wmfilename[0] = "woman001.plf";
    wmfilename[1] = "woman002.plf";
    wmfilename[2] = "woman003.plf";
    wmfilename[3] = "woman004.plf";
    wmfilename[4] = "woman005.plf";
    fmfilename = "flame16.fnt";
    j=0;

     thnd=fopen(thndfilename,"rb");
     for (i=0;i<2;i++)
      fread(thunder[i],516,1,thnd);
     fclose(thnd);

     fpw=fopen(pwfilename,"rb");
     fread(powerup,10,1,fpw);
     fclose(fpw);

     fe1=fopen(e1filename,"rb");
     for(i=0;i<2;i++)
      fread(enemy1[i],516,1,fe1);
     fclose(fe1);

     fe2=fopen(e2filename,"rb");
     for(i=0;i<2;i++)
      fread(enemy2[i],516,1,fe2);
     fclose(fe2);

     fe3=fopen(e3filename,"rb");
     for(i=0;i<2;i++)
      fread(enemy3[i],516,1,fe3);
     fclose(fe3);

     fe4=fopen(e4filename,"rb");
     for(i=0;i<4;i++)
      fread(enemy4[i],756,1,fe4);
     fclose(fe4);

     ftbl=fopen(tblfilename,"rb");
     fread(bulletf,10,1,ftbl);
     fclose(ftbl);

     febl=fopen(eblfilename,"rb");
     fread(bullete,10,1,febl);
     fclose(febl);

    for(i=0;i<5;i++)
    {
     fwm[i]=fopen(wmfilename[i],"rb");
     fread(woman[i],516,1,fwm[i]);
     fclose(fwm[i]);
    }

     flame[0]=39;
     flame[1]=0;
     flame[2]=39;
     flame[3]=0;

     ffm=fopen(fmfilename,"rb");
     fread(&flame[4],800,1,ffm);
     fclose(ffm);
  }

 void firesound(void)
  {
   for(i=5;i>1;i--)
   {
    sound(i*1000);
    delay(2);
    nosound();
   }
  }

 void hitingsound(void)
   {
   sound(100);
   delay(3);
   nosound();
   sound(500);
   delay(2);
   nosound();
   }

 void shitingsound(void)
   {
   sound(300);
   delay(20);
   nosound();
   }

 void openscen(void)
  {
    int graphdriver,graphmode,errorcode;
    int rnx,rny,tm;

    readfont();

    graphdriver=EGA;
    graphmode=EGAHI;
    initgraph(&graphdriver,&graphmode,"");
    errorcode=graphresult();
    if (errorcode != 0 )
    {
      printf("Error : %s\n",grapherrormsg(errorcode));
      exit(1);
    }
  maxx=getmaxx();
  maxy=getmaxy();
  setcolor(1);
  setfillstyle(SOLID_FILL,1);
  bar(0,0,maxx,maxy-30);
  setfillstyle(SOLID_FILL,2);
  bar(10,10,maxx-10,maxy-40);
  setfillstyle(SOLID_FILL,3);
  bar(100,100,maxx-100,maxy-130);
  setfillstyle(SOLID_FILL,4);
  bar(103,103,maxx-103,maxy-133);

  rectangle(0,maxy-28,maxx,maxy);
  bar(2,maxy-26,maxx-2,maxy-2);
  settextjustify(CENTER_TEXT,TOP_TEXT);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
  setcolor(WHITE);
  outtextxy(maxx / 2,(maxy / 2)-30,"THUNDER");
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  outtextxy(maxx / 2,maxy / 2,"JULY. 1993   by KIM S.W.");
  outtextxy(maxx / 2,maxy - 18,"Press any key to start");
  putimage((maxx / 2)-16,maxy-100,thunder[1],AND_PUT);
  putimage((maxx / 2)-16,maxy-100,thunder[0],OR_PUT);
  randomize();
   for(i=0;i<5;i++)
   {
     rnx=random(maxx);
     rny=random(maxy / 3);
     putimage(rnx,rny,enemy1[1],OR_PUT);
     putimage(rnx,rny,enemy1[0],XOR_PUT);
   }
     rnx=random(maxx);
     rny=random(maxy / 3);
     putimage(rnx,rny,enemy2[1],OR_PUT);
     putimage(rnx,rny,enemy2[0],XOR_PUT);
     rnx=random(maxx);
     rny=random(maxy / 3);
     putimage(rnx,rny,enemy3[1],OR_PUT);
     putimage(rnx,rny,enemy3[0],XOR_PUT);
     rnx=random(maxx);
     rny=random(maxy / 3);
     putimage(rnx,rny,enemy4[2],OR_PUT);
     putimage(rnx+32,rny,enemy4[3],OR_PUT);
     putimage(rnx,rny,enemy4[0],XOR_PUT);
     putimage(rnx+32,rny,enemy4[1],XOR_PUT);
     getch();
   }

void grpagedetect(void)
  {
  switch (vs) {
       case  0:  vs=0; ac=1; ;break;
       case  1:  vs=1; ac=0; ;break;
  }
  setvisualpage(vs);
  setactivepage(ac);
 }

void grpagechange(void)
  {
  switch (vs) {
       case  1:  vs=0; ac=1; ;break;
       case  0:  vs=1; ac=0; ;break;
  }
  setvisualpage(vs);
  setactivepage(ac);
 }

void thunderdraw(int x, int y)
 {
   if((thunderpresent>0) && (thunderpresent<100))
    {
     if( ac==thndac ) {
      getimage(x-16,y-16,x+15,y+15,thndbk[ac]);
      getimage(x-16,y-16,x+15,y+15,thndbk[vs]);
      putimage(x-16,y-16,thunder[1],AND_PUT);
      putimage(x-16,y-16,thunder[0],OR_PUT);
     }
   }
   if(thunderpresent>101)
    {
      getimage(x-16,y-16,x+15,y+15,thndbk[ac]);
      putimage(x-16,y-16,thunder[1],AND_PUT);
      putimage(x-16,y-16,thunder[0],OR_PUT);
   }
 }

void thundererase(int x,int y)
 {
   if((thunderpresent>1) && (thunderpresent<101))
    {
     if( ac==thndac )  putimage(x-16,y-16,thndbk[ac],COPY_PUT);
    }
   if( thunderpresent>102 ) {
     putimage(x-16,y-16,thndbk[ac],COPY_PUT);
     thunderpresent=103;
   }
   if( thunderpresent<0 ) {
     thunderpresent=0;
     putimage(x-16,y-16,thndbk[ac],COPY_PUT);
   }
   if( thunderpresent>0 ) thunderpresent++;
 }

void enemy1draw(int x,int y)
 {
   if( enemy1present[i]>0 )
   {
    getimage(x-16,y-16,x+15,y+15,en1bk[i][ac]);
    putimage(x-16,y-16,enemy1[1],OR_PUT);
    putimage(x-16,y-16,enemy1[0],XOR_PUT);
   }
 }


void enemy1erase(int x,int y)
 {
  if((enemy1present[i]>1) || (enemy1present[i]<0) ) {
  putimage(x-16,y-16,en1bk[i][ac],COPY_PUT);
  if( enemy1present[i]<0 ) enemy1present[i]=0;
  }
  if( enemy1present[i]==1 ) enemy1present[i]=2;
 }

void enemy2draw(int x,int y)
 {
   if( enemy2present>0 ) {
   getimage(x-16,y-16,x+15,y+15,en2bk[ac]);
   putimage(x-16,y-16,enemy2[1],OR_PUT);
   putimage(x-16,y-16,enemy2[0],XOR_PUT);
   }
 }

void enemy2erase(int x,int y)
 {
   if((enemy2present>1) || (enemy2present<0))
   {
   putimage(x-16,y-16,en2bk[ac],COPY_PUT);
   if( enemy2present<0 ) enemy2present=0;
   }
   if( enemy2present==1 ) enemy2present=2;
 }

void enemy3draw(int x,int y)
 {
   if( enemy3present>0 ) {
   getimage(x-16,y-16,x+15,y+15,en3bk[ac]);
   putimage(x-16,y-16,enemy3[1],OR_PUT);
   putimage(x-16,y-16,enemy3[0],XOR_PUT);
   }
 }

void enemy3erase(int x,int y)
 {
   if((enemy3present>1) || (enemy3present<0)) {
   putimage(x-16,y-16,en3bk[ac],COPY_PUT);
   if( enemy3present<0 ) enemy3present=0;
   }
   if( enemy3present==1 ) enemy3present=2;
 }

void enemy4draw(int x,int y)
 {
   if( enemy4present>0 ) {
   getimage(x-32,y-23,x+31,y+23,en4bk[ac]);
   putimage(x-32,y-23,enemy4[2],OR_PUT);
   putimage(x,y-23,enemy4[3],OR_PUT);
   putimage(x-32,y-23,enemy4[0],XOR_PUT);
   putimage(x,y-23,enemy4[1],XOR_PUT);
   }
 }

void enemy4erase(int x,int y)
 {
   if((enemy4present>1) || (enemy4present<0)) {
   putimage(x-32,y-23,en4bk[ac],COPY_PUT);
   if( enemy4present<0 ) enemy4present=0;
   }
   if( enemy4present==1 ) enemy4present=2;
 }

void nprintxy(int x, int y, int number)
 {
  char text[2];
  itoa(number,text,10);
  outtextxy(x,y,text);
 }

void start(void)
 {

  int pnt3,pnt2;
  FILE *score ;
  char *scorefname="hiscore.rec";

  score=fopen(scorefname,"rb");
  fread(&hiscore,2,1,score);
  fclose(score);


  cleardevice();
  vs=0; ac=1;

  for(i=0;i<2;i++)
   {
    grpagedetect();
    cleardevice();
    thunderx[i]=maxx / 2;
    thundery[i]=maxy - 50;
    grpagechange();
   }
  thunderxd=0;
  thunderpresent=0;

  for(j=0;j<2;j++)
  {
  grpagedetect();
  setcolor(WHITE);
  setfillstyle(SOLID_FILL,8);
  bar(width,0,maxx-width,maxy);
  settextjustify(RIGHT_TEXT,CENTER_TEXT);
  outtextxy(width-10,90," QUIT : <Esc> ");
  outtextxy(width-10,130," MOVE : Arrow Keys");
  outtextxy(width-10,190," FIRE : <Lctrl>");
  outtextxy(width-10,210," BOMB : <Lalt>");
  outtextxy(width-90,230,"5");
  rectangle(maxx-width+10,10,maxx-10,50);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  outtextxy(maxx-(width / 2),20,"HIT  SCORE");
  outtextxy(maxx-30,40,"0");
  outtextxy(maxx-50,40,"0");
  outtextxy(maxx-70,40,"0");
  outtextxy(maxx-90,40,"0");
  outtextxy(maxx-110,40,"0");
  outtextxy(maxx-130,40,"0");

  outtextxy(maxx-(width / 2),60,"HIGH  SCORE");
  npnt0000=(int)(hiscore*0.001);
  pnt3=hiscore-npnt0000*1000;
  npnt000=(int)(pnt3*0.01);
  pnt2=pnt3-npnt000*100;
  npnt00=(int)(pnt2*0.1);
  npnt0=pnt2-npnt00*10;

  nprintxy(maxx-50,80,npnt0);
  nprintxy(maxx-70,80,npnt00);
  nprintxy(maxx-90,80,npnt000);
  nprintxy(maxx-110,80,npnt0000);
  nprintxy(maxx-30,80,0);

  for(i=0;i<4;i++){
   putimage(maxx-width+20,(maxy-40)-(i*40),thunder[1],OR_PUT);
   putimage(maxx-width+20,(maxy-40)-(i*40),thunder[0],XOR_PUT);
   }
  /* ------------draw backgraund------------------------------- */
  circle((maxx / 2),(maxy / 2),60);
  circle((maxx / 2),(maxy / 2),50);
  circle((maxx / 2)+70,(maxy / 2)+20,10);
  circle((maxx / 2),(maxy / 2)+70,20);
  circle((maxx / 2),maxy-30,20);
  setfillstyle(INTERLEAVE_FILL,WHITE);
  bar(width+10,10,width+110,80);
  setfillstyle(SOLID_FILL,WHITE);
  bar(width+130,10,maxx-width-10,80);
  bar((maxx / 2)-20,(maxy / 2)+70,(maxx / 2)+20,maxy-30);
  rectangle(maxx-width-10,90,maxx-width,100);
  settextjustify(CENTER_TEXT,TOP_TEXT);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
  setcolor(BLACK);
  outtextxy(width+200,20,"THUNDER");
  setcolor(WHITE);
  settextjustify(CENTER_TEXT,TOP_TEXT);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  grpagechange();
  }

  /* --------------- initialize -------------------------------- */
  svthnd=5;
  en1nb=0;
  pwno=0;
  hit2no=0;
  hit3no=0;
  hit4no=0;
  j=0;
  k=0;
  m=0;
  ii=0;
  newpnt=0;
  oldpnt=0;
  opnt0=0;
  opnt00=0;
  opnt000=0;
  opnt0000=0;
  obmpnt=5;
  setcolor(WHITE);
  for(i=1;i<=blno;i++){
   blpresent[i]=0;
   blx[i][ac]=thunderx[ac];
   bly[i][ac]=thundery[ac]-10;
  }
  for(i=1;i<=enblno;i++){
   enblpresent[i]=0;
  }
  thunderpresent=0;
  for (i=1;i<=en1no;i++)
   enemy1present[i]=0;
  enemy2present=0;
  enemy2fireyes=false;
  enemy3present=0;
  enemy3fireyes=false;
  enemy4present=0;
  enemy4fireyes=false;
  flamepresent=0;
  bombpresent=false;
  poweruppresent=0;
  powerupyes=false;
  bonus=false;
  calsvthndyes=false;
  bombyes=false;
  for(i=0;i<129;i++)
   scancode[i]=false;
  key=0;
  }

void erase(void)
 {
   for (i=enblno;i>0;i--) {
    if((enblpresent[i]>1) || (enblpresent[i]<0))
    putimage((int)enblx[i][ac]-4,(int)enbly[i][ac]-4,bullete,XOR_PUT);
    if( enblpresent[i]< 0 ) enblpresent[i]=0;
    if( enblpresent[i]==1 ) enblpresent[i]=2;
   }

   for (i=blno;i>0;i--) {
    if((blpresent[i]>1) || (blpresent[i]<0))
    putimage(blx[i][ac]-4,bly[i][ac]-4,bulletf,XOR_PUT);
    if( blpresent[i]<0 ) blpresent[i]=0;
    if( blpresent[i]==1 ) blpresent[i]=2;
   }

   if((poweruppresent>1) || (poweruppresent<0) )
    putimage(pwx[ac]-4,pwy[ac]-3,powerup,XOR_PUT);
    if( poweruppresent<0 ) poweruppresent=0;
    if( poweruppresent==1 ) poweruppresent=2;

   if((flamepresent>1) || (flamepresent<0) )
    putimage(fx,fy,flame,XOR_PUT);
    if( flamepresent<0 ) flamepresent=0;
    if( flamepresent==1 ) flamepresent=-1;

   thundererase(thunderx[ac],thundery[ac]);
   for (i= en1no;i>0;i--)
    enemy1erase(en1x[i][ac],en1y[i][ac]);
   enemy3erase(en3x[ac],en3y[ac]);
   enemy2erase(en2x[ac],en2y[ac]);
   enemy4erase(en4x[ac],en4y[ac]);

 };

void draw(void)
 {

   enemy4draw(en4x[ac],en4y[ac]);
   enemy2draw(en2x[ac],en2y[ac]);
   enemy3draw(en3x[ac],en3y[ac]);
   for (i= 1;i<=en1no;i++) {
    enemy1draw(en1x[i][ac],en1y[i][ac]);
   }
   thunderdraw(thunderx[ac],thundery[ac]);

   if( poweruppresent>0 )
    putimage(pwx[ac]-4,pwy[ac]-3,powerup,XOR_PUT);
   if( flamepresent>0 )
    putimage(fx,fy,flame,XOR_PUT);
   for (i=1;i<=blno;i++) {
    if( blpresent[i]>0 ) putimage(blx[i][ac]-4,bly[i][ac]-4,bulletf,XOR_PUT);
   }
   for (i=1;i<=enblno;i++) {
    if( enblpresent[i]>0 )
    putimage((int)(enblx[i][ac]-4),(int)(enbly[i][ac]-4),bullete,XOR_PUT);
   }
 }


 void bombpoint(int bn )
  {
   nbmpnt=obmpnt+bn;
   if( nbmpnt<0 ) nbmpnt=0;
   for(i=1;i<3;i++) {
    if( i == 1 ) setactivepage(vs);
    settextjustify(RIGHT_TEXT,CENTER_TEXT);
    setcolor(BLACK);
    nprintxy(width-90,230,obmpnt);
    setcolor(WHITE);
    nprintxy(width-90,230,nbmpnt);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setactivepage(ac);
   }
   obmpnt=nbmpnt;
  }

 void thunderappear(void)
  {
     for(i=0;i<2;i++) {
      thunderx[i]=maxx / 2;
      thundery[i]=maxy - 50;
     }
     thunderpresent=1;
     switch (ac % 2) {
	   case   0 : thndac=0; break;
	   case   1 : thndac=1; break;
     }
  }

 void flameappear(void)
  {
   flamepresent=1;
   fx=thunderx[ac]-20;
   fy=thundery[ac]-20;
  }

 void bonusup(void)
  {
   int kk;
   if ((bonus==false) && (svthnd<9) && (newpnt>=500) && ((newpnt % 500) < 250))
   {
    bonus=true;
    svthnd=svthnd+1;
    for (kk=1;kk<3;kk++)
    {
     if( kk==1 ) setactivepage(vs);
     putimage(maxx-width+20,(maxy-40)-(svthnd-2)*40,thunder[1],XOR_PUT);
     putimage(maxx-width+20,(maxy-40)-(svthnd-2)*40,thunder[0],XOR_PUT);

     setactivepage(ac);
    }
   }
   if((newpnt % 500) >= 250) bonus=false;
  }

 void calsvthnd(void)   /*  calculate survived thunder  */
  {
   sound(200);
   delay(500);
   nosound();
   if( svthnd>=0 ) {
    svthnd--;
    powerupyes=false;
    pwno=0;
    if( svthnd!=0 ) {
    for(i=1;i<3;i++) {
     if( i==1 )  setactivepage(vs);
     putimage(maxx-width+20,(maxy-40)-(svthnd-1)*40,thunder[1],COPY_PUT);
     putimage(maxx-width+20,(maxy-40)-(svthnd-1)*40,thunder[1],XOR_PUT);
     delay(300);
    nbmpnt=5;
    settextjustify(RIGHT_TEXT,CENTER_TEXT);
    setcolor(BLACK);
    nprintxy(width-90,230,obmpnt);
    setcolor(WHITE);
    nprintxy(width-90,230,nbmpnt);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setactivepage(ac);
    }
    thunderpresent = -1;
    obmpnt=nbmpnt;
    }
   }
  }

 void enemy1appear(void)
   {
   int kk;
   for (k=1;k<=en1no;k++) {
   if((enemy1present[k]<1) && (enemy1present[k]>-1) ) {
     en1xd[k]=random(4)-2;
     en1x[k][0]=random(maxx-width*2)+width;
     en1yr[k][0]=16;
     en1y[k][0]=(int)(en1yr[k][0]);
     en1x[k][1]=en1x[k][0];
     en1yr[k][1]=en1yr[k][0];
     en1y[k][1]=en1y[k][0];
     en1nb++;
     enemy1present[k]++;
     }
   }
  }

void enemy2appear(void)
  {
   if((random(100)==0) && (enemy2present<1) &&(enemy2present>-1) && (en1nb>500) )
   {
     for (i=0;i<2;i++) {
     en2x[i]=(maxx / 2) + random(200)-100;
     en2y[i]=16;
     }
     en2xd=(random(3)-1);
     en2yd=1;
     enemy2present++;
     }
   }

 void enemy3appear(void)
   {
   if((random(80)==0) && (enemy3present<1) && (enemy3present>-1) && (en1nb>250) )
    {
     for (i=0;i<2;i++) {
     en3x[i]=(maxx / 2) + random(200)-100;
     en3y[i]=16;
     }
     en3xd=random(3)-1;
     en3yd=5;
     enemy3present++;
    }
   }

 void enemy4appear(void)
   {
   int ranx;
   if((random(300)==0) && (enemy4present<1) && (enemy4present>-1) && (en1nb>850) )
    {
     enemy4present=1;
     ranx=random(100);
     for (i=0;i<2;i++) {
     en4x[i]=(maxx / 2) + ranx - 50;
     en4yr[i]=30;
     en4y[i]=30;
     }
     en4yd=0.8;
    }
   }

 void powerupappear(void)
   {
   if((random(100)==0) && (poweruppresent<1) && (poweruppresent>-1))
    {
     poweruppresent++;
     for (i=0;i<2;i++) {
     pwx[i]=en1x[n][i];
     pwy[i]=en1y[n][i];
     }
     pwyd=1;
    }
   }

 void bulletmove(void)
   {
     blxr[i][ac]=blxr[i][vs]+blxd[i];
     blyr[i][ac]=blyr[i][vs]+blyd[i];
     blx[i][ac]=(int)(blxr[i][ac]);
     bly[i][ac]=(int)(blyr[i][ac]);
    if ((blx[i][ac]>width) && (blx[i][ac]<(maxx-width)) && (bly[i][ac]>0) &&
     (bly[i][ac]<maxy)){}

    else blpresent[i]=-1;
   }

 void enemybulletmove(void)
   {
    enblx[l][ac]=enblx[l][vs]+enblxd[l];
    enbly[l][ac]=enbly[l][vs]+enblyd[l];
    if((enblx[l][ac]>width) && (enblx[l][ac]<(maxx-width)) &&
       (enbly[l][ac]>0) && (enbly[l][ac]<maxy))
     {
       if((abs(enblx[l][ac]-thunderx[ac])<15) && (abs(enbly[l][ac]-thundery[ac])<15)
	  && (thunderpresent>103))
	  {
	   enblpresent[l]=-1;
	   thunderpresent=-1;
	  }
       }
      else enblpresent[l]=-1;
    }

 void thundermove(void)
  {
   thunderx[ac]=thunderx[vs]+txd;
   if((thunderx[ac]<(maxx-width)) && (thunderx[ac]>width))
   {}else {
    thunderx[ac]=thunderx[ac]-txd;
   }
   thundery[ac]=thundery[vs]+tyd;
   if((thundery[ac]>16) && (thundery[ac]<(maxy-16)) )
   {}else {
    thundery[ac]=thundery[ac]-tyd;
   }
   txd=0; tyd=0;
  }

 void fire(void)
 {
  int jjj,pwnb;
  float th,dth;

  if( fireyes )
   {
    switch (powerupyes)
     {
      case false :
	   j++;
	   if((j>=1) && (j<=blno) && (blpresent[j]<1) && (blpresent[j]>-1))
	    {
	     for (i=0;i<2;i++)
	      {
	       blxr[j][i]=thunderx[vs];
	       blyr[j][i]=thundery[vs]-20;
	       blx[j][i]=(int)(blxr[j][i]);
	       bly[j][i]=(int)(blyr[j][i]);
	      }
	     blxd[j]=0;
	     blyd[j]=-bld;
	     firesound();
	     blpresent[j]++;
	    }
	   else j=0;
	   break;

     case true :
	  firesound();
	  if( pwno==1 ) { th=1.3; dth=0.2; pwnb=2; }
	  if( pwno==2 ) { th=1; dth=0.3; pwnb=3; }
	  if( pwno==3 ) { th=1.3; dth=0.1; pwnb=4; }
	  if( pwno>=4 ) { th=1.3; dth=0.08; pwnb=5; }
	  for (jjj=1;jjj<=pwnb;jjj++) {
	   j++;
	   if((j>=1) && (j<=blno) && (blpresent[j]<1) && (blpresent[j]>-1))
	    {
	     th=th+dth;
	     for(i=0;i<2;i++) {
	       blxr[j][i]=thunderx[vs];
	       blyr[j][i]=thundery[vs]-20;
	       blx[j][i]=(int)(blxr[j][i]);
	       bly[j][i]=(int)(blyr[j][i]);
	      }
	     blpresent[j]=blpresent[j]+1;
	     blxd[j]=bld*cos(th);
	     blyd[j]=-bld*sin(th);
	    }
	   else j=0;
	  }


     }
   }
   fireyes=false;
  }

 void enemy1fire(void)
  {
    float x,y,z;
    if((m>=1) && (m<=enblno) && (enblpresent[m]<1) && (enblpresent[m]>-1)
     && (en1nb>30)) {
      for (i=0;i<2;i++) {
      enblx[m][i]=en1x[n][ac];
      enbly[m][i]=en1y[n][ac];
      }
      enblpresent[m]++;
      x=thunderx[ac]-en1x[n][ac];
      y=thundery[ac]-en1y[n][ac];
      z=sqrt(x*x+y*y);
      enblxd[m]=x*enbld/z;
      enblyd[m]=y*enbld/z;
     }
    else m=0;
  }

 void enemy2fire(void)
  {
    float th,dth;
    int burnno,jj;
    jj=0;
    if( en1nb <700  ) {
      burnno=3;
      th=0.75;
     }
     else {
      burnno=5;
      th=0.3;
     }
    dth=0.45;
    for (jj=1;jj<=burnno;jj++) {
     m++;
     if((m>=1) && (m<=enblno) && (enblpresent[m]<1)
	&& (enblpresent[m]>-1) ) {
      th=th+dth;
      for (i=0;i<2;i++){
      enblx[m][i]=en2x[ac];
      enbly[m][i]=en2y[ac];
      }
      enblpresent[m]++;
      enblxd[m]=enbld*cos(th)*2;
      enblyd[m]=enbld*sin(th)*2;
     }
     else  m=0;
    enemy2fireyes=false;
  }
  }

 void enemy3fire(void)
  {
    float x,y,z;
    int jj;
    jj=0;

    x=thunderx[ac]-en3x[ac];
    y=thundery[ac]-en3y[ac];
    z=sqrt(x*x+y*y);

    for (jj=1;jj<3;jj++)
    {
     m++;
     if((m>=1) && (m<=enblno) && (enblpresent[m]<1)
	&& (enblpresent[m]>-1))
      {
       for(i=0;i<2;i++) {
	 enblx[m][i]=en3x[ac];
	 enbly[m][i]=en3y[ac];
	}
       enblpresent[m]++;
       switch (jj){
	   case 1 :  { enblxd[m]=x*enbld*2.5/z+1;
		      enblyd[m]=y*enbld*2.5/z;break; }
	   case 2 :  { enblxd[m]=x*enbld*2.5/z-1;
		      enblyd[m]=y*enbld*2.5/z;break; }

	}
      }
     else  m=0;
    }
    enemy3fireyes=false;

  }

 void enemy4fire(void)
  {
    float x,y,z, th,dth;
    int jj ;
    th=0.4;
    dth=0.3;
    for (jj=1;jj<11;jj++){
     m++;
     if((m>=1) && (m<=enblno) && (enblpresent[m]<1)
	&& (enblpresent[m]>-1) ) {
      th=th+dth;
      for (i=0;i<2;i++){
      enblx[m][i]=en4x[ac];
      enbly[m][i]=en4y[ac];
      }
      enblpresent[m]++;
      enblxd[m]=enbld*cos(th)*2.3;
      enblyd[m]=enbld*sin(th)*2.3;
      }
     else m=0;
    }
    x=thunderx[ac]-en4x[ac];
    y=thundery[ac]-en4y[ac];
    z=sqrt(x*x+y*y);
    for (jj=1;jj<3;jj++) {
     m++;
     if((m>=1) && (m<=enblno) && (enblpresent[m]<1)
	&& (enblpresent[m]>-1) ) {
      for (i=0;i<2;i++){
      enblx[m][i]=en4x[ac];
      enbly[m][i]=en4y[ac];
      }
      enblpresent[m]++;
      switch (jj){
	   case 1 :  { enblxd[m]=x*enbld*4/z+1;
		      enblyd[m]=y*enbld*4/z;break; }
	   case 2 :  { enblxd[m]=x*enbld*4/z-1;
		      enblyd[m]=y*enbld*4/z;break; }
       }

     }
     else m=0;
    }
    enemy4fireyes=false;
  }

 void pointup(void)
   {
    int pnt3,pnt2,kk;
    newpnt=oldpnt+pntup;
    if( newpnt<10000 ) {
      npnt0000=(int)(newpnt*0.001);
      pnt3=newpnt-npnt0000*1000;
      npnt000=(int)(pnt3*0.01);
      pnt2=pnt3-npnt000*100;
      npnt00=(int)(pnt2*0.1);
      npnt0=pnt2-npnt00*10;
     }
     else  {
      newpnt=0;
      npnt0=0;
      npnt00=0;
      npnt000=0;
      npnt0000=0;
     }
     settextjustify(CENTER_TEXT,CENTER_TEXT);
     for(i=0;i<2;i++) {
      if( i==0 ) setactivepage(vs);
      setcolor(BLACK); nprintxy(maxx-50,40,opnt0);
      setcolor(WHITE); nprintxy(maxx-50,40,npnt0);
      setcolor(BLACK); nprintxy(maxx-70,40,opnt00);
      setcolor(WHITE); nprintxy(maxx-70,40,npnt00);
      setcolor(BLACK); nprintxy(maxx-90,40,opnt000);
      setcolor(WHITE); nprintxy(maxx-90,40,npnt000);
      setcolor(BLACK); nprintxy(maxx-110,40,opnt0000);
      setcolor(WHITE); nprintxy(maxx-110,40,npnt0000);
      setactivepage(ac);
     }
     oldpnt=newpnt;
     opnt0=npnt0;
     opnt00=npnt00;
     opnt000=npnt000;
     opnt0000=npnt0000;
    }

 void enemy1hiting(void)
   {
   setcolor(WHITE);
   if((bombpresent==false) && (blpresent[i]>1)) {
    blpresent[i]=-1;
   }
   hitingsound();
   setactivepage(vs);
   putimage(en1x[n][ac]-20,en1y[n][ac]-20,flame,XOR_PUT);
   delay(15);
   putimage(en1x[n][ac]-20,en1y[n][ac]-20,flame,XOR_PUT);
   putimage(en1x[n][ac]-20,en1y[n][ac]-20,flame,XOR_PUT);
   delay(7);
   putimage(en1x[n][ac]-20,en1y[n][ac]-20,flame,XOR_PUT);
   en1x[n][ac]=0;
   en1y[n][ac]=0;
   enemy1present[n]=-1;
   setactivepage(ac);
   pntup=1;
   pointup();
   };

 void enemy2hiting(void)
  {
   setcolor(WHITE);
   hit2no++;
   shitingsound();
   if((bombpresent==false) && (blpresent[i]>1) ) {
    blpresent[i]=-1;
   };
   if( hit2no >=fullhit
    ) {
    enemy2present=-1;
    setactivepage(vs);
    putimage(en2x[ac]-16,en2y[ac]-16,flame,XOR_PUT);
    delay(10);
    putimage(en2x[ac]-16,en2y[ac]-16,flame,XOR_PUT);
    putimage(en2x[ac],en2y[ac],flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en2x[ac],en2y[ac],flame,XOR_PUT);
    putimage(en2x[ac]-6,en2y[ac]-16,flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en2x[ac]-6,en2y[ac]-16,flame,XOR_PUT);
    putimage(en2x[ac]-20,en2y[ac]-20,flame,XOR_PUT);
    delay(20);
    putimage(en2x[ac]-20,en2y[ac]-20,flame,XOR_PUT);
    setactivepage(ac);
    pntup=20;
    pointup();
    hit2no=0;
   };
  };

 void enemy3hiting(void)
  {
   setcolor(WHITE);
   hit3no++;
   shitingsound();
   if((bombpresent==false) && (blpresent[i]>1) ) {
    blpresent[i]=-1;
   };
   if( hit3no >=5 ) {
    enemy3present=-1;
    setactivepage(vs);
    putimage(en3x[ac]-20,en3y[ac]-20,flame,XOR_PUT);
    delay(10);
    putimage(en3x[ac]-20,en3y[ac]-20,flame,XOR_PUT);
    putimage(en3x[ac],en3y[ac],flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en3x[ac],en3y[ac],flame,XOR_PUT);
    putimage(en3x[ac]-6,en3y[ac]-16,flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en3x[ac]-6,en3y[ac]-16,flame,XOR_PUT);
    setactivepage(ac);
    if( random(2)==0 ) bombpoint(1);
    pntup=10;
    pointup();
    hit3no=0;
   }
  }

 void enemy4hiting(void)
  {
   setcolor(WHITE);
   hit4no++;
   shitingsound();
   if((bombpresent==false) && (blpresent[i]>1) ) {
    blpresent[i]=-1;
   }
   if( hit4no >=75 ) {
    enemy4present=-1;
    setactivepage(vs);
    putimage(en4x[ac]-16,en4y[ac]-16,flame,XOR_PUT);
    delay(10);
    putimage(en4x[ac]-16,en4y[ac]-16,flame,XOR_PUT);
    putimage(en4x[ac],en4y[ac],flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en4x[ac],en4y[ac],flame,XOR_PUT);
    putimage(en4x[ac]-6,en4y[ac]-16,flame,XOR_PUT);
    delay(10);
    shitingsound();
    putimage(en4x[ac]-6,en4y[ac]-16,flame,XOR_PUT);
    setactivepage(ac);
    pntup=50;
    pointup();
    hit4no=0;
   };
  };

 void bomb(void)
  {
  int x,y;
   if( bombpresent==false ) {
    bombpresent=true;
    x=thunderx[ac];
    y=thundery[ac]-100;
    setactivepage(vs);
    putimage(x-16,y-16,flame,XOR_PUT);
    delay(5);
    shitingsound();
    putimage(x-16,y-16,flame,XOR_PUT);
    putimage(x+30,y+30,flame,XOR_PUT);
    delay(5);
    shitingsound();
    putimage(x+30,y+30,flame,XOR_PUT);
    putimage(x+26,y-16,flame,XOR_PUT);
    delay(5);
    shitingsound();
    putimage(x+26,y-16,flame,XOR_PUT);
    putimage(x-20,y+20,flame,XOR_PUT);
    delay(5);
    putimage(x-20,y+20,flame,XOR_PUT);
    setactivepage(ac);

    for (i=1;i<=enblno;i++) {
     if( enblpresent[i]==2 )  enblpresent[i]=-1;
    };

    for (bb=1;bb<=bmno;bb++) {
     for (n=1;n<=en1no;n++)
      if( enemy1present[n]>1 ) enemy1hiting();

     if( enemy2present>1 ) enemy2hiting();

     if( enemy3present>1 ) enemy3hiting();

     if( enemy4present>1 ) enemy4hiting();
    }
   }
   bombpresent=false;
   bombyes=false;
  }

 void enemy1move(void)
  {
   powerupappear();
   if( enemy1present[n]>1 ) {
     rny=en1yd+random(1)*2+en1nb*0.01/3;
     en1yr[n][ac]=en1yr[n][vs]+en1yd;
     en1y[n][ac]=(int)(en1yr[n][ac]);
     en1x[n][ac]=en1x[n][vs]+en1xd[n];
     if((en1x[n][ac]>width) && (en1x[n][ac]<(maxx-width)) && (en1y[n][ac]<maxy))
      {
	if((random(400)==0)) {
	   m++;
	   enemy1fire();
	 };
	if((abs(en1x[n][ac]-thunderx[ac])<15) && (abs(en1y[n][ac]-thundery[ac])<15)
	   && (thunderpresent>103) )
	  {
	   enemy1present[n]=-1;
	   thunderpresent=-1;
	   flamepresent=1;
	  }
       }
	  else  enemy1present[n]=-1;
   }
   }

 void enemy2move(void)
   {
    if( enemy2present>1 ) {
      en2x[ac]=en2x[vs]+en2xd;
      en2y[ac]=en2y[vs]+en2yd;
      if((en2x[ac]>width) && (en2x[ac]<(maxx-width)) &&
	  (en2y[ac]>0) && (en2y[ac]<maxy) )  {
	 if((random(50)==0) && (en2y[ac]<250) && (enemy2fireyes==false) )
	  {
	   enemy2fireyes=true;
	   enemy2fire();
	  };
	 if((abs(en2x[ac]-thunderx[ac])<30) && (abs(en2y[ac]-thundery[ac])<30)
	    && (thunderpresent>103) )
	  {
	   enemy2present=-1;
	   hit2no=0;
	   thunderpresent=-1;
	   flamepresent=1;
	  };
       }
      else {
	enemy2present=-1;
	hit2no=0;
       };
    };
   };

 void enemy3move(void)
   {
    if( enemy3present>1 ) {
      en3x[ac]=en3x[vs]+en3xd;
      en3y[ac]=en3y[vs]+en3yd;
      if((en3x[ac]>width) && (en3x[ac]<(maxx-width)) &&
	  (en3y[ac]>0) && (en3y[ac]<maxy) )  {
	 if((random(60)==0) && (enemy3fireyes==false) )
	  {
	   enemy3fireyes=true;
	   enemy3fire();
	  };
	 if((abs(en3x[ac]-thunderx[ac])<30) && (abs(en3y[ac]-thundery[ac])<30)
	    && (thunderpresent>103) )
	  {
	   enemy3present=-1;
	   hit3no=0;
	   thunderpresent=-1;
	   flamepresent=1;
	  };
       }
      else {
	enemy3present=-1;
	hit3no=0;
       };
    };
   };

 void enemy4move(void)
   {
    if( enemy4present>1 ) {
      en4yr[ac]=en4yr[vs]+en4yd;
      en4y[ac]=(int)(en4yr[ac]);
      if((en4x[ac]>width) && (en4x[ac]<(maxx-width)) &&
	  (en4y[ac]>0) && (en4y[ac]<maxy) )  {
	 if((random(35)==0) && (en4y[ac]<200) && (enemy4fireyes==false) )
	  {
	   enemy4fireyes=true;
	   enemy4fire();
	  };
       }
      else {
	enemy4present=-1;
	hit4no=0;
       };
    };
   };

 void powerupmove(void)
   {
    if( poweruppresent>1 ) {
      pwy[ac]=pwy[vs]+pwyd;
      if((pwx[ac]>width) && (pwx[ac]<(maxx-width)) &&
	  (pwy[ac]>0) && (pwy[ac]<maxy) )  {
	 if((abs(pwx[ac]-thunderx[ac])<20) && (abs(pwy[ac]-thundery[ac])<20)
	    && (thunderpresent>1) )
	  {
	   poweruppresent=-1;
	   powerupyes=true;
	   pwno=pwno+1;
	   sound(1000);
	   delay(20);
	   nosound();
	  };
       }
       else poweruppresent=-1;
    }
   }

void gameover(void)
   {
    FILE *score;
    char *scorefilename = "hiscore.rec";

    setcolor(WHITE);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
    outtextxy(maxx / 2,150,"GAME    OVER");
    settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
    outtextxy(maxx / 2,200,"PLAY GAME AGAIN ? (Y/N)");
    svthnd=5;
    if( newpnt>hiscore ) {
      hiscore=newpnt;
      score=fopen(scorefilename,"wb");
      fwrite(&hiscore,sizeof(hiscore),1,score);
      fclose(score);
     };
    do
     ch=getch();
    while ((ch!='y') && (ch!='Y') && (ch!='n') && (ch!='N'));
   };

 void endscene(void)
   {
     int j=0;
    do  {
     j=j+6;
     setcolor(WHITE);
     outtextxy(100-j,120,"Good-Bye !");
     for (i= 0;i<=4;i++)
      {
	putimage(50-i-j,150,woman[i],XOR_PUT);
	delay(200);
	putimage(50-i-j,150,woman[i],XOR_PUT);
      }
     setcolor(BLACK);
     outtextxy(100-j,120,"Good-Bye !");
    }
    while( j< 45);
    closegraph();
   }

void interrupt ( *oldkeyboard ) (void);  /* interrupt 0x09 (bios keyboard routin) */

void interrupt keyboard(void)
{
    __emit__(0xfa); /* in inline asembly : asm cli */

    /* read key value from io port 0x60 (buffer on the keyboard) */
    if(inportb(0x60)<128) scancode[inportb(0x60)]=1;  /* key down */
    if(inportb(0x60)>128) scancode[inportb(0x60)-128]=0; /* key up */

    *k_e=*k_s;      /* key buffer ( ring buffer) clear : end pointer = start pointer */
		    /* this buffer is not on the keyboard but on the system memory */

    oldkeyboard();  /* call original bios keyboard routin */

    __emit__(0xfb); /* asm sti */
}



