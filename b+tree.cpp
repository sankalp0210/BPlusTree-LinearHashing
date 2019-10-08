#include<bits/stdc++.h>
using namespace std;

struct Bnode
{
    vector< pair<int, int> > nodes;
    vector<Bnode *> ptrs;
    Bnode * par;
    int totNodes, numNodes;
    bool isLeaf;
    Bnode(){
        isLeaf = true;
        totNodes = numNodes = 0;
        nodes.resize(4, {0, 0});
        ptrs.resize(5, NULL);
        par = NULL;
    }
};
Bnode * root;

void insertbyShift(pair<int, int> a, Bnode *cur, Bnode * left, Bnode * right)
{
    int num = (cur->numNodes);
    while(num>0 && a.first < (cur->nodes)[num-1].first){
        (cur->ptrs)[num+1]=(cur->ptrs)[num];
        (cur->nodes)[num] = (cur->nodes)[num-1];
        num--;
    }
    (cur->numNodes)++;
    (cur->ptrs)[num]= left;
    (cur->ptrs)[num+1] = right;
    (cur->nodes)[num] = a;
}

Bnode * getSplit(Bnode* cur, char typ){
    Bnode *p = new Bnode();
    (p->par) = (cur->par);
    (p->isLeaf) = (cur->isLeaf);
    int start = 0, end = (cur->numNodes-1)/2;

    if(typ == 'r'){
        start = (cur->numNodes-1)/2;
        end = (cur->numNodes);
        if(!(cur->isLeaf)) start++;
    }

    for(int i=start;i<end;i++){
        if(p->isLeaf)
            (p->totNodes)+=(cur->nodes)[i].second;
        (p->numNodes)++;
        (p->nodes)[i-start] = (cur->nodes)[i];
    }
    for(int i=start;i<=end;i++){
        (p->ptrs)[i-start]= (cur->ptrs)[i];
        if((cur->ptrs)[i]){
            (p->ptrs)[i-start]->par = p;
            (p->totNodes) += ((cur->ptrs)[i]->totNodes);
        }
    }
    return p;
}

void insertbySplit(Bnode * cur)
{
    Bnode * p = (cur->par);
    if( p == NULL){
        p = new Bnode();
        root = p;
        cur->par =  p;
        p->isLeaf = false;
    }

    Bnode * left = getSplit(cur, 'l');
    Bnode * right =  getSplit(cur, 'r');

    insertbyShift((cur->nodes)[(cur->numNodes-1)/2], p, left, right);
    if((p->numNodes) > 3) insertbySplit(p);    
}

bool isPresent(Bnode *x, int value){
    int i = 0;
    while(i < (x->numNodes) and (x->nodes)[i].first != value) i++;
    if(i<(x->numNodes)) {
        (x->nodes)[i].second++;
        return true;
    }
    return false;
}

void insert(pair<int, int> a){

    if(root == NULL){
        root = new Bnode();
        (root->totNodes)=1;
        (root->nodes)[0]= a;
        (root->numNodes)++;
        return;
    }

    Bnode * x = root;
    while(!(x->isLeaf)){
        int cur= 0;
        while(cur<(x->numNodes) && (x->nodes)[cur].first <= a.first) cur++;
        x->totNodes++;
        x = (x->ptrs)[cur];
    }
    x->totNodes++;

    if(!isPresent(x, a.first)){
        insertbyShift(a, x, NULL, NULL);
        if((x->numNodes) > 3) insertbySplit(x);
    }

}

int getCount(Bnode *x, int value, char typ){
    if(x == NULL) return 0;
    int ans = 0;
    if(x->isLeaf){
        for(int i=0;i<(x->numNodes);i++){
            if(typ == 'g' and (x->nodes)[i].first >= value)
                ans+= (x->nodes)[i].second;
            else if(typ == 'l' and (x->nodes)[i].first < value)
                ans+= (x->nodes)[i].second;
        }
        return ans;
    }
    int i = 0;
    if(typ == 'l'){
        while(i <(x->numNodes) and (x->nodes)[i].first < value)
            ans += ((x->ptrs)[i]->totNodes), i++;
    }
    else{
        i = (x->numNodes)-1;
        while(i >=0 and (x->nodes)[i].first >= value)
            ans += ((x->ptrs)[i+1]->totNodes), i--;
        i++;
    }
    return ans + getCount(x->ptrs[i], value, typ);

}

int rangeCount(int l, int r)
{
    if(root==NULL) return 0;
    Bnode *x = root;
    int ans = 0;
    while(!(x->isLeaf)){
        int st = 0, en = (x->numNodes);
        for(;st < (x->numNodes) and l>=(x->nodes)[st].first;st++);
        for(;en > 0 and r < (x->nodes)[en-1].first;en--);
        if(st < en){
            ans += getCount((x->ptrs)[st], l, 'g');
            ans += getCount((x->ptrs)[en], r, 'l');
            for(int i=st+1;i<en;i++)
                ans += (x->ptrs)[i]->totNodes;
            return ans;
        }
        else
            x = (x->ptrs)[st];
    }
    for(int i=0;i<(x->numNodes);i++){
        if((x->nodes)[i].first < r and (x->nodes)[i].first >= l)
            ans += (x->nodes)[i].second;
    }
    return ans;
}

void solve(string &s)
{
    int sp = s.find(" ");
    string cmd = s.substr(0, s.find(" ")), vals = s.substr(sp+1, s.size()-sp-1);

    if(cmd=="INSERT")
        insert({stoi(vals), 1});
    else if(cmd=="FIND")
        cout<<(rangeCount(stoi(vals), stoi(vals)+1)?"YES":"NO")<<endl;
    else if(cmd=="COUNT")
        cout<<to_string(rangeCount(stoi(vals), stoi(vals)+1))<<endl;
    else if(cmd == "RANGE"){
        int sp = vals.find(" ");
        int val1 = stoi(vals.substr(0, sp)), val2 = stoi(vals.substr(sp+1, vals.size()-sp-1));
        cout<<to_string(rangeCount(val1, val2+1))<<endl;
    }
    cout<<"";
}

int main(int argc, const char *argv[]){
    if(argc!=2){
        cout<<"Enter 1 arguments- filename\n";
        return 0;
    }
    ifstream infile(argv[1]);
    string s;
    while(getline(infile,s)){
        solve(s);
    }
    return 0;
}