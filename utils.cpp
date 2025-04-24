#include <iostream>
#include <tuple>
#include <string>
#include "utils.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace std;
// Splits a byte into 3 parts: 3 bits, 3 bits, 2 bits
tuple<unsigned char, unsigned char, unsigned char> splitByte(unsigned char byte)
{
    unsigned char first = byte >> 5;         // 3 MSB
    unsigned char mid = (byte >> 2) & 0b111; // 3 middle bits
    unsigned char last = byte & 0b11;        // 2 LSB
    return make_tuple(first, mid, last);
}

// Merges 3 parts: 3 + 3 + 2 bits into one byte
unsigned char mergeBits(unsigned char first, unsigned char mid, unsigned char last)
{
    return (((first << 3) | mid) << 2) | last;
}

// Encrypts or decrypts using XOR with a key
string crypt(const string &input, const string &key)
{
    string result;
    int keyLength = key.length();
    for (size_t i = 0; i < input.length(); ++i)
    {
        result += input[i] ^ key[i % keyLength];
    }
    return result;
}

// Helper to format metadata (file size and name)
string getMetaData(const string &filePath)
{
    if (!filesystem::exists(filePath))
        return "";

    ifstream f(filePath, ios::binary | ios::ate);
    size_t fileSize = f.tellg();
    f.close();

    if (fileSize > 9999999999)
        return "";

    ostringstream ss;
    ss << setw(10) << setfill('*') << fileSize;

    string fileName = filesystem::path(filePath).filename().string();
    if (fileName.length() > 20)
        fileName = fileName.substr(fileName.length() - 20);
    else
        fileName.append(20 - fileName.length(), '*');

    return ss.str() + fileName;
}

// Resize image file until it fits into available space
string resizeFileIfNeeded(const string &filePath, size_t maxSizeBytes)
{
    ifstream f(filePath, ios::binary | ios::ate);
    size_t fileSize = f.tellg();
    f.close();
    if (fileSize <= maxSizeBytes)
        return filePath;

    cv::Mat img = cv::imread(filePath);
    if (img.empty())
    {
        cout << "❌ Cannot resize — image load failed: " << filePath << endl;
        return "";
    }

    double scale = 0.9;
    string resizedPath = "resized_secret.jpg";

    while (scale > 0.1)
    {
        cv::Mat resized;
        cv::resize(img, resized, cv::Size(), scale, scale);
        cv::imwrite(resizedPath, resized);

        ifstream resizedFile(resizedPath, ios::binary | ios::ate);
        size_t newSize = resizedFile.tellg();
        resizedFile.close();

        if (newSize <= maxSizeBytes)
        {
            cout << "✅ Resized file to fit. New size: " << newSize << " bytes\n";
            return resizedPath;
        }

        scale -= 0.1;
    }

    cout << "❌ Could not resize file small enough.\n";
    return "";
}
// Embed function with resizing support
void embed(const string &vesselPath, const string &outPath, const string &fileToEmbed, const string &passcode)
{
    // 1. Load vessel image
    cv::Mat image = cv::imread(vesselPath);
    if (image.empty())
    {
        cout << "❌ Failed to load vessel image." << endl;
        return;
    }
    int capacity = image.rows * image.cols;
    string adjustedFile = resizeFileIfNeeded(fileToEmbed, capacity - 30);
    if (adjustedFile.empty())
    {
        cout << "❌ File could not be resized to fit.\n";
        return;
    }

    // 2. Load resized file
    ifstream inputFile(adjustedFile, ios::binary);
    if (!inputFile)
    {
        cout << "❌ Failed to open secret file." << endl;
        return;
    }

    vector<unsigned char> buffer((istreambuf_iterator<char>(inputFile)), {});
    inputFile.close();

    // 3. Create and encrypt metadata
    string header = getMetaData(adjustedFile);
    if (header.empty())
    {
        cout << "❌ Metadata creation failed." << endl;
        return;
    }

    header = crypt(header, passcode);

    // 4. Capacity check
    size_t totalSize = header.length() + buffer.size();
    if (totalSize > capacity)
    {
        cout << "❌ File too big to embed in image." << endl;
        return;
    }

    // 5. Embed
    int width = image.cols;
    int idx = 0;

    for (int i = 0; i < totalSize; ++i)
    {
        int r = idx / width;
        int c = idx % width;

        unsigned char byte = (i < 30) ? header[i] : buffer[i - 30];
        auto [f, m, l] = splitByte(byte);

        cv::Vec3b &pixel = image.at<cv::Vec3b>(r, c);
        pixel[0] = (pixel[0] & 0xFC) | l; // Blue: 2 bits
        pixel[1] = (pixel[1] & 0xF8) | m; // Green: 3 bits
        pixel[2] = (pixel[2] & 0xF8) | f; // Red: 3 bits

        idx++;
    }

    // 6. Save image
    cv::imwrite(outPath, image);
    cout << "✅ Embedding complete. Output saved to: " << outPath << endl;
}

void extract(const std::string &embPath, const std::string &passcode)
{
    // 1. Load the stego image
    cv::Mat image = cv::imread(embPath);
    if (image.empty())
    {
        std::cout << "❌ Failed to load stego image." << std::endl;
        return;
    }

    int width = image.cols;
    int idx = 0;
    std::string buffer;
    std::vector<unsigned char> extractedData;

    // 2. Extract first 30 bytes for metadata
    for (int i = 0; i < 30; ++i)
    {
        int r = idx / width;
        int c = idx % width;
        cv::Vec3b pixel = image.at<cv::Vec3b>(r, c);

        unsigned char f = pixel[2] & 0x07; // Red: 3 bits
        unsigned char m = pixel[1] & 0x07; // Green: 3 bits
        unsigned char l = pixel[0] & 0x03; // Blue: 2 bits

        unsigned char byte = mergeBits(f, m, l);
        buffer += byte;
        idx++;
    }

    // 3. Decrypt and extract metadata
    std::string decryptedMeta = crypt(buffer, passcode);
    int fileSize = std::stoi(decryptedMeta.substr(0, 10).erase(decryptedMeta.find('*')));
    std::string fileName = decryptedMeta.substr(10, 20);
    fileName.erase(fileName.find('*'));

    std::string outPath = "extracted_" + fileName;

    // 4. Extract actual file data
    for (int i = 0; i < fileSize; ++i)
    {
        int r = idx / width;
        int c = idx % width;
        cv::Vec3b pixel = image.at<cv::Vec3b>(r, c);

        unsigned char f = pixel[2] & 0x07;
        unsigned char m = pixel[1] & 0x07;
        unsigned char l = pixel[0] & 0x03;

        unsigned char byte = mergeBits(f, m, l);
        extractedData.push_back(byte);
        idx++;
    }

    // 5. Save the extracted file
    std::ofstream outputFile(outPath, std::ios::binary);
    outputFile.write(reinterpret_cast<char *>(&extractedData[0]), extractedData.size());
    outputFile.close();

    std::cout << "✅ Extraction complete. File saved as: " << outPath << std::endl;
}
