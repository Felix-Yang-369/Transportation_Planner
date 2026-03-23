# Project Introduction

<img align="right" width="260" src="https://github.com/user-attachments/assets/3430c2cc-d373-4d4e-9e7e-34070234b1f1">

**Student 1:** Linjie Yang 
**Student 2:** Ruoyu Xi
<br clear="right"/>

---

## Project Status
The project is completely implemented and functioning as expected. We have successfully integrated the `DSV` and `XML` parsers from Project 2, and 'StrtingUtil" from Project 1.

## Known Issues
There are no major known issues at this time.

## Code References
* [Different ways to initialize variables](https://stackoverflow.com/questions/22543875/different-ways-to-initialize-variables)
* [Google Test Primer (for writing unit tests)](https://google.github.io/googletest/primer.html)
* [Expat XML Parser Documentation](https://libexpat.github.io/doc/)

## Generative AI Use
I used ChatGPT for Prompt 1, and hatGPT for Prompt 2 to assist with code debugging and test case design.

### Prompt 1
How do I iterate over all characters in a string in C++17?

### Response 1
In C++17, you can iterate over all characters in a string using a range-based for loop. Here's an example:
```cpp
#include <iostream>
#include <string>

int main() {
    std::string myString = "Hello, World!";

    // Using range-based for loop to iterate over characters in the string
    for (char c : myString) {
        std::cout << c << " ";
    }

    return 0;
}
```
In this example, the `for (char c : myString)` loop iterates over each character in the string `myString`, and the variable `c` takes on the value of each character in turn. The loop body prints each character followed by a space.

This is a concise and readable way to iterate over the characters in a string in C++17.

### Chagnes 1
I only needed the `for` loop part, and my string was named `bar` so I changed it to just:
```cpp
// AI Generated
for (char c : bar) {
```

### Prompt 2
My COpenStreetMap implementation uses two separate while loops (ParseNodes and ParseWays) to read the XML file, but it drops the <tag> attributes and exhausts the stream before ParseWays can read anything. How to fix this?

### Response 2
The issue occurs because the CXMLReader acts like a single-pass forward stream. Once your first while loop finishes reading all nodes, the end of the file is reached. To fix this and capture nested <tag> attributes, you should combine the logic into a single global pass.

Use a single while(src->ReadEntity(TempEntity)) loop. Keep a temporary shared_ptr for the CurrentNode and CurrentWay. When you encounter a <node> or <way>, update the pointer. When you encounter a <tag>, attach it to whichever pointer is currently active. When you encounter </node> or </way>, set the pointer back to nullptr.

### Changes 2
I completely refactored the SImplementation in OpenStreetMap.cpp. I removed the separate ParseNodes and ParseWays functions and implemented the AI's suggested single-pass state machine logic directly inside ParseOpenStreetMap.
