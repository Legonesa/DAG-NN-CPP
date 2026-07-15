#ifndef DENSE_LAYER_HPP
#define DENSE_LAYER_HPP

#include "Node.hpp"
#include <cmath>
#include <random>
#include <algorithm>


class DenseLayer : public Node {
public:
    enum class Act { Tanh, Sigmoid };

private:
    Matrix weights;
    Matrix biases;
    Matrix zCache;

    double learningRate;
    Act activation;

    bool gradIsPreActivation;

    static double sigmoid(double x) {
        if (x >= 40.0) return 1.0;
        if (x <= -40.0) return 0.0;
        return 1.0 / (1.0 + std::exp(-x));
    }

    static double act(double x, Act a) {
        if (a == Act::Tanh) return std::tanh(x);
        return sigmoid(x);
    }

    static double actPrimeFromOutput(double y, Act a) {
        if (a == Act::Tanh) return 1.0 - (y * y);
        return y * (1.0 - y);
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
        weights.randomizeXavier(inputSize, outputSize);

        outputCache = Matrix(outputSize, 1);
        gradientCache = Matrix(outputSize, 1);
    }

    void forward() override {
        if (parents.empty()) {
            std::cerr << "HATA: " << name << " bir girdiye bagli degil!" << std::endl;
            return;
        }

        const Matrix& input = parents[0]->getOutput();
        Matrix dotProd = weights.multiply(input);
        zCache = dotProd.add(biases);

        outputCache = zCache.map([&](double v) { return act(v, activation); });
    }

    void backward() override {
        if (parents.empty()) return;

        Matrix dL_dOutOrDz = this->gradientCache;
        Matrix delta(dL_dOutOrDz.rows, dL_dOutOrDz.cols);

        if (gradIsPreActivation) {
            delta = dL_dOutOrDz;
        } else {
            Matrix activationPrime = outputCache.map([&](double y) { return actPrimeFromOutput(y, activation); });
            for (size_t i = 0; i < delta.data.size(); i++) {
                delta.data[i] = dL_dOutOrDz.data[i] * activationPrime.data[i];
            }
        }

        Matrix weightsT = weights.transpose();
        Matrix parentError = weightsT.multiply(delta);

        NodePtr parent = parents[0];
        const Matrix& pg = parent->getGradient();
        if (pg.rows == parentError.rows && pg.cols == parentError.cols) {
            parent->addGradient(parentError);
        } else {
            parent->setGradient(parentError);
        }

        Matrix inputT = parents[0]->getOutput().transpose();
        Matrix dW = delta.multiply(inputT);

        for (size_t i = 0; i < weights.data.size(); i++) {
            weights.data[i] -= learningRate * dW.data[i];
        }
        for (size_t i = 0; i < biases.data.size(); i++) {
            biases.data[i] -= learningRate * delta.data[i];
        }
    }

    void printWeights() {
        std::cout << "Weights of " << name << ":" << std::endl;
        weights.print();
    }
};

#endif
