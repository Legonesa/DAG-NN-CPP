#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#include "Matrix.hpp"
#include "Node.hpp"
#include "DenseLayer.hpp"
#include "SpecialNodes.hpp"

double randomDouble(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

int main() {
    std::cout << "=== NETWORK BASLATILIYOR: [1, 3, 4, 1] ===" << std::endl;

    std::vector<std::shared_ptr<Node>> layers;

    auto inputNode = std::make_shared<InputNode>(1, "Input");
    layers.push_back(inputNode);

    auto h1 = std::make_shared<DenseLayer>(1, 3, DenseLayer::Act::Tanh, 0.01, false, "Hidden_1");
    h1->connect(inputNode);
    layers.push_back(h1);

    auto h2 = std::make_shared<DenseLayer>(3, 4, DenseLayer::Act::Tanh, 0.01, false, "Hidden_2");
    h2->connect(h1);
    layers.push_back(h2);

    auto split_h2_1 = std::make_shared<SplitNode>(0, 0, 2, 1, "Split_h2_1");
    split_h2_1->connect(h2);
    layers.push_back(split_h2_1);
    auto split_h2_2 = std::make_shared<SplitNode>(2, 0, 2, 1, "Split_h2_2");
    split_h2_2->connect(h2);
    layers.push_back(split_h2_2);

    auto h3_1 = std::make_shared<DenseLayer>(2, 2, DenseLayer::Act::Tanh, 0.01, false, "Hidden_3_1");
    h3_1->connect(split_h2_1);
    layers.push_back(h3_1);
    auto h3_2 = std::make_shared<DenseLayer>(2, 2, DenseLayer::Act::Tanh, 0.01, false, "Hidden_3_2");
    h3_2->connect(split_h2_2);
    layers.push_back(h3_2);

    auto h3_concat = std::make_shared<ConcatNode>(0, "Hidden3_Concat");
    h3_concat->connect(h3_1); h3_concat->connect(h3_2);
    layers.push_back(h3_concat);

    /*auto h3 = std::make_shared<DenseLayer>(4, 4, DenseLayer::Act::Tanh, 0.01, false, "Hidden_3");
    h3->connect(h2);
    layers.push_back(h3);*/

    auto outputNode = std::make_shared<DenseLayer>(4, 1, DenseLayer::Act::Sigmoid, 0.01, true, "Output");
    outputNode->connect(h3_concat);
    layers.push_back(outputNode);

    int epochs = 10000;
    std::cout << "Egitim basliyor (" << epochs << " iterasyon)..." << std::endl;

    for (int i = 0; i < epochs; i++) {
        double x_val = randomDouble(0.0, 6.0);
        double target_val = (x_val > 3.0) ? 1.0 : 0.0;

        Matrix inputMat(1, 1);
        inputMat.at(0, 0) = x_val / 6.0;
        inputNode->setValue(inputMat);

        for (auto& layer : layers) layer->zeroGradient();

        for (auto& layer : layers) layer->forward();

        double output_val = outputNode->getOutput().at(0, 0);

        double dz = output_val - target_val;

        Matrix outputGrad(1, 1);
        outputGrad.at(0, 0) = dz;
        outputNode->setGradient(outputGrad);

        for (int j = (int)layers.size() - 1; j >= 0; j--) {
            layers[j]->backward();
        }

        std::cout << "Epoch " << i
                  << " | Input: " << std::fixed << std::setprecision(2) << x_val
                  << " | Target: " << (int)target_val
                  << " | Output: " << std::setprecision(4) << output_val
                  << " | dZ: " << std::setprecision(4) << dz
                  << std::endl;
    }

    std::cout << "\n--- EGITIM TAMAMLANDI ---\n" << std::endl;

    while (true) {
        double user_input;
        std::cout << "Bir sayi girin (Cikis icin -1): ";
        std::cin >> user_input;

        if (user_input == -1) break;

        Matrix inputMat(1, 1);
        inputMat.at(0, 0) = user_input / 6.0;
        inputNode->setValue(inputMat);

        for (auto& layer : layers) layer->forward();

        double result = outputNode->getOutput().at(0, 0);
        std::string prediction = (result > 0.5) ? "BUYUK (>3)" : "KUCUK (<=3)";

        std::cout << "Model Tahmini: " << std::fixed << std::setprecision(4) << result
                  << " -> " << prediction << "\n" << std::endl;
    }

    return 0;
}
