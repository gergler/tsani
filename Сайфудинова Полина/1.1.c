#include <utility.h>
#include <ansi_c.h>

 void CalculateRoots(float A, float B, float C, float *pD,  float *pValue1, float *pValue2) {
		*pD=B*B-4*A*C;
	 	if (*pD<0) {
			*pValue1= -B/(2*A);
		  	*pValue2= sqrt(-*pD)/(2*A);
	 	}
	 	else {
		 	*pValue1=(-B-sqrt(*pD))/(2*A);
		 	*pValue2=(-B+sqrt(*pD))/(2*A); 
	 	}	
}					  
 
 void Parabola(float A, float B,  float *top) {
	 *top=-B/2*A;
 }
 

int main () {
	float A, B, C, pD, pValue1, pValue2, top;
	int exit=0;
	
	while (1) { 
		printf("\nPrint: A, B, C \t"); 
		scanf("%f %f %f", &A, &B, &C);
		if (A==0) {
			if (B==0)
				printf("Not equation \n");
			else
				printf("Linear function: x=%f \n", -C/B);
		}
		else {
			CalculateRoots(A, B, C, &pD, &pValue1, &pValue2); 
			if (pD<0)
				printf("x1=%f+i%f \nx2=%f-i%f \n", pValue1, pValue2, pValue1, pValue2);
			else {
				printf("x1=%f, x2=%f", pValue1, pValue2);
				Parabola(A, B, &top); 
				printf("\nThe top of a parabola: %f \n", top);
			}
		}
		printf("\n\n");
		printf("If u want exit, print <q>\n");
		exit=getchar();
		if (exit=='q') 
			break;
	}  
} 
