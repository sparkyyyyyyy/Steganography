# Steganography in C++ (Image-based File Hiding)

This project is a fully functional C++ application for performing steganography<br> 
— the art of hiding information inside media,<br> 
— specifically by embedding any file inside an image using bit-level manipulation and simple XOR encryption.<br> 
Built using OpenCV and standard C++17.

---

## 🚀 Features

- Hide any file (image, text, PDF, etc.) inside a PNG or JPEG image.
- XOR encryption for lightweight obfuscation with a passcode.
- Metadata embedding (file size and name) for seamless extraction.
- Automatic file resizing to fit within image capacity.
- Simple CLI menu interface.
- Extracts hidden file to disk in original format.
- Fully modular and extensible C++ codebase.

---

## 📸 How It Works

- Each byte of your file is split into 3 parts:
  - 3 most significant bits → Red channel
  - 3 middle bits → Green channel
  - 2 least significant bits → Blue channel
- First 30 bytes of the image store encrypted metadata (file size + name).
- Remainder of the file is embedded pixel by pixel.
- Extraction reverses the process using the same passcode.

---

## 🧱 Project Structure
  ├── main.cpp   # CLI menu + user interaction<br>
  ├── utils.cpp  # Core steganography logic<br> 
  ├── utils.hpp  # Function declarations<br> 
  └── README.md<br>

---

## 🛠️ Dependencies

- OpenCV (C++ bindings)
- C++17 compiler (e.g., g++ 9+, Clang 10+)

Install OpenCV (Linux/macOS):

```bash
sudo apt install libopencv-dev
or
brew install opencv
```

---

🧪 Build and Run

Compile: g++ -std=c++17 main.cpp utils.cpp -o steg `pkg-config --cflags --libs opencv4`<br>
Run: ./steg

---

💻 Usage Demo
Upon running, you’ll see:
🕵️‍♀️ C++ Steganography Tool
1. Embed a file into an image
2. Extract a file from an image
Choose an option (1 or 2):
<br>🧩 For embedding, enter:
- Path to carrier image (e.g., snake.jpeg)
- Path to the file to hide (e.g., certificate.jpg)
- Output image path (e.g., new_snake.png)
- A passcode for encryption

<br>🔓 For extraction, just provide:
- Path to the stego image (e.g., new_snake.png)
- Same passcode used during embedding
- The extracted file will be saved as: extracted_filename.ext

---

📌 Notes
- Only one file is embedded per image.
- Resizing is applied only to image-type secret files.
- Extraction requires correct passcode.

---

📷 Example
Input:
- vessel image: 1920x1080 JPEG
- file to embed: 250KB PNG

Output:
- stego image: PNG with secret hidden
- extracted file: byte-perfect match

---

✨ Credits<br>
Developed by Mohit Gajraj<br>
C++17, OpenCV, and a passion for security + creativity

