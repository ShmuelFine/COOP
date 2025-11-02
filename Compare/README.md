# ⚙️ COOP Benchmarks Guide

> Official guide for building and running COOP benchmark measurements on Linux / WSL.

This document explains how to **build**, **run**, and **measure performance** for the COOP data-structure benchmarks:

- `coop_bench_binary_tree.c`
- `bench_list_ops.c`
- `coop_bench_queue.c`

> The project folder should be located in Linux.

All located under:
```
~/COOP/Compare/benchmarks/coop
```

---

##  1. Prerequisites

Install required packages:

```bash
sudo apt update
sudo apt install -y build-essential cmake time binutils   linux-tools-common linux-tools-generic
```

---

## 2. Build the Project (Release Mode)
#### ⚙️ MEM_SIZE_T (Macro) Configuration
COOP controls its internal size/index type via a **compile-time macro** MEM_SIZE_T.

Default in source:

```c
#ifndef MEM_SIZE_T
#define MEM_SIZE_T unsigned short
#endif
```
For example, to build COOP with a wider type int:

CMake output will confirm the selection:
```c
COOP: MEM_SIZE_T is set to 'int' (MEM_SIZE='int')
```
Options:

- MEM_SIZE=ushort → MEM_SIZE_T=unsigned short (default)

- MEM_SIZE=int → MEM_SIZE_T=int (for large datasets)

Important:

After changing the MEM_SIZE value, perform a clean build (delete the build/ directory) to ensure all files recompile correctly.


When running COOP benchmarks on Windows (Visual Studio),
execute the benchmark .exe directly

Example:
```c
out/build/x64-Debug/Compare/benchmarks/coop/coop_bench_queue.exe
```

Run: benchmark/UnitTestC .exe (e.g., coop_bench_queue.exe, UnitTestC.exe)

---

#### Construction for GObject / Pure C / OOC See instructions in section 6

From your COOP root folder:

```bash
cd ~/COOP
rm -rf build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
```

Binaries are created at:
```
~/COOP/build/Compare/benchmarks/coop/
```

| Benchmark | Executable |
|------------|-------------|
| Binary Tree | `coop_bench_binary_tree` |
| List        | `bench_list_ops` |
| Queue       | `coop_bench_queue` |

---

## 3. Running Benchmarks

Move into the benchmarks folder:

```bash
cd ~/COOP/build/Compare/benchmarks/coop
```

Each program accepts:
```
./<binary_name> <phase> <N>
```

| Benchmark | phase |
|------------|-------------|
| Binary Tree | `insert, remove, size, empty, all` |
| List        | `push_back, push_front, pop_back, _pop_front, size, empty, front, clear, all` |
| Queue       | `enqueue, dequeue, front, front_cref, size, empty, clear, all` |

---

- `<N>` → number of elements (e.g., `1000000`)
-  The runtime of `remove` and `all` in the binary tree is very long, it is recommended to measure up to 10,000.

### Examples:
```bash
./bench_list_ops all 1000000
./coop_bench_binary_tree all 10000
./coop_bench_queue all 1000000
```

---

## 4. Measuring Performance

### CPU Cycles, Instructions, IPC, Wall-Clock (Perf)

Run each benchmark 5 times:

```bash
perf stat -r 5 -e cycles,instructions ./<binary_name> <phase> <N>
```

Examples:
```bash
perf stat -r 5 -e cycles,instructions ./coop_bench_binary_tree insert 1000000
```

### Wall Time, Peak RAM, Page faults, CPU

```bash
/usr/bin/time -v ./<binary_name> <N>
```
Examples:
```bash
/usr/bin/time -v ./coop_bench_binary_tree 10000
```

Look for:
- `Elapsed (wall clock) time`
- `Maximum resident set size (kbytes)`
- `Minor (reclaiming a frame) page faults` 
- `Percent of CPU this job got`

---

## 5. Measuring Binary Size

Check executable size before and after `strip`:

```bash
du -b ./<binary_name>
cp ./<binary_name> ./<binary_name>.stripped
strip ./<binary_name>.stripped
du -b ./<binary_name>.stripped
```
Examples:
```bash
du -b ./bench_list_ops
cp ./bench_list_ops ./bench_list_ops.stripped
strip ./bench_list_ops.stripped
du -b ./bench_list_ops.stripped
```

## 


---

## 6. Building and Running Other Implementations (GObject / Pure C / OOC)

> The measurement procedure is identical to COOP (sections 3–5).  
> The only difference is in **how the binaries are built** and **where they are located**.

---

### GObject Version 

####  Prerequisites (Ubuntu/WSL)
Install GLib development headers and build tooling:
```bash
sudo apt update
sudo apt install -y build-essential pkg-config libglib2.0-dev
```
> `libglib2.0-dev` includes **glib-2.0** and **gobject-2.0** headers & libs.

---

####  Create a new project (outside COOP)
```bash
mkdir -p ~/gobj-gqueue-bench/src
cd ~/gobj-gqueue-bench
```

Copy the benchmark source **into** `src/`:
```bash
cp ~/COOP/Compare/benchmarks/gobject/bench_gqueue_ops.c ./src/
```

> After this, you should have:
> ```
> ~/gobj-gqueue-bench/
>   └─ src/
>      └─ bench_gqueue_ops.c
> ```

---

#### Build with GCC + pkg-config

```bash
mkdir -p build && cd build
gcc ../src/bench_gqueue_ops.c ../src/gqueue_gobject.c \
  $(pkg-config --cflags --libs glib-2.0 gobject-2.0) \
  -O3 -o bench_gobject.out
```
##

### Pure C Version

**Source location:**
```
~/COOP/Compare/benchmarks/pure_c
```

**Build example:**
```bash
cd ~/COOP/Compare/benchmarks/pure_c
gcc pure_c_bench_binary_tree.c ../../Pure_C/bt_pure.c     -I../../Pure_C -o bench_pure_c.out
```

##

### OOC Version

**Prerequisites (WSL/Ubuntu):**
```bash
sudo apt update
sudo apt install -y build-essential autoconf automake libtool pkg-config \
  texinfo wget tar
# For building PDFs/HTML docs (optional):
# sudo apt install -y texlive-latex-recommended texlive-fonts-recommended texlive-latex-extra
```

**Install upstream OOC :**
```bash
cd ~
wget https://master.dl.sourceforge.net/project/ooc-coding/source/ooc-1.3c.tar.gz
tar -xvf ooc-1.3c.tar.gz
cd ooc-1.3c

./configure
make all pdf html
make check
sudo make install install-pdf install-html
sudo ldconfig
```

> After install, headers are typically under `/usr/local/include/ooc/` and the library under `/usr/local/lib/`.

**Build benchmark (linking installed OOC):**
```bash
cd ~/COOP/Compare/benchmarks/ooc
gcc ooc_bench_queue.c ../../OOC/Queue_ooc.c \
    -I/usr/local/include/ooc -L/usr/local/lib -looc \
    -o ooc_bench_queue.out
```


---

### Notes
- After building each version, follow **the same measurement steps** as in sections 3–5 (Perf, `/usr/bin/time`, and binary size).  
- All results can be compared directly in the same summary table.  
- Ensure that you run all binaries under **the same environment** (same machine, same load).

---



