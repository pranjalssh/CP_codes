int l,r;cin>>l>>r;
    bool S[r-l+1];fill(S,0);
	if(l==1)S[0]=1;
   	tr(p,it)
   	{
   		if(*it>r)break;
   		if(*it>=l)cout<<*it<<"\n";
   		j=(l/(*it))*(*it);
   		if(j<l)j+=*it;
   		if(((*it-l)<=r) && ((*it-l)>=r))S[*it-l]=1;
   		for(;j<=r;j+=*it)  S[j-l]=1;
   	}
   	FOR(i,0,r-l)
   	if(!S[i])cout<<i+l<<"\n";