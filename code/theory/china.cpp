\subsubsection{中国剩余定理（非互质)}
\begin{verbatim}
long long exgcd(long long a,long long b,long long &x,long long &y) {
	if (!a){
		x = 0;
		y = 1;
		return b;
	}
	LL g = exgcd(b % a, a, x, y);
	LL t = y;
	y = x;
	x = t - (b / a) * y;
    return g;
}

long long CRT(const vector<long long>& m,const vector<long long>& b,long long& lcm) {
    bool flag = false;
    long long x, y, i,d,result,a1,m1,a2,m2,Size=m.size();
    m1 = m[0]; a1 = b[0];
    for(i = 1; i < Size; ++i){
        m2 = m[i]; a2 = b[i];
        d = exgcd(m1, m2, x, y );
        if((a2-a1) % d != 0) flag = true;
        result = (x * ((a2-a1) / d ) % m2 + m2 ) % m2;
        a1 = a1 + m1 * result; //对于求多个方程
        m1 = (m1 * m2) / d;    //lcm(m1,m2)最小公倍数
        a1 = (a1 % m1 + m1) % m1;
    }
    lcm = m1;
    if (flag) return -1;
    else return a1;
}
\end{verbatim}
