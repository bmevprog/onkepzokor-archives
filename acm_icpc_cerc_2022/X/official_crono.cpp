#include <iostream>
using namespace std;

int main() {
	int t;
	string s;
	cin >> t;
	getline(cin,s);
	while (t--) {
		getline(cin,s);
		int r=0;
		for (int i=0;i<s.size();i++) {
			if (s[i]=='I') r+=1;
			if (s[i]=='V') r+=5;
			if (s[i]=='X') r+=10;
			if (s[i]=='L') r+=50;
			if (s[i]=='C') r+=100;
			if (s[i]=='D') r+=500;
			if (s[i]=='M') r+=1000;
		}
		cout << r << endl;
	}
	return 0;
}
