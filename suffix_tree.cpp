#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

struct Node {
	struct Node *arr[256];
	string label;	
};

struct Node *root;

void insert(string s, int start_index, int length) {
	int i = start_index;
	struct Node *curr = root;
	while(i < length) {
		if(curr->arr[s[i]] == NULL) {
			curr->arr[s[i]] = (struct Node *) malloc(sizeof(struct Node));
			curr = curr->arr[s[i]];
			while(i < length) {
				curr->label += s[i];
				i++;
			}
			return;
		}

		int j = 0, x = i;
		struct Node *temp = curr->arr[s[i]];
		while(j < temp->label.length() && i < length && temp->label[j] == s[i]) {
			i++;
			j++;
		}

		if(i == length) {
			return;
		}
		
		int k;
		if(j == temp->label.length()) {
			int cnt = 0;
			for(k = 'a'; k <= 'z'; k++) {
				if(temp->arr[k] == NULL) {
					cnt++;
				}
			}

			if(cnt == 27) {
				while(i < length) {
					temp->label = temp->label + s[i];
					i++;
				}
			} else {
				curr = temp;
			}			

		} else {
			struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
			k = 0;
			while(k < j) {
				newNode->label = newNode->label + temp->label[k];
				k++;
			}

			newNode->arr[s[j]] = (struct Node *) malloc(sizeof(struct Node));
			while(k < temp->label.length()) {
				newNode->arr[s[j]]->label += temp->label[k]; 
				k++;
			}

			for(k = 'a'; k <= 'z'; k++) {
				newNode->arr[s[j]]->arr[k] = temp->arr[k];
			}

			curr->arr[s[x]] = newNode;
			curr = curr->arr[s[x]];
		
		}

	}

}
 
int main(int argc, char **argv) {
	root = (struct Node *) malloc(sizeof(struct Node));
	
	ifstream file;
    	file.open(argv[1]);
    	string input, word;

	while(file >> word) {
		input += word;
		word.clear();
	}

	//cout<<input<<endl;

        /*ifstream ifs(argv[1]);
        input.assign( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()    ) );
	input += "}";*/

	for(int i = 0; i < input.length(); i++) {
		insert(input, i, input.length());
	}
	
	string pattern;
	string exit = "q";
	int found = 1;
	cout<<"Enter a pattern to search in the suffix tree"<<endl;
	while(1) {
		getline(cin, pattern);
		if(exit.compare(pattern) == 0) {
			break;
		}
		found = 1;
		struct Node *curr = root;
		int i = 0, j = 0;
		while(i < pattern.length()) {
			struct Node *temp = curr->arr[pattern[i]];
			if(temp == NULL) {
				cout<<"Pattern DOES NOT EXIST in the suffix tree"<<endl;
				found = 0;
				break;
			}
			j = 0;
			while(j < temp->label.length() && i < pattern.length()) {
				if(temp->label[j] == pattern[i]) {
					i++;
					j++;
				} else {
					cout<<"Pattern does DOES NOT EXIST in the suffix tree"<<endl;
					found = 0;
					break;
				}
			}
			if(found == 0) {
				break;
			}
			
		}
		
		if(found == 1) {
			cout<<"Pattern EXISTS in the suffix tree"<<endl;
		}
	}

	return 0;
}
