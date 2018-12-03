struct new_q
{
	deque<int> d,mn;
	void Push(int x) { d.pb(x); while( (!mn.empty()) && mn.back()>x) mn.pop_back();mn.push_back(x);}
	void Pop(){if(mn.front()==d.front()) mn.pop_front(); d.pop_front();}
	int chota(){return mn.front();}
};