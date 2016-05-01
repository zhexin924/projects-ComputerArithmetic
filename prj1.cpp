
/*Carry Completion Adder---Project 1*/				  
/*Zhexin Zhang,ipk693*/    					  
/*carryIn1[i+1]= numA[i]*numB[i]+CarryIn1[i]*(numA[i]⨁numB[i])*/
/*carryIn0[i+1]= numA[i]*numB[i]+CarryIn0[i]*(numA[i]⨁numB[i])*/
/*sum[i]=numA[i]⨁numB[i]⨁CarryIn1[i]*/                         

#include<stdio.h>
#include<cmath>
#include<time.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
#define TESTSET 1000
int main()
{
	//initialize the parameters
	int numA[48], numB[48], operandSize, carryIn1[48], carryIn0[48], carryOut0[48], carryOut1[48], sum[48];
	int i, j, cycle, temp,temp1;
	bool done[48], CC;
	float delay[48];
	srand((unsigned)time(NULL));
	for (i = 1; i <= 48; i++) delay[i] = 0.0;//initialize delay for every operand size
	for (operandSize = 1; operandSize <= 48; operandSize++)//the first loop that changes operand size
	{
		for (i = 0; i < TESTSET; i++)//the second loop to test 1000 times for each operand size
		{
			cycle = 0;//at the beginning of every test set,reset the clock cycle to 0;
			CC = false;
			for (j = 0; j < operandSize; j++)/*generate the two operands and initialize other outputs to zero*/
			{
				numA[j] = rand() % 2;//generate a random binary number A
				numB[j] = rand() % 2;//generate a random binary number B
				carryIn0[j] = 0;
				carryIn1[j] = 0;
				carryOut0[j] = 0;
				carryOut1[j] = 0;
				sum[j] = 0;
			}
			
			/*at the beginning of each test set,carryin0 on the first bit is always 1,that means the first bit has no carry in.*/
			carryIn0[0] = 1;
			carryIn1[0] = 0;
			while (CC == false)//start the simulation
			{
				if (cycle != 0)
				{
					CC = true;
					temp1 = 1;
					while ((CC == true)&&(temp1<operandSize))/*every 2d delay,check if the carries has been completed*/
					{
						for (j = 0; j<operandSize; j++)//set all the done for each bits in this cycle
						{
							done[j] = ((carryOut0[j] || carryOut1[j]) ? true : false);
						}
						for (j = 0; j<operandSize; j++)//if one bit has not been finalized,set CC to false,and break the loop
						{
							if (done[j] == false)
							{
								CC = false;
								break;
							}
							temp1++;
						}
					}
				}
				if (CC == true) break;
				/*generate new sum,carryout0,carryout1 in this cycle*/
				for (j = 0; j<operandSize; j++)
				{
					carryOut1[j] = ((numA[j] && numB[j]) || (carryIn1[j] && (numA[j] ^ numB[j])));
					carryOut0[j] = ((!numA[j] && !numB[j]) || (carryIn0[j] && (numA[j] ^ numB[j])));
					sum[j] = numA[j] ^ numB[j] ^ carryIn1[j];
				}
			
				for (j = 1; j<operandSize; j++)
				{
					carryIn0[j] = carryOut0[j - 1];
					carryIn1[j] = carryOut1[j - 1];
				}
				cycle++;//need 1 more cycle
			}
			temp = cycle * 2 + 2;
			delay[operandSize] += temp;
		}
		
		cout <<"operand size="<<operandSize<<endl;
		cout<<"average delay="<<(delay[operandSize] / TESTSET) << endl;
	}
	getchar();
}
//老师的版本：
/* CCA */
#include<stdio.h>
#include<math.h>

main()
{
   unsigned int a, b;
   int  abit[64], bbit[64], n_bit;
   int sum[64], sum1[64], cin0[64], cout0[64], cin1[64], cout1[64];
   float result;
   int numtest, test, i, j,x,y, done, time, diff;

   printf("number of tests =");  scanf("%d", &numtest);

  for(n_bit=1;n_bit<=48;n_bit++){   
   result=0.0;
   for(test=0;test<numtest;test++){
     for(i=0;i<64;i++){
	 sum[i]=0;cin0[i]=0;cout0[i]=0;cin1[i]=0;cout1[i]=0;
       }
     cin0[0]=1;
     a=lrand48(); b=lrand48();  x=a;y=b;  /* 31-bit rand */
     for(i=0;i<31;i++){         /* extract bits from a and b */
       abit[i]=x%2; bbit[i]=y%2;
       x=x/2; y=y/2;
     }
     a=lrand48(); b=lrand48();  x=a;y=b;  /* 31-bit rand */
       for(i=31;i<62;i++){         /* extract bits from a and b */
       abit[i]=x%2; bbit[i]=y%2;
       x=x/2; y=y/2;
     }
     abit[62]=lrand48()%2; bbit[62]=lrand48()%2; /* MSB generated seperately */
     abit[63]=lrand48()%2; bbit[63]=lrand48()%2; /* MSB generated seperately */
     /*
     for(i=n_bit-1;i>=0;i--)
       printf(" %d",abit[i]);
     printf("\n");
     for(i=n_bit-1;i>=0;i--)
       printf(" %d",bbit[i]);
     printf("\n");
     */
     done=0; time=0;
     while(done==0){  /* per cycle, check if done */

       if(time!=0){ /* not 1st cycle */
	 i=0; done=1;  /* check if carry completion*/
	 while((done==1) && (i<n_bit)){
	   if(cout0[i]==0 && cout1[i]==0)  done=0;
	   i++;
	 }
       }

       for(i=0;i<n_bit;i++){  	/* calculate new sums and new carries */
	   cout0[i]=(!abit[i]&&!bbit[i]) || ((abit[i]^bbit[i])&& cin0[i]);
	   cout1[i]=(abit[i]&&bbit[i]) || ((abit[i]^bbit[i])&& cin1[i]);
	   sum[i]= abit[i] ^ bbit[i] ^ cin1[i];
       }
       /*
       printf("=======================================\n");
       printf("  ");
       for(i=n_bit-1;i>=0;i--)
	 printf("%d ",cin0[i]);
       printf("\n");
       printf("  ");
       for(i=n_bit-1;i>=0;i--)
	 printf("%d ",cin1[i]);
       printf("\n");
       for(i=n_bit-1;i>=0;i--)
	 printf("%d ",cout0[i]);
       printf("\n");
       for(i=n_bit-1;i>=0;i--)
	 printf("%d ",cout1[i]);
       printf("\n");
       printf(" ");
       for(i=n_bit-1;i>=0;i--)
	 printf("%d ",sum[i]);
       printf("\n");
       */

       for(i=1;i<n_bit;i++){
	 cin0[i]=cout0[i-1]; cin1[i]=cout1[i-1]; 
       }
       time++;
     }
     result=result+time;
   }
   printf("n_bit=%2d delay=%f\n", n_bit, result*2/numtest);
 }

 }*/
