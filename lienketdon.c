#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_L 256
typedef struct Profile{
    char name[MAX_L];
    char email[MAX_L];
    struct Profile* next;
}Profile;

Profile* first, *last;

Profile *makeProfile(char *name, char *email)
{
    Profile *node = (Profile *)malloc(sizeof(Profile));
    strcpy(node->name, name);
    strcpy(node->email, email);
    node->next = NULL;
    return node;
}
void initList()
{
    first = NULL;
    last = NULL;
}
int listEmpty() { return first == NULL && last == NULL; }

void insertLast(char *name, char *email)
{
    Profile *profile = makeProfile(name, email);
    if (listEmpty())
    {
        first = profile;
        last = profile;
    }
    else
    {
        last->next = profile;
        last = profile;
    }
}
void printList()
{
    for (Profile *p = first; p != NULL; p = p->next)
        printf("%s, %s\n", p->name, p->email);
}

Profile *removeProfile(Profile *f, char *name)
{
    if (listEmpty())
        return NULL;
    if (strcmp(f->name, name) == 0)
    {
        Profile *tmp = f->next;
        free(f);
        if (tmp == NULL)
            last = NULL;
        return tmp;
    }
    else
    {
        f->next = removeProfile(f->next, name);
        return f;
    }
}

void load(char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        printf("Load data -> file not found\n");
    while (!feof(f))
    {
        char name[256], email[256];
        fscanf(f, "%s%s", name, email);
        insertLast(name, email);
    }
    fclose(f);
}

void processFind()
{
    char name[256];
    scanf("%s", name);
    Profile *profile = NULL;
    for (Profile *p = first; p != NULL; p = p->next)
    {
        if (strcmp(p->name, name) == 0)
        {
            profile = p;
            break;
        }
    }
    if (profile == NULL)
    {
        printf("NOT FOUND profile %s\n", name);
    }
    else
    {
        printf("FOUND profile %s, %s\n", profile->name, profile->email);
    }
}

void processLoad()
{
    printf("Enter file name :");
    char filename[256];
    scanf("%s", filename);
    load(filename);
}
void processStore()
{
    char filename[256];
    scanf("%s", filename);
    FILE *f = fopen(filename, "w");
    for (Profile *p = first; p != NULL; p = p->next)
    {
        fprintf(f, "%s%s", p->name, p->email);
        if (p->next != NULL)
            fprintf(f, "\n");
    }
    fclose(f);
}

void processInsert()
{
    char name[256], email[256];
    scanf("%s%s", name, email);
    insertLast(name, email);
}
void processRemove()
{
    char name[256];
    scanf("%s", name);
    first = removeProfile(first, name);
}

int main()
{
    initList();
    while (1)
    {
        printf("Enter command: ");
        char cmd[256];
        scanf("%s", cmd);
        if (strcmp(cmd, "Quit") == 0)
            break;
        else if (strcmp(cmd, "Load") == 0)
            processLoad();
        else if (strcmp(cmd, "Print") == 0)
            printList();
        else if (strcmp(cmd, "Find") == 0)
            processFind();
        else if (strcmp(cmd, "Insert") == 0)
            processInsert();
        else if (strcmp(cmd, "Remove") == 0)
            processRemove();
        else if (strcmp(cmd, "Store") == 0)
            processStore();
    }
}