#include <bits/stdc++.h>
using namespace std;

int main(){
   int n;
   cin>>n;
   int a=-1;
   vector<long double> numbers;
   for(int i=0;i<n;i++){
       long double in;
       cin>>in;
       numbers.push_back(in);
   }
   if(n==1){
       cout<< numbers[0];
       return 0;
   };
    
   // Divide by largest element in vector.
    long double max =  *max_element(numbers.begin(), numbers.end());
  
   // Start from i=1.
    for(int i=1;i<10001;i++){
        int j;
        long double ratio = i/max;
        for(j=0;j<n;j++){
            long double error = 1e-6;
            long double diff = roundl(numbers[j] * ratio)-(numbers[j] * ratio);
          
            if(diff > error || diff < -error){
                break;
            }
        }
        if(j==n){
            a=i;
            break;
        }     
    }
    for(int i=0;i<n;i++){
        cout<<round(numbers[i]/max*a) <<endl;
    }
 
    return 0;
}