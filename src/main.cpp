#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#include "Matrix.hpp"
#include "Node.hpp"
#include "DenseLayer.hpp"
#include "SpecialNodes.hpp"

// Utility function to generate random doubles in a specific range
double randomDouble(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

int main() {
    std::cout << "=== NETWORK INITIALIZING: [1, 3, 4, 1] ===" << std::endl;

    // Vector storing pointers to all nodes. Used for iterating passes.
    std::vector<std::shared_ptr<Node>> layers;

    // 1) Input Layer: Accepts scalar feature (1x1 Matrix)
    auto inputNode = std::make_shared<InputNode>(1, "Input");
    layers.push_back(inputNode);

    // 2) Hidden Layer 1: Transforms 1 feature to 3 neurons
    auto h1 = std::make_shared<DenseLayer>(1, 3, DenseLayer::Act::Tanh, 0.01, false, "Hidden_1");
    h1->connect(inputNode);
    layers.push_back(h1);

    // 3) Hidden Layer 2: Transforms 3 neurons to 4 neurons
    auto h2 = std::make_shared<DenseLayer>(3, 4, DenseLayer::Act::Tanh, 0.01, false, "Hidden_2");
    h2->connect(h1);
    layers.push_back(h2);

    // 4) Branching the Graph: Split the 4 neurons from h2 into two independent sets of 2
    auto split_h2_1 = std::make_shared<SplitNode>(0, 0, 2, 1, "Split_h2_1");
    split_h2_1->connect(h2);
    layers.push_back(split_h2_1);

    auto split_h2_2 = std::make_shared<SplitNode>(2, 0, 2, 1, "Split_h2_2");
    split_h2_2->connect(h2);
    layers.push_back(split_h2_2);

    // 5) Parallel Processing: Each branch goes into its own sub-network
    auto h3_1 = std::make_shared<DenseLayer>(2, 2, DenseLayer::Act::Tanh, 0.01, false, "Hidden_3_1");
    h3_1->connect(split_h2_1);
    layers.push_back(h3_1);

    auto h3_2 = std::make_shared<DenseLayer>(2, 2, DenseLayer::Act::Tanh, 0.01, false, "Hidden_3_2");
    h3_2->connect(split_h2_2);
    layers.push_back(h3_2);

    // 6) Merging Branches: Re-concatenate the branches back to 4 neurons
    auto h3_concat = std::make_shared<ConcatNode>(0, "Hidden3_Concat");
    h3_concat->connect(h3_1);
    h3_concat->connect(h3_2);
    layers.push_back(h3_concat);

    // 7) Output Layer: Maps the 4 concatenated neurons to a single binary prediction (Sigmoid)
    auto outputNode = std::make_shared<DenseLayer>(4, 1, DenseLayer::Act::Sigmoid, 0.01, true, "Output");
    outputNode->connect(h3_concat);
    layers.push_back(outputNode);

    // --- TRAINING PHASE ---
    int epochs = 10000;
    std::cout << "Training started (" << epochs << " iterations)..." << std::endl;

    for (int i = 0; i < epochs; i++) {
        // Generate a random number [0.0, 6.0]
        double x_val = randomDouble(0.0, 6.0);
        // Objective: Predict 1 if > 3.0, otherwise 0
        double target_val = (x_val > 3.0) ? 1.0 : 0.0;

        // Create input matrix and normalize the value
        Matrix inputMat(1, 1);
        inputMat.at(0, 0) = x_val / 6.0;
        inputNode->setValue(inputMat);

        // Step 1: Zero gradients before backpropagation
        for (auto& layer : layers) layer->zeroGradient();

        // Step 2: Forward pass through all nodes
        for (auto& layer : layers) layer->forward();

        double output_val = outputNode->getOutput().at(0, 0);

        // Step 3: Compute Loss Gradient (dZ for BCE + Sigmoid simplifies to Output - Target)
        double dz = output_val - target_val;

        Matrix outputGrad(1, 1);
        outputGrad.at(0, 0) = dz;
        outputNode->setGradient(outputGrad); // Inject gradient into the end of the graph

        // Step 4: Backward pass (Backpropagation) through all nodes in reverse order
        for (int j = (int)layers.size() - 1; j >= 0; j--) {
            layers[j]->backward();
        }

        // Display progress periodically or constantly based on preferences
        std::cout << "Epoch " << i
                  << " | Input: " << std::fixed << std::setprecision(2) << x_val
                  << " | Target: " << (int)target_val
                  << " | Output: " << std::setprecision(4) << output_val
                  << " | dZ: " << std::setprecision(4) << dz
                  << std::endl;
    }

    std::cout << "\n--- TRAINING COMPLETE ---\n" << std::endl;

    // --- INTERACTIVE INFERENCE PHASE ---
    while (true) {
        double user_input;
        std::cout << "Enter a number (Type -1 to exit): ";
        std::cin >> user_input;

        if (user_input == -1) break;

        // Format and normalize user input
        Matrix inputMat(1, 1);
        inputMat.at(0, 0) = user_input / 6.0;
        inputNode->setValue(inputMat);

        // Single forward pass for prediction
        for (auto& layer : layers) layer->forward();

        // Interpret result using Sigmoid threshold
        double result = outputNode->getOutput().at(0, 0);
        std::string prediction = (result > 0.5) ? "GREATER (>3)" : "SMALLER (<=3)";

        std::cout << "Model Prediction: " << std::fixed << std::setprecision(4) << result
                  << " -> " << prediction << "\n" << std::endl;
    }

    return 0;
}
