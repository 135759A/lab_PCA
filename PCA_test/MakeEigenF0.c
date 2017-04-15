//音声スペクトル主成分分析プログラム
//coded by ooishi dec.,2013
//音声スペクトル主成分分析プログラム

// get spectrum data and output eigen value
// haisaiシステムからのスペクトルデータを元に、F0値が50を超える値の主成分分析を行い、
//  寄与率の降順に１２８次元分の軸ベクトルおよび平均スペクトルを出力する。

//how to compile,
//cc MakeEigenF0.c eigenV.c -o MakeEigenF0.exec

// 実行例）  ./MakeEigenF0.exe   F0data  <  spec.txt

//入力
// 　引数として
//       haisaiシステム出力ファイル F0data
//     標準入力として
//       haisaiシステム出力ファイル spectrum text data

//出力
//   ./npca.data　(１２８次元分の軸ベクトル)ファイル
//   ./avespec.data　(平均スペクトル)ファイル


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int eigenV(double[][128], double[], double[][128], int, int);

int main(int argc, char *argv[]) {
	char readline[2001], oneStr[20], dataNumStr[20], c;
	int  i,j,dataNum, startP,endP, xd, yd, countNum;
	double (*spec)[130];

	int x,xx;
	double cov[128][128];

	int kmax;
	#define NNN 128
	double lambda[NNN], v[NNN][NNN];
	int lambdaOrder[NNN], lambdaTemp;
	double lambdaSum, lambdaPartSum;
	float *f0;
	FILE *fp;
	
//load F0 data to eliminate nonspeech period
	fp=fopen(argv[1],"r");
	if(fp==NULL) return 1;
	fgets(readline,10,fp);
	dataNum=atoi(readline);
//	printf("#dataNum=%d\n", dataNum);
	f0=malloc( (dataNum+1)*sizeof(float) );
	i=0;
	while(i<dataNum) {
		oneStr[0]='\0';
		j=0;
		do {
			if( (c=fgetc(fp)) ==EOF) return 1;
			
			if( (c>='0' && c<='9') || c=='.' || c=='-') { 
				oneStr[j++]=c;
				oneStr[j]='\0';				
			} else {
				j=0;
			}
		} while(j>0) ;
		if(strlen(oneStr)>0) {
			f0[i]=atof(oneStr);
			i++;
			printf("F0[%d]=%f\n", i-1, f0[i-1]);
		}
	}
	fclose(fp);
	
// read spectrum data from standard input to spec[][]
	if (fgets(dataNumStr,100,stdin)==NULL) return 1;
	else {
		dataNum=atoi(dataNumStr);
		printf("c");
		printf("\n num of spectrum : <%d>\n\n",dataNum);
		printf("b");
		spec=malloc(sizeof(double)*(dataNum+2)*130);
		printf("a");
		if(spec==NULL) {
			printf("malloc error\n");return 1;
		}
	}
	yd=0; //frame#

	for(i=0; i<dataNum; i++) {
		startP=endP=0;
		xd=0; //dimension
		fgets(readline,1200,stdin);
		for(j=0; j<130; j++) {
			while(readline[endP]!=' ') { // ' 'か'\t'を選んで。
		  //while(readline[endP]!=' ') {
				oneStr[endP-startP]=readline[endP];
				endP++;
			}
			oneStr[endP-startP]='\0';
			if(f0[i]>50.0) { // spec threshold 
				spec[yd][xd]=atof(oneStr);
//				printf("\t{%d} spec[%d][%d] : %f\n", endP,yd,xd,spec[yd][xd]);
//				printf("%f\t", spec[yd][xd]);
				xd++;
			} else {
			}
			endP=startP=endP+1;
		}
		if(f0[i]>50.0) {
			yd++;
		} else {
			printf("frame[%d] skipped, F0=%10.5f\n", i, f0[i]);
		}
	}

	dataNum=yd-1;

#ifdef debug
	printf("\n set vector mean value\n"); //using spec[dataNum][*] as Sum
#endif
 
// spectrum data normalization
	for(j=0; j<128; j++)   {  
		spec[dataNum][j]=0.0;  //spec[dataNum][] << each dim temp area
		for(i=0; i<dataNum; i++) {
			spec[dataNum][j] += spec[i][j];  //make summation
		}
		spec[dataNum][j] /= (double)dataNum;
	}
// spectrum data differentialization
	for(i=0; i<dataNum; i++) {
		for(j=0; j<128; j++)   {
			spec[i][j] -= spec[dataNum][j];
		}
	}  //spec[dataNum][] << average spectrum vector

#ifdef debug
	printf("\n binSpec produced\n");
#endif

	for(i=0; i<128; i++) {
		for(j=0; j<128; j++) {
			cov[i][j]=0.0;
		}
	}
#ifdef debug
	printf("\n cov[][] initialized  %d\n",dataNum);
#endif	
	for(i=0; i<dataNum; i++) {
		for(x=0; x<128; x++)   {
			for(xx=0; xx<128; xx++) {
				cov[x][xx] += spec[i][x]*spec[i][xx];
#ifdef debug
				printf("cov[%d][%d]=%f\t", xx,x,cov[x][xx]);
#endif
			}
		}
	}	
	for(i=0; i<128; i++) { // dim
		for(j=0; j<128; j++) { // eigen val#
			cov[j][i]=cov[j][i]/(double)dataNum;
		}
	}
#ifdef debug
	printf("\n cov[][] calculated \n");
#endif

	kmax=128*128;
	eigenV(cov, lambda, v, kmax, 128);

#ifdef debug
	printf("\n");
#endif

	for(j=0; j<128; j++) {
//		printf("lambda[%d]=%f\n", j, lambda[j]);
		
		for(i=0; i<128; i++) {
#ifdef debug
			printf("v[%3d][%3d]=%8.5f\n", i, j, v[i][j]);
#endif
//			printf("%8.5f \t", v[i][j]);
		}
//		printf("\n");
		
		lambdaOrder[j]=j;
	}
	
	for(j=0; j<127; j++) {
		for(i=127; i>j; i--) {
			if(fabs(lambda[lambdaOrder[i-1]])<fabs(lambda[lambdaOrder[i]])) {
				lambdaTemp=lambdaOrder[i]; lambdaOrder[i]=lambdaOrder[i-1]; lambdaOrder[i-1]=lambdaTemp;
			}
		}
	}
	printf("\n");
	lambdaSum=0.0;
	for(j=0; j<128; j++) {
		lambdaSum += fabs(lambda[lambdaOrder[j]]);
		printf("#%3d(accumulation %3.1f) : lambda[%3d]=%10.7f\n", j, lambdaSum, lambdaOrder[j], lambda[lambdaOrder[j]]);
	}
	printf("\n sum of absolute value of lambda : %f\n\n", lambdaSum);

	lambdaPartSum=0.0;
	for(j=0; j<128; j++) {
		lambdaPartSum += fabs(lambda[lambdaOrder[j]]);
		printf("#%3d(contributionSum %1.5f) : lambda[%3d]=%10.7f\n", j, lambdaPartSum/lambdaSum, lambdaOrder[j], lambda[lambdaOrder[j]]);
	}

// pcaNdim data output

	if(argc == 3)  countNum=atoi(argv[2]);
	else countNum=128;
	
	fp=fopen("npca.data","w");	
	if(fp==NULL)  return 1;
	
	printf("\npca<%d>dim data \n\n", countNum);
	
	fprintf(fp,"%d\n",countNum);
				      
	for(i=0; i<countNum; i++) {
		for(j=0; j<128; j++) {
			fprintf(fp,"%9.5f \t", v[j][lambdaOrder[i]]  );
		}
		fprintf(fp,"\n\n");
	}	
	fclose(fp);
	
// average spectrum vector output

	fp=fopen("avespec.data","w");	
	if(fp==NULL)  return 1;

	printf("average spectrum vector \n");
	fprintf(fp,"%d\n", 128);	
	for(j=0; j<128; j++)   {
		fprintf(fp,"%9.5f \t", spec[dataNum][j] );
	}
	fprintf(fp,"\n\n");
	fclose(fp);
}

