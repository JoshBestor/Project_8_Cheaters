#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <cctype>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <bits/stdc++.h>

using namespace std;

struct Node {
    int index;
    Node *next;
};

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


unsigned long getKey(string chunk, unsigned long HASH_TABLE_SIZE){              //Hash function
    unsigned long result = 0;
    for(int i=0;i<chunk.length();i++){
        result += chunk[i] * pow(3,i);
        result = result % HASH_TABLE_SIZE;
    }
    return result;
}


void hash(string chunk, unsigned long HASH_TABLE_SIZE, Node *hashTable[], int index){
    unsigned long key = getKey(chunk, HASH_TABLE_SIZE);

    Node *temp = new Node;
    temp->index = index;
    temp->next = hashTable[key];
    hashTable[key] = temp;
}


void graphKey(Node *hashTable[], int key, int collisions[][1400]){            //need to change based on what size it is
    Node *head;
    Node *other;
    for(head = hashTable[key]; head != NULL; head = head->next){
        for(other = head->next; other != NULL; other = other->next){
            if(head->index > other->index){
                collisions[head->index][other->index] += 1;
            }else if(head->index < other->index){
                collisions[other->index][head->index] += 1;
            }
        }
    }
    head = NULL;
    other = hashTable[key];
    for(other; other != NULL; other = other->next){
        if(head != NULL){
            delete head;
        }
        head = other;
    }
    delete head;
}


int main(int argc, char *argv[])
{
    string dir = string(argv[1]);
    vector<string> files = vector<string>();

    getdir(dir,files);
    files.erase(files.begin(),files.begin()+2);

    const int CHUNK_LENGTH = atoi(argv[2]);
    const unsigned long HASH_TABLE_SIZE = 1000003;

    Node *hashTable[HASH_TABLE_SIZE] = {NULL};
    int collisions[files.size()][1400];
    memset(collisions, 0, sizeof(collisions));


    for (unsigned int i = 0;i < files.size();i++) {         //opens every file
        if(files[i] != ".txt") {
            cout << files[i] << endl;
            vector <string> chunk = vector<string>();

            string input = "sm_doc_set";
            input += "/";
            input += files[i];

            ifstream inFile;
            inFile.open(input.c_str());

            string s;
            inFile >> s;


            while (!inFile.eof()) {                      //gets every chunk for each file and hashes them
                if (chunk.size() > CHUNK_LENGTH) {
                    chunk.erase(chunk.begin());
                    chunk.push_back(s);
                } else {
                    chunk.push_back(s);
                }

                inFile >> s;
                string output = "";

                if (chunk.size() > CHUNK_LENGTH - 1) {
                    for (int j = 0; j < CHUNK_LENGTH; j++) {

                        for (int k = 0; k < chunk[j].size(); k++) {        //removes non alphanum
                            if (ispunct(chunk[j][k])) {
                                chunk[j].erase(k);
                            }
                        }
                        transform(chunk[j].begin(), chunk[j].end(), chunk[j].begin(), ::tolower);

                        output += chunk[j];
                    }
                    hash(output, HASH_TABLE_SIZE, hashTable, i);
                }
            }
        }
    }


    for(int i=1;i<HASH_TABLE_SIZE;i++){                         //puts the correct values in the collisions table
        if(hashTable[i] != NULL){
            graphKey(hashTable, i, collisions);
        }
    }


    int numCols = atoi(argv[3]);
    for(int i=1;i<files.size();i++){                        //prints out the files with more than argv3 chunks in common
        for(int j=0;j<i;j++){
            if(collisions[i][j] > numCols){
                cout << files[i] << ", " << files[j] << " - " << collisions[i][j] << endl;
            }
        }
    }


    return 0;
}