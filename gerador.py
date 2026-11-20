import random, time, subprocess, os


testes = [
    10000, 25000, 50000, 100000, 200000, 
    250000, 500000, 750000, 1000000, 1250000, 
    1500000, 1750000, 2000000, 2500000, 3000000
]

print(f"{'N':<12} | {'Tempo (s)':<10}")
print("-" * 25)

for n in testes:
    
    with open("input.txt", "w") as f:
        f.write(f"{n}\n")
        for _ in range(n):
            w = random.randint(1, 1000)
            l = random.randint(1, 1000)
            h = random.randint(1, 1000)
            f.write(f"{w} {l} {h}\n")
    
    
    inicio = time.time()
    
    subprocess.run(["./a.out"], stdin=open("input.txt", "r"), stdout=subprocess.DEVNULL)
    fim = time.time()
    
    print(f"{fim - inicio:.4f}s")


if os.path.exists("input.txt"):
    os.remove("input.txt")