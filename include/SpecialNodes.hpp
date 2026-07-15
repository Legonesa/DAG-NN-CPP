#ifndef SPECIAL_NODES_HPP
#define SPECIAL_NODES_HPP

#include "Node.hpp"

// ==========================================
// 1) SPLIT NODE
// ==========================================
// A structural node used to branch the computational graph.
// Forward: Extracts a sub-matrix from its parent's output.
// Backward: Scatters the accumulated gradient back to the correct offset in the parent's gradient cache.
class SplitNode : public Node {
private:
    size_t rowOffset, colOffset;
    size_t rowsToTake, colsToTake;

public:
    SplitNode(size_t r_off, size_t c_off, size_t r_len, size_t c_len,
              std::string name = "Split")
        : Node(name),
          rowOffset(r_off), colOffset(c_off),
          rowsToTake(r_len), colsToTake(c_len) {}

    // Extracts the requested slice from the parent matrix
    void forward() override {
        if (parents.empty()) return;

        const Matrix& in = parents[0]->getOutput();
        outputCache = in.slice(rowOffset, colOffset, rowsToTake, colsToTake);
    }

    // Maps local gradients back to the original full-sized parent gradient matrix
    void backward() override {
        if (parents.empty()) return;

        NodePtr parent = parents[0];

        const Matrix& pOut = parent->getOutput();
        Matrix scatter(pOut.rows, pOut.cols);

        // Place gradients exactly where they were sliced from
        for (size_t i = 0; i < rowsToTake; i++) {
            for (size_t j = 0; j < colsToTake; j++) {
                scatter.at(rowOffset + i, colOffset + j) += gradientCache.at(i, j);
            }
        }

        parent->addGradient(scatter);
    }
};


// ==========================================
// 2) CONCAT NODE
// ==========================================
// A structural node used to merge branches in the computational graph.
// Forward: Concatenates matrices from all parent nodes along the specified axis.
// Backward: Slices the incoming accumulated gradient and distributes it back to each parent.
class ConcatNode : public Node {
private:
    int axis; // 0 for vertical concatenation, 1 for horizontal

public:
    ConcatNode(int axis = 0, std::string name = "Concat")
        : Node(name), axis(axis) {}

    // Merges parent outputs dynamically
    void forward() override {
        if (parents.empty()) return;

        Matrix result = parents[0]->getOutput();

        // Iteratively append each parent's output matrix
        for (size_t i = 1; i < parents.size(); i++) {
            result = result.concatenate(result, parents[i]->getOutput(), axis);
        }

        outputCache = result;
    }

    // Splits the accumulated gradient and routes segments back to corresponding parents
    void backward() override {
        if (parents.empty()) return;

        const Matrix& totalGrad = gradientCache;

        size_t offset = 0;

        for (auto& parent : parents) {
            const Matrix& pOut = parent->getOutput();
            size_t pRows = pOut.rows;
            size_t pCols = pOut.cols;

            Matrix subGrad(0, 0);

            // Slice gradients dynamically based on parent output sizes
            if (axis == 0) {
                subGrad = totalGrad.slice(offset, 0, pRows, pCols);
                offset += pRows;
            } else {
                subGrad = totalGrad.slice(0, offset, pRows, pCols);
                offset += pCols;
            }

            parent->addGradient(subGrad);
        }
    }
};

#endif
