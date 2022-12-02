from dataclasses import dataclass
from typing import Optional, List
import sys
from time import sleep
import math

sys.setrecursionlimit(10**6)


@dataclass
class Segment:
    weight: int
    full_weight: Optional[int]
    children: List[int]


@dataclass
class Tree:
    tree: List[Segment]

    def cut(self, w: int) -> int:
        stack = [(0, False)]
        cuts = 0
        while stack:
            i, d = stack.pop()
            if d:
                branches = [self.tree[j].full_weight for j in self.tree[i].children]
                branches.sort()
                full = sum(branches)
                while full > w:
                    cuts += 1
                    full -= branches.pop()
                c = (full + self.tree[i].weight) // w
                cuts += c
                self.tree[i].full_weight = self.tree[i].weight + full - w * c
                continue
            stack.append((i, True))
            stack += [(j, False) for j in self.tree[i].children]
        return cuts + (self.tree[0].full_weight > 0)


def read() -> Tree:
    result = Tree([])
    q = [[None, 1]]
    while q:
        i, n = q[-1]
        if n == 0:
            q.pop()
            continue
        if i is not None:
            result.tree[i].children.append(len(result.tree))
        q[-1][1] -= 1
        m, n = map(int, input().split())
        q.append([len(result.tree), n])
        result.tree.append(Segment(m, None, []))
    return result


def main():
    w = int(input())
    assert 1 <= w <= 10**9
    tree = read()
    assert 1 <= sum(t.weight for t in tree.tree) <= 10**9
    assert 1 <= len(tree.tree) <= 10**5

    print(tree.cut(w))


if __name__ == "__main__":
    main()
