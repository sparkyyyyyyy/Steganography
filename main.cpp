#include <iostream>
#include <string>
#include <tuple>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "utils.hpp"

int main()
{
    std::string vesselImage, outputImage, secretFile, passcode;
    int choice;

    std::cout << "🕵️‍♀️ C++ Steganography Tool\n";
    std::cout << "1. Embed a file into an image\n";
    std::cout << "2. Extract a file from an image\n";
    std::cout << "Choose an option (1 or 2): ";
    std::cin >> choice;
    std::cin.ignore(); // Clear input buffer

    if (choice == 1)
    {
        std::cout << "Enter path to vessel image: ";
        std::getline(std::cin, vesselImage);
        std::cout << "Enter path to secret file to embed: ";
        std::getline(std::cin, secretFile);
        std::cout << "Enter path to save stego image: ";
        std::getline(std::cin, outputImage);
        std::cout << "Enter passcode: ";
        std::getline(std::cin, passcode);

        // Debug: Show image capacity and file size
        cv::Mat img = cv::imread(vesselImage);
        if (img.empty())
        {
            std::cout << "❌ Could not load vessel image.\n";
            return 1;
        }

        int capacity = img.rows * img.cols; // each pixel = 1 byte
        auto fileSize = std::filesystem::file_size(secretFile);
        std::cout << "\n📦 Vessel image capacity: " << capacity << " bytes";
        std::cout << "\n📁 File to embed size: " << fileSize << " bytes\n";

        if (fileSize + 30 > capacity)
        {
            std::cout << "⚠️ File too large! Consider using a larger image or compressing the file.\n\n";
        }
        else
        {
            embed(vesselImage, outputImage, secretFile, passcode);
        }
    }
    else if (choice == 2)
    {
        std::cout << "Enter path to stego image: ";
        std::getline(std::cin, outputImage);
        std::cout << "Enter passcode: ";
        std::getline(std::cin, passcode);

        extract(outputImage, passcode);
    }
    else
    {
        std::cout << "Invalid option. Exiting.\n";
        return 1;
    }

    // Optional Testing (can be removed or commented out)
    std::cout << "\n\n--- Testing Split/Merge/Crypt ---\n";
    unsigned char original = 'A'; // 65 = 01000001
    auto [f, m, l] = splitByte(original);
    unsigned char combined = mergeBits(f, m, l);
    std::cout << "Original: " << (int)original << "\n";
    std::cout << "Split => F:" << (int)f << " M:" << (int)m << " L:" << (int)l << "\n";
    std::cout << "Merged: " << (int)combined << "\n";

    std::string message = "HelloStego";
    std::string key = "key123";
    std::string encrypted = crypt(message, key);
    std::string decrypted = crypt(encrypted, key);
    std::cout << "Original:  " << message << "\n";
    std::cout << "Encrypted: " << encrypted << "\n";
    std::cout << "Decrypted: " << decrypted << "\n";

    return 0;
}
