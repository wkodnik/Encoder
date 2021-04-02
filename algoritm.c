#include <algoritm.h>
#include <stdlib.h>
unsigned int16 summ(){
DISABLE_INTERRUPTS(GLOBAL);
// inits
   
 //  for(i=1;i<11;i++) x[i] =  500 + (int16)(1000.0*(rand()/(RAND_MAX+1.0)));
   for(i = 0; i < 20; i++){
      for(j = 0; j< 10; j++){
         arrnumber[i][j] = 0;   
      }
      arrsumm[i] = 0;
   }

//inits
   for(i=0;i<10;i++){
      a[i] = 0;
      buf[i] = 0;
   }

   max = 2000;
   n = 15;
   j =0;
   number = 0;   

   p=0; r=0; z=0; c=0;
   for(i = 0; i< n; i++){
      total_summa += x[i];
   }
   total_summa /= n;
   k = m/total_summa;
      
    while(p<= k-1)
	{
       s = 0;
       summa = 0;
       for(i=1;i<=k;i++) s += x[a[i]];

       if(s<=(m+50) && s>=(m-50))
	    {
            for(i=1;i<=k;i++)
			{ 
               buf[i] = a[i];   // esli kombinacija najdena to zapolnitsja buffer indeksami
            	if(j<20)
				{
            		arrsumm[j] +=x[a[i]]; 
               		arrnumber[j][i] = a[i];
            	}
				else
					goto m2;	// massiv zapolnen
            }
            j++;
            z++;
        }
      
         while (a[k-p]==n-p) p++;
         a[k-p]++;
         for(r=0;r<p;r++) a[k-p+r+1] = a[k-p+r]+1;
         if(p<=k-1) p=0;
    }

// poisk luchshego rezultata
m2:
    if(z)
	{
      int16 last_summa = 0;
      int8 last_number = 0;
      
      for(i = 0; i< 10; i++)
	    {
         if(arrsumm[i])
		 {

            if(arrsumm[i] > m)
			{
               summa = arrsumm[i] - m;
               number = i;
            }
            else if (arrsumm[i] < m){
               summa = m - arrsumm[i];
               number = i;
            }
            else{
               summa = arrsumm[i];
               number = i;
               break;
            }
            if(!last_summa) {
               last_summa = summa;
               last_number = number;
            }
            if(last_summa <= summa){
               summa = last_summa;
               number = last_number;
            }
            else
			{
               last_summa = summa;
               last_number = number;
            }
               
          }
         else
            break;
        }
      
   }
   
   ENABLE_INTERRUPTS(GLOBAL);
   return arrsumm[number];
}
