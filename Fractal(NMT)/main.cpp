#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <complex>
#include <thread>

// This project uses stb_image.h for image processing. Check on https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "handMade.h"

int main() {
    // More Than 20000 * 20000 crashes due to vector size
    int width = 20000; 
    int height = 20000;
    std::cout << "width: ";

    std::cin >> width;
    width = hm::CinFail(width);

    if (width > 20000) {
        width = 20000;
        std::cout << "Max size: 20000\n";
    }
    if (width < 1) {
        std::cout << "Min size: 1\n";
        width = 1;
    }

    height = width;

    std::vector<unsigned char> image(width * height * 3);

    unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "CPU threads: " << numThreads << std::endl;

    int Ztimes = 256;

    std::cout << "depth: ";

    std::cin >> Ztimes;
    Ztimes = hm::CinFail(Ztimes);

    if (Ztimes > 1000) {
        Ztimes = 1000;
        std::cout << "Max depth: 1000\n";
    }
    if (Ztimes < 1) {
        Ztimes = 1000;
        std::cout << "Min depth: 1\n";
    }

    std::vector<std::thread> threads;

    for (int y = 0; y < height; y++) {
        auto lambda = [&image, width, height, Ztimes, y]() {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                std::complex<double> c = { (double)(-2.0 + x * 4.0 / width), (double)(-2 + y * 4.0 / height) };
                std::complex<double> z = { 0,0 };
                for (int i = 0; i < Ztimes; i++) {
                    z *= z;
                    z += c;
                    if (std::abs(z) >= 2) {
                        image[index + 0] = i * 2;
                        image[index + 1] = i * 2;
                        image[index + 2] = i * 2;
                        break;
                    }
                }
            }
        };
        if (threads.size() < numThreads) {
            threads.emplace_back(lambda);
            continue;
        }
        threads[y%16].join();
        threads[y%16] = std::thread(lambda);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::string fileName = "W = " + std::to_string(width) + ", H = " + std::to_string(height) + ", iter = " + std::to_string(Ztimes) + " output.png";
    if (stbi_write_png(fileName.c_str(), width, height, 3, image.data(), width * 3)) {
        std::cout << "Image Saved Successfully." << std::endl;
    }
    else {
        std::cout << "Error Saving The Image." << std::endl;
    }

    return 0;
}
