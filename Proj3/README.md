# Project X README

Student 1: Partner1 Name (123456789)

Student 2: Partner2 Name (123456987)

## Project Status
This the current status of the project ...

## Known Issues
The project fails to function properly in ...

## Code References
* [Different ways to initialize variables](https://stackoverflow.com/questions/22543875/different-ways-to-initialize-variables)
* [Some other link](https://stackoverflow.com)

## Generative AI Use
I used ChatGPT for Prompt 1, and ZZZZZ for Prompt 2....

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
My other prompt...

### Response 2
The other response...

### Changes 2
The other changes made...
