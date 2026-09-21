/*
 * CS 2414 – Data Structures – Fall 2026 – Project One
 * SensorStream-Compress
 *
 * Student boilerplate. Fill in the TODO sections.
 * Allowed headers: <iostream>, <string>, <vector>, <sstream>
 * Add your Full Name and ID Here.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class SensorStreamCompress {
protected:
    vector<int> _seq;           // current sequence of readings / symbols
    int _freq[640][640];        // pair-frequency matrix
    int _rules[512][3];         // learned rules: a b -> z
    int _ruleCount;             // number of rules stored so far
    int _maxSymbol;             // highest symbol ID in use (first new symbol is 128)

public:
    SensorStreamCompress() {
        _ruleCount = 0;
        _maxSymbol = 127;       // so the first merge creates symbol 128

        // initialize _freq
        for (int i = 0; i < 640; i++)
            for (int j = 0; j < 640; j++)
                _freq[i][j] = 0;
    }

    
    void initialize(int k, int numStreams) {
        _seq.clear();
        string line;
        
        // Read numStreams lines of integers from cin and append them to _seq.
        // Parse every integer on the current into _seq (concatenate streams;
        // do not insert a separator).
        for (int s = 0; s < numStreams; s++) {
            if (!getline(cin, line))
                break;
            
            // skip blank lines if any
            if (line.empty()) {
                s--;
                continue;
            }
            
            stringstream ss(line);
            int v;
           
            while (ss >> v) {
                //TODO: insert v into _seq
            }
        }
        
        // TODO: ensure _maxSymbol = 127

    }

    
    void train(int k) {
        // TODO: 
        // Perform up to k merges. Stop early if no pair occurs more than once.
    }

    
    void encode() {
        //  TODO:
        //  Apply the stored rules in LEARNING order (first rule first) to _seq.
        //  For each rule (a, b -> z) from index 0 to _ruleCount-1:
        //    scan _seq left to right, non-overlapping, replace (a, b) by z.
    }

    
    void decode() {
        // TODO:
        // Expand _seq using stored rules in REVERSE order (last rule first).
        // Each z becomes the pair (a, b).
    }

    
    void displaySequence() {
        // TODO:
        // Print _seq as space-separated integers with NO trailing space.
    }

    
    void displayRules() {
        // TODO:
        // Print each learned rule on its own line as: a b z
    }

    // Used by main() to load the sequence after an E or D command.
    void setSequence(const vector<int>& s) { _seq = s; }
};

int main() {
    int k, numStreams;
    cin >> k >> numStreams;
    cin.ignore();   // consume the newline after the first line

    SensorStreamCompress compressor;

    // Step 1: read the sensor streams into _seq
    compressor.initialize(k, numStreams);

    // Step 2: learn up to k merge rules and compress _seq
    compressor.train(k);

    // Step 3
    cout << "Rules learned from Compression:" << endl;
    compressor.displayRules();

    // Step 4
    cout << "Compressed sequence:" << endl;
    compressor.displaySequence();

    // Step 5: remaining lines are commands that reuse the remembered rules
    //   E n1 n2 ...    encode this fresh sequence (rules in learning order)
    //   D c1 c2 ...    decode this fresh sequence (rules in reverse order)
    string line;
    while (getline(cin, line)) {
        if (line.empty())
            continue;

        char cmd = line[0];
        vector<int> vals;
        // TODO: parse the integers after the command letter into vals

        if (cmd == 'E' || cmd == 'e') {
            compressor.setSequence(vals);
            compressor.encode();
            cout << "Encoded sequence:" << endl;
            compressor.displaySequence();
        } else if (cmd == 'D' || cmd == 'd') {
            compressor.setSequence(vals);
            compressor.decode();
            cout << "Decompressed sequence:" << endl;
            compressor.displaySequence();
        }
    }

    return 0;
}
