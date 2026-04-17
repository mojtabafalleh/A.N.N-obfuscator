#include <iostream>
#include <vector>
#include <future>
#include <iomanip>
#include <cstring>

const unsigned char encrypted_code[] = {
    0x85, 0x53, 0x23, 0xa5, 0x31, 0xc0, 0x1f, 0xd9, 0x45, 0x2f,
    0x7a, 0x0f, 0x95, 0x84, 0x2f, 0x12, 0xa4, 0xc5, 0xc0, 0x42,
    0x91, 0xcc, 0x27, 0xa6, 0xfc, 0x84
};

double neuron_compute(const std::vector<double>& inputs, const std::vector<double>& weights, double bias) {
    double sum = 0.0;
    for (size_t i = 0; i < weights.size(); ++i) sum += inputs[i] * weights[i];
    return sum + bias;
}

std::vector<double> run_layer(const std::vector<double>& inputs,
                              const std::vector<std::vector<double>>& weights,
                              const std::vector<double>& biases) {
    std::vector<std::future<double>> futures;
    for (size_t i = 0; i < weights.size(); ++i)
        futures.push_back(std::async(std::launch::async, neuron_compute, inputs, weights[i], biases[i]));

    std::vector<double> results;
    for (auto& f : futures) results.push_back(f.get());
    return results;
}

std::vector<double> forward_pass(const std::vector<double>& inputs) {
    std::vector<std::vector<double>> weights_layer1 = {
        {0.10, 0.20, 0.30, 0.40},
        {0.20, 0.30, 0.40, 0.50},
        {0.60, 0.70, 0.80, 0.90},
        {0.12, 0.23, 0.34, 0.45}
    };

    std::vector<std::vector<double>> weights_layer2 = {
        {0.13, 0.32, 0.030, 0.040},
        {0.280, 0.306, 0.450, 0.520},
        {0.680, 0.760, 0.850, 0.920},
        {0.182, 0.263, 0.354, 0.425}
    };

    std::vector<std::vector<double>> weights_layer3 = {
        {0.103, 0.312, 0.300, 0.020}
    };

    std::vector<double> bias_l1 = {0.2, 0.5, 0.5, 0.5};
    std::vector<double> bias_l2 = {0.5, 0.5, 0.5, 0.5};
    std::vector<double> bias_l3 = {0.5};

    auto layer1_out = run_layer(inputs, weights_layer1, bias_l1);
    auto layer2_out = run_layer(layer1_out, weights_layer2, bias_l2);
    auto layer3_out = run_layer(layer2_out, weights_layer3, bias_l3);
    
    return layer3_out;
}

unsigned char real_code[256];

bool dummy_function(long long number) {
    int count = 0;
    for (int i = 0; i < 10; i++)
        count++;
    return (count != 10);
}

int main() {
    long long inputNumber;
    std::cin >> inputNumber;
    
    int lastThreeDigits = static_cast<int>(inputNumber % 1000);
    if (inputNumber < 100) {
        lastThreeDigits = static_cast<int>(inputNumber);
    }
    
    std::vector<double> numbers;
    for (int i = 0; i <= 51; i++) {
        numbers.push_back(static_cast<double>(lastThreeDigits + i));
    }
    
    std::vector<int> all_outputs;
    
    for (size_t i = 0; i < numbers.size(); i += 4) {
        std::vector<double> batch;
        for (size_t j = i; j < i + 4 && j < numbers.size(); j++) {
            batch.push_back(numbers[j]);
        }
        while (batch.size() < 4) {
            batch.push_back(0.0);
        }
        std::vector<double> output = forward_pass(batch);
        all_outputs.push_back(static_cast<int>(output[0]));
    }
    
    size_t code_size = sizeof(encrypted_code) / sizeof(encrypted_code[0]);
    size_t outputs_size = all_outputs.size();
    
    for (size_t i = 0; i < code_size; i++) {
        size_t output_index = i % outputs_size;
        real_code[i] = encrypted_code[i] ^ (static_cast<unsigned char>(all_outputs[output_index] % 256));
    }
    
    memcpy((void*)dummy_function, real_code, code_size);
    
    bool result = dummy_function(inputNumber);
    
    if (result)
        std::cout << "Success!" << std::endl;
    else
        std::cout << "Wrong, please try reverse engineering again." << std::endl;
    
    return 0;
}