![](COOP%2logo_ST.png)


**COOP** stands for **C** **O**bject **O**riented **P**rogramming framework, developed by FineALGs (now part of [Extra.Tech](https://extra.tech/)) with real time embedded systems in mind, 
along with our students from the Codalleh program (now part of [KamaTech](https://www.kamatech.org.il/))

COOP implements object-oriented programming in standard C through a macro-based layer. It provides **classes**, **inheritance** (with virtual functions), **exception with automatic destructor unwinding**, **memory management**, our own **unit-testing framework**, and data-structure implementations designed to remain compatible to static analysis tools and comply with MISRA guidelines—while preserving type safety and IntelliSense support in mainstream IDEs.

It was tested on Windows (Visual Studio/MSBuild) and Linux (GCC). COOP aims to supply expressive, safe abstractions with minimal runtime overhead so you can adopt it by simply copying the `COOP` folder into your project and:

```cpp
#include "COOP.h"
```

You will find a usage cheat sheet in `The COOP Cheat Sheet.docx`, and the `UnitTestC` suite showcases idiomatic patterns.
## Object-Oriented Principles in COOP
- **Class construction macros** (`DEF_CLASS`, `END_DEF`, `FUNCTIONS`) generate a strongly typed struct together with constructor and destructor declarations and the associated method tables.
- **Inheritance and polymorphism** rely on `DEF_DERIVED_CLASS`, `DERIVED_FUNCTIONS`, and `FUN_OVERRIDE`, which expand into virtual table definitions and override-safe call-sites. Iterators (`Iterator.h`) and containers (`List`, `Queue`, `BinaryTree`, `Vector`, `Tensor`) demonstrate substitutability across hierarchies.
- **Deterministic lifetime management** uses constructor/destructor macros (`INIT_CLASS`, `END_FUNCTIONS`) and shared-ownership helpers (`SharedObjPtr`, `SharedPodPtr`) to express ownership semantics that mirror RAII.
- **Exception-safe semantics** are provided via COOP's `TRY`/`CATCH`/`FINALLY` macros. When an exception path is taken, the framework walks registered destructors, ensuring resources owned by the current scope are released.

## Core Data Structures
- **List (`GenericList`)**: a doubly linked list with configurable element size, type tags for diagnostics, and iterator support. Typed adapters (`List_int`, `List_float`, etc.) expose strongly typed push/pop/front/back methods while retaining the underlying generic buffer.
- **Queue (`GenericQueue`)**: thin compositional layer over `GenericList`, offering FIFO semantics, iterator access, constant-time front retrieval, and typed enqueue/dequeue helpers for primitive values and object smart pointers.
- **Binary Tree (`GenericBinaryTree`)**: pointer-based tree with configurable element type, pre/in/post-order traversals, removal semantics that preserve heap property, and an in-order iterator that supports bidirectional navigation.
- **Vector (`GenericVector`)**: contiguous dynamic array with manual capacity control, typed `at`/`get`/`set` accessors, iterator integration, and zeroing utilities geared toward cache-friendly workloads.
- **Tensor (`GenericTensor`)**: multi-dimensional façade built atop vectors and integer shape descriptors; supports coordinate-based indexing, reshape operations, and zeroing for multidimensional numeric workloads.

Each structure ships with dedicated unit tests under `UnitTestC`, demonstrating construction, mutation, traversal, and iterator behavior.

## Performance Summary
![](List.png) 
 illustrates that COOP's list implementation maintains lower operation latency than both the GLib/GObject queue and the OOC queue across the measured ranges, with the advantage widening on high-volume workloads thanks to contiguous node allocations and iterator reuse.

![](Queue.png)
 shows COOP queues sustaining steadier throughput than the OOC and GObject counterparts, especially for mixed enqueue/dequeue workloads, while keeping memory churn flat due to pooled nodes.

![](BinaryTree.png) 
 highlights near-parity with the pure C baseline, with COOP's macro-generated dispatch adding negligible overhead; insert/remove curves remain within the measurement noise envelope of the PUREC implementation.

 The benchmarking methodology, hardware notes, and reproduction steps are documented in [`COMPARE/README.md`](COMPARE/README.md). Raw datasets and additional comparisons live in `COMPARE/`.

## Safety and Compliance
- COOP passes the MISRA C:2012 checks used in our validation pipeline. Documented suppressions and justifications are bundled in [`MISRA_Compliance.txt`](MISRA_Compliance.txt) and summarized for auditors in [`MISRA_CMPLAINECE`](MISRA_CMPLAINECE).
- Deterministic resource handling, macro-driven include guards, and iterator contracts are designed to keep static-analysis noise low on safety-critical toolchains.

## Computer Vision Extensions
- The `OpenCV` directory packages a COOP-based image-processing toolkit that mirrors key OpenCV primitives (grayscale buffers, Sobel, Gaussian, thresholding, non-maximum suppression). A dedicated guide with module walkthroughs and integration notes is available in [`OpenCV/README.md`](OpenCV/README.md).
- Unit tests under `OpenCV/UnitTest` exercise GrayImage pipelines end to end, providing ready-to-run validation on sample BMP assets.

---

COOP aims to give lightweight syntax and minimal performance overhead for achieving efficient and **safe** embedded code. Have fun!
