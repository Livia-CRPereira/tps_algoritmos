import random, sys
H, W, Q, seed, mx = map(int, sys.argv[1:6])
random.seed(seed)
print(H, W)
for _ in range(H):
    print(*[random.randint(0, mx) for _ in range(W)])
print(Q)
for _ in range(Q):
    print(random.randint(1, H), random.randint(1, W), random.randint(1, H), random.randint(1, W))
