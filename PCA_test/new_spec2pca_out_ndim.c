// coded by ooishi in Feb. 2014 v2
//spec2pca.c


// 例）./spec2pca.exe   調査主成分次数 < aiueo.spec

// 入力として上記２つの出力ファイル
//     平均スペクトル(avespec.data)
// 　128次元軸降順主成分データ(npca.data)
// 引数
//　　調査主成分次数
// 標準入力
//　　調査音声スペクトル
//を与えます。

// 出力としては
//　　主成分パラメタにより圧縮後復元したテスト音声スペクトルファイル(out.para)
//　　そしてデバッグデータが標準出力されます。



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[]) {
	char readline[3001], oneStr[20], dataNumStr[20];
	int  i,j,k, dataNum, startP,endP, xd, yd, dataDim;
	char *fgetflg;
	double spec[130], difspec[130], pcaSpec[130], respec[130];
	double aveSpec[130];
	double vlen0, vlen1, vlen2, div;

	double v[128][128], vtest;

	FILE *fp;
	if(argc!=2) return 1;

// load average spectrum from training data to aveSpec[128]
	fp=fopen("avespec.data","r");
	if( fp==NULL ) return 1;

	fgets(readline,10,fp);
	dataNum=atoi(readline);
	i=0;
	startP=endP=0;
	if(fgets(readline,3000,fp)==NULL) return 1;	
	while(i<128) {
		while((readline[endP]!=0x09) && (readline[endP]>0x20) ) { 		//! tab && space
				oneStr[endP-startP]=readline[endP];
				endP++;
//				printf("startP,endP:  %d, %d\n", startP, endP);					
		}
		oneStr[endP-startP]='\0';
		
		if(strlen(oneStr)>3) {
			aveSpec[i]=atof(oneStr);
//			printf("\t aveSpec[%d]:%f\n", i, aveSpec[i]);
			i++;
		}
		endP=startP=endP+1;
	}

	fclose(fp);
	
// load pca 3dim parameter to v[dim][128]
	fp=fopen("npca.data","r");
	if( fp==NULL ) return 1;

	fgets(readline,10,fp);

///	dataDim=atoi(readline);
dataDim=atoi(argv[1]);  ///debugged on Feb. 3rd

printf("v[%d][128] loading \n", dataDim); 
	i=0;
	while(i<dataDim) {
		if( fgets(readline,3000,fp)==NULL) return 1;	
		if(strlen(readline)<5) continue;
		
//		printf("\nv[%d][] = \n", i); 
		startP=endP=0;
		j=0;
		while(j<128) {
			while((readline[endP]!=0x09) && (readline[endP]>0x20) ) { 		//! tab && space
					oneStr[endP-startP]=readline[endP];
					endP++;
		//				printf("startP,endP:  %d, %d\n", startP, endP);					
			}
			oneStr[endP-startP]='\0';
			
			if(strlen(oneStr)>3) {
				v[i][j]=atof(oneStr);
//				printf("\t v[%d][%d]:%f\n", i, j, v[i][j]);
				j++;
				if(j>127) i++;
			}
			endP=startP=endP+1;
		}
	}
	fclose(fp);
	
printf(" 軸の直交性検査 in\n");

// 軸の直交性＆単位ベクトル検査
	vtest=vlen0=vlen1=vlen2=0;
	for(j=0; j<128; j++) {
		vtest += v[0][j] * v[1][j];
		vlen0 += v[0][j] * v[0][j];
		vlen1 += v[1][j] * v[1][j];
		vlen2 += v[2][j] * v[2][j];
/*
		vtest += v[j][0] * v[j][1];
		vlen0 += v[j][0] * v[j][0];
		vlen1 += v[j][1] * v[j][1];
		vlen2 += v[j][2] * v[j][2];
*/		
	}
	printf("\nlength of V0=%10.5f , length of V1=%10.5f, length of V2=%10.5f\n", vlen0, vlen1, vlen2);
	
	printf("v0*v1=%10.5f\n", vtest);
	
	vtest=0;
	for(j=0; j<128; j++) {
		vtest += v[2][j] * v[1][j];
	}
	printf("v2*v1=%10.5f\n", vtest);
	vtest=0;
	for(j=0; j<128; j++) {
		vtest += v[0][j] * v[2][j];
	}
	printf("v0*v2=%10.5f\n", vtest);
	

// read spectrum data
printf("\n reconstructed spec data\n");

printf("|n num of data  is \n");

	if (fgets(dataNumStr,100,stdin)==NULL) return 1;
	dataNum=atoi(dataNumStr);
	printf("%d\n",dataNum);

	fp=fopen("vowel_space.para","w");
	if(fp==NULL) return 1;
	
	fprintf(fp,"%d\n",dataNum);
	xd=0;
	while( fgets(readline,2000,stdin)!=NULL) {
		endP=startP=0;
		for(i=0;i<128;i++) {
			while(readline[endP]!=' ') { // ' 'か'\t'を選んで。
//				printf("<%c>",readline[endP]);
				oneStr[endP-startP]=readline[endP];
				endP++;
			}
			oneStr[endP-startP]='\0';

			spec[i]=atof(oneStr);
			difspec[i] = spec[i]-aveSpec[i];
//			printf("\t{#%d} spec[], spec-ave[%d] : %f , %f \n", xd, i, spec[i], difspec[i] );
			endP=startP=endP+1;
		}
		xd++;

// produce pcaSpec[] data from spec[] with v[][]

/*		printf("spec data\n");
		for(i=0; i<128; i++)  {	//v param
			printf("%8.5f\t", spec[i]);
		}
		printf("\n");
*/

//		printf("pcaSpec data\n");
		for(j=0; j<dataDim; j++) {  //v dim
			pcaSpec[j]=0.0;
			for(i=0; i<128; i++)  {	//v param
				pcaSpec[j] += difspec[i]*v[j][i];
//				printf("s%8.5fs, v%8.5fv \t", spec[i], v[j][i]);
			}
			printf("%10.5f\t", pcaSpec[j]);
	      fprintf(fp,"%8.5f\t", pcaSpec[j]);
		}
		printf("\n");
		fprintf(fp, "\n");

//printf("reconstructed spec data\n");
		
		for(i=0; i<128; i++)  {
			respec[i]=0;
			for(j=0; j<dataDim; j++) {
				respec[i] += pcaSpec[j]*v[j][i];
			}
			respec[i] += aveSpec[i];
			//fprintf(fp,"%8.5f\t", respec[i]);
		}
		//fprintf(fp,"0.0\t");
		//fprintf(fp,"0.0\t");
		//fprintf(fp,"\n");

/*		printf("div respec from spec\n");
		div=0;
		for(i=0; i<128; i++)  {	//v param
			for(j=0; j<dataDim; j++) {  //v dim
				div+=(spec[i]*spec[i]-respec[i]*respec[i])*(spec[i]*spec[i]
											-respec[i]*respec[i]);
			}
		}
		printf(" div[%d]=%10.5f\n\n", xd, div);
	}
	printf("\n");
*/

	}
}


