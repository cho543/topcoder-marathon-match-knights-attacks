// C++11
#include "../KnightsAttacks.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < int(v.size()); ++i)
        cin >> v[i];
}

int main() {
    KnightsAttacks ka;
    int S;
    cin >> S;
    vector<string> board(S);
    getVector(board);

    vector<string> ret = ka.placeKnights(board);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
}
