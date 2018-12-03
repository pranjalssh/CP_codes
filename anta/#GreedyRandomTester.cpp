//入力値の範囲・制約はきちんとコーディングすること
//明らかなコーナーケースは人力で試したほうが良い

int test() {
	vi v;
	rep(i, 1000) {
		//vectorのサイズを設定する
		int n = rand()%50+1;
		// ここで入力値を生成する。0などコーナー値の確率をあげても良い
		rep(j, n) v.pb(rand()%2 ? (rand()%3-1) : rand()%2000-1000);
		//解答の値
		ll a = maximalSum(v);
		rep(j, 1000) {
			random_shuffle(all(v));
			int k = 0;
			ll t = 0;
			vi h;
			//ここで何らかのランダム解を作る。
			//hにそのランダムの履歴を入れておく
			while(k < n) {
				if(rand()%2 && k+1 < n) t += v[k]*v[k+1], h.pb(2), k += 2;
				else t += v[k], h.pb(1), k ++;
			}
			//lt/gtに注意
			if(t > a) {
				cout<<"NG: ";each(l, v) cout<<*l<<",";cout<<endl;
				cout<<"hist: ";each(l, h) cout<<*l<<",";cout<<endl;
				cout<<a<<", "<<t<<endl;
				goto ng;
			}
		}
		cout<<"OK: ";each(l, v) cout<<*l<<",";cout<<endl;
		ng:;
	}
}

