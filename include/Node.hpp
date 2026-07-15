#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Matrix.hpp"

class Node;

using NodePtr = std::shared_ptr<Node>;
using WeakNodePtr = std::weak_ptr<Node>;

class Node : public std::enable_shared_from_this<Node> {
    protected:
        std::string name;
        std::vector<NodePtr> parents; 
        std::vector<WeakNodePtr> children; 

        Matrix outputCache;
        Matrix gradientCache;
    public:
        Node(const std::string& name) : name(name), outputCache(0, 0), gradientCache(0, 0) {}
        virtual ~Node() = default;
        void connect(NodePtr parent){
            parents.push_back(parent);
            parent->children.push_back(shared_from_this());
        }
        virtual void forward() = 0;
        virtual void backward() = 0;
        const Matrix& getOutput() const { return outputCache; }
        const Matrix& getGradient() const { return gradientCache; }
        std::string getName() const { return name; }
        void setGradient(const Matrix& grad) { gradientCache = grad; }
        void zeroGradient() {
            gradientCache = Matrix(outputCache.rows, outputCache.cols);
        }
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

class InputNode : public Node{
    public:
    InputNode(size_t size, const std::string& name = "Input") : Node(name){
        outputCache = Matrix(size, 1);
    }
    void setValue(const Matrix& value){
        if(outputCache.cols != value.cols || outputCache.rows != value.rows)
            throw std::invalid_argument("Input size is invalid!");
        outputCache = value;
    }
    void forward() override {}
    void backward() override {}
};

#endif