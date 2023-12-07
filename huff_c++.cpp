#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
using namespace std;

// Node structure for the Huffman tree
struct Node {
    char data;
    int freq;
    Node *left, *right;
};

// Functor for comparing nodes in the priority queue
struct compare {
    bool operator()(Node *l, Node *r) {
        return l->freq > r->freq;
    }
};

// Function to print Huffman codes
void printCode(Node *root, string s) {
    if (root->left) {
        // Traverse left and append "0" to the current Huffman code
        printCode(root->left, s + "0");
    }
    if (root->right) {
        // Traverse right and append "1" to the current Huffman code
        printCode(root->right, s + "1");
    }
    if (root->left == NULL && root->right == NULL) {
        // Leaf node reached, print the character and its Huffman code
        cout << root->data << " : " << s << endl;
    }
}

// Function to store Huffman codes in a map
void storeCodes(Node *root, string str, map<char, string> &codes) {
    if (root == NULL) {
        // Base case: reached the end of a branch
        return;
    }
    if (root->data != '-') {
        // If not a combined node, store the character and its Huffman code
        codes[root->data] = str;
    }
    // Recursively traverse left and right branches
    storeCodes(root->left, str + "0", codes);
    storeCodes(root->right, str + "1", codes);
}

// Function to compress the file and save as 'compressed.txt'
int compressAndSave(string filename) {
    string line;
    ifstream myfile(filename);
    map<char, int> freqs;

    if (myfile.is_open()) {
        // Read the entire file character by character and count frequencies
        char c;
        while (myfile.get(c)) {
            freqs[c]++;
        }
        myfile.close();
    } else {
        // Unable to open file for reading
        cout << "Unable to open file";
        return 0;
    }

    // Priority queue for building the Huffman tree
    priority_queue<Node *, vector<Node *>, compare> pq;

    // Create a leaf node for each character and its frequency
    for (auto &freq : freqs) {
        Node *node = new Node;
        node->data = freq.first;
        node->freq = freq.second;
        node->left = node->right = NULL;
        pq.push(node);
    }

    // Build the Huffman tree by combining nodes with the lowest frequencies
    while (pq.size() != 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        Node *combined = new Node;
        combined->data = '-';
        combined->freq = left->freq + right->freq;
        combined->left = left;
        combined->right = right;
        pq.push(combined);
    }

    // Get the root of the Huffman tree
    Node *root = pq.top();

    // Map to store generated Huffman codes
    map<char, string> codes;
    // Generate Huffman codes and store them in the 'codes' map
    storeCodes(root, "", codes);

    // Reopen the file for compression
    myfile.open(filename);
    string compressed = "";

    if (myfile.is_open()) {
        // Replace characters with their Huffman codes
        char c;
        while (myfile.get(c)) {
            compressed += codes[c];
        }
        myfile.close();
    } else {
        // Unable to open file for reading
        cout << "Unable to open file for reading";
        return 0;
    }

    // Write the compressed data to a new file
    ofstream outfile("compressed.txt");
    outfile << compressed;
    outfile.close();

    // Display compression information
    cout << "File compressed successfully and saved as 'compressed.txt'" << endl;
    cout << "Compression Information:" << endl;
    cout << "Original File Size: " << freqs.size() << " bytes" << endl;
    cout << "Compressed File Size: " << compressed.size() / 8 << " bytes" << endl;
    cout << "Compression Ratio: " << (1.0 - (compressed.size() / (freqs.size() * 8.0))) * 100 << "%" << endl;

    return 1;  // Return 1 to indicate successful compression
}

int main() {
    // Entry point of the program
    cout << "Huffman Compression Program" << endl;
    cout << "Enter the path of the file to compress: ";
    string filename;
    getline(cin, filename);

    // Call the compressAndSave function
    if (compressAndSave(filename)) {
        // Compression completed successfully
        cout << "Compression completed successfully." << endl;
        // Add code for decompression here if needed
    }

    return 0;  // Return 0 to indicate successful execution
}
