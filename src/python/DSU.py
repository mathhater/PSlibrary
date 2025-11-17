class DSU:
    def __init__(self, N):
        self.parent = list(range(N + 1))
        self.rank = [0] * (N + 1)

    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, x, y):
        xx, yy = self.find(x), self.find(y)
        if xx == yy:
            return False
        if self.rank[xx] < self.rank[yy]:
            self.parent[xx] = yy
        else:
            self.parent[yy] = xx
            if self.rank[xx] == self.rank[yy]:
                self.rank[xx] += 1
        return True