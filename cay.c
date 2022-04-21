#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char name[256];
    struct Node* leftMostChild;
    struct Node* rightSibling;
}Node;

Node* root;

Node *makeNode(char *name)
{
    Node *p = (Node *)malloc(sizeof(Node));
    strcpy(p->name, name);
    p->leftMostChild = NULL;
    p->rightSibling = NULL;
    return p;
}

Node *find(Node *r, char *name)
{
    if (r == NULL)
        return NULL;
    if (strcmp(r->name, name) == 0)
        return r;
    Node *p = r->leftMostChild;
    while (p != NULL)
    {
        Node *q = find(p, name);
        if (q != NULL)
            return q;
        p = p->rightSibling;
    }
}

Node *addLast(Node *p, char *name)
{
    if (p == NULL)
        return makeNode(name);
    p->rightSibling = addLast(p->rightSibling, name);
    return p;
}

void loadFileTree(FILE *f)
{
    char buff[256];
    int left = 0;
    fscanf(f, "%s", buff);
    root = makeNode(buff);
    Node *p = root;
    while (feof(f) != EOF)
    {
        fscanf(f, "%s", buff);
        if (strcmp(buff, "$") == 0)
        {
            left = 0;
            fscanf(f, "\n%s", buff);
            if (strcmp(buff, "$$") == 0)
                break;
            p = find(root, buff);
        }
        else
        {
            if (left == 0)
            {
                left = 1;
                p->leftMostChild = makeNode(buff);
                p = p->leftMostChild;
            }
            else
            {
                p->rightSibling = makeNode(buff);
                p = p->rightSibling;
            }
        }
    }
}

void processLoad()
{
    char filename[256];
    printf("\nFilename: ");
    scanf("%s", filename);
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("\nCan't open file");
        fclose(f);
        return;
    }
    loadFileTree(f);
    fclose(f);
}
void addChild(char *name, char *child)
{
    Node *r = find(root, name);
    if (r == NULL)
        return;
    r->leftMostChild = addLast(r->leftMostChild, child);
}

void processAddChild()
{
    char name[256];
    printf("\nName: ");
    scanf("%s", name);
    char child[256];
    printf("\nChild: ");
    scanf("%s", child);
    addChild(name, child);
}


void printTree(Node *r)
{
    if (r == NULL)
        return;
    printf("%s: ", r->name);
    Node *p = r->leftMostChild;
    while (p != NULL)
    {
        printf("%s ", p->name);
        p = p->rightSibling;
    }
    printf("\n");
    p = r->leftMostChild;
    while (p != NULL)
    {
        printTree(p);
        p = p->rightSibling;
    }
}

void processPrint()
{
    printf("\nTree:-----------------\n");
    printTree(root);
}

void printTreeF(Node *r, FILE *f)
{
    if (r == NULL)
        return;
    fprintf(f, "%s", r->name);
    Node *p = r->leftMostChild;
    while (p != NULL)
    {
        fprintf(f, "%s", p->name);
        p = p->rightSibling;
    }
    fprintf(f, " $\n");
    p = r->leftMostChild;
    while (p != NULL)
    {
        printTreeF(p, f);
        p = p->rightSibling;
    }
}

void processFind()
{
    char name[256];
    scanf("%s", name);
    Node *p = find(root, name);
    if (p == NULL)
        printf("Not Found%s\n", name);
    else printf("Found%s\n", name);
}

void processFindChildren()
{
    char name[256];
    scanf("%s", name);
    Node *p = find(root, name);
    if (p == NULL)
        printf("Not Found%s\n", name);
    else
    {
        printf("Found%s withchildren: ", name);
        Node *q = p->leftMostChild;
        while (q != NULL)
        {
            printf("%s ", q->name);
            q = q->rightSibling;
        }
    }
    printf("\n");
}

int height(Node *p)
{
    if (p == NULL)
        return 0;
    int maxH = 0;
    Node *q = p->leftMostChild;
    while (q != NULL)
    {
        int h = height(q);
        maxH = maxH < h ? h : maxH;
        q = q->rightSibling;
    }
    return maxH + 1;
}

void processHeight()
{
    char name[256];
    scanf("%s", name);
    Node *p = find(root, name);
    if (p == NULL)
        printf("Not Found%s\n", name);
    else
    {
        printf("Found%s havingheight= %d\n", name, height(p));
    }
}

int count(Node *r)
{
    if (r == NULL)
        return 0;
    int cnt = 1;
    Node *q = r->leftMostChild;
    while (q != NULL)
    {
        cnt += count(q);
        q = q->rightSibling;
    }
    return cnt;
}

void processCount() { printf("Numberof membersis%d\n", count(root)); }

void processStore()
{
    char filename[256];
    scanf("%s", filename);
    FILE *f = fopen(filename, "w");
    printTreeF(root, f);
    fprintf(f, "$$");
    fclose(f);
}
void freeTree(Node *r)
{
    if (r == NULL)
        return;
    Node *p = r->leftMostChild;
    while (p != NULL)
    {
        Node *sp = p->rightSibling;
        freeTree(p);
        p = sp;
    }
    printf("free node%s\n", r->name);
    free(r);
    r = NULL;
}
void main()
{
    while (1)
    {
        char cmd[256];
        printf("Enter command: ");
        scanf("%s", cmd);
        if (strcmp(cmd, "Quit") == 0)
            break;
        else if(strcmp(cmd, "Load") == 0) processLoad();
        else if(strcmp(cmd, "Print") == 0) processPrint();
        else if(strcmp(cmd, "Find") == 0) processFind();
        else if(strcmp(cmd, "FindChildren") == 0) processFindChildren();
        else if(strcmp(cmd, "Height") == 0) processHeight();
        else if(strcmp(cmd, "Count") == 0) processCount();
        else if(strcmp(cmd, "AddChild") == 0) processAddChild();
        else if(strcmp(cmd, "Store") == 0) processStore();
    }
    freeTree(root);
}