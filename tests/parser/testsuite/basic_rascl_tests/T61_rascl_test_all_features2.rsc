float a;
float g[10][5];
int b, c;
int d[5][10];

{
  a = 5.;
  b = 1;
  if (a == b)
  {
    d[a] = 5;
    c = a + -b;
  }
  else
  {
    while (b < 5) {
        c = -a * b;
	b = b + 1;
    };
  };
  print c;
}
