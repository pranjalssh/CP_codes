//彩色数だけ求めたい時は本当はpriority_queue必要ない
int coloringIntervals(vpii v) {
	sort(all(v));
	priority_queue<int,vi,greater<int> > q;
	int r = 0;
	each(i, v) {
		if(q.empty() || i->first < q.top()) {
			r ++;
			q.push(i->second);
		}else {
			q.pop();
			q.push(i->second);
		}
	}
	return r;
}
