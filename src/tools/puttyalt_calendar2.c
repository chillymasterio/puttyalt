/* puttyalt_calendar2.c - Calendar math: leap year, days, weekday.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int cal_is_leap(int y){ return (y%4==0&&y%100!=0)||y%400==0; }
int cal_days_in_month(int y,int m){ static const int d[]={31,28,31,30,31,30,31,31,30,31,30,31}; if(m<1||m>12)return 0; return (m==2&&cal_is_leap(y))?29:d[m-1]; }
int cal_day_of_year(int y,int m,int d){ int doy=d; for(int i=1;i<m;i++)doy+=cal_days_in_month(y,i); return doy; }
int cal_weekday(int y,int m,int d){ static const int t[]={0,3,2,5,0,3,5,1,4,6,2,4}; if(m<3)y--; return (y+y/4-y/100+y/400+t[m-1]+d)%7; }
