#include <bits/stdc++.h>

using LL = long long;

const int N = 2e5 + 7;

struct dat {
  LL al, fe;
  bool operator < (const dat &o) const {
    if((al >= fe) != (o.al >= o.fe))
      return (al >= fe) < (o.al >= o.fe);
    if(al >= fe)
      return fe > o.fe;
    return al < o.al;
  }
};

int n, fa[N];
LL a[N], b[N], m;
int set[N];
dat val[N];

dat merge(dat x, dat y) {
  return {std::max(x.al, x.al - x.fe + y.al),
	  std::max(x.al, x.al - x.fe + y.al) - (x.al - x.fe + y.al - y.fe)};
}

int find(int x) {
  return set[x] == x ? x : set[x] = find(set[x]);
}

void uni(int x, int y) {
  x = find(x), y = find(y);
  set[x] = y;
  val[y] = merge(val[x], val[y]);
}

int main() {
  scanf("%d", &n);
  for(int i = 1; i <= n; ++i)
    scanf("%lld", &a[i]);
  for(int i = 2; i <= n; ++i) {
    scanf("%d", &fa[i]);
    b[fa[i]] += a[i];
  }

  LL leaf = 0;
  for(int i = 1; i <= n; ++i)
    if(!b[i])
      leaf += a[i];

  std::priority_queue<std::pair<dat, int>> q;
  for(int i = 1; i <= n; ++i)
    if(b[i]) {
      set[i] = i;
      val[i] = {a[i], b[i]};
      if(i != 1)
	q.push({val[i], i});
    }

  while(!q.empty()) {
    auto [v, i] = q.top();
    q.pop();
    if(v.al != val[i].al || v.fe != val[i].fe || i != find(i) || i == 1)
      continue;
    uni(i, fa[i]);
    q.push({val[find(i)], find(i)});
  }

  printf("%lld\n", leaf + val[1].al);

  return 0;
}
