bool isPrime(int n) {
  if(n==1|| n==2||n==3||n==7||n==61)  return (n!=1);
  int r=0,d=n-1;
  while(!(d&1))
    r++,d>>=1;  
  vector<int> a={2,7,61};  // 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,37 for ull
  for(int i=0;i<a.size();i++)
  {
    long long x=expo(a[i],d,n);
    if(x==1||x==n-1)  continue;
    for(int j=1;j<r;j++)
    {
      x=(x*x)%n;
      if(x==1)  return false;
      if(x==n-1)  break;
    }
    if(x==n-1)  continue;
    else  return false;
  }
  return true;
}
