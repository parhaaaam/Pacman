#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "pacman_graph.h"

int dis[100010] , que[100010] , diss[2510][2510], size[100010];
int a[200010][5] , arr[50][50];
int verr[100010];
int barr[100010] , par[100010];
char s[55][55];
int tmpp ;
int ans[100010][10];
int anz[500][500][100];
int diz[2500][2500];
int ST;
int zeze[100010];
//part four
/*
void clear() {
  sleep(1);
  system("@cls||clear");
}

void draw(int n, int m, int table[50][50]) {
  clear();
  for(int i = 0; i < n; i++, printf("\n"))
    for(int j = 0; j < m; j++) {
      switch(table[i][j]) {
        case 0: printf("%c", 254); break;
        case 1: printf("%c", 176); break;
        case 2: printf("%c", 178); break;
        case 3: printf("%c", 160); break;
      }
    }
}
*/
//end of part four
// part two

int n, m, counter , N;
int *ARR;
void update(){
  if(!N){
    return ;
  } 
  char chh = KExx;
  int XX , YY , nXX , nYY ;
  for(int i = 0 ; i < n ; ++ i)
    for(int j = 0 ; j < m ; ++ j)
      if( arr[i][j] == 0)
       XX = i , YY = j; 
  if( chh == 'a'){
    nXX = XX ;
    nYY = YY - 1;
  }
  else if (chh == 's'){
    nXX = XX + 1; 
    nYY = YY;
  }
  else if ( chh == 'w'){
    nXX = XX - 1;
    nYY = YY;
  }
  else if( chh == 'd'){
    nXX = XX;
    nYY = YY + 1;
  }
  if( nXX < 0 || nXX >= n ) nXX = XX;
  if( nYY < 0 || nYY >= m ) nYY = YY;
  if( arr[nXX][nYY] == 3) N --;
  if( arr[nXX][nYY] != 2){
    ARR[XX * m + YY] = 1;
    arr[XX][YY] = 1;
    arr[nXX][nYY] = 0;
    ARR[nXX * m + nYY] = 0;
  }
  if(!N) {

    for(int i = 0 ; i < n * m ; ++ i){
      if( ARR[i] )
        ARR[i] = 3;
    }
  }
}
signed main(int argc, char*argv){
  
	// part one 

	FILE *tmmp ;
  tmmp = fopen("input01" , "r");
  char ch;
  int t = 0 , k = 0 , tt;
  while( ( ch = fgetc(tmmp ) ) != EOF){
    if( ch == '\n'){
      tt = k;
      k = 0 ;
      t ++;
      continue;
    }
    s[t][k++] = ch;
  }
  int X , Y;
  for(int i = 0 ;i < t ; ++ i ){
    for(int j = 0 ; j < tt ; ++ j ){
      if( s[i][j] == '0' ) 
        X = i , Y = j , arr[i][j] = 0;
      else if ( s[i][j] == '1' ) 
        arr[i][j] = 1;
      else if (s[i][j] == '#')
        arr[i][j] = 2;
      else 
        arr[i][j] = 3;
    } 
  }
  n = t , m = tt , N = 0;
  for(int i = 0 ; i < n ; ++ i)
    for(int j = 0 ; j < m ; ++ j ) 
      if(arr[i][j] == 3 ) N ++;
  ARR = malloc(n * m * sizeof(int) + 10);
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++) ARR[i * m + j] = arr[i][j];
  graphics_init_ui(argc, argv, update,n,m, ARR, 1);
  
  return printf("\n") , 0;
}

