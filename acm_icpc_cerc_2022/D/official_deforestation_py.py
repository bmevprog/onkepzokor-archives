import sys
nPieces = 0

def Recurse():
    global nPieces, capacity
    L = sys.stdin.readline().split(); weight = int(L[0]); fanout = int(L[1])
    children = []; childSum = 0
    for i in range(fanout):
        child = Recurse()
        if child > 0: children.append(child); childSum += child
    if childSum > capacity:
        children.sort(reverse = True)
        for child in children:
            childSum -= child; nPieces += 1
            if childSum <= capacity: break
    weight += childSum; nPieces += weight // capacity; return weight % capacity
        
capacity = int(sys.stdin.readline())
sys.setrecursionlimit(100000 + 1234)
if Recurse() > 0: nPieces += 1
print(nPieces)
