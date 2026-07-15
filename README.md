# DAG-NN-CPP 🧠

A lightweight, zero-dependency Neural Network framework built from scratch in C++. 

Instead of being limited to basic sequential models, **DAG-NN-CPP** implements a **Directed Acyclic Graph (DAG) Computational Graph**. This allows tensors to branch out into multiple parallel sub-networks and merge back together, offering high flexibility for custom architectures.

> **Note:** This project is actively under development and continuously updated with new enhancements.

---

## 🚀 Key Features

* **DAG Computation Graph:** Support for custom branching (`SplitNode`) and merging (`ConcatNode`) operations within the network.
* **Custom Matrix Engine:** A self-contained linear algebra library supporting dot product, transposition, slicing, and matrix mapping.
* **Smart Initialization:** Built-in **Xavier (Glorot) Initialization** to prevent vanishing/exploding gradients.
* **Safe Activations:** Numerical-clipping protected implementations of Tanh and Sigmoid.
* **Memory Safe:** Built using modern C++ smart pointers (`std::shared_ptr`, `std::weak_ptr`) to safely manage graph nodes.
* **Integrated Data Loading:** Comes equipped with a custom CSV reading parser (`DataSet`) to load datasets directly.

---

## 📁 Directory Structure

```text
.
├── include/                   # Header files (.hpp)
│   ├── Matrix.hpp             # Custom Matrix operations
│   ├── Node.hpp               # Abstract Node and InputNode classes
│   ├── DenseLayer.hpp         # Fully connected layers with activations
│   ├── SpecialNodes.hpp       # Split & Concat graph nodes
│   └── readCSV.hpp            # Dataset loading utility
├── src/                       # Source files (.cpp)
│   ├── Matrix.cpp             # Matrix logic implementation
│   ├── readCSV.cpp            # CSV parser logic implementation
│   └── main.cpp               # Network definition, training loop & UI
└── README.md
```

🏗️ Computational Graph Architecture
The default topology defined in main.cpp demonstrates the flexibility of the DAG model:

```Plaintext
       [ Input (1) ]
             │
      [ Hidden_1 (3) ]
             │
      [ Hidden_2 (4) ]
             │
      ┌──────┴──────┐
 [Split_1]     [Split_2]       <-- SplitNode slices matrices
     │             │
[Hidden_3_1]  [Hidden_3_2]     <-- Parallel processing
     │             │
      └──────┬──────┘
       [Concat (4)]            <-- ConcatNode merges matrices
             │
        [Output (1)]           <-- Output Layer (Sigmoid)
```

💻 Getting Started
Prerequisites
A C++ compiler supporting C++17 or higher (e.g., g++ or clang++).

Compilation
Run the following command from the root directory:

```Bash
g++ -std=c++17 -Iinclude src/main.cpp src/Matrix.cpp src/readCSV.cpp -o DagNN
```
Running the Project
```Bash
./DagNN
```
The network will automatically train on a generated non-linear dataset for 10,000 epochs, displaying real-time loss reduction. Once trained, you can interactively input numbers and get predictions.

📄 License
This project is licensed under the MIT License - see the LICENSE file for details.
