# Box-Stacker

An efficient, high-performance algorithm solving the **3D Box Stacking Optimization Problem** under strict 2D base stability constraints, developed for the **Analysis and Synthesis of Algorithms (ASA)** course in the BSc in Computer Science and Engineering (LEIC-A) at **Instituto Superior Técnico** (2026/2027).

---

## 📌 Problem Overview

Given $n$ distinct types of 3D rectangular blocks (paralelepipeds) with dimensions $(w, l, h)$ available in unlimited supply, the objective is to build the highest possible stable tower.

### Constraints & Rules
- **Rotations:** Any block can be rotated in 3D space, producing up to 3 unique base orientations $(w, l)$ with corresponding height $h$.
- **Strict Stability Constraint:** A block can only be placed on top of another if **both** dimensions of its base are strictly smaller than the base of the block directly beneath it:
  $$\text{width}_{\text{top}} < \text{width}_{\text{bottom}} \quad \text{and} \quad \text{length}_{\text{top}} < \text{length}_{\text{bottom}}$$
- **Objective:** Maximize total tower height $H = \sum h_i$, while tracking and outputting the full sequence of blocks from bottom to top.

---

## 🚀 Algorithm & Optimization ($O(N \log N)$)

While a standard Dynamic Programming approach (longest path in a DAG / LIS variant) yields $O(N^2)$ time complexity, this implementation scales to millions of blocks using an **$O(N \log N)$** algorithm:

1. **Orientation Expansion:** Each block type produces 3 distinct rotations with normalized dimensions ($w \le l$).
2. **Multi-Key Sorting:** Blocks are sorted by `width` ascending. For ties in width, they are sorted by `length` descending to strictly enforce the condition $w_{\text{top}} < w_{\text{bottom}}$ without allowing blocks of identical width to stack on each other.
3. **Coordinate Compression:** The discrete values of block lengths are mapped into a contiguous index range $[0, U-1]$ using `std::unique` and binary search (`std::lower_bound`).
4. **Segment Tree Range Maximum Query (RMQ):** A Segment Tree maintains the maximum achievable tower height and block indices for each compressed length coordinate. For each block, querying the range $[0, \text{pos}(l)-1]$ yields the optimal lower block in $O(\log N)$ time.
5. **Backtracking / Reconstruction:** State arrays store the optimal previous block index and chain count, allowing $O(M)$ reconstruction of the final tower.

### Theoretical Complexity
- **Time Complexity:** $\mathcal{O}(N \log N)$ — sorting $3n$ boxes and performing $3n$ queries/updates on the Segment Tree.
- **Space Complexity:** $\mathcal{O}(N)$ — linear auxiliary memory for box states, coordinate mappings, and tree nodes.

---

## 📂 Project Structure

```text
.
├── project.cpp                 # Main C++ source code (Segment Tree + DP solution)
├── gerador.cpp                 # C++ instance generator for stress/random testing
├── gerador.py                  # Python benchmark script for empirical complexity validation
├── relatorio.pdf               # Theoretical analysis & experimental evaluation report
└── README.md                   # Project documentation
```

---

## 🛠️ Build & Execution

### Compilation
The solution is implemented in standard C++ and optimized using `-O3`:

```bash
g++ -std=c++11 -O3 -Wall project.cpp -lm -o project
```

### Running

The program reads from standard input (`stdin`) and writes to standard output (`stdout`):

```bash
./project < input.txt
```

#### Input Format
```text
n
w1 l1 h1
w2 l2 h2
...
```

#### Output Format
```text
<total_height> <num_blocks>
<base_width> <base_length> <height>
...
<top_width> <top_length> <height>
```

---

## 📊 Experimental Evaluation

The repository includes `gerador.py`, which generates synthetic problem instances ranging from $10^4$ up to $3 \times 10^6$ blocks to benchmark execution times and verify that experimental performance matches the theoretical $\mathcal{O}(N \log N)$ bound.

---

## 👥 Authors
- **Ricardo Vicente** ([@ricardomvicente](https://github.com/ricardomvicente))
- Project developed for **Análise e Síntese de Algoritmos (ASA)** — Instituto Superior Técnico.