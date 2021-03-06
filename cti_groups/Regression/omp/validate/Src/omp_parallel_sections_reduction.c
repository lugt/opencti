
#include <stdio.h>
#include <math.h>
#include "omp_testsuite.h"


int test_omp_parallel_sections_reduction(FILE * logFile){
	int sum=7;
	int known_sum;	
	double dpt=1,dsum=0;
	double dknown_sum;
	double dt=0.5;				/* base of geometric row for + and - test*/
	double rounding_error= 1.E-5;
	int diff;
	double ddiff;
	int product=1;
	int known_product;
	int logic_and=1;
	int bit_and=1;
	int logic_or=0;
	int bit_or=0;
	int exclusiv_bit_or=0;
	int logics[1000];
	int i;
	int result=0;

	/*  int my_islarger;*/
	/*int is_larger=1;*/
	known_sum = (999*1000)/2+7;
#pragma omp parallel sections private(i) reduction(+:sum)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				sum=sum+i;
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				sum=sum+i;
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				sum=sum+i;
			}
		}
	}

	if(known_sum!=sum)
	{
		result++;
		fprintf(logFile,"Error in sum with integers: Result was %d instead of %d.\n",sum, known_sum);
	}

	diff = (999*1000)/2;
#pragma omp parallel sections private(i) reduction(-:diff)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				diff=diff-i;
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				diff=diff-i;
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				diff=diff-i;
			}
		}
	}


	if(diff != 0)
	{
		result++;
		fprintf(logFile,"Error in Difference with integers: Result was %d instead of 0.\n",diff);
	}
	for (i=0;i<20;++i)
	{
		dpt*=dt;
	}
	dknown_sum = (1-dpt)/(1-dt);
#pragma omp parallel sections private(i) reduction(+:dsum)
	{
#pragma omp section
		{
			for (i=0;i<6;++i)
			{
				dsum += pow(dt,i);
			}
		}
#pragma omp section
		{
			for (i=6;i<12;++i)
			{
				dsum += pow(dt,i);
			}
		}
#pragma omp section
		{
			for (i=12;i<20;++i)
			{
				dsum += pow(dt,i);
			}
		}
	}


	if( fabs(dsum-dknown_sum) > rounding_error )
	{
		result++; 
		fprintf(logFile,"Error in sum with doubles: Result was %f instead of %f (Difference: %E)\n",dsum,dknown_sum, dsum-dknown_sum);
	}

	dpt=1;

	for (i=0;i<20;++i)
	{
		dpt*=dt;
	}
	fprintf(logFile,"\n");
	ddiff = (1-dpt)/(1-dt);
#pragma omp parallel sections private(i) reduction(-:ddiff)
	{
#pragma omp section
		{
			for (i=0;i<6;++i)
			{
				ddiff -= pow(dt,i);
			}
		}
#pragma omp section
		{
			for (i=6;i<12;++i)
			{
				ddiff -= pow(dt,i);
			}
		}
#pragma omp section
		{
			for (i=12;i<20;++i)
			{
				ddiff -= pow(dt,i);
			}
		}
	}

	if( fabs(ddiff) > rounding_error)
	{
		result++;
		fprintf(logFile,"Error in Difference with doubles: Result was %E instead of 0.0\n",ddiff);
	}

	known_product = 3628800;
#pragma omp parallel sections private(i) reduction(*:product)
	{
#pragma omp section
		{	
			for(i=1;i<3;i++)
			{
				product *= i;
			}
		}
#pragma omp section
		{
			for(i=3;i<7;i++)
			{
				product *= i;
			}
		}
#pragma omp section
		{
			for(i=7;i<11;i++)
			{
				product *= i;
			}
		}
	}


	if(known_product != product)
	{
		result++;
		fprintf(logFile,"Error in Product with integers: Result was %d instead of %d\n",product,known_product);
	}

	for(i=0;i<1000;i++)
	{
		logics[i]=1;
	}

#pragma omp parallel sections private(i) reduction(&&:logic_and)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
	}

	if(!logic_and)
	{
		result++;
		fprintf(logFile,"Error in logic AND part 1\n");
	}

	logic_and = 1;
	logics[501] = 0;

#pragma omp parallel sections private(i) reduction(&&:logic_and)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				logic_and = (logic_and && logics[i]);
			}
		}
	}

	if(logic_and)
	{
		result++;
		fprintf(logFile,"Error in logic AND part 2");
	}

	for(i=0;i<1000;i++)
	{
		logics[i]=0;
	}

#pragma omp parallel sections private(i) reduction(||:logic_or)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
	}

	if(logic_or)
	{
		result++;
		fprintf(logFile,"Error in logic OR part 1\n");
	}

	logic_or = 0;
	logics[501]=1;

#pragma omp parallel sections private(i) reduction(||:logic_or)
	{
#pragma omp section
		{
			for (i=1;i<300;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
#pragma omp section
		{
			for (i=300;i<700;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
#pragma omp section
		{
			for (i=700;i<1000;i++)
			{
				logic_or = (logic_or || logics[i]);
			}
		}
	}

	if(!logic_or)
	{
		result++;
		fprintf(logFile,"Error in logic OR part 2\n");
	}

	for(i=0;i<1000;++i)
	{
		logics[i]=1;
	}

#pragma omp parallel sections private(i) reduction(&:bit_and)
	{
#pragma omp section
		{	
			for(i=0;i<300;++i)
			{
				bit_and = (bit_and & logics[i]);
			}
		}
#pragma omp section
		{	
			for(i=300;i<700;++i)
			{
				bit_and = (bit_and & logics[i]);
			}
		}
#pragma omp section
		{	
			for(i=700;i<1000;++i)
			{
				bit_and = (bit_and & logics[i]);
			}
		}
	}
	if(!bit_and)
	{
		result++;
		fprintf(logFile,"Error in BIT AND part 1\n");
	}

	bit_and = 1;
	logics[501]=0;

#pragma omp parallel sections private(i) reduction(&:bit_and)
	{
#pragma omp section
		{
			for(i=0;i<300;++i)
			{
				bit_and = bit_and & logics[i];
			}
		}
#pragma omp section
		{
			for(i=300;i<700;++i)
			{
				bit_and = bit_and & logics[i];
			}
		}
#pragma omp section
		{
			for(i=700;i<1000;++i)
			{
				bit_and = bit_and & logics[i];
			}
		}
	}
	if(bit_and)
	{
		result++;
		fprintf(logFile,"Error in BIT AND part 2");
	}

	for(i=0;i<1000;i++)
	{
		logics[i]=0;
	}

#pragma omp parallel sections private(i) reduction(|:bit_or)
	{
#pragma omp section
		{
			for(i=0;i<300;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
#pragma omp section
		{
			for(i=300;i<700;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
#pragma omp section
		{
			for(i=700;i<1000;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
	}
	if(bit_or)
	{
		result++;
		fprintf(logFile,"Error in BIT OR part 1\n");
	}
	bit_or = 0;
	logics[501]=1;

#pragma omp parallel sections private(i) reduction(|:bit_or)
	{
#pragma omp section
		{
			for(i=0;i<300;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
#pragma omp section
		{
			for(i=300;i<700;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
#pragma omp section
		{
			for(i=700;i<1000;++i)
			{
				bit_or = bit_or | logics[i];
			}
		}
	}
	if(!bit_or)
	{
		result++;
		fprintf(logFile,"Error in BIT OR part 2\n");
	}

	for(i=0;i<1000;i++)
	{
		logics[i]=0;
	}

#pragma omp parallel sections private(i) reduction(^:exclusiv_bit_or)
	{
#pragma omp section
		{	
			for(i=0;i<300;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
#pragma omp section
		{	
			for(i=300;i<700;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
#pragma omp section
		{	
			for(i=700;i<1000;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
	}
	if(exclusiv_bit_or)
	{
		result++;
		fprintf(logFile,"Error in EXCLUSIV BIT OR part 1\n");
	}

	exclusiv_bit_or = 0;
	logics[501]=1;

#pragma omp parallel sections private(i) reduction(^:exclusiv_bit_or)
	{
#pragma omp section
		{
			for(i=0;i<300;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
#pragma omp section
		{
			for(i=300;i<700;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
#pragma omp section
		{
			for(i=700;i<1000;++i)
			{
				exclusiv_bit_or = exclusiv_bit_or ^ logics[i];
			}
		}
	}
	if(!exclusiv_bit_or)
	{
		result++;
		fprintf(logFile,"Error in EXCLUSIV BIT OR part 2\n");
	}

	/*printf("\nResult:%d\n",result);*/
	return (result==0);
}
int main()
{
	int i;			/* Loop index */
	int result;		/* return value of the program */
	int failed=0; 		/* Number of failed tests */
	int success=0;		/* number of succeeded tests */
	static FILE * logFile;	/* pointer onto the logfile */
	static const char * logFileName = "ctest_omp_parallel_sections_reduction.log";	/* name of the logfile */


	/* Open a new Logfile or overwrite the existing one. */
	logFile = fopen(logFileName,"w+");

	printf("######## OpenMP Validation Suite V %s ######\n", OMPTS_VERSION );
	printf("## Repetitions: %3d                       ####\n",REPETITIONS);
	printf("## Loop Count : %6d                    ####\n",LOOPCOUNT);
	printf("##############################################\n");
	printf("Testing omp parallel sections reduction\n\n");

	fprintf(logFile,"######## OpenMP Validation Suite V %s ######\n", OMPTS_VERSION );
	fprintf(logFile,"## Repetitions: %3d                       ####\n",REPETITIONS);
	fprintf(logFile,"## Loop Count : %6d                    ####\n",LOOPCOUNT);
	fprintf(logFile,"##############################################\n");
	fprintf(logFile,"Testing omp parallel sections reduction\n\n");

	for ( i = 0; i < REPETITIONS; i++ ) {
		fprintf (logFile, "\n\n%d. run of test_omp_parallel_sections_reduction out of %d\n\n",i+1,REPETITIONS);
		if(test_omp_parallel_sections_reduction(logFile)){
			fprintf(logFile,"Test succesfull.\n");
			success++;
		}
		else {
			fprintf(logFile,"Error: Test failed.\n");
			printf("Error: Test failed.\n");
			failed++;
		}
	}

    if(failed==0){
		fprintf(logFile,"\nDirectiv worked without errors.\n");
		printf("Directiv worked without errors.\n");
		result=0;
	}
	else{
		fprintf(logFile,"\nDirective failed the test %i times out of %i. %i were successful\n",failed,REPETITIONS,success);
		printf("Directive failed the test %i times out of %i.\n%i test(s) were successful\n",failed,REPETITIONS,success);
		result = (int) (((double) failed / (double) REPETITIONS ) * 100 );
	}
	printf ("Result: %i\n", result);
	return result;
}
