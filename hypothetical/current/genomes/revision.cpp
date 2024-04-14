#include <iostream>
#include "AminoCodon.h"
#include "CodonTree.h"
#include <fstream>
#include <vector>
using namespace std;

struct genePosition {
    int startIndex;
    int endIndex;
};

struct rnaAndExcision {
    string* excised;
    genePosition* indecies;
    string rna;
};

class Node {
    private:
        genePosition gene;
        bool gen;
        AminoCodon acPair;
        Node* next;
    public:
        Node() {
            gene.startIndex = 0;
            gene.endIndex = 0;
            gen = false;
            acPair.aminoAcid = "";
            acPair.codon = "";
            acPair.startCodon = false;
            acPair.terminationCodon = false;
            next = nullptr;
        }

        Node(genePosition geen) {
            gene = geen;
            gen = true;
            acPair.aminoAcid = "";
            acPair.codon = "";
            acPair.startCodon = false;
            acPair.terminationCodon = false;
            next = nullptr;
        }

        Node(AminoCodon acPair) {
            gene.startIndex = 0;
            gene.endIndex = 0;
            gen = false;
            this->acPair = acPair;
            next = nullptr;
        }

        // Node(genePosition geen, Node* nxt) {
        //     gene = geen;
        //     isGene = true;
        //     next = nxt;
        // }

        genePosition getGene(void) {
            return gene;
        }

        void setGene(genePosition geen) {
            gene  = geen;
            gen = true;
        }

        bool isGene(void) {
            return gen;
        }

        AminoCodon getAmino(void) {
            return acPair;
        }

        void setAmino(AminoCodon acPair) {
            this->acPair = acPair;
        }

        Node* getNext(void) {
            return next;
        }

        void setNext(Node* nxt) {
            next = nxt;
        }
};

/*
––––––––––––––––––––––––––
*   FUNCTION SIGNATURES  *
––––––––––––––––––––––––––
*/

Node* examine(string sequence, int length);
rnaAndExcision matureRNA(string sequence, int length, Node* head);
void compare(string premature, int length, rnaAndExcision preAmino);
string dnaTOrna(string dna, int length);
AminoCodon parse(string input);
vector<Node*> peptideSynthesis(string matureRna, int length, CodonTree* head);

/*
––––––––––––––––––––––––––
*   FUNCTION SIGNATURES  *
––––––––––––––––––––––––––
*/

int main(void) {
    //takes a genetic sequence and returns the mRNA sequence with the introns excised and compares the original mRNA to 
    //the solely exon mRNA
    string sequence = "gggctcctgctcgttctcacggtatgcatagggcagctatactcaagtataattcaaccacacattgatgagacgttgtcgtacttctgctgaatgctatgaaagtgagcgatcgtggaatattgagcgcgacccgagtctggcctcggcataggcgcgaggaaaggtagtgttgggcggccgttaataaccttcgaccgaccatcttcattccgaatttctggtgagtttagtcaggtcgaccgtcccctggatgagccctatcgttccatcatgatctaccagtcgtactatggatgtatagaaatacataaaaagtatagtcctcacatgacggcacggaccacgaactgaataaacggtgttgtctcgttggaaatgttttcggccgtcacgtcgataacatgtttcaagagtcctcagtcttaccctggtatgcgttcagtattcgatgggtaggcaagagtcccgttaattggataaatgatggggacaaaatgtttaaaccatactaatttgggactgcaaaaattatgtgcaaaactacgagtcgtttttcctcagcgtctaaccctgggggcttgacaagattctctacccccgtcgacgaataaaatggggtgttaggcacttgagtatcccagagcatctattgacagtgagagccgcacggaatgctgcacgagaagctgccgcaacggcaactagctgaatccacgacgcttctcagaaaattgagtaggtcgggaagaattctggattcgctattttcagtctcgtgctagcgcgataaattgtaacatggactgctataaagcggaagaaaattccatctttgctctaacttatccctgctatctggagtcctcggatgttagggttgtcccatcatttacattctaatgatcctggtaagtgagatgcatgtggtatctagtgagctaaaacagtcgcgcactttgttgctagtcaatgcgtaaaccagcagacattatgtagc";
    string rnaSequence = dnaTOrna(sequence, sequence.length());
    Node* output = examine(rnaSequence, rnaSequence.length());
    rnaAndExcision preAmino = matureRNA(rnaSequence, rnaSequence.length(), output);
    //compare(rnaSequence, rnaSequence.length(), preAmino);

    //create a codonTree to store all the amino acids
    CodonTree* head = new CodonTree();
    string aminoCodontext;
    ifstream myFile("aminoCodon.txt");

    //add all the amino acids and their path to the tree
    while(getline(myFile, aminoCodontext, '\n')) {
        AminoCodon acPair = parse(aminoCodontext);
        head->addAminoCodon(head, acPair, -1);
    }

    //this isn't working right now
    vector<Node*> peptides = peptideSynthesis(preAmino.rna, preAmino.rna.length(), head);
    Node* cursor;
    int index = 0;
    cout << peptides[5]->getAmino().aminoAcid << endl;
    // for (cursor = peptides[index]; cursor; cursor = cursor->getNext()) {
    //     if (cursor->getAmino().startCodon) {
    //         cout << "Start amino: " << cursor->getAmino().aminoAcid << endl;
    //     } else if (cursor->getAmino().terminationCodon) {
    //         cout << "Termination codon: " << cursor->getAmino().aminoAcid << endl;
    //     } //else {
    //     //     cout << "regular codon: " << cursor->getAmino().aminoAcid << endl;
    //     // }
    // }

    return 0;
}

Node* examine(string sequence, int length) {
   bool start = false;
   Node* head = new Node();
   Node* cursor = head;
   genePosition genCur;
   genCur.startIndex = -1;
   genCur.endIndex = -1;

    for (int i = 0; i < length; i++)
    {
        if (!start && (sequence[i] == 'g') && (sequence[i+1] == 'u') && (i < (length - 1)) && (i != genCur.endIndex)) {
            start = true;
            genCur.startIndex = i;
        } else if (start && (sequence[i] == 'a') && (sequence[i+1] == 'g') && (i < (length - 1)) && (i != genCur.startIndex)) {
            genCur.endIndex = i+1;
            if (!cursor->isGene()) {
                cursor->setGene(genCur);
            } else {
                Node* next = new Node(genCur);
                cursor->setNext(next);
                cursor = cursor->getNext();
            }
            start = false;
        }
    }

    return head;
}

rnaAndExcision matureRNA(string sequence, int length, Node* head) {
    string matureRNA;
    Node* cursor;
    int index = 0;
    for (cursor = head; cursor; cursor = cursor->getNext()) {
        index++;
    }
    rnaAndExcision returnRNA;
    returnRNA.excised = new string[index];
    returnRNA.indecies = new genePosition[index];
    string excisions[index];
    genePosition pairs[index];
    index = 0;
    for (cursor = head; cursor; cursor = cursor->getNext()) {
        pairs[index] = cursor->getGene();
        index++;
    }

    
    index = 0;
    for (int i = 0; i < length; i++) {
        if (i == pairs[index].startIndex) { 
            excisions[index] += sequence[i];
            returnRNA.indecies[index].startIndex = i;
            continue;
        }
        if ((i > pairs[index].startIndex) && (i < pairs[index].endIndex)) {
            excisions[index] += sequence[i];
            continue;
        }
        if (i == pairs[index].endIndex) {
            excisions[index] += sequence[i];
            returnRNA.indecies[index].endIndex = i;
            index++;
            continue;
        }
        matureRNA += sequence[i];
    }
    
    index = 0;
    for (string render : excisions) {
        returnRNA.excised[index] = render;
        index++;
    }

    returnRNA.rna = matureRNA;
    return returnRNA;
}

void compare(string premature, int length, rnaAndExcision preAmino) {
    cout << premature << endl;
    int index = 0;
    for (int i = 0; i < length; i++) {
        if (i == preAmino.indecies[index].startIndex) {
            cout << '^';
        } else if (i > preAmino.indecies[index].startIndex && i < preAmino.indecies[index].endIndex) {
            cout << '-';
        } else if (i == preAmino.indecies[index].endIndex) {
            cout << '^';
            index++;
        } else {
            cout << " ";
        }
        if (i == (length - 1)) {
            cout << endl;
        }
    }

    index = 0;
    int rnaIndex = 0;
    for (int i = 0; i < length; i++) {
        if (i == preAmino.indecies[index].startIndex) {
            cout << ' ';
        } else if (i > preAmino.indecies[index].startIndex && i < preAmino.indecies[index].endIndex) {
            cout << ' ';
        } else if (i == preAmino.indecies[index].endIndex) {
            cout << ' ';
            index++;
        } else {
            cout << preAmino.rna[rnaIndex];
            rnaIndex++;
            if (rnaIndex == (preAmino.rna.length())) {
                cout << endl;
            }
        }
    }
}

string dnaTOrna(string dna, int length) {
    string prematureRNA;
    for (int i = 0; i < length; i++) {
        if (dna[i] == 'a') {
            prematureRNA += 'u';
        } else if (dna[i] == 't') {
            prematureRNA += 'a';
        } else if (dna[i] == 'c') {
            prematureRNA += 'g';
        } else {
            prematureRNA += 'c';
        }
    }

    return prematureRNA;
}

AminoCodon parse(string input) {
    bool colon = false;
    AminoCodon tmp;
    tmp.startCodon = false;
    tmp.terminationCodon = false;
    for (int index = 0; index < input.length(); index++) {
        if (input[index] == ':') {
            colon = true;            
        } else if (!colon && input[index] != ' ') {
            tmp.codon += input[index];
        } else {
            if (input[index] == '!') {
                tmp.startCodon = true;
                tmp.terminationCodon = false;
            } else if (input[index] == '/') {
                tmp.terminationCodon = true;
                tmp.startCodon = false;
            } else {
                tmp.aminoAcid += input[index];
            }
        }
    }

return tmp;
}

//takes in a string matureRNA and converts it into a string of
//polypeptide chains. Takes codons mapped to their respective
//amino acid and replaces the codon with the amino acid
vector<Node*> peptideSynthesis(string matureRna, int length, CodonTree* head) {
    vector<Node*> sequences;
    Node* node = new Node();
    sequences.push_back(node);
    Node* cursor = sequences[0];
    string codon = "";
    bool start = false;

    for (int i = 0; i < length; i++) {
        if ((i % 3 == 0) && (i != 0)) {
            AminoCodon amino = head->getAminoCodon(codon, head);
            if (amino.startCodon && (cursor->getAmino().aminoAcid == "") && !start) {
                cout << "Cursor is: " << cursor->getAmino().aminoAcid << endl;
                cout << "Setting cursor to: " << amino.aminoAcid << endl;
                cout << "Starting acid: " << amino.startCodon << endl;
                cout << "Terminating acid: " << amino.terminationCodon << endl;
                cout << 0 << endl;
                cursor->setAmino(amino);
                start = true;
            } else if (amino.startCodon && (cursor->getAmino().aminoAcid != "") && !start) {
                cout << "Cursor is: " << cursor->getAmino().aminoAcid << endl;
                cout << "Setting cursor to: " << amino.aminoAcid << endl;
                cout << "Starting acid: " << amino.startCodon << endl;
                cout << "Terminating acid: " << amino.terminationCodon << endl;
                cout << 1 << endl;
                Node* next = new Node(amino);
                cursor->setNext(next);
                cursor = cursor->getNext();
                start = true;
            } else if (amino.terminationCodon && (cursor->getAmino().aminoAcid == "") && start) {
                cout << "Cursor is: " << cursor->getAmino().aminoAcid << endl;
                cout << "Setting cursor to: " << amino.aminoAcid << endl;
                cout << "Starting acid: " << amino.startCodon << endl;
                cout << "Terminating acid: " << amino.terminationCodon << endl;
                cout << 2 << endl;
                cursor->setAmino(amino);

                Node* increment = new Node();
                sequences.push_back(increment);
                cursor = increment;
                start = false;
            } else if (amino.terminationCodon && (cursor->getAmino().aminoAcid != "") && start) {
                cout << "Cursor is: " << cursor->getAmino().aminoAcid << endl;
                cout << "Setting cursor to: " << amino.aminoAcid << endl;
                cout << "Starting acid: " << amino.startCodon << endl;
                cout << "Terminating acid: " << amino.terminationCodon << endl;
                cout << 3 << endl;
                Node* next = new Node(amino);
                cout << 'a' << endl;
                cursor->setNext(next);

                cout << 'b' << endl;
                Node* increment = new Node();
                cout << 'c' << endl;
                sequences.push_back(increment);
                cout << 'd' << endl;
                cursor = increment;
                cout << 'e' << endl;
                start = false;
                cout << 'f' << endl;
            }
            cout << 'g' << endl;
            codon = matureRna[i];
        } else {
            codon += matureRna[i];
        }
    }

    return sequences;
}