  int n;
  cin >> n;
  string s;
  cin >> s;

  vi odd(n), even(n);
      
  int l, r;
  // Odd case
  l = r = -1;
    for (int i = 0; i < n; i++) {
        int cur = 1;
        if (i < r)
            cur = min(r - i + 1, odd[l + r - i]);
        while (i + cur < n && i - cur >= 0 && s[i - cur] == s[i + cur])
            cur++;
        odd[i] = cur;
        if (i + cur - 1 > r) {
            l = i - cur + 1; 
            r = i + cur - 1;
        }
    }

    // Even case 
    l = r = -1;
    for (int i = 0; i < n; i++) {
        int cur = 0;
        if (i < r) 
            cur = min(r - i + 1, even[l + r - i + 1]);
        while (i + cur < n && i - 1 - cur >= 0 && s[i - 1 - cur] == s[i + cur])
            cur++;
        even[i] = cur;
        if (i + cur - 1 > r) {
            l = i - cur;
            r = i + cur - 1;
        }
    }