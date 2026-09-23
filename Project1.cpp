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
                _seq.push_back(v);
            }
        }
        
        _maxSymbol = 127;

    }

    
    void train(int k) {
        
    for (int merge = 0; merge < k; merge++) {

        for (int i = 0; i < 640; i++)
            for (int j = 0; j < 640; j++)
                _freq[i][j] = 0;

        for (int i = 0; i + 1 < (int)_seq.size(); i++) {
            int a = _seq[i];
            int b = _seq[i + 1];
            _freq[a][b]++;
        }

        int bestA = -1;
        int bestB = -1;
        int bestFreq = 0;

        for (int a = 0; a < 640; a++) {
            for (int b = 0; b < 640; b++) {
                if (_freq[a][b] > bestFreq) {
                    bestFreq = _freq[a][b];
                    bestA = a;
                    bestB = b;
                }
            }
        }

        if (bestFreq <= 1)
            break;

        _maxSymbol++;
        int newSymbol = _maxSymbol;

        _rules[_ruleCount][0] = bestA;
        _rules[_ruleCount][1] = bestB;
        _rules[_ruleCount][2] = newSymbol;
        _ruleCount++;

        vector<int> newSeq;

        int i = 0;
        while (i < (int)_seq.size()) {
            if (i + 1 < (int)_seq.size() &&
                _seq[i] == bestA &&
                _seq[i + 1] == bestB) {

                newSeq.push_back(newSymbol);
                i += 2;
            }
            else {
                newSeq.push_back(_seq[i]);
                i++;
            }
        }

        _seq = newSeq;
    }
}

    
    void encode() {
        for (int r = 0; r < _ruleCount; r++) {
        int a = _rules[r][0];
        int b = _rules[r][1];
        int z = _rules[r][2];

        vector<int> newSeq;

        int i = 0;
        while (i < (int)_seq.size()) {
            if (i + 1 < (int)_seq.size() &&
                _seq[i] == a &&
                _seq[i + 1] == b) {

                newSeq.push_back(z);
                i += 2;
            }
            else {
                newSeq.push_back(_seq[i]);
                i++;
            }
        }

        _seq = newSeq;
    }
}

    void decode() {
        for (int r = _ruleCount - 1; r >= 0; r--) {
        int a = _rules[r][0];
        int b = _rules[r][1];
        int z = _rules[r][2];

        vector<int> newSeq;

        for (int i = 0; i < (int)_seq.size(); i++) {
            if (_seq[i] == z) {
                newSeq.push_back(a);
                newSeq.push_back(b);
            }
            else {
                newSeq.push_back(_seq[i]);
            }
        }

        _seq = newSeq;
    }
}
    
    void displaySequence() {
        for (int i = 0; i < (int)_seq.size(); i++) {
        if (i > 0)
            cout << " ";

        cout << _seq[i];
    }

    cout << endl;
    }
    
    void displayRules() {
        for (int i = 0; i < _ruleCount; i++) {
        cout << _rules[i][0] << " "
             << _rules[i][1] << " "
             << _rules[i][2] << endl;
        }
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
        stringstream ss(line); 
        ss >> cmd;
        int v;
        while (ss >> v) {
            vals.push_back(v);
        }

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
