//IP address are simulated as  a.b.c.d as a string, a,b,c,d is used in the programm according to this conventon.
//Main opens the



#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#define N 255256

//Input files from which the data stream will be read.
#define INPUTSTREAM1 "test.txt"
#define INPUTSTREAM2 "test1.txt"
//Output file which  will contain the result intersect IP data.
#define INTERSECTIPSTREAM "intersectIP.txt"

// Data Part
typedef struct stud {
    int val;
    char status;
    char a[4];
    char d[4];
    struct stud *next;
}Node;

typedef struct hashstruct
{
    char b[4];
    char c[4];
    Node *nPtr;
}Hash;

//initialises the map for providing hashing..
static inline void hashInit(Hash *mp)
{
    int i;
    for(i=0;i<N;i++)
        mp[i].nPtr=NULL;
}

//This fucntion frees the memory reclaimed by the hash function at the end of the program
static void hashDestroy(Hash *mp)
{
    int i=0;
    if(mp==NULL) return;
    Node *cur=NULL,*next=NULL;
    for(i=0;i<N;i++)
    {
        cur=mp[i].nPtr;
        while (cur){
            next=cur->next;
            free(cur);
            cur=next;
        }
    }
    free(mp);
}


//insert the Node in the sorted list of particular hash index in the increasing order.This will improve
//efficiency of inserting the list data of a particular index. Also It will be beneficial when
//we find the intersected IP's from the hash strucure, it will give the linear efficiency.
void insert(Hash *mp,Node *dest,int idx)
{
    Node *src=NULL,*p=NULL;
    src=mp[idx].nPtr;

    if(!src)
    {
        mp[idx].nPtr=dest;
    }

    while (src&&src->val<=dest->val) {
        if(src->val==dest->val){
            free(dest);
            return;
        }
        p=src;
        src=src->next;

    }
    if(!p)
    {
        dest->next=src;
        mp[idx].nPtr=dest;
    }
    else if(!src)
    {
        p->next=dest;
    }
    else
    {
        dest->next=p->next;
        p->next=dest;
    }

}


//get the Integer value from the string
//This is used to get the Hash value from the string b.c
//It provides the unique hashing value , which insures that collision will not occure.
int strToInt(const char *a,const char *b)
{
    int i=0,idx=0,flag=0,j=0,mHash=10;
    while(a[i]!='\0')
    {
        while (a[i]=='0'&&flag==0) {
            i++;
        }
        if(a[i]=='\0')
        {
            break;
        }
        flag=1;
        idx = idx*mHash+(a[i]-'0');
        i++;
    }
    flag=0;j=0;
    i=0;

    j=(int)strlen(b);
    int k=0;
    while (k<3-j) {
        idx*=10;
        k++;
    }

    while(b[i]!='\0')
    {
        idx = idx*mHash+(b[i]-'0');
        i++;
    }
    return idx;
}


//copies the src string value to dest string different value
void strCopy(char *dest,char *src,int s,size_t e)
{
    int j=0;
    while (s<=e) {
        dest[j]=src[s];
        j++;s++;
    }
    dest[j]='\0';
}

//Generates the different possible cominations of IP addresses.
void generateComb(const char *a,const char *d,const char *b,const char *c,Hash *mp)
{
    int i,j,alen,dlen;
    int idx=0;
    alen=(int)strlen(a);
    dlen=(int)strlen(d);
    if(atoi(b)>255||atoi(c)>255)
    {
        return;
    }

    idx=strToInt(b,c);
    if(mp[idx].nPtr==NULL){
            strCopy(mp[idx].b,b,0,strlen(b)-1);
            strCopy(mp[idx].c,c,0,strlen(c)-1);
    }

    for(i=alen-1;i>=0;i--)
    {
        for(j=0;j<dlen;j++)
        {
            Node *ptr=(Node *)malloc(sizeof(Node));
            strCopy(ptr->a, a, i, alen);
            strCopy(ptr->d, d, 0, j);
            ptr->val=strToInt(ptr->a, ptr->d);
            ptr->next=NULL;
            if(atoi(ptr->a)>255||atoi(ptr->d)>255||atoi(ptr->a)==0||atoi(ptr->d)==0)
            {
                free(ptr);
            }
            else{
                insert(mp,ptr,idx);
            }
        }
    }
}

//common parser method.
//This function takes the file name and get the data as stream.
Hash* parseFileDataStream(const char *fname)
{
    Hash *mp=(Hash *)malloc(sizeof(Hash)*N);
    hashInit(mp);
    int k=0,tempIdx=0,nOctact=0,len=0;
    char temp[4],temp1[4],charcter[4];
    char a[4],b[4],c[4],d[4];
    char ch,prvChar='\0';
    FILE *fptr=fopen(fname, "r");
    if(!fptr){
        perror("falied to open the file..");
        return NULL;
    }
    a[0]=b[0]=c[0]=d[0]='\0';
    while (( fscanf(fptr,"%c",&ch) ) != EOF) {
        if(ch!='.'){
            len++;
            temp[(len-1)%3]=ch;

            if(len<4)
                temp1[len-1]=ch;

            if(len>3)
            {
                tempIdx=(len-1)%3;
            }
        }
        else if(prvChar=='.'&&ch=='.')
        {
            len=0;
            nOctact=0;
        }

        else
        {
            nOctact++;
            temp1[3]='\0';
            if(len<=3)
                tempIdx=0;
            else
                tempIdx=(tempIdx+1)%3;

            int i=tempIdx,j;
            j=0;
            do{

                charcter[j]=temp[i];
                j++;
                if(len>3)
                    i=(i+1)%3;
                else
                    i=(i+1)%len;
            }while(i!=tempIdx);

            if(len>3)
                charcter[3]='\0';
            else
                charcter[len]='\0';
            switch(nOctact)
            {
                case 1:
                    strCopy(a,charcter,0,strlen(charcter)-1);
                    break;
                case 2:
                    strCopy(b,charcter,0,strlen(charcter)-1);
                    break;
                case 3:
                    strCopy(c,charcter,0,strlen(charcter)-1);
                    break;
                case 4:
                    if(len>3)
                        k=3;
                    else
                        k=len;
                    strCopy(d,temp1,0,k-1);
                    generateComb(a,d,b,c,mp);
            }
            if(len>3)
            {
                nOctact=1;
                strCopy(a,charcter,0,strlen(charcter)-1);
            }
            else if(nOctact==4)
            {
                nOctact=3;
                strCopy(a,b,0,strlen(b)-1);
                strCopy(b,c,0,strlen(c)-1);
                strCopy(c,d,0,strlen(d)-1);
            }
            len=0;
        }
        prvChar=ch;
    }

    if(nOctact==3)
    {
        if(len>3)
            k=3;
        else
            k=len;
        strCopy(d,temp1,0,k-1);
        generateComb(a,d,b,c,mp);
    }
    fclose(fptr);
    return mp;
}


//Function that gets the intersected IP addresses.
FILE* getIntersection(const Hash *mp1,const Hash *mp2)
{
    FILE *intersectIPstream;
    intersectIPstream=fopen(INTERSECTIPSTREAM, "w");
    if(!intersectIPstream)
    {
        perror("Some problem in Openning the file");
        return NULL;
    }
    int i=0;
    Node *ptr1,*ptr2;
    for(i=0;i<N;i++)
    {
        ptr1=mp1[i].nPtr;
        ptr2=mp2[i].nPtr;
        while (ptr1!=NULL&&ptr2!=NULL) {
            if(ptr1->val==ptr2->val)
            {
                if(intersectIPstream)
                fprintf(intersectIPstream,"%d.%d.%d.%d\n",atoi(ptr1->a),atoi(mp1[i].b),atoi(mp1[i].c),atoi(ptr1->d));
                ptr1=ptr1->next;
                ptr2=ptr2->next;
            }
            else if(ptr1->val<ptr2->val)
                ptr1=ptr1->next;
            else
                ptr2=ptr2->next;
        }
    }
    return intersectIPstream;
}


//this is the main function
int main(int argc,char **argv)
{

    FILE *intersectIPstream;
    Hash *mp1,*mp2;
    mp1=parseFileDataStream(INPUTSTREAM1);
    mp2=parseFileDataStream(INPUTSTREAM2);
    if(!mp1||!mp2)
    {
        perror("either of the file is not read properly");
        return -1;
    }
    intersectIPstream=getIntersection(mp1,mp2);
    hashDestroy(mp1);
    hashDestroy(mp2);
    mp1=mp2=NULL;
    fclose(intersectIPstream);

    return  0;
}
