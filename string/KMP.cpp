vi fail(string s)
{
  int n = s.length();
  vi F(n + 1);
  FOR(i, 2, n)
  {
    int x = F[i - 1];
    while (1)
    {
      if (s[i - 1] == s[x])
      {F[i] = x + 1; break;}
      if (x == 0) {F[i] = 0; break;}
      x = F[x];
    }
  }
  return F;
}

int i = 0;
int j = 0;
bool f = 0;
while (1)
{
  if (j == t.length())break;

  if (s[i] == t[j])
  {
    i++;
    j++;
    if (i == s.length())
    {

    }
  }

  else if (i > 0)i = F[i];

  else j++;
}
