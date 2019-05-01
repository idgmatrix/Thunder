PROGRAM thunder_game;
 uses crt,dos,graph;
 label 10;
 const
   wd=15;        {range of hiting}
   bld=19;       {displacement of bullet}
   enbld=7;      {displacement of enemy bullet}
   en1yd=3;      {enemy1 y-displacement}
   width=170;    {of x}
   blno=50;      {maximam numbers of bullet}
   enblno=50;    {maximam numbers of enemy bullet}
   bmno=5;       {power of bomb}
   en1no=5;      {numbers of enemy1}
   fullhit=15;   {fullhit of enemy2}
   movethunder=7;    {displacement of thunder}
   scorefilename='hiscore.rec';  {hiscore record}
   ESC=$01;
   UP=$48;
   DOWN=$50;
   LEFT=$4B;
   RIGHT=$4D;
   SPC=$39;
   Lalt=$38;
 var
   score : text;
   txd,tyd:integer;
   intsave9:pointer;
   key:integer;
   scancode:array[0..128] of boolean;
   blpresent,enblpresent,enemy1present:array[1..50] of integer;
   enemy2present,enemy3present,enemy4present,poweruppresent,
   thunderpresent,flamepresent : integer;
   vs,ac,thndac:integer;
   powerupyes,calsvthndyes,bombpresent,
   fireyes,bombyes,enemy2fireyes,enemy3fireyes,enemy4fireyes,bonus : boolean;
   i,ii,j,k,l,m,n:integer;
   svthnd,obmpnt,nbmpnt:integer;
   rnx,rny : integer;
   en1nb,pwno : integer;
   hit2no,hit3no,hit4no : integer;
   oldpnt,newpnt,pntup : integer;
   opnt0000,opnt000,opnt00,opnt0 : integer;
   npnt0000,npnt000,npnt00,npnt0 : integer;
   a,b,bb : integer;
   en1x,en1y : array[1..15,0..1] of integer;
   en1xd : array[1..15]of integer;
   en1yr : array [1..15,0..1] of real;
   en2x,en2y : array[0..1] of integer;
   en2xd,en2yd : integer;
   en3x,en3y : array[0..1] of integer;
   en3xd,en3yd : integer;
   en4x,en4y : array[0..1] of integer;
   en4xd : integer;
   en4yr : array[0..1] of real;
   en4yd : real;
   thunderx,thundery : array[0..1]of integer;
   thunderxd,thunderyd,fx,fy : integer;
   pwx,pwy : array[0..1] of integer;
   pwyd : integer;
   flagx,flagy : integer;
   blx,bly : array[1..50,0..1] of integer;
   blxr,blyr : array[1..50,0..1] of real;
   blxd,blyd : array[1..50] of real;
   enblx,enbly : array[1..50,0..1] of real;
   enblxd,enblyd : array[1..50] of real;
   ch : char;
   maxx,maxy : integer;
   highscore : integer;
   thunder : array[1..2,1..132] of byte; {-------- variable for font ------}
   enemy1 : array[1..2,1..132] of byte;  {                                 }
   enemy2 : array[1..2,1..132] of byte;  {                                 }
   enemy3 : array[1..2,1..132] of byte;  {                                 }
   enemy4 : array[1..4,1..192] of byte;  {---------------------------------}
   thndbk : array[0..1,1..1028] of byte;    {------ buffer for backgraund }
   en1bk : array[1..15,0..1,1..1028] of byte;
   en2bk : array[0..1,1..1028] of byte;
   en3bk : array[0..1,1..1028] of byte;
   en4bk : array[0..1,1..2052] of byte;   {-----------------------------}
   powerup : array[1..10] of byte;
   bulletf : array[1..10] of byte;
   bullete : array[1..10] of byte;
   woman : array[1..5,1..132] of byte;
   flame : array[1..2,1..204] of byte;   {---------------------------------}

 function inttostr(i : integer) : string;
  var
   s : string[3];
  begin
   str(i,s);
   inttostr:=s;
  end;

 procedure readfont;
  var
    thnd : file;
    fpw,
    fe1,fe2,fe3,fe4 : file;
    ftbl : file;
    febl : file;
    fwm : array[1..5] of file;
    ffm : file;
    thndfilename : string;
    pwfilename,
    e1filename : string;
    e2filename : string;
    e3filename : string;
    e4filename : string;
    tblfilename : string;
    eblfilename : string;
    wmfilename : array[1..5] of string;
    fmfilename : string;
    numread : word;
  begin
    thndfilename:='THUNDER.fnt';
    pwfilename:='powerup.fnt';
    e1filename:='enemy1.fnt';
    e2filename:='enemy2.fnt';
    e3filename:='enemy3.fnt';
    e4filename:='enemy4.fnt';
    tblfilename:='bulletf.fnt';
    eblfilename:='bullete.fnt';
    wmfilename[1] := 'woman001.fnt';
    wmfilename[2] := 'woman002.fnt';
    wmfilename[3] := 'woman003.fnt';
    wmfilename[4] := 'woman004.fnt';
    wmfilename[5] := 'woman005.fnt';
    fmfilename := 'flame.fnt';
    j:=0;

     assign(thnd,thndfilename);
     reset(thnd,1);
     for i:=1 to 2 do
      blockread(thnd,thunder[i],sizeof(thunder[i]));
     close(thnd);

     assign(fpw,pwfilename);
     reset(fpw,1);
     blockread(fpw,powerup,sizeof(powerup));
     close(fpw);

     assign(fe1,e1filename);
     reset(fe1,1);
     for i:=1 to 2 do
      blockread(fe1,enemy1[i],sizeof(enemy1[i]));
     close(fe1);

     assign(fe2,e2filename);
     reset(fe2,1);
     for i:=1 to 2 do
      blockread(fe2,enemy2[i],sizeof(enemy2[i]));
     close(fe2);

     assign(fe3,e3filename);
     reset(fe3,1);
     for i:=1 to 2 do
      blockread(fe3,enemy3[i],sizeof(enemy3[i]));
     close(fe3);

     assign(fe4,e4filename);
     reset(fe4,1);
     for i:=1 to 4 do
      blockread(fe4,enemy4[i],sizeof(enemy4[i]));
     close(fe4);

     assign(ftbl,tblfilename);
     reset(ftbl,1);
      blockread(ftbl,bulletf,sizeof(bulletf));
     close(ftbl);

     assign(febl,eblfilename);
     reset(febl,1);
      blockread(febl,bullete,sizeof(bullete));
     close(febl);

    for i:= 1 to 5 do begin
     assign(fwm[i],wmfilename[i]);
     reset(fwm[i],1);
      blockread(fwm[i],woman[i],sizeof(woman[i]));
     close(fwm[i]);
    end;{of for}

    for i:=1 to 2 do begin
     flame[i,1]:=39;
     flame[i,2]:=0;
     flame[i,3]:=39;
     flame[i,4]:=0;
    end;

     assign(ffm,fmfilename);
     reset(ffm,200);
     for i:= 1 to 2 do
      blockread(ffm,flame[i,5],sizeof(flame[i,5]));
     close(ffm);
  end;

 procedure firesound;
  begin
   for i:=5 downto 2 do begin
    sound(i*500);
    delay(2);
    nosound;
   end
  end;

 procedure hitingsound;
   begin
   sound(100);
   delay(3);
   nosound;
   sound(500);
   delay(2);
   nosound;
   end;

 procedure shitingsound;
   begin
   sound(300);
   delay(20);
   nosound;
   end;

 procedure openscen;
  var
   graphdriver,graphmode,errorcode : integer;
   palette : palettetype;
   rnx,rny,tm : integer;
  begin
    readfont;
    graphdriver:=detect;
    initgraph(graphdriver,graphmode,'');
    errorcode:=graphresult;
    If errorcode <> grok then begin
      writeln('Error :',grapherrormsg(errorcode));
      halt(1);
    end;
  getpalette(palette);
  maxx:=getmaxx;
  maxy:=getmaxy;
  setcolor(white);
  rectangle(0,0,maxx,maxy-30);
  rectangle(10,10,maxx-10,maxy-40);
  rectangle(100,100,maxx-100,maxy-130);
  rectangle(103,103,maxx-103,maxy-133);
  floodfill(15,15,white);
  floodfill(105,105,white);
  rectangle(0,maxy-28,maxx,maxy);
  rectangle(2,maxy-26,maxx-2,maxy-2);
  floodfill(4,maxy-5,white);
  settextjustify(centertext,toptext);
  settextstyle(defaultfont,horizdir,3);
  setcolor(black);
  outtextxy(maxx div 2,(maxy div 2)-30,'THUNDER');
  settextstyle(defaultfont,horizdir,1);
  outtextxy(maxx div 2,maxy div 2,'May. 1992   by KIM S.W.');
  outtextxy(maxx div 2,maxy - 18,'Press any key to start');
  putimage((maxx div 2)-16,maxy-100,thunder,xorput);
  randomize;
   for i:=1 to 5 do begin
     rnx:=random(maxx);
     rny:=random(maxy div 3);
     putimage(rnx,rny,enemy1[2],orput);
     putimage(rnx,rny,enemy1[1],xorput);
   end;
     rnx:=random(maxx);
     rny:=random(maxy div 3);
     putimage(rnx,rny,enemy2[2],orput);
     putimage(rnx,rny,enemy2[1],xorput);
     rnx:=random(maxx);
     rny:=random(maxy div 3);
     putimage(rnx,rny,enemy3[2],orput);
     putimage(rnx,rny,enemy3[1],xorput);
     rnx:=random(maxx);
     rny:=random(maxy div 3);
     putimage(rnx,rny,enemy4[3],orput);
     putimage(rnx+32,rny,enemy4[4],orput);
     putimage(rnx,rny,enemy4[1],xorput);
     putimage(rnx+32,rny,enemy4[2],xorput);
  repeat
  until keypressed;
  end;

procedure grpagedetect;
 begin
  case vs of
          0: begin vs:=0; ac:=1; end;
          1: begin vs:=1; ac:=0; end;
  end;
  setvisualpage(vs);
  setactivepage(ac);
 end;

procedure grpagechange;
 begin
  case vs of
          1: begin vs:=0; ac:=1; end;
          0: begin vs:=1; ac:=0; end;
  end;
  setvisualpage(vs);
  setactivepage(ac);
 end;

procedure thunderdraw(x:integer; y:integer);
 begin
   if (thunderpresent>0) and (thunderpresent<100) then begin
     if ac=thndac then begin
      getimage(x-16,y-16,x+15,y+15,thndbk[ac]);
      thndbk[vs]:=thndbk[ac];
      putimage(x-16,y-16,thunder[2],orput);
      putimage(x-16,y-16,thunder[1],xorput);
     end;
   end;
   if (thunderpresent>101) then begin
      getimage(x-16,y-16,x+15,y+15,thndbk[ac]);
      putimage(x-16,y-16,thunder[2],orput);
      putimage(x-16,y-16,thunder[1],xorput);
   end;
 end;

procedure thundererase(x:integer; y:integer);
 begin
   if (thunderpresent>1) and (thunderpresent<101) then begin
     if ac=thndac then  putimage(x-16,y-16,thndbk[ac],copyput);
   end;
   if thunderpresent>102 then begin
     putimage(x-16,y-16,thndbk[ac],copyput);
     thunderpresent:=103;
   end;
   if thunderpresent<0 then begin
     thunderpresent:=0;
     putimage(x-16,y-16,thndbk[ac],copyput);
   end;
   if thunderpresent>0 then thunderpresent:=thunderpresent+1;
 end;

procedure enemy1draw(x:integer; y:integer);
 begin
   if enemy1present[i]>0 then begin
   getimage(x-16,y-16,x+15,y+15,en1bk[i,ac]);
   putimage(x-16,y-16,enemy1[2],orput);
   putimage(x-16,y-16,enemy1[1],xorput);
   end;
 end;


procedure enemy1erase(x:integer; y:integer);
 begin
  if (enemy1present[i]>1) or (enemy1present[i]<0) then begin
  putimage(x-16,y-16,en1bk[i,ac],copyput);
  if enemy1present[i]<0 then enemy1present[i]:=0;
  end;
  if enemy1present[i]=1 then enemy1present[i]:=2;
 end;

procedure enemy2draw(x:integer; y:integer);
 begin
   if enemy2present>0 then begin
   getimage(x-16,y-16,x+15,y+15,en2bk[ac]);
   putimage(x-16,y-16,enemy2[2],orput);
   putimage(x-16,y-16,enemy2[1],xorput);
   end;
 end;

procedure enemy2erase(x:integer; y:integer);
 begin
   if (enemy2present>1) or (enemy2present<0) then begin
   putimage(x-16,y-16,en2bk[ac],copyput);
   if enemy2present<0 then enemy2present:=0;
   end;
   if enemy2present=1 then enemy2present:=2;
 end;

procedure enemy3draw(x:integer; y:integer);
 begin
   if enemy3present>0 then begin
   getimage(x-16,y-16,x+15,y+15,en3bk[ac]);
   putimage(x-16,y-16,enemy3[2],orput);
   putimage(x-16,y-16,enemy3[1],xorput);
   end;
 end;

procedure enemy3erase(x:integer; y:integer);
 begin
   if (enemy3present>1) or (enemy3present<0) then begin
   putimage(x-16,y-16,en3bk[ac],copyput);
   if enemy3present<0 then enemy3present:=0;
   end;
   if enemy3present=1 then enemy3present:=2;
 end;

procedure enemy4draw(x:integer; y:integer);
 begin
   if enemy4present>0 then begin
   getimage(x-32,y-23,x+31,y+23,en4bk[ac]);
   putimage(x-32,y-23,enemy4[3],orput);
   putimage(x,y-23,enemy4[4],orput);
   putimage(x-32,y-23,enemy4[1],xorput);
   putimage(x,y-23,enemy4[2],xorput);
   end;
 end;

procedure enemy4erase(x:integer; y:integer);
 begin
   if (enemy4present>1) or (enemy4present<0) then begin
   putimage(x-32,y-23,en4bk[ac],copyput);
   if enemy4present<0 then enemy4present:=0;
   end;
   if enemy4present=1 then enemy4present:=2;
 end;

procedure start;
 var
   pnt3,pnt2:integer;
 begin
  cleardevice;

  assign(score,scorefilename);
  reset(score);                 {read highscore}
  read(score,highscore);
  close(score);

  vs:=0; ac:=1;
  for i:=0 to 1 do begin
  thunderx[i]:=maxx div 2;
  thundery[i]:=maxy - 50;
  end;
  thunderxd:=0;
  thunderpresent:=0;
  for j:=0 to 1 do begin
  grpagedetect;
  setcolor(white);
  rectangle(width,0,maxx-width,maxy);
  settextjustify(righttext,centertext);
  outtextxy(width-10,90,' QUIT : <Esc> ');
  outtextxy(width-10,130,' MOVE : Arrow Key');
  outtextxy(width-10,190,' FIRE : <Spc>');
  outtextxy(width-10,210,' BOMB : < B >');
  outtextxy(width-90,230,'5');
  rectangle(maxx-width+10,10,maxx-10,50);
  settextjustify(centertext,centertext);
  outtextxy(maxx-(width div 2),20,'HIT  SCORE');
  outtextxy(maxx-30,40,'0');
  outtextxy(maxx-50,40,'0');
  outtextxy(maxx-70,40,'0');
  outtextxy(maxx-90,40,'0');
  outtextxy(maxx-110,40,'0');
  outtextxy(maxx-130,40,'0');
  outtextxy(maxx-(width div 2),60,'HIGH  SCORE');
  npnt0000:=round(INT(Highscore*0.001));
  pnt3:=highscore-npnt0000*1000;
  npnt000:=round(int(pnt3*0.01));
  pnt2:=pnt3-npnt000*100;
  npnt00:=round(int(pnt2*0.1));
  npnt0:=pnt2-npnt00*10;
  outtextxy(maxx-50,80,chr(48+npnt0));
  outtextxy(maxx-70,80,chr(48+npnt00));
  outtextxy(maxx-90,80,chr(48+npnt000));
  outtextxy(maxx-110,80,chr(48+npnt0000));
  outtextxy(maxx-30,80,'0');
  for i:=0 to 3 do
  putimage(maxx-width+20,(maxy-40)-(i*40),thunder,xorput);
{------------draw backgraund-------------------------------}
  circle((maxx div 2),(maxy div 2),60);
  circle((maxx div 2),(maxy div 2),50);
  circle((maxx div 2)+70,(maxy div 2)+20,10);
  circle((maxx div 2),(maxy div 2)+70,20);
  circle((maxx div 2),maxy-30,20);
  setfillstyle(interleavefill,white);
  bar(width+10,10,width+110,80);
  setfillstyle(solidfill,white);
  bar(width+130,10,maxx-width-10,80);
  bar((maxx div 2)-20,(maxy div 2)+70,(maxx div 2)+20,maxy-30);
  rectangle(maxx-width-10,90,maxx-width,100);
  settextjustify(centertext,toptext);
  settextstyle(defaultfont,horizdir,2);
  setcolor(black);
  outtextxy(width+200,20,'THUNDER');
  setcolor(white);
  settextjustify(centertext,centertext);
  settextstyle(defaultfont,horizdir,1);
  grpagechange;
  end;
{--------------- initialize --------------------------------}
  svthnd:=5;
  en1nb:=0;
  pwno:=0;
  hit2no:=0;
  hit3no:=0;
  hit4no:=0;
  j:=0;
  k:=0;
  m:=0;
  ii:=0;
  oldpnt:=0;
  opnt0:=0;
  opnt00:=0;
  opnt000:=0;
  opnt0000:=0;
  obmpnt:=5;
  setcolor(white);
  for i:=1 to blno do begin
   blpresent[i]:=0;
   blx[i,ac]:=thunderx[ac];
   bly[i,ac]:=thundery[ac]-10;
  end;
  for i:=1 to blno do begin
   enblpresent[i]:=0;
  end;
  thunderpresent:=1;
  for i:=1 to en1no do
   enemy1present[i]:=0;
  enemy2present:=0;
  enemy2fireyes:=false;
  enemy3present:=0;
  enemy3fireyes:=false;
  enemy4present:=0;
  enemy4fireyes:=false;
  flamepresent:=0;
  bombpresent:=false;
  poweruppresent:=0;
  powerupyes:=false;
  bonus:=false;
  calsvthndyes:=false;
  bombyes:=false;
  for i:=0 to 128 do
   scancode[i]:=false;
  key:=0;
  end;

procedure erase;
 begin
   for i:=enblno downto 1 do begin
    if (enblpresent[i]>1) or (enblpresent[i]<0) then
    putimage(round(enblx[i,ac]-4),round(enbly[i,ac]-4),bullete,xorput);
    if enblpresent[i]<0 then enblpresent[i]:=0;
    if enblpresent[i]=1 then enblpresent[i]:=2;
   end;

   for i:=blno downto 1 do begin
    if (blpresent[i]>1) or (blpresent[i]<0) then
    putimage(blx[i,ac]-4,bly[i,ac]-4,bulletf,xorput);
    if blpresent[i]<0 then blpresent[i]:=0;
    if blpresent[i]=1 then blpresent[i]:=2;
   end;

   if (poweruppresent>1) or (poweruppresent<0) then
    putimage(pwx[ac]-4,pwy[ac]-3,powerup,xorput);
    if poweruppresent<0 then poweruppresent:=0;
    if poweruppresent=1 then poweruppresent:=2;

   if (flamepresent>1) or (flamepresent<0) then
    putimage(fx,fy,flame[1],xorput);
    if flamepresent<0 then flamepresent:=0;
    if flamepresent=1 then flamepresent:=-1;

   thundererase(thunderx[ac],thundery[ac]);
   for i:= en1no downto 1 do begin
    enemy1erase(en1x[i,ac],en1y[i,ac]);
   end;
   enemy3erase(en3x[ac],en3y[ac]);
   enemy2erase(en2x[ac],en2y[ac]);
   enemy4erase(en4x[ac],en4y[ac]);

 end;

procedure draw;
 begin

   enemy4draw(en4x[ac],en4y[ac]);
   enemy2draw(en2x[ac],en2y[ac]);
   enemy3draw(en3x[ac],en3y[ac]);
   for i:= 1 to en1no do begin
    enemy1draw(en1x[i,ac],en1y[i,ac]);
   end;
   thunderdraw(thunderx[ac],thundery[ac]);

   if poweruppresent>0 then
    putimage(pwx[ac]-4,pwy[ac]-3,powerup,xorput);
   if flamepresent>0 then
    putimage(fx,fy,flame[1],xorput);
   for i:=1 to blno do begin
    if blpresent[i]>0 then
    putimage(blx[i,ac]-4,bly[i,ac]-4,bulletf,xorput);
   end;
   for i:=1 to enblno do begin
    if enblpresent[i]>0 then
    putimage(round(enblx[i,ac]-4),round(enbly[i,ac]-4),bullete,xorput);
   end;
 end;


 procedure bombpoint(bn :integer);
  begin
   nbmpnt:=obmpnt+bn;
   if nbmpnt<0 then nbmpnt:=0;
   for i:=1 to 2 do begin
    if i = 1 then setactivepage(vs);
    settextjustify(righttext,centertext);
    setcolor(black);
    outtextxy(width-90,230,inttostr(obmpnt));
    setcolor(magenta);
    outtextxy(width-90,230,inttostr(nbmpnt));
    settextjustify(centertext,centertext);
    setactivepage(ac);
   end;
   obmpnt:=nbmpnt;
  end;

 procedure thunderappear;
  begin
     for i:=0 to 1 do begin
      thunderx[i]:=maxx div 2;
      thundery[i]:=maxy - 50;
     end;
     thunderpresent:=1;
     case (ac mod 2) of
              0 : thndac:=0;
              1 : thndac:=1;
     end;
  end;
 procedure flameappear;
  begin
   flamepresent:=1;
   fx:=thunderx[ac]-20;
   fy:=thundery[ac]-20;
  end;

 procedure bonusup;
  var kk:integer;
  begin
   if (bonus=false) and (svthnd<9) and (newpnt>=500)
      and ((newpnt mod 500) < 250) then begin
    bonus:=true;
    svthnd:=svthnd+1;
    for kk:=1 to 2 do begin
     if kk=1 then setactivepage(vs);
     putimage(maxx-width+20,(maxy-40)-(svthnd-2)*40,thunder[1],xorput);
     setactivepage(ac);
     end;
    end;
   if ((newpnt mod 500) >= 250) then bonus:=false;
  end;

 procedure calsvthnd;   { calculate survived thunder }
  begin
   sound(200);
   delay(500);
   nosound;
   if svthnd>=0 then begin
    svthnd:=svthnd-1;
    powerupyes:=false;
    pwno:=0;
    if svthnd<>0 then begin
    for i:=1 to 2 do begin
     if i=1 then  setactivepage(vs);
     putimage(maxx-width+20,(maxy-40)-(svthnd-1)*40,thunder[1],xorput);
     delay(300);
    nbmpnt:=5;
    settextjustify(righttext,centertext);
    setcolor(black);
    outtextxy(width-90,230,inttostr(obmpnt));
    setcolor(white);
    outtextxy(width-90,230,inttostr(nbmpnt));
    settextjustify(centertext,centertext);
    setactivepage(ac);
    end;
    thunderpresent:=-1;
    obmpnt:=nbmpnt;
    end;
   end;
  end;

 procedure enemy1appear;
  var kk:integer;
   begin
   for k:=1 to en1no do begin
   if (enemy1present[k]<1) and (enemy1present[k]>-1) then begin
     en1xd[k]:=random(4)-2;
     en1x[k,0]:=random(maxx-width*2)+width;
     en1yr[k,0]:=16;
     en1y[k,0]:=round(en1yr[k,0]);
     en1x[k,1]:=en1x[k,0];
     en1yr[k,1]:=en1yr[k,0];
     en1y[k,1]:=en1y[k,0];
     en1nb:=en1nb+1;
     enemy1present[k]:=enemy1present[k]+1
     end
   end;
  end;

procedure enemy2appear;
  begin
   if (random(100)=0) and (enemy2present<1) and(enemy2present>-1)
     and (en1nb>500) then begin
     for i:=0 to 1 do begin
     en2x[i]:=(maxx div 2) + random(200)-100;
     en2y[i]:=16;
     end;
     en2xd:=random(3)-1;
     en2yd:=1;
     enemy2present:=enemy2present+1;
     end
   end;

 procedure enemy3appear;
   begin
   if (random(80)=0) and (enemy3present<1) and (enemy3present>-1)
      and (en1nb>250) then begin
     for i:=0 to 1 do begin
     en3x[i]:=(maxx div 2) + random(200)-100;
     en3y[i]:=16;
     end;
     en3xd:=random(3)-1;
     en3yd:=5;
     enemy3present:=enemy3present+1;
     end
   end;

 procedure enemy4appear;
  var ranx:integer;
   begin
   if (random(300)=0) and (enemy4present<1) and (enemy4present>-1)
      and (en1nb>850) then begin
     enemy4present:=1;
     ranx:=random(100);
     for i:=0 to 1 do begin
     en4x[i]:=(maxx div 2) + ranx - 50;
     en4yr[i]:=30;
     en4y[i]:=30;
     end;
     en4yd:=0.8;
     end
   end;

 procedure powerupappear;
   begin
   if (random(100)=0) and (poweruppresent<1) and (poweruppresent>-1) then begin
     poweruppresent:=poweruppresent+1;
     for i:=0 to 1 do begin
     pwx[i]:=en1x[n,i];
     pwy[i]:=en1y[n,i];
     end;
     pwyd:=1;
     end
   end;

 procedure bulletmove;
   begin
     blxr[i,ac]:=blxr[i,vs]+blxd[i];
     blyr[i,ac]:=blyr[i,vs]+blyd[i];
     blx[i,ac]:=round(blxr[i,ac]);
     bly[i,ac]:=round(blyr[i,ac]);
    if (blx[i,ac]>width) and (blx[i,ac]<(maxx-width)) and (bly[i,ac]>0) and
     (bly[i,ac]<maxy) then
    else blpresent[i]:=-1;
   end;

 procedure enemybulletmove;
   begin
    enblx[l,ac]:=enblx[l,vs]+enblxd[l];
    enbly[l,ac]:=enbly[l,vs]+enblyd[l];
    if (enblx[l,ac]>width) and (enblx[l,ac]<(maxx-width)) and
       (enbly[l,ac]>0) and (enbly[l,ac]<maxy) then begin
       if (abs(enblx[l,ac]-thunderx[ac])<15) and (abs(enbly[l,ac]-thundery[ac])<15)
          and (thunderpresent>103) then
          begin
           enblpresent[l]:=-1;
           thunderpresent:=-1;
          end;
       end
      else enblpresent[l]:=-1;
    end;

 procedure thundermove;
  begin
   thunderx[ac]:=thunderx[vs]+txd;
   if (thunderx[ac]<(maxx-width)) and (thunderx[ac]>width) then
   else begin
    thunderx[ac]:=thunderx[ac]-txd;
   end;
   thundery[ac]:=thundery[vs]+tyd;
   if (thundery[ac]>16) and (thundery[ac]<(maxy-16)) then
   else begin
    thundery[ac]:=thundery[ac]-tyd;
   end;
   txd:=0; tyd:=0;
  end;

 procedure fire;
 var
  jjj,pwnb : integer;
  th,dth : real;
 begin
  if fireyes then begin
   case powerupyes of
          false :  begin     j:=j+1;
    if (j>=1) and (j<=blno) and (blpresent[j]<1) and (blpresent[j]>-1)
       then begin
     for i:=0 to 1 do begin
     blxr[j,i]:=thunderx[vs];
     blyr[j,i]:=thundery[vs]-20;
     blx[j,i]:=round(blxr[j,i]);
     bly[j,i]:=round(blyr[j,i]);
     end;
     blxd[j]:=0;
     blyd[j]:=-bld;
     firesound;
     blpresent[j]:=blpresent[j]+1;
    end
    else j:=0;
    end;
          true :  begin   firesound;
     if pwno=1 then begin th:=1.3; dth:=0.2; pwnb:=2; end;
     if pwno=2 then begin th:=1; dth:=0.3; pwnb:=3; end;
     if pwno=3 then begin th:=1.3; dth:=0.1; pwnb:=4; end;
     if pwno>=4 then begin th:=1.3; dth:=0.08; pwnb:=5; end;
    for jjj:=1 to pwnb do begin
     j:=j+1;
     if (j>=1) and (j<=blno) and (blpresent[j]<1) and (blpresent[j]>-1)
        then begin
      th:=th+dth;
      for i:=0 to 1 do begin
      blxr[j,i]:=thunderx[vs];
      blyr[j,i]:=thundery[vs]-20;
      blx[j,i]:=round(blxr[j,i]);
      bly[j,i]:=round(blyr[j,i]);
      end;
      blpresent[j]:=blpresent[j]+1;
      blxd[j]:=bld*cos(th);
      blyd[j]:=-bld*sin(th);
      end
     else j:=0;
     end;
    end;end;end;
    fireyes:=false;
  end;

 procedure enemy1fire;
  var
    x,y,z : real;
  begin
    if (m>=1) and (m<=enblno) and (enblpresent[m]<1) and (enblpresent[m]>-1)
     and (en1nb>30) then begin
      for i:=0 to 1 do begin
      enblx[m,i]:=en1x[n,ac];
      enbly[m,i]:=en1y[n,ac];
      end;
      enblpresent[m]:=enblpresent[m]+1;
      x:=thunderx[ac]-en1x[n,ac];
      y:=thundery[ac]-en1y[n,ac];
      z:=sqrt(x*x+y*y);
      enblxd[m]:=x*enbld/z;
      enblyd[m]:=y*enbld/z;
     end
    else m:=0;
  end;

 procedure enemy2fire;
  var
    th,dth : real;
    burnno,jj : integer;
  begin
    jj:=0;
    if en1nb <700  then begin
      burnno:=3;
      th:=0.75; end
     else begin
      burnno:=5;
      th:=0.3;
     end;
    dth:=0.45;
    for jj:=1 to burnno do begin
     m:=m+1;
     if (m>=1) and (m<=enblno) and (enblpresent[m]<1)
        and (enblpresent[m]>-1) then begin
      th:=th+dth;
      for i:=0 to 1 do begin
      enblx[m,i]:=en2x[ac];
      enbly[m,i]:=en2y[ac];
      end;
      enblpresent[m]:=enblpresent[m]+1;
      enblxd[m]:=enbld*cos(th)*2;
      enblyd[m]:=enbld*sin(th)*2;
     end
     else  m:=0;
    enemy2fireyes:=false;
  end;
  end;

 procedure enemy3fire;
  var
    x,y,z : real;
        jj: integer;
  begin
    jj:=0;
    x:=thunderx[ac]-en3x[ac];
    y:=thundery[ac]-en3y[ac];
    z:=sqrt(x*x+y*y);
    for jj:=1 to 2 do begin
     m:=m+1;
     if (m>=1) and (m<=enblno) and (enblpresent[m]<1)
        and (enblpresent[m]>-1) then begin
      for i:=0 to 1 do begin
      enblx[m,i]:=en3x[ac];
      enbly[m,i]:=en3y[ac];
      end;
      enblpresent[m]:=enblpresent[m]+1;
      case jj of
           1 :  begin enblxd[m]:=x*enbld*2.5/z+1;
                      enblyd[m]:=y*enbld*2.5/z; end;
           2 :  begin enblxd[m]:=x*enbld*2.5/z-1;
                      enblyd[m]:=y*enbld*2.5/z; end;
      end;   {of case}
     end
     else  m:=0;
    end;
    enemy3fireyes:=false;
  end;

 procedure enemy4fire;
  var
    x,y,z : real;
        jj: integer;
    th,dth : real;
  begin
    th:=0.4;
    dth:=0.3;
    for jj:=1 to 10 do begin
     m:=m+1;
     if (m>=1) and (m<=enblno) and (enblpresent[m]<1)
        and (enblpresent[m]>-1) then begin
      th:=th+dth;
      for i:=0 to 1 do begin
      enblx[m,i]:=en4x[ac];
      enbly[m,i]:=en4y[ac];
      end;
      enblpresent[m]:=enblpresent[m]+1;
      enblxd[m]:=enbld*cos(th)*2.3;
      enblyd[m]:=enbld*sin(th)*2.3;
      end
     else m:=0;
    end;
    x:=thunderx[ac]-en4x[ac];
    y:=thundery[ac]-en4y[ac];
    z:=sqrt(x*x+y*y);
    for jj:=1 to 2 do begin
     m:=m+1;
     if (m>=1) and (m<=enblno) and (enblpresent[m]<1)
        and (enblpresent[m]>-1) then begin
      for i:=0 to 1 do begin
      enblx[m,i]:=en4x[ac];
      enbly[m,i]:=en4y[ac];
      end;
      enblpresent[m]:=enblpresent[m]+1;
      case jj of
           1 :  begin enblxd[m]:=x*enbld*4/z+1;
                      enblyd[m]:=y*enbld*4/z; end;
           2 :  begin enblxd[m]:=x*enbld*4/z-1;
                      enblyd[m]:=y*enbld*4/z; end;
      end;   {of case}
     end
     else m:=0;
    end;
    enemy4fireyes:=false;
  end;

 procedure pointup;
   var
    pnt3,pnt2,kk:integer;
   begin
    newpnt:=oldpnt+pntup;
    if newpnt<10000 then begin
      npnt0000:=round(INT(newpnt*0.001));
      pnt3:=newpnt-npnt0000*1000;
      npnt000:=round(int(pnt3*0.01));
      pnt2:=pnt3-npnt000*100;
      npnt00:=round(int(pnt2*0.1));
      npnt0:=pnt2-npnt00*10;
     end
     else  begin
      newpnt:=0;
      npnt0:=0;
      npnt00:=0;
      npnt000:=0;
      npnt0000:=0;
     end;
     for i:=1 to 2 do begin
     if i=1 then setactivepage(vs);
     setcolor(black); outtextxy(maxx-50,40,chr(48+opnt0));
     setcolor(white); outtextxy(maxx-50,40,chr(48+npnt0));
     setcolor(black); outtextxy(maxx-70,40,chr(48+opnt00));
     setcolor(white); outtextxy(maxx-70,40,chr(48+npnt00));
     setcolor(black); outtextxy(maxx-90,40,chr(48+opnt000));
     setcolor(white); outtextxy(maxx-90,40,chr(48+npnt000));
     setcolor(black); outtextxy(maxx-110,40,chr(48+opnt0000));
     setcolor(white); outtextxy(maxx-110,40,chr(48+npnt0000));
     setactivepage(ac);
     end;
     oldpnt:=newpnt;
     opnt0:=npnt0;
     opnt00:=npnt00;
     opnt000:=npnt000;
     opnt0000:=npnt0000;
    end;

 procedure enemy1hiting;
   begin
   setcolor(white);
   if (bombpresent=false) and (blpresent[i]>1) then begin
    blpresent[i]:=-1;
   end;
   hitingsound;
   setactivepage(vs);
   putimage(en1x[n,ac]-20,en1y[n,ac]-20,flame[1],xorput);
   delay(15);
   putimage(en1x[n,ac]-20,en1y[n,ac]-20,flame[1],xorput);
   putimage(en1x[n,ac]-20,en1y[n,ac]-20,flame[2],xorput);
   delay(7);
   putimage(en1x[n,ac]-20,en1y[n,ac]-20,flame[2],xorput);
   en1x[n,ac]:=0;
   en1y[n,ac]:=0;
   enemy1present[n]:=-1;
   setactivepage(ac);
   pntup:=1;
   pointup;
   end;

 procedure enemy2hiting;
  begin
   setcolor(white);
   hit2no:=hit2no+1;
   shitingsound;
   if (bombpresent=false) and (blpresent[i]>1) then begin
    blpresent[i]:=-1;
   end;
   if hit2no >=fullhit
    then begin
    enemy2present:=-1;
    setactivepage(vs);
    putimage(en2x[ac]-16,en2y[ac]-16,flame[1],xorput);
    delay(10);
    putimage(en2x[ac]-16,en2y[ac]-16,flame[1],xorput);
    putimage(en2x[ac],en2y[ac],flame[2],xorput);
    delay(10);
    shitingsound;
    putimage(en2x[ac],en2y[ac],flame[2],xorput);
    putimage(en2x[ac]-6,en2y[ac]-16,flame[1],xorput);
    delay(10);
    shitingsound;
    putimage(en2x[ac]-6,en2y[ac]-16,flame[1],xorput);
    putimage(en2x[ac]-20,en2y[ac]-20,flame[2],xorput);
    delay(20);
    putimage(en2x[ac]-20,en2y[ac]-20,flame[2],xorput);
    setactivepage(ac);
    pntup:=20;
    pointup;
    hit2no:=0;
   end;
  end;

 procedure enemy3hiting;
  begin
   setcolor(white);
   hit3no:=hit3no+1;
   shitingsound;
   if (bombpresent=false) and (blpresent[i]>1) then begin
    blpresent[i]:=-1;
   end;
   if hit3no >=5 then begin
    enemy3present:=-1;
    setactivepage(vs);
    putimage(en3x[ac]-20,en3y[ac]-20,flame[1],xorput);
    delay(10);
    putimage(en3x[ac]-20,en3y[ac]-20,flame[1],xorput);
    putimage(en3x[ac],en3y[ac],flame[2],xorput);
    delay(10);
    shitingsound;
    putimage(en3x[ac],en3y[ac],flame[2],xorput);
    putimage(en3x[ac]-6,en3y[ac]-16,flame[1],xorput);
    delay(10);
    shitingsound;
    putimage(en3x[ac]-6,en3y[ac]-16,flame[1],xorput);
    setactivepage(ac);
    if random(2)=0 then bombpoint(1);
    pntup:=10;
    pointup;
    hit3no:=0;
   end;
  end;

 procedure enemy4hiting;
  begin
   setcolor(white);
   hit4no:=hit4no+1;
   shitingsound;
   if (bombpresent=false) and (blpresent[i]>1) then begin
    blpresent[i]:=-1;
   end;
   if hit4no >=75 then begin
    enemy4present:=-1;
    setactivepage(vs);
    putimage(en4x[ac]-16,en4y[ac]-16,flame[1],xorput);
    delay(10);
    putimage(en4x[ac]-16,en4y[ac]-16,flame[1],xorput);
    putimage(en4x[ac],en4y[ac],flame[2],xorput);
    delay(10);
    shitingsound;
    putimage(en4x[ac],en4y[ac],flame[2],xorput);
    putimage(en4x[ac]-6,en4y[ac]-16,flame[1],xorput);
    delay(10);
    shitingsound;
    putimage(en4x[ac]-6,en4y[ac]-16,flame[1],xorput);
    setactivepage(ac);
    pntup:=50;
    pointup;
    hit4no:=0;
   end;
  end;

 procedure bomb;
  var
   x,y:integer;
  begin
   if bombpresent=false then begin
    bombpresent:=true;
    x:=thunderx[ac];
    y:=thundery[ac]-100;
    setactivepage(vs);
    putimage(x-16,y-16,flame[1],xorput);
    delay(5);
    shitingsound;
    putimage(x-16,y-16,flame[1],xorput);
    putimage(x+30,y+30,flame[2],xorput);
    delay(5);
    shitingsound;
    putimage(x+30,y+30,flame[2],xorput);
    putimage(x+26,y-16,flame[1],xorput);
    delay(5);
    shitingsound;
    putimage(x+26,y-16,flame[1],xorput);
    putimage(x-20,y+20,flame[2],xorput);
    delay(5);
    putimage(x-20,y+20,flame[2],xorput);
    setactivepage(ac);

    for i:=1 to enblno do begin
     if enblpresent[i]=2 then  enblpresent[i]:=-1;
    end;

    for bb:=1 to bmno do  begin
     for n:=1 to en1no do
      if enemy1present[n]>1 then enemy1hiting;

     if enemy2present>1 then enemy2hiting;

     if enemy3present>1 then enemy3hiting;

     if enemy4present>1 then enemy4hiting;
    end;
   end;
   bombpresent:=false;
   bombyes:=false;
  end;

 procedure enemy1move;
  var
   rny:real;
  begin
   if enemy1present[n]>1 then begin
     rny:=en1yd+random*2+en1nb*0.01/3;
     en1yr[n,ac]:=en1yr[n,vs]+en1yd;
     en1y[n,ac]:=round(en1yr[n,ac]);
     en1x[n,ac]:=en1x[n,vs]+en1xd[n];
     if (en1x[n,ac]>width) and (en1x[n,ac]<(maxx-width)) and (en1y[n,ac]<maxy) then begin
        if (random(130)=0) then begin
           m:=m+1;
           enemy1fire;
         end;
        if (abs(en1x[n,ac]-thunderx[ac])<15) and (abs(en1y[n,ac]-thundery[ac])<15)
           and (thunderpresent>103) then
          begin
           enemy1present[n]:=-1;
           thunderpresent:=-1;
           flamepresent:=1;
          end
       end
          else  enemy1present[n]:=-1;
   end;
   end;

 procedure enemy2move;
   begin
    if enemy2present>1 then begin
      en2x[ac]:=en2x[vs]+en2xd;
      en2y[ac]:=en2y[vs]+en2yd;
      if (en2x[ac]>width) and (en2x[ac]<(maxx-width)) and
          (en2y[ac]>0) and (en2y[ac]<maxy) then  begin
         if (random(40)=0) and (en2y[ac]<250) and (enemy2fireyes=false) then
          begin
           enemy2fireyes:=true;
           enemy2fire;
          end;
         if (abs(en2x[ac]-thunderx[ac])<30) and (abs(en2y[ac]-thundery[ac])<30)
            and (thunderpresent>103) then
          begin
           enemy2present:=-1;
           hit2no:=0;
           thunderpresent:=-1;
           flamepresent:=1;
          end;
       end
      else begin
        enemy2present:=-1;
        hit2no:=0;
       end;
    end;
   end;

 procedure enemy3move;
   begin
    if enemy3present>1 then begin
      en3x[ac]:=en3x[vs]+en3xd;
      en3y[ac]:=en3y[vs]+en3yd;
      if (en3x[ac]>width) and (en3x[ac]<(maxx-width)) and
          (en3y[ac]>0) and (en3y[ac]<maxy) then  begin
         if (random(20)=0) and (enemy3fireyes=false) then
          begin
           enemy3fireyes:=true;
           enemy3fire;
          end;
         if (abs(en3x[ac]-thunderx[ac])<30) and (abs(en3y[ac]-thundery[ac])<30)
            and (thunderpresent>103) then
          begin
           enemy3present:=-1;
           hit3no:=0;
           thunderpresent:=-1;
           flamepresent:=1;
          end;
       end
      else begin
        enemy3present:=-1;
        hit3no:=0;
       end;
    end;
   end;

 procedure enemy4move;
   begin
    if enemy4present>1 then begin
      en4yr[ac]:=en4yr[vs]+en4yd;
      en4y[ac]:=round(en4yr[ac]);
      if (en4x[ac]>width) and (en4x[ac]<(maxx-width)) and
          (en4y[ac]>0) and (en4y[ac]<maxy) then  begin
         if (random(35)=0) and (en4y[ac]<200) and (enemy4fireyes=false) then
          begin
           enemy4fireyes:=true;
           enemy4fire;
          end;
       end
      else begin
        enemy4present:=-1;
        hit4no:=0;
       end;
    end;
   end;

 procedure powerupmove;
   begin
    if poweruppresent>1 then begin
      pwy[ac]:=pwy[vs]+pwyd;
      if (pwx[ac]>width) and (pwx[ac]<(maxx-width)) and
          (pwy[ac]>0) and (pwy[ac]<maxy) then  begin
         if (abs(pwx[ac]-thunderx[ac])<20) and (abs(pwy[ac]-thundery[ac])<20)
            and (thunderpresent>1) then
          begin
           poweruppresent:=-1;
           powerupyes:=true;
           pwno:=pwno+1;
           sound(1000);
           delay(20);
           nosound;
          end;
       end
       else poweruppresent:=-1;
    end;
   end;

procedure GAMEOVER;
   begin
    setcolor(white);
    settextjustify(centertext,centertext);
    settextstyle(defaultfont,horizdir,2);
    outtextxy(maxx div 2,150,'GAME    OVER');
    settextstyle(defaultfont,horizdir,1);
    outtextxy(maxx div 2,200,'PLAY GAME AGAIN ? (Y/N)');
    svthnd:=5;
    if newpnt>highscore then begin
      highscore:=newpnt;
      assign(score,scorefilename);
      {$I-}
      rewrite(score);
      {$I+}
      write(score,highscore);
      close(score);
     end;
    repeat
    CH:=READKEY;
    until ((ch='y') or (ch='Y') or (ch='n') or (ch='N'));
   end;

 procedure endscene;
   begin
     j:=0;
    repeat
     j:=j+6;
     setcolor(white);
     outtextxy(100-j,120,'Good-Bye !');
     for i:= 1 to 5 do begin
        putimage(50-i-j,150,woman[i],xorput);
        delay(200);
        putimage(50-i-j,150,woman[i],xorput);
     end;
     setcolor(black);
     outtextxy(100-j,120,'Good-Bye !');
    until j> 45;
    closegraph;
   end;

procedure CLI;inline($FA);

procedure STI;inline($FB);

procedure keyinput;
 interrupt;
 var
   reg:registers;
 begin
   key:=port[$60];
   if key >=127 then scancode[key-128]:=false
     else scancode[key]:=true;
   CLI;
   memw[$0040:$001A]:=memw[$0040:$001C]; {key buffer clear}
   intr($60,reg);                        {call original intr09}
   STI;
 end;

 BEGIN {------------------ main ------------------------------------}
   openscen;
10:start;
   getintvec(9,intsave9);
   setintvec(9,@keyinput);
   setintvec($60,intsave9);
   thunderappear;
   repeat
    grpagedetect;
    ERASE;
    {------------------------------key input--------------------------------------}
    if scancode[up] then begin tyd:=-movethunder; thundermove; end;
    if scancode[left] then begin txd:=-movethunder; thundermove; end;
    if scancode[right] then begin txd:=movethunder; thundermove; end;
    if scancode[down] then begin tyd:=movethunder; thundermove; end;
    if not(scancode[up] or scancode[left] or scancode[right] or scancode[down]) then
      begin thunderx[ac]:=thunderx[vs]; thundery[ac]:=thundery[vs]; end;
    if scancode[spc] then fireyes:=true;
    if scancode[lalt] then bombyes:=true;
    if scancode[esc] then begin; setintvec(9,intsave9); closegraph; halt(1); end;
    {-----------------------------------------------------------------------------}
   enemy2move;
   enemy3move;
   enemy4move;
   powerupmove;
   for n:=1 to en1no do
    enemy1move;
   for i:=1 to blno do begin
   if blpresent[i]>1 then
    bulletmove
    end;
   for l:=1 to enblno do begin
    if enblpresent[l]>1 then
     enemybulletmove;
   end;
{--------------check for bullet hiting of enemy---------------}
   for i:=1 to blno do
    if blpresent[i]>1 then begin
     for n:=1 to en1no do begin
      if enemy1present[n]>1 then begin
       flagy:=abs(en1y[n,ac]-bly[i,ac]);
       flagx:=abs(en1x[n,ac]-blx[i,ac]);
       if (flagy<=wd) and (flagx<=wd)  then
        enemy1hiting;
      end;
     end;

     if enemy2present>1 then begin
      flagx:=abs(en2x[ac]-blx[i,ac]);
      flagy:=abs(en2y[ac]-bly[i,ac]);
      if (flagx<20) and (flagy<20)  then enemy2hiting;
     end;

     if enemy3present>1 then begin
      flagx:=abs(en3x[ac]-blx[i,ac]);
      flagy:=abs(en3y[ac]-bly[i,ac]);
      if (flagx<20) and (flagy<20)  then
       enemy3hiting;
     end;

     if enemy4present>1 then begin
      flagx:=abs(en4x[ac]-blx[i,ac]);
      flagy:=abs(en4y[ac]-bly[i,ac]);
      if (flagx<35) and (flagy<30)  then
       enemy4hiting;
     end;
   end;

   if thunderpresent=-1 then
    flameappear;
   if flamepresent=-1 then
    begin
     calsvthnd;
     if svthnd>0 then thunderappear;
    end;
   enemy1appear;
   enemy2appear;
   enemy3appear;
   enemy4appear;
   powerupappear;
   bonusup;
   fire;
   if (obmpnt > 0) and (bombyes) then  begin
     bomb; bombpoint(-1);
   end;
   DRAW;

   grpagechange;

   until svthnd <= 0;

   setactivepage(vs);
   setvisualpage(vs);
   setintvec(9,intsave9);
   GAMEOVER;

    CASE CH OF
        'Y','y' : goto 10;
        'N','n' : endscene;
    end;
 end.