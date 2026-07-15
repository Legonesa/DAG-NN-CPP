#ifndef SPECIAL_NODES_HPP
#define SPECIAL_NODES_HPP

#include "Node.hpp"

// ==========================================
// 1) SPLIT NODE
// ==========================================
// Forward: parent output'un bir parçasını slice edip verir.
// Backward: kendisine gelen küçük gradient'i, parent'ın gradientine
// doğru offset'e yerleştirip (scatter) EKLER.
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

    void forward() override {
        if (parents.empty()) return;

        const Matrix& in = parents[0]->getOutput();
        outputCache = in.slice(rowOffset, colOffset, rowsToTake, colsToTake);
    }

    void backward() override {
        if (parents.empty()) return;

        NodePtr parent = parents[0];

        const Matrix& pOut = parent->getOutput();
        Matrix scatter(pOut.rows, pOut.cols);

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
// Forward: parent'lardan gelen matrisleri axis boyunca birleştirir.
// Backward: kendisine gelen gradient'i, parent output boyutlarına göre slice edip
// her parent'a geri dağıtır.
class ConcatNode : public Node {
private:
    int axis;

public:
    ConcatNode(int axis = 0, std::string name = "Concat")
        : Node(name), axis(axis) {}

    void forward() override {
        if (parents.empty()) return;

        Matrix result = parents[0]->getOutput();

        for (size_t i = 1; i < parents.size(); i++) {
            result = result.concatenate(result, parents[i]->getOutput(), axis);
        }

        outputCache = result;
    }

    void backward() override {
        if (parents.empty()) return;

        const Matrix& totalGrad = gradientCache;

        size_t offset = 0;

        for (auto& parent : parents) {
            const Matrix& pOut = parent->getOutput();
            size_t pRows = pOut.rows;
            size_t pCols = pOut.cols;

            Matrix subGrad(0, 0);

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
