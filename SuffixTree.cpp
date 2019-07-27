class SuffixTree {
public:
    struct Node {
    public:
        int start, *end, size;
        Node *slink;
    private:
        Node *next[27];
    public:
        Node(int s, int *e, Node *sl) : start(s), end(e), slink(sl), size(0) {
            for (int i = 0; i < 27; i++)
                next[i] = NULL;
        }

        Node() {
            Node(-1, new int(-1), NULL);
        }

        int edge_length() {
            return *end - start;
        }

        void setNext(char c, Node *n) {
            if (n) {
                int idx = (c == '$') ? 26 : (c - 'a');
                if (next[idx] == NULL)
                    size++;
                next[idx] = n;
            }
        }

        Node *getNext(char c) {
            int idx = (c == '$') ? 26 : (c - 'a');
            return next[idx];
        }
    };

    Node *root, *activeNode, *needSuffixLink;
    int activeEdge, activeLength, remainder, position;
    string text;

    SuffixTree(string s) : activeEdge(0), activeLength(0), remainder(0), position(-1) {
        root = activeNode = new Node(-1, new int(-1), NULL);
        root->slink = root;
        needSuffixLink = root;
        text = s + "$";
    }

    ~SuffixTree() {
        clear(root);
    }

    void clear(Node *n) {
        if (n) {
            for (char i = 0; i < 27; i++) {
                clear(n->getNext(i + 'a'));
            }
            delete (n);
        }
    }

    bool walkDown(Node *node) {
        if (activeLength >= node->edge_length()) {
            activeEdge += node->edge_length();
            activeLength -= node->edge_length();
            activeNode = node;
            return true;
        }
        return false;
    }

    void addPosition() {
        position++;
        needSuffixLink = root;
        remainder++;
        char c = text[position];
#define active_edge text[activeEdge]
        while (remainder > 0) {
            if (activeLength == 0)
                activeEdge = position;
            if (activeNode->getNext(active_edge) == NULL) {
                activeNode->setNext(active_edge, new Node(position, &position, root));
                if (needSuffixLink != root)
                    needSuffixLink->slink = activeNode;
                needSuffixLink = activeNode;
            } else {
                Node *nxt = activeNode->getNext(active_edge);
                if (walkDown(nxt))
                    continue;
                char temp = text[nxt->start + activeLength - 1];
                if (text[nxt->start + activeLength] == c) {
                    activeLength++;
                    if (needSuffixLink != root)
                        needSuffixLink->slink = activeNode;
                    needSuffixLink = activeNode;
                    break;
                }
                Node *split = new Node(nxt->start, new int(nxt->start + activeLength), root);
                activeNode->setNext(active_edge, split);
                Node *leaf = new Node(position, &position, root);
                split->setNext(c, leaf);
                nxt->start += activeLength;
                split->setNext(text[nxt->start], nxt);
                if (needSuffixLink != root)
                    needSuffixLink->slink = split;
                needSuffixLink = split;
            }
            remainder--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = position - remainder + 1;
            } else {
                activeNode = activeNode->slink;
            }
        }
    }

    string longestSubstring() {
        vector<Node *> longest;
        vector<Node *> trial;
        int maxLength = 0;
        int length = 0;
        longestSubstring(root, longest, trial, length, maxLength);
        string s = "";
        for (Node *n : longest) {
            s += text.substr(n->start, *n->end - n->start);
        }
        return s;
    }

    void longestSubstring(Node *node, vector<Node *> &longest, vector<Node *> &trial, int &length, int &maxLength) {
        if (node->size > 1) {
            if (length > maxLength) {
                maxLength = length;
                longest = trial;
            }
            for (int i = 0; i < 27; i++) {
                Node *temp = node->getNext(i + 'a');
                if (temp) {
                    trial.push_back(temp);
                    length += (*temp->end - temp->start);
                    longestSubstring(temp, longest, trial, length, maxLength);
                    length -= (*temp->end - temp->start);
                    trial.pop_back();
                }
            }
        }
    }
};
