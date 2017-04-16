#include<iostream>

using namespace std;

string code;
int counter;
struct queueList;
struct z;

void getAlphabetList(int,string);
bool newCharacter(char);
void sortQueue();
int numberOfCharacters(struct queueList*);
void copyQueue();
struct queueList* getElement(int);
struct queueList* getPrevious(struct queueList*);
void printList(struct queueList*);
void getTree();
struct queueList* dequeue();
void addElement(struct queueList*);
void getCode(string);
void preorder(struct z*, char);

struct z
{
    char leftValue;
    char rightValue;
    struct z* left;
    struct z* right;
};

struct queueList
{
    char value;
    int weight;
    struct queueList* next;
    struct z* tree;
};

struct queueList* mainList = NULL;
struct queueList* subList = NULL;

int main()
{
    string inputString;
    cout<<"Enter the String for Hoffman Coding :";
    cin>>inputString;
    int length = inputString.length();
    getAlphabetList(length, inputString);
    //printList(mainList);
    sortQueue();
    //printList(mainList);
    copyQueue();
    getTree();
    getCode(inputString);
}

void getAlphabetList(int length, string inputString)
{
    for(int i=0; i<length; i++)
    {
        if(newCharacter(inputString[i]))
        {
            struct queueList *temp = new queueList;
            temp->tree = NULL;
            temp->next = NULL;
            temp->value = inputString[i];
            temp->weight = 1;
            if(mainList==NULL)
            {
                mainList = temp;
                continue;
            }
            struct queueList* traverse;
            traverse = mainList;
            while(traverse->next!=NULL)
            {
                traverse = traverse->next;
            }
            traverse->next = temp;
        }

    }
}

bool newCharacter(char ch)
{
    struct queueList* temp;
    temp = mainList;
    if(temp==NULL)
        return true;
    while(temp!=NULL)
    {
        if(temp->value==ch)
        {
            temp->weight+=1;
            return false;
        }
        else
            temp = temp->next;
    }
    return true;
}

void sortQueue()
{
    int n, i=1, pos = 1;
    n = numberOfCharacters(mainList);
    struct queueList* currentElement;
    currentElement = mainList;
    while(i<n)
    {
        currentElement = getElement(pos);
        struct queueList* listElement;
        listElement = mainList;
        while(listElement!=NULL && (currentElement->weight>=listElement->weight))
        {
            listElement = listElement->next;
        }
        struct queueList* temp;
        temp = getPrevious(currentElement);
        temp->next = currentElement->next;
        temp = getPrevious(listElement);
        if(temp!=mainList)
        {
            currentElement->next = temp->next;
            temp->next = currentElement;
        }
        else
        {
            currentElement->next = temp;
            mainList = currentElement;
        }
        if(getElement(i)==currentElement)
        {
            pos++;
        }

        i++;

    }
}

int numberOfCharacters(struct queueList* temp)
{
    int n = 0;
    while(temp!=NULL)
    {
        temp = temp->next;
        n++;
    }
    return n;
}

void copyQueue()
{
    struct queueList* temp = mainList;
    while(temp!=NULL)
    {
        struct queueList* copyTemp = new queueList;
        copyTemp->next = NULL;
        copyTemp->tree = NULL;
        copyTemp->value = temp->value;
        copyTemp->weight = temp->weight;
        if(subList==NULL)
        {
            subList = copyTemp;
        }
        else
        {
            struct queueList* temp2 = subList;
            while(temp2->next!=NULL)
            {
                temp2 = temp2->next;
            }
            temp2->next = copyTemp;
        }
        temp = temp->next;

    }
}

struct queueList* getElement(int i)
{
    struct queueList* temp = mainList;
    while(i!=0)
    {
        temp = temp->next;
        i--;
    }
    return temp;
}

struct queueList* getPrevious(struct queueList* curr)
{
    struct queueList* temp = mainList;
    if(temp==curr)
    {
        return temp;
    }
    while(temp->next!=curr)
    {
        temp = temp->next;
    }
    return temp;
}

void printList(struct queueList* temp)
{
    cout<<endl;
    while(temp!=NULL)
    {
        cout<<temp->value<<"  "<<temp->weight<<endl;
        temp= temp->next;
    }
}

void getTree()
{
    while(mainList->next!=NULL)
    {
        struct queueList *element1;
        struct queueList *element2;
        element1 = dequeue();
        element2 = dequeue();
        struct z* subTree = new z;
        subTree->leftValue = element1->value;
        subTree->rightValue = element2->value;
        subTree->left = element1->tree;
        subTree->right = element2->tree;
        struct queueList* newElement = new queueList;
        newElement->next = NULL;
        newElement->tree = subTree;
        newElement->value = '.';
        newElement->weight = element1->weight + element2->weight;
        delete element1;
        delete element2;
        addElement(newElement);
    }
}

struct queueList* dequeue()
{
    struct queueList* temp;
    temp = mainList;
    mainList = mainList->next;
    return temp;
}

void addElement(struct queueList* newElement)
{
    struct queueList* temp = mainList;
    if(temp==NULL)
    {
        mainList = newElement;
        return;
    }
    while(temp!=NULL && temp->weight<=newElement->weight)
    {
        temp = temp->next;
    }
    struct queueList* temp2;
    if(temp==mainList)
    {
        newElement->next = temp;  //point to element pointed by mainList
        mainList = newElement;  //Point mainList to newElement
    }
    else
    {
        temp2 = getPrevious(temp);
        temp2->next = newElement;
        newElement->next = temp;
    }
}

void getCode(string inputString)
{
    int n = numberOfCharacters(subList);
    string charCode[n];
    struct queueList* temp = subList;
    //printList(subList);
    for(int i=0; i<n; i++)
    {
        code = "";
        counter = 0;
        preorder(mainList->tree,temp->value);
        charCode[i] = code;
        cout<<temp->value<<"  "<<charCode[i]<<endl;
        temp = temp->next;
    }

    cout<<"Code: ";
    for(int i=0; i<inputString.length(); i++)
    {
        temp = subList;
        int j = 0;
        while(inputString[i]!= temp->value)
        {
            temp = temp->next;
            j++;
        }
        cout<<charCode[j]<<" ";
    }
    cout<<endl;
}

void preorder(struct z* temp, char value)
{
    //cout<<"In Preorder"<<endl;
    if(temp == NULL)
    {
        if(code!="" && counter == 0)
            code.pop_back();
        return;
    }
     if(value==temp->leftValue)
     {
         code+="0";
         counter = 1;
         return;
     }
     if(value==temp->rightValue)
     {
         code+="1";
         counter = 1;
         return;
     }
     if(counter==0)
        code += "0";
     preorder(temp->left, value);
     if(counter == 0)
        code += "1";
     preorder(temp->right, value);
     if(counter==0)
         code.pop_back();
}







