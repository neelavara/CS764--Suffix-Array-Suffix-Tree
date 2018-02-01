#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include <fstream>

#define MAXLEN 100000
#define MAXSTEPS 20

using namespace std;

typedef struct suffix_t {
	int index;
	int rank;
	int next_rank;
}suffix;

typedef struct comparator_t {
	bool operator() (suffix& a, suffix& b) {
		if(a.rank < b.rank) {
			return true;
		}
		if(a.rank == b.rank && a.next_rank < b.next_rank) {
			return true;
		}
		return false;
	}
}comparator;


int main(int argc, char **argv) {
	string input, pattern;
	ifstream ifs(argv[1]);
	input.assign( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()    ) );
	//cout<<input<<endl;
	
	int i,j;
	int n = input.length(), num_steps = (int)log2 (n) + 2;
		
	//Initialize all the suffix structures
	vector<suffix> suffixes;
	for(i = 0; i < n; i++) {
		suffix temp;
		temp.index = 0;
		temp.rank = MAXLEN + 1;
		temp.next_rank = MAXLEN + 1;
		suffixes.push_back(temp);
	}
		
	//Initialize prev, a vector that holds previous ranks
	vector< vector<int> > prev;
	for(i = 0; i < num_steps ; i++) {
		prev.push_back(vector<int>(n, MAXLEN + 1));
	}
	
	//Sort according to first two characters
	for(i = 0; i < n; i++) {
		prev[0][i] = (int) input.at(i);
	}
	
	comparator c;
	int step = 0;
	int len;
	for(len = 1, step = 1; len < n; step++, len *= 2) {
		for(i = 0; i < n; i++) {
			suffixes[i].rank = prev[step-1][i];
			suffixes[i].next_rank = (i + len < n) ? prev[step-1][i+len] : -1;
			suffixes[i].index = i;		
		}
		sort(suffixes.begin(), suffixes.end(), c);
		for(i = 0; i < n; i++) {
			if(i > 0 && suffixes[i].rank == suffixes[i-1].rank && suffixes[i].next_rank == suffixes[i-1].next_rank) {
				prev[step][suffixes[i].index] = prev[step][suffixes[i-1].index];
			} else {
				prev[step][suffixes[i].index] = i;
			} 
		}
	}
	
	/*for(i = 0; i < n; i++) {
		cout<<suffixes[i].index<<" "<<input.substr(suffixes[i].index)<<endl;
	}*/

	//At this point we have suffix array from suffixes[0].index......suffixes[n-1].index
	//To search pattern in this array, we can use Binary Search
	cout<<"Please enter pattern to be searched in the input file"<<endl;
	string exit = "q";
	while(1) {
		int found = 0;
		string pattern;
		getline(cin, pattern);
		if(exit.compare(pattern) == 0) {
			break;
		}
		int l = 0, r = n-1;
		int len = pattern.length();
		cout<<"Pattern to be searched is: "<<pattern<<endl;
		while(l <= r) {
			int m = l + (r-l)/2;
			//cout<<"m = "<<m<<" "<<input.substr(suffixes[m].index)<<endl;
			//cout<<suffixes[m].index<<" "<<pattern<<endl;
			int comp = pattern.compare(input.substr(suffixes[m].index, len));
			if(comp == 0) {
				found = 1;
				cout<<"Input pattern found at index: "<<suffixes[m].index<<endl;
				break;
			}

			if(comp < 0) {
				r = m-1;
			} else {
				l = m+1;
			}
		}

		if(found == 0) {
			cout<<"The input pattern was not found"<<endl;
		}
		
	}

	return 0;
}

