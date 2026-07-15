#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Matrix.hpp"

class Node;

// Smart pointers to safely manage graph hierarchy and prevent memory leaks
using NodePtr = std::shared_ptr<Node>;
using WeakNodePtr = std::weak_ptr<Node>;

// Base abstract class representing a node in the computational graph
class Node : public std::enable_shared_from_this<Node> {
    protected:
        std::string name;
        std::vector<NodePtr> parents;      // Nodes feeding into this node
        std::vector<WeakNodePtr> children; // Nodes this node feeds into (weak_ptr avoids cyclic reference)

        Matrix outputCache;   // Stores the output value during forward pass
        Matrix gradientCache; // Stores the accumulated gradients during backward pass
    public:
        Node(const std::string& name) : name(name), outputCache(0, 0), gradientCache(0, 0) {}
        virtual ~Node() = default;

        // Links this node to its parent, establishing the DAG structure
        void connect(NodePtr parent){
            parents.push_back(parent);
            parent->children.push_back(shared_from_this());
        }

        // Virtual methods to be implemented by specific node types
        virtual void forward() = 0;
        virtual void backward() = 0;

        const Matrix& getOutput() const { return outputCache; }
        const Matrix& getGradient() const { return gradientCache; }
        std::string getName() const { return name; }

        // Overwrites the current gradient cache
        void setGradient(const Matrix& grad) { gradientCache = grad; }

        // Clears gradients for the next epoch
        void zeroGradient() {
            gradientCache = Matrix(outputCache.rows, outputCache.cols);
        }

        // Accumulates gradients from multiple children (crucial for DAGs with branching)
        void addGradient(const Matrix& grad) {
            if (gradientCache.rows == 0 && gradientCache.cols == 0) {
                gradientCache = grad;
                return;
            }
            if (gradientCache.rows != grad.rows || gradientCache.cols != grad.cols)
                throw std::invalid_argument("Gradient shape mismatch!");
            gradientCache = gradientCache.add(grad);
        }

};

// Specialized node for injecting data into the graph
class InputNode : public Node{
    public:
    InputNode(size_t size, const std::string& name = "Input") : Node(name){
        outputCache = Matrix(size, 1);
    }

    // Feeds new external data into the graph
    void setValue(const Matrix& value){
        if(outputCache.cols != value.cols || outputCache.rows != value.rows)
            throw std::invalid_argument("Input size is invalid!");
        outputCache = value;
    }

    // Input nodes do not compute logic during forward/backward passes
    void forward() override {}
    void backward() override {}
};

#endif
