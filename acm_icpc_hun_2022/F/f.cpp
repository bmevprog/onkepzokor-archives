// Az alábbi megoldásért köszönet a エンジニア csapatnak! :)

#include <iostream>
#include <vector>

int main()
{
	int n, k;
	std::cin >> n >> k;
	double dk, dn;
	dn = (double)n;
	dk = (double)k;
	std::vector<double> s(n);
	for (int i = 0; i < n; i++) {
		std::cin >> s[i];
		s[i] /= 100;
	}
	double chance;
	double chance_sum;
	double s_sum;
	for (int i = 0; i < n; i++) {
		chance = dk / dn;
		chance_sum = chance * s[i];
		s_sum = s[i];
		for (int j = i - 1, l = 0; j != i && n - k - l>0; j--,l++) {
			if (j < 0) j = n-1;
			s_sum += s[j];
			chance *= (dn - dk - l) / (dn - 1 - l);
				chance_sum += chance * s[j];
		}
		printf("%.48lf ", chance_sum * 100);
	}
	return 0;
}
