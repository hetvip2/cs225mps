/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num){
    for (int i = 0; i < num; i++) {
        vec.push_back(-1);
    }

}

int DisjointSets::find(int elem){
    if (vec[elem] < 0) {
        return elem;
    } else {
        vec[elem] = find(vec[elem]);
        return vec[elem];
    }
}

void DisjointSets::setunion(int a, int b){
    if (find(a) == find(b)){
        return;
    }
    if (-vec[find(a)] >= -vec[find(b)]) {
        vec[find(a)] += vec[find(b)];
        vec[find(b)] = find(a);
    } else {
        vec[find(b)] += vec[find(a)];
        vec[find(a)] = find(b); 
    }
}

int DisjointSets::size(int elem){
    return -vec[find(elem)]; 
}

int DisjointSets::getValue(int elem) const{
    return vec[elem];
}

