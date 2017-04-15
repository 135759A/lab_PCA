/*************************************************************************
    Vowel2Spec.c -- 主軸成分から対数スペクトルを再生する。
  ------------------------------------------------------------
    Written:  2015/07/13 Akichika HIGA
    
    Update:  
    Reason:  
    
    ./a.out 次元数 vowelA.para
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char const *argv[]){
  char readLine[3001], oneStr[20];
  double reSpec[130], v[128][128], aveSpec[130];
  int dataDim, F_Num, dataNum;
  int i, j, k, startP, endP;

  FILE *fp, *fp1, *fp2, *fp3;

  dataDim = atoi(argv[1]);
  fp  = fopen("npca.data","r");
  fp1 = fopen(argv[2],"r");
  fp2 = fopen("reSpec.spec","w");
  fp3 = fopen("avespec.data","r"); /*ここで追加とは(;;)*/

  /*平均スペクトルを読み取る*/
  if(fp3 == NULL) return 1;
  fgets(readLine, 10, fp3);
  dataNum = atoi(readLine);
  i = 0;
  startP = endP = 0;
  if(fgets(readLine, 3000, fp3) == NULL) return 1; 
  while(i < 128) {
    while((readLine[endP] != 0x09) && (readLine[endP] > 0x20) ) {     
          //! tab && space
      oneStr[endP - startP] = readLine[endP];
      endP++;
    }
    oneStr[endP - startP] = '\0';
    if(strlen(oneStr) > 3){
      aveSpec[i] = atof(oneStr);
      i++;
    }
    endP = startP = endP + 1;
  }
  fclose(fp3);

  /*
  npca.dataを読み取る
  読み取った内容はv[][]へ
  */
  
  fgets(readLine, 10, fp);
  i = 0;
  while(i < dataDim){
    if(fgets(readLine, 3000, fp) == NULL) return 1; 
    if(strlen(readLine) < 5) continue;
    startP = endP = 0;
    j = 0;
    while(j < 128){
      while((readLine[endP] != 0x09) && (readLine[endP] > 0x20) ) { 
      //! tab && space
        oneStr[endP - startP] = readLine[endP];
        endP++;
      }
      oneStr[endP - startP] = '\0';
      if(strlen(oneStr) > 3){
        v[i][j] = atof(oneStr);
        //printf("v[%d][%d] = %8.5f\n", i, j, v[i][j]);
        j++;
        if(j>127) i++;
      }
      endP = startP = endP + 1;
    }
  }
  fclose(fp);
  
  /*主軸成分を読み取る*/
  fgets(readLine, 2000, fp1);
  F_Num = atoi(readLine);
  double pcaSpec[F_Num][dataDim]; /*主軸成分を格納する配列がここでしか宣言できない(;;)*/

  i = 0;
  while(i < F_Num){
    if(fgets(readLine, 2000, fp1) == NULL) return 1; 
    if(strlen(readLine) < 5) continue;
    startP = endP = 0;
    j = 0;
    while(j < dataDim){
      while((readLine[endP] != 0x09) && (readLine[endP] > 0x20) ) { 
      //! tab && space
        oneStr[endP - startP] = readLine[endP];
        //printf("%c\n", oneStr[endP - startP]);
        endP++;
      }
      oneStr[endP - startP] = '\0';
      if(strlen(oneStr) > 3){
        pcaSpec[i][j] = atof(oneStr);
        printf("pcaSpec[%d][%d] = %8.5f\n", i, j, pcaSpec[i][j]);
        j++;
        if(j > dataDim - 1) i++;
      }
      endP = startP = endP + 1;
    }
  }
  fclose(fp1);
  
  /*対数スペクトルへ再生する*/
  
  fprintf(fp2, "%d\n", F_Num);
  k = 0;
  while(k < F_Num){
    for(i = 0; i < 128; i++){
      reSpec[i] = 0;
      for (j = 0; j < dataDim; j++){
        reSpec[i] += pcaSpec[k][j] * v[j][i];
      }
      reSpec[i] += aveSpec[i];
      fprintf(fp2, "%8.5f\t", reSpec[i]);
    }
    fprintf(fp2, "0.0\t0.0\t\n");
    k++;
  }
  fclose(fp2);
  
  return 0;
}



