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

void print(const int *v, const int size) {
  if (v != 0) {
    for (int i = 0; i < size; i++) {
      ans[tmpp][i] = v[i];
    }
    tmpp ++;
  }
}
void swap(int *v, const int i, const int j) {
  int t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
}
void rotateLeft(int *v, const int start, const int n) {
  int tmp = v[start];
  for (int i = start; i < n-1; i++) {
    v[i] = v[i+1];
  }
  v[n-1] = tmp;
} 
void permute(int *v, const int start, const int n) {
  print(v, n);
  if (start < n) {
    int i, j;
    for (i = n-2; i >= start; i--) {
      for (j = i + 1; j < n; j++) {
    swap(v, i, j);
    permute(v, i+1, n);
      } 
      rotateLeft(v, i, n);
    } 
  }
} 
void init_a(int *v, int N) {
  for (int i = 0; i < N; i++) {
    v[i] = i+1;
  }
}

// end of part two

int make_path(int v ){
  int counter = 0;
  while(~v){
    barr[counter ++] = v;
    v = par[v];
  }
  return counter;
}
void BFS(int v ){
  int start = 0 , end = 0;
  par[v]  = -1;
  que[end++] = v;
  dis[v] = 1;
  while( start < end ){
    int now = que[start++];
    for(int i = 0 ; i < size[now] ; ++ i){
      int u = a[now][i];
      if(dis[u])
        continue;
      dis[u] = dis[now] + 1;
      par[u] = now;
      que[end ++ ] = u;
    }
  }
} 
int n, m, counter;
int *ARR;
void update() {
  static int i = 0;
  i++;
  if(i >= counter) {
    for(int i = 0; i < n; i++)
      for(int j = 0; j < m; j++)
        if(ARR[i * m + j] != 0) ARR[i * m + j] = 3;
    return ;
  }
  int X = zeze[i];
  ARR[X] = 0;
  if(i) {
    X = zeze[i - 1];
    ARR[X] = 1;
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
  n = t , m = tt ; int N = 0;
  for(int i = 0 ; i < n ; ++ i)
    for(int j = 0 ; j < m ; ++ j ) 
      if(arr[i][j] == 3 ) N ++;
	
	// end of part one  
  //part three

	int *v = (int*) malloc(sizeof(int)*N);
  init_a(v, N);
  permute(v, 0, N); 
  //end of part three

	for(int i = 0 ; i < n ; ++ i){
    for(int j = 0 ; j < m ; ++ j){
      if( arr[i][j] != 2){
        int cnt = 0;
        if(j != 0 && arr[i][j - 1] != 2) 
          a[m * i + j][cnt ++] = m * i + j - 1;
        if(i != 0 && arr[i - 1 ][j] != 2)
          a[m * i + j][cnt ++] = m * ( i - 1) + j;
        if(i != n - 1 && arr[i + 1 ][j] != 2)
          a[m * i + j][cnt ++] = m *( i+ 1 ) + j;
        if( j != m - 1 && arr[i][j + 1 ] != 2)
          a[m * i + j][cnt ++] = m * i + j + 1 ;
        size[i * m + j ] = cnt;
      }
    }
  }

  int cnt = 0;
  for(int i = 0 ; i < n ; ++ i ){
    for(int j = 0 ; j < m ; ++ j){
      if( arr[i][j] == 3 ){
        verr[cnt ++ ] = i * m + j;
      }
      if( arr[i][j] == 0)
        verr[cnt ++ ] = i * m + j , ST = cnt - 1;
    }
  }
  if( ST != 0){
    int kkk = verr[ST];
    verr[ST] = verr[0];
    verr[0] = kkk;
  }
  for(int i = 0 ; i < cnt ; ++ i )
    for(int j = i + 1 ; j < cnt ; ++ j ){
      diss[verr[i]][verr[j]] = diss[verr[j]][verr[i]] = 10000007;
    }
  for(int i = 0 ; i < cnt ; ++ i){
    for(int k = 0 ; k < 100010 ; ++ k)
      dis[k] = que[k] = 0;
    BFS(verr[i] ) ;
    for(int j = 0 ; j < cnt ; ++ j){
      if( verr[i] != verr[j])
        diss[verr[i]][verr[j]] = diss[verr[j]][verr[i]] = dis[verr[j]];
    }
  }
  for(int i = 0 ; i < cnt ; ++ i){
    for(int k = 0 ;k < 100010 ; ++ k)
      par[k] = que[k] = dis[k] = 0;
    BFS(verr[i] );
    for(int j = 0 ; j < cnt ; ++ j){
      int NN = make_path(verr[j]);
      diz[verr[i]][verr[j]] = diz[verr[j]][verr[i]] = NN;
      for(int k = 0 ; k < NN ; ++ k)
        anz[verr[j]][verr[i]][k] = barr[k];
      for(int k = NN - 1; ~k ; -- k)
        anz[verr[i]][verr[j]][k] = barr[ NN - k -1];
    }
  }
  int mn = 10001010 , IND;
  for(int i = 0 ; i < tmpp ; ++ i){
    int counts = 0;
    counts += diss[verr[0]][verr[ans[i][0]]];
    for(int j = 0 ; j < N - 1 ; ++ j){
      counts += diss[verr[ans[i][j]]][verr[ans[i][j+1]]];
    }
    if( mn > counts )
      mn = counts , IND = i; 
  }
  /*
  printf("pretest\n");
  for(int i = 0 ; i < cnt ; ++ i)
    for(int j = 0 ; j < cnt ; ++ j , printf("\n"))
      if(i != j){
        printf("vers are %d %d    size is %d\n" ,verr[i] , verr[j] ,  diz[verr[i]][verr[j]]);
        for(int k = 0 ; k < diz[verr[i]][verr[j]] ; ++ k)
          printf("%d " , anz[verr[i]][verr[j]][k]);
      }
  printf("pretest\n");
  printf("test\n");
  for(int i = 0 ; i < N ; ++ i)
    printf("%d " , verr[ans[IND][i]]);
  printf("\ntest\n");
  for(int i = 0 ; i < cnt ; ++ i)
    printf("%d " , verr[i] );
  printf("\ntest\n");
  */
  counter = 0;
  for(int i = 0 ; i < diz[verr[0]][verr[ans[IND][0]]] ; ++ i)
    zeze[counter ++ ] = anz[verr[0]][verr[ans[IND][0]]][i] ;
  for(int i = 0 ; i < N - 1 ; ++ i )
    for(int j = 0 ; j < diz[verr[ans[IND][i]]][verr[ans[IND][i + 1]]] ; ++ j)
      if(zeze[counter - 1] != anz[verr[ans[IND][i]]][verr[ans[IND][i + 1]]][j]){
        zeze[counter ++ ] = anz[verr[ans[IND][i]]][verr[ans[IND][i + 1]]][j] ;
      }
  ARR = malloc(n * m * sizeof(int) + 10);
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++) ARR[i * m + j] = arr[i][j];
  graphics_init_ui(argc, argv, update,n,m, ARR, 0);
  
  return printf("\n") , 0;
}

