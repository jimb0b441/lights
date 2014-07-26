//light_control.c

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"
int cport_nr = 5;
int bdrate=19200; 

char convert(int num);
void send_color(int r, int g, int b);
void rainbow(int step);
void seizure();
void po_po();
void water();
void sequential_fade(int r1, int g1, int b1, int r2, int g2, int b2);
void simultaneous_fade(int step, int r1, int g1, int b1, int r2, int g2, int b2);

void send_color(int r, int g, int b)
{
   //unsigned char buf[4096];
   //send string to controller
    RS232_SendByte(cport_nr, 'c');
    RS232_SendByte(cport_nr, convert(r/16));
    RS232_SendByte(cport_nr, convert(r%16));
    RS232_SendByte(cport_nr, convert(g/16));
    RS232_SendByte(cport_nr, convert(g%16));
    RS232_SendByte(cport_nr, convert(b/16));
    RS232_SendByte(cport_nr, convert(b%16));


  #ifdef _WIN32
     Sleep(25);
  #else
      usleep(50000);  /* sleep for 100 milliSeconds */
  #endif
  
  
}
char convert(int num){
    if(num < 10)
      return (char)(((int)'0')+num);
    switch(num){
      case 10: return 'a';
      case 11: return 'b';
      case 12: return 'c';
      case 13: return 'd';
      case 14: return 'e';
      case 15: return 'f';
    }
    return '0';
}

void rainbow(int step){
    simultaneous_fade(step, 255, 0, 0, 255, 255, 0);
    simultaneous_fade(step, 255, 255, 0, 0, 255, 0);
    simultaneous_fade(step, 0, 255, 0, 0, 255, 255);
    simultaneous_fade(step, 0, 255, 255, 0, 0, 255);
    simultaneous_fade(step, 0, 0, 255, 255, 0, 255);
    simultaneous_fade(step, 255, 0, 255, 255, 0, 0);
}

void seizure(){
   if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");
  time_t start = time(NULL);

  while(1){
     int r = rand()%256;
     int g = rand()%256;
     int b = rand()%256;
     send_color(r,g,b);
     send_color(0,0,0);
     if(difftime(time(NULL), start) >= 10)
       break;
  }

  RS232_CloseComport(cport_nr);
}
void po_po(){

    if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");
     int num;
    while(1){
        send_color(255, 0, 0);
        send_color(0, 0, 0);
        send_color(255, 0, 0);
        send_color(0, 0, 0);
        send_color(255, 0, 0);
        send_color(0, 0, 0);
        send_color(0, 0, 255);
        send_color(0, 0, 0);
        send_color(0, 0, 255);
        send_color(0, 0, 0);
         send_color(0, 0, 255);
        send_color(0, 0, 0);
    }


    RS232_CloseComport(cport_nr);
}

void water(){
  int start_r = 0;
  int start_g = 0;
  int start_b = 255;
  int step = 17;

  simultaneous_fade(step, 255, 255, 255, start_r, start_g, start_b);
  while(1){
      int color = rand() % 10;
      switch(color){
        case 0:  simultaneous_fade(step, start_r, start_g, start_b, 0, 85, 242); start_r = 0; start_g = 85; start_b = 242; break;
        case 1:  simultaneous_fade(step, start_r, start_g, start_b, 30, 213, 203); start_r = 30; start_g = 213; start_b = 203; break;
        case 2:  simultaneous_fade(step, start_r, start_g, start_b, 3, 121, 239); start_r = 3; start_g =121; start_b =239; break;
        case 3:  simultaneous_fade(step, start_r, start_g, start_b, 46, 46, 197); start_r = 46; start_g = 46; start_b = 197; break;
        case 4:  simultaneous_fade(step, start_r, start_g, start_b, 1, 20, 197); start_r = 1; start_g = 20; start_b = 197; break;
        case 5:  simultaneous_fade(step, start_r, start_g, start_b, 95, 238, 250); start_r = 95; start_g = 238; start_b = 250; break;
        case 6:  simultaneous_fade(step, start_r, start_g, start_b, 4, 74, 159); start_r = 4; start_g = 74; start_b = 159; break;
        case 7:  simultaneous_fade(step, start_r, start_g, start_b, 19, 7, 156); start_r = 19; start_g = 7; start_b = 156; break;
        case 8:  simultaneous_fade(step, start_r, start_g, start_b, 60, 119, 230); start_r = 60; start_g = 119; start_b = 230; break;
        case 9:  simultaneous_fade(step, start_r, start_g, start_b, 0, 0, 255); start_r = 0; start_g = 0; start_b = 255; break;
      }
  }
}

//step represents how many steps fade takes (lower == faster)
void simultaneous_fade(int step, int r1, int g1, int b1, int r2, int g2, int b2){

  if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");

  int r = r1;
  int g = g1;
  int b = b1;

  int diff_r = r2-r1;
  int diff_g = g2-g1;
  int diff_b = b2-b1;

  int delta_r;
  int delta_g;
  int delta_b;

  for(; step > 0; step--){
    delta_r = diff_r / step;
    delta_g = diff_g / step;
    delta_b = diff_b / step;

    r += delta_r;
    g += delta_g;
    b += delta_b;

    send_color(r, g, b);

    diff_r -= delta_r;
    diff_g -= delta_g;
    diff_b -= delta_b;
  }

  RS232_CloseComport(cport_nr);

}

void sequential_fade(int r1, int g1, int b1, int r2, int g2, int b2){
  if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");
  int r = r1;
  int g = g1;
  int b = b1;

  //fade reds
  if(r1 < r2){
    for(; r < r2-5; r+= 5)
      send_color(r, g, b);
  }
  else
  {
    for(; r > r2+5; r -= 5)
      send_color(r, g, b);
  }
  r = r2;

  //fade greens
  if(g1 < g2){
    for(; g < g2-5; g+= 5)
      send_color(r, g, b);
  }
  else
  {
    for(; g > g2+5; g -= 5)
      send_color(r, g, b);
  }
  g = g2;

  //fade blues
  if(b1 < b2){
    for(; b < b2-5; b+= 5)
      send_color(r, g, b);
  }
  else
  {
    for(; b > b2+5; b -= 5)
      send_color(r, g, b);
  }
  b = b2;

  send_color(r, g, b);

  RS232_CloseComport(cport_nr);
}
int main()
{
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    srand(time(NULL));
/*
    if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");
     send_color(242, 185, 66);
     sleep(2);
     RS232_CloseComport(cport_nr);
     sequential_fade(242,185,66,140,64,244);
     sleep(2);

     if(RS232_OpenComport(cport_nr, bdrate))
       printf("Can not open comport\n");
     send_color(242, 185, 66);
     sleep(2);
     RS232_CloseComport(cport_nr);
     simultaneous_fade(50, 242, 185, 66, 140, 64,244);

     sleep(2);
     */
     water();
    //seizure();
    //po_po();
    // while(1){
    //  rainbow(50);
    // }


    return 0;
}
