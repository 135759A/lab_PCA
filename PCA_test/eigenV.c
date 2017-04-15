//#include "eigen.h"
#include <math.h>
//  a[][]:data    lambda[]:eigenValue    v[][]:eigenVector    kmax:repeatTimes    n:# of data
int eigenV(double a[][128], double lambda[], double v[][128], int kmax, int n)
{
	int i,j,k,p,q;
	double d, eps, theta, s,c;
	double oldapj,oldaqj,oldaip,oldaiq,oldvip,oldviq;
	double fabs(), atan();
	for(i=0;i<n; i++) {
		for(j=0; j<n; j++) {
			v[i][j] = (i==j)? (1.0):(0.0);
		}
	}
	// find adbMAX of a[n][n]
	d=0.0;
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			d = (fabs(a[i][j]) > d)? (fabs(a[i][j])):(d);
		}
	}
	//set epsilon
	eps=d*1.0e-7;
	//main loop
	kmax = (kmax>=n*n)? (kmax):(n*n);
	for(k=0; k<kmax; k++) {
		p=0; q=1;
		for(i=0; i<n-1; i++) {
			for(j=i+1; j<n; j++) {
				if(fabs(a[i][j])>fabs(a[p][q])) { p=i; q=j; }
			}
		}
		if(fabs(a[p][q])<eps) break;
		//theta calculation
		if(fabs(a[q][q]-a[p][p])<eps) theta=0.7853981634;
		else theta=0.5*atan(2.0*a[p][q]/(a[q][q]-a[p][p]));
		s=sin(theta); c=cos(theta);
		for(j=0; j<n; j++) {
			oldapj=a[p][j]; oldaqj=a[q][j];
			a[p][j]=oldapj*c-oldaqj*s;
			a[q][j]=oldapj*s+oldaqj*c;
		}
		for(i=0; i<n; i++) {
			oldaip=a[i][p]; oldaiq=a[i][q];
			a[i][p]=oldaip*c-oldaiq*s;
			a[i][q]=oldaip*s+oldaiq*c;
			oldvip=v[i][p]; oldviq=v[i][q];
			v[i][p]=oldvip*c-oldviq*s;
			v[i][q]=oldvip*s+oldviq*c;
		}
	}
	//store eigenvalues
	for(i=0;i<n;i++) lambda[i]=a[i][i];
	
	return 0;
}