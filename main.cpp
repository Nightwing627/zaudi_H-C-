#include <cstdlib>
#include <exception>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;


struct node {
    string key;
    string val;
    int compare(node* b) {
        return this->key.compare(b->key);
    }
};


struct link {
    node it;
    link* prev = NULL;
    link* next = NULL;
};

class listmap {
private:
    link* root;
public:
    listmap() {
        this->root = new link();
    };
    listmap(const link* root) {
        this->root = const_cast<link*>(root);
    };
    listmap operator=(const link* root) {
        this->root = const_cast<link*>(root);
        return *this;
    };
    listmap( link* root ) {
        this->root = root;
    };
    listmap operator=( link* root ) {
        this->root = root;
        return *this;
    };
    ~listmap() {
        delete root;
    };
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = link;
        using pointer = link*;  // or also value_type*
        using reference = link&;  // or also value_type&
        
        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr++; return *this; }

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
    };

    public:

        // Iterator definition 
        Iterator begin() { 
            return Iterator(this->root->next);
        }
        Iterator end() {
            link* temp = this->root->next;
            while (temp->next)
            {
                temp = temp->next;
            }
            return Iterator(temp);
        } // 200 is out of bounds
        Iterator insert(node& it) {
            link* temp = this->root;
            while (temp->next != NULL && temp->next->it.compare(&it)) {
                temp = temp->next;
            }
            if (temp->next == NULL) {
                temp->next = new link();
                temp->next->it = it;
                temp->next->prev = temp;
            }
            else {
                temp->next->it.val = it.val;
            }
            return Iterator(temp->next);
        }
        Iterator find() {
            link* temp = this->root->next;
            while (temp->next)
            {
                temp = temp->next;
            }
            return Iterator(temp);
        } // 200 is out of bounds
};
void addOrReplace(link* root, node it) {
    link* temp = root;
    while (temp->next != NULL && temp->next->it.compare(&it)) {
        temp = temp->next;
    }
    if (temp->next == NULL) {
        temp->next = new link();
        temp->next->it = it;
        temp->next->prev = temp;
    }
    else {
        temp->next->it.val = it.val;
    }
    return;
}

void delnode(link* root, string key) {
    link* temp = root;
    while (temp->next != NULL && key.compare(temp->next->it.key)) {
        temp = temp->next;
    }
    if (temp->next == NULL) {
        return;
    }
    temp->prev->next = temp->next;
    return;
}
string trim(string tp) {
    // Trim line
    tp.erase(tp.begin(), std::find_if(tp.begin(), tp.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));

    tp.erase(std::find_if(tp.rbegin(), tp.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), tp.end());
    return tp;
}
void sortlink(link* root) {
    link* indicator = root->next;
    while (indicator) {
        link* next = indicator->next;
        while (next) {
            if (indicator->it.compare(&next->it) > 0) {
                link* temp = next;
                indicator->prev->next = next;
                indicator->next->prev = next;
                next->prev = indicator->prev;
                next->next = indicator->next;
                temp->prev->next = indicator;
                temp->next->prev = indicator;
                indicator->prev = temp->prev;
                indicator->next = temp->next;
            }
            next = next->next;
        }
        indicator = indicator->next;
    }
}
void showlink(link* root) {
    sortlink(root);
    link* temp = root->next;
    while ( temp!=NULL )
    {
        cout << temp->it.key << " - " << temp->it.val << endl;
        temp = temp->next;
    }
    cout << endl;
}
void showVallink(link* root, string val) {
    sortlink(root);
    link* temp = root->next;
    while (temp != NULL)
    {
        if(!temp->it.val.compare(val))
            cout << temp->it.key << " - " << temp->it.val << endl;
        temp = temp->next;
    }
    cout << endl;
}
void getnode(link* root, string key) {
    link* temp = root;
    while (temp->next != NULL && key.compare(temp->next->it.key)) {
        temp = temp->next;
    }
    if (temp->next == NULL) {
        cout << "Not Found" << endl;
        return;
    }
    cout << temp->it.key << " - " << temp->it.val << endl;
    return;
}
int main(int argc, char** argv) {

    fstream newfile;
    /*
    newfile.open("output.txt", ios::out);  // open a file to perform write operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {
        newfile << "Tutorials point \n";   //inserting text
        newfile.close();    //close the file object
    }*/
    newfile.open("input.txt", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) {   //checking whether the file is open
        string tp, key, val, temp;
        link root;
        while (getline(newfile, tp)) { //read data from file object and put it into string.

            tp = trim(tp);

            if (tp.empty())
                break;

            if (tp.front() == '#') {
                cout << endl;
            }
            else{
                if (tp.front() == '=') {
                    temp = tp.substr(1, tp.length() - 2);
                    temp = trim(temp);
                    if (temp.empty()) {
                        showlink(&root); // Show all link
                    }
                    else {
                        showVallink(&root, temp);
                    }
                }
                else {
                    if (tp.find('=') == string::npos) {
                        key = tp.substr(0, tp.find(' '));
                        getnode(&root, key);
                    }
                    else {
                        key = tp.substr(0, tp.find(' '));
                        temp = tp.substr(tp.find('=')+1, tp.find(':') - tp.find('=')-2);
                        temp = trim(temp);
                        if (temp.empty()) {
                            delnode(&root, key);
                        }
                        else {
                            val = tp.substr(tp.rfind(' '), tp.length() - tp.rfind(' '));
                            node it;
                            key = trim(key);
                            val = trim(val);
                            it.key = key;
                            it.val = val;
                            addOrReplace(&root, it);
                        }
                    }
                }
            }

        }
        newfile.close(); //close the file object.
    }
    return 0;
}


