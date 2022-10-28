// Az alábbi megoldásért köszönet az Árvíztűrő tükörfúrógép csapatnak! :)

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int hossz=0;
int bemenhossz=0;

int current [20];
int cel [26];

bool joe()
{
    for (int i = hossz-1;i>=0;i--)
    {
        int sum=0;
        for (int j=i;j<hossz;j++)
        {
            sum+=current[j]*current[hossz-(j-i)-1];
            sum%=10;
        }
        if (sum!=cel[i+hossz-1])
        {
            return false;
        }
    }
    return true;
}

bool rek (int index)
{
    //cout<<"a";
    bool vanjo=false;
    int sum=0;
    for (int i = 1;i<index;i++)
    {
        sum+=(current[i]*current[index-i])%10;
    }
    sum%=10;
    int a=current[0];
    int k=cel[index]-sum;
    if(k<0)
    {
        k+=10;
    }
    //------------
    for (int i = 0; i <10;i++)
    {
        if ((2*a*i)%10==k)
        {
            //cout<<"O"<<i<<"O";
            current[index]=i;
            if (index==hossz-1)
            {
                if (joe())
                {
                    vanjo=true;
                    break;
                }
            }
            else
            {
                if (rek(index+1))
                {
                    vanjo=true;
                    break;
                }
            }
        }
    }
    return vanjo;
}
//123476544

int main()
{
    string a;
    cin>>a;
    bemenhossz=a.size();
    if (bemenhossz%2==0)
    {
        cout<<-1;
    }
    else
    {
        hossz=bemenhossz/2+1;

        for (int i = 0; i <a.size();i++)
        {
            cel[i]=a[i]-'0';
        }

        bool joe=false;

        for (int i = 0; i <10;i++)
        {
            if ((i*i)%10==cel[0])
            {
                current[0]=i;
                if (hossz==1)
                {
                    joe=true;
                    break;
                }
                if (hossz>1 && rek(1))
                {
                    joe=true;
                    break;
                }
            }
        }

        if (joe)
        {
            for (int i = 0; i <hossz;i++)
            {
                cout<<current[i];
            }
        }
        else
        {
            cout<<-1;
        }

    }
    /*
    for (int i = 0; i <hossz;i++)
    {
        cout<<current[i];
    }
    */
    return 0;
}
