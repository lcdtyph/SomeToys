#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <queue>
#include <cstdio>
using namespace std;

template<class T>
struct HuffmanNode {
    float p;
    T sym;
    HuffmanNode *left, *right;

    HuffmanNode(T sym, float p, HuffmanNode *l = nullptr, HuffmanNode *r = nullptr)
        :p(p), sym(sym), left(l), right(r) { }

    ~HuffmanNode() {
        delete left;
        delete right;
    }

};

using NodeType = HuffmanNode<int>;
void getHuffmanCode(vector<pair<int, string>> &result, NodeType *root, string code = "");

int main() {
    vector<float> freq{0.2, 0.19, 0.18, 0.17, 0.15, 0.1, 0.01};// {0.4, 0.18, 0.1, 0.1, 0.07, 0.06, 0.05, 0.04};
    auto cmp = [](NodeType *a, NodeType *b) { return a->p > b->p; };
    priority_queue<NodeType *, vector<NodeType *>, decltype(cmp)> nodes(cmp);

    for_each(freq.begin(), freq.end(), [&nodes](float p){
                static int i;
                nodes.push(new NodeType(i++, p));
            });

    for (size_t i = 1; i < freq.size(); ++i) {
        NodeType *right = nodes.top();
        nodes.pop();
        NodeType *left = nodes.top();
        nodes.pop();
        NodeType *root = new NodeType(-1, left->p + right->p, left, right);
        nodes.push(root);
    }
    vector<pair<int, string>> result;
    getHuffmanCode(result, nodes.top());
    sort(result.begin(), result.end(), [](const auto &a, const auto &b) {return a.first < b.first;});

    float avg = 0.0;
    puts("sig       p(x)     W            K");
    puts("---------------------------------");
    for (const auto &line : result) {
        printf("%-6d    %-6g   %-11s  %-3lu\n", line.first, freq[line.first], line.second.c_str(), line.second.size());
        avg += freq[line.first] * line.second.size();
    }
    puts("---------------------------------");
    printf("avg(K) = %g sym/sig\n", avg);
    delete nodes.top();
    return 0;
}

void getHuffmanCode(vector<pair<int, string>> &result, NodeType *root, string code) {
    if (!root->left && !root->right) {
        result.emplace_back(root->sym, code);
        return;
    }

    getHuffmanCode(result, root->left, code + "0");
    getHuffmanCode(result, root->right, code + "1");
}

