#include <iostream>
#include <string>
#include <tuple>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "utils.hpp"

using namespace std;

int main()
{
    string vesselImage, outputImage, secretFile, passcode;
    int choice;

    cout << "🕵️‍♀️ C++ Steganography Tool\n";
    cout << "1. Embed a file into an image\n";
    cout << "2. Extract a file from an image\n";
    cout << "Choose an option (1 or 2): ";
    cin >> choice;
    cin.ignore(); // Clear input buffer

    if (choice == 1)
    {
        cout << "Enter path to vessel image: ";
        getline(cin, vesselImage);
        cout << "Enter path to secret file to embed: ";
        getline(cin, secretFile);
        cout << "Enter path to save stego image: ";
        getline(cin, outputImage);
        cout << "Enter passcode: ";
        getline(cin, passcode);

        // Debug: Show image capacity and file size
        cv::Mat img = cv::imread(vesselImage);
        if (img.empty())
        {
            cout << "❌ Could not load vessel image.\n";
            return 1;
        }

        int capacity = img.rows * img.cols; // each pixel = 1 byte
        auto fileSize = std::filesystem::file_size(secretFile);
        cout << "\n📦 Vessel image capacity: " << capacity << " bytes";
        cout << "\n📁 File to embed size: " << fileSize << " bytes\n";

        if (fileSize + 30 > capacity)
        {
            cout << "⚠️ File too large! Consider using a larger image or compressing the file.\n\n";
        }
        else
        {
            embed(vesselImage, outputImage, secretFile, passcode);
        }
    }
    else if (choice == 2)
    {
        cout << "Enter path to stego image: ";
        getline(cin, outputImage);
        cout << "Enter passcode: ";
        getline(std::cin, passcode);

        extract(outputImage, passcode);
    }
    else
    {
        cout << "Invalid option. Exiting.\n";
        return 1;
    }

    // Optional Testing (can be removed or commented out)
    // cout << "\n\n--- Testing Split/Merge/Crypt ---\n";
    // unsigned char original = 'A'; // 65 = 01000001
    // auto [f, m, l] = splitByte(original);
    // unsigned char combined = mergeBits(f, m, l);
    // cout << "Original: " << (int)original << "\n";
    // cout << "Split => F:" << (int)f << " M:" << (int)m << " L:" << (int)l << "\n";
    // cout << "Merged: " << (int)combined << "\n";

    // string message = "HelloStego";
    // string key = "key123";
    // string encrypted = crypt(message, key);
    // string decrypted = crypt(encrypted, key);
    // cout << "Original:  " << message << "\n";
    // cout << "Encrypted: " << encrypted << "\n";
    // cout << "Decrypted: " << decrypted << "\n";

    return 0;
}
