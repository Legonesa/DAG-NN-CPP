#ifndef DENSE_LAYER_HPP
#define DENSE_LAYER_HPP

#include "Node.hpp"
#include <cmath>
#include <random>
#include <algorithm>

// Fully connected (Dense) neural network layer.
// Implements standard linear transformation (z = Wx + b) and non-linear activation.
class DenseLayer : public Node {
public:
    enum class Act { Tanh, Sigmoid };

private:
    Matrix weights;
    Matrix biases;
    Matrix zCache; // Pre-activation values stored for backprop

    double learningRate;
    Act activation;

    bool gradIsPreActivation; // Flag indicating if incoming gradient is dL/dZ instead of dL/dOut

    // Safe Sigmoid implementation to prevent numerical overflow
    static double sigmoid(double x) {
        if (x >= 40.0) return 1.0;
        if (x <= -40.0) return 0.0;
        return 1.0 / (1.0 + std::exp(-x));
    }

    // Applies chosen activation function
    static double act(double x, Act a) {
        if (a == Act::Tanh) return std::tanh(x);
        return sigmoid(x);
    }

    // Calculates the derivative of the activation function using the output directly
    static double actPrimeFromOutput(double y, Act a) {
        if (a == Act::Tanh) return 1.0 - (y * y); // Tanh derivative: 1 - y^2
        return y * (1.0 - y);                     // Sigmoid derivative: y * (1 - y)
    }

public:
    DenseLayer(size_t inputSize,
               size_t outputSize,
               Act actType = Act::Tanh,
               double lr = 0.01,
               bool gradIsDz = false,
               const std::string& name = "Dense")
        : Node(name),
          weights(outputSize, inputSize),
          biases(outputSize, 1),
          zCache(0, 0),
          learningRate(lr),
          activation(actType),
          gradIsPreActivation(gradIsDz)
    {
        // Use Xavier Initialization for optimal learning stability
        weights.randomizeXavier(inputSize, outputSize);

        outputCache = Matrix(outputSize, 1);
        gradientCache = Matrix(outputSize, 1);
    }

    // Forward pass computes z = (weights * input) + biases, then applies activation
    void forward() override {
        if (parents.empty()) {
            std::cerr << "HATA: " << name << " bir girdiye bagli degil!" << std::endl;
            return;
        }

        const Matrix& input = parents[0]->getOutput();
        Matrix dotProd = weights.multiply(input);
        zCache = dotProd.add(biases);

        // Apply activation function mapping element by element
        outputCache = zCache.map([&](double v) { return act(v, activation); });
    }

    // Backward pass computes gradients for weights, biases, and the previous layer
    void backward() override {
        if (parents.empty()) return;

        Matrix dL_dOutOrDz = this->gradientCache;
        Matrix delta(dL_dOutOrDz.rows, dL_dOutOrDz.cols);

        // Calculate delta: If gradient is not already dZ (usually from loss func), compute it here
        if (gradIsPreActivation) {
            delta = dL_dOutOrDz;
        } else {
            Matrix activationPrime = outputCache.map([&](double y) { return actPrimeFromOutput(y, activation); });
            for (size_t i = 0; i < delta.data.size(); i++) {
                delta.data[i] = dL_dOutOrDz.data[i] * activationPrime.data[i];
            }
        }

        // Calculate error to pass back to the parent layer (dL/dX = W^T * delta)
        Matrix weightsT = weights.transpose();
        Matrix parentError = weightsT.multiply(delta);

        // Accumulate gradient in parent
        NodePtr parent = parents[0];
        const Matrix& pg = parent->getGradient();
        if (pg.rows == parentError.rows && pg.cols == parentError.cols) {
            parent->addGradient(parentError);
        } else {
            parent->setGradient(parentError);
        }

        // Calculate weight gradients (dL/dW = delta * X^T)
        Matrix inputT = parents[0]->getOutput().transpose();
        Matrix dW = delta.multiply(inputT);

        // Update weights and biases using standard Stochastic Gradient Descent (SGD)
        for (size_t i = 0; i < weights.data.size(); i++) {
            weights.data[i] -= learningRate * dW.data[i];
        }
        for (size_t i = 0; i < biases.data.size(); i++) {
            biases.data[i] -= learningRate * delta.data[i];
        }
    }

    // Debugging utility to inspect trained weights
    void printWeights() {
        std::cout << "Weights of " << name << ":" << std::endl;
        weights.print();
    }
};

#endif
