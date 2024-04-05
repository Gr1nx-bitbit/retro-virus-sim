#include <iostream>
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
        Node* next;
    public:
        Node() {
            gene.startIndex = 0;
            gene.endIndex = 0;
            gen = false;
            next = nullptr;
        }

        Node(genePosition geen) {
            gene = geen;
            gen = true;
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

        Node* getNext(void) {
            return next;
        }

        void setNext(Node* nxt) {
            next = nxt;
        }
};

Node* examine(string sequence, int length);
rnaAndExcision matureRNA(string sequence, int length, Node* head);
void compare(string premature, int length, rnaAndExcision preAmino);
string dnaTOrna(string dna, int length);

int main(void) {
    string sequence = "acgatttcgatgtgccggtttattatatagccgcggcccaccttatagccgccggtataccaccgggcattggctacctcgcatggcaacgattctctca";
    string input = dnaTOrna(sequence, sequence.length());
    Node* output = examine(input, input.length());
    rnaAndExcision preAmino = matureRNA(input, input.length(), output);
    compare(input, input.length(), preAmino);
    return 0;
}

Node* examine(string sequence, int length) {
   bool start = false;
   Node* head = new Node();
   Node* cursor = head;
   genePosition genCur;

    for (int i = 0; i < length; i++)
    {
        if (!start && (sequence[i] == 'g') && (sequence[i+1] == 'u') && (i < (length - 1))) {
            start = true;
            genCur.startIndex = i;
        } else if (start && (sequence[i] == 'a') && (sequence[i+1] == 'g') && (i < (length - 1))) {
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

//takes in a string matureRNA and converts it into a string of
//polypeptide chains. Takes codons mapped to their respective
//amino acid and replaces the codon with the amino acid
string peptideSynthesis(string matureRna) {

}