# A - Airline

An airline company offers regular flights involving $n$ different airports. Each flight links two airports directly (i.e. without stopping at any other airport) and allows travel in both directions. The flights are arranged such that for any choice of starting airport $s$ and destination airport $t$, there exists exactly one sequence of flights between the two airports without visiting any airport more than once. The number of flights in this sequence is called the _distance_ between $s$ and $t$.

Were the airline to add another flight, say between airports $x$ and $y$, it is possible that for some pairs $(s,t)$, another, shorter sequence of flights from $s$ to $t$ would form. The more pairs affected, the more promising the new connection between $x$ and $y$ is considered to be. The airline is asking you to help them evaluate several possible additions $(x,y)$ with respect to this criterion.

## Input data

The first line contains two integers, $n$, the number of airports, and $q$, the number of possible additions $(x,y)$ that are to be evaluated.

The next $n-1$ lines describe the original flights before any additions. The $i$-th of these lines contains two integers $u_i$ and $v_i$, indicating that there is a direct flight connection between airports $u_i$ and $v_i$.

The remaining $q$ lines describe the possible additional flights that are being considered. The $i$-th of these lines contains two integers $x_i$ and $y_i$, indicating that in the $i$-th scenario the original $n-1$ flights would be supplemented by a new direct flight connection between airports $x_i$ and $y_i$.

### Input limits

- $2\leq{}n\leq{}10^6$
- $1\leq{}q\leq{}10^5$
- $1\leq{}u_i\leq{}n$ and $1\leq{}v_i\leq{}n$ and $u_i\neq{}v_i$
- $1\leq{}x_i\leq{}n$ and $1\leq{}y_i\leq{}n$ and $x_i\neq{}y_i$
- $\sum\limits_{i=1}^{q} d_i \leq{} 10^7$, where $d_i$ is the distance between $x_i$ and $y_i$ in the original flight network.
