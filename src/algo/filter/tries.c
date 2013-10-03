#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define N 1000
#define ALPHABETS 26

/**
Trie datastruture... for validating the key in the coming expression.

This stores the Keys in the trie structure and stores the values of those corresponding keys
in the leaf node of that trie key. So, when we need the value corresponding to any key , we
just traverse down to the leaf node of that particular key , and the corresponding value from
there.
*/

typedef  int Integer;

typedef enum bool
{
  false,true
}boolean;

typedef struct trieNode {
	struct trieNode  *children[ALPHABETS];
    boolean isEnd;
    char value[N];
}Tries;

   Tries root;

	void Triesnodeinit(Tries *root){
	    int i;
		root->isEnd=false;
		root->value[0]='\0';
		for(i=0;i<ALPHABETS;i++)
		root->children[i]=NULL;
	}


	void addWordHelper(Tries *root ,char* word,Integer idx,char  *value){
		Tries* node=root->children[word[idx]-'a'];
		if(node==NULL){
			node=(Tries *)malloc(sizeof(Tries));
			Triesnodeinit(node);
			root->children[word[idx]-'a']=node;
		}
		if(word[idx+1]=='\0'){
			node->isEnd=true;
			strcpy(node->value,value);
		}
		else{
			addWordHelper(node,word,idx+1,value);
		}
	}



	 Tries* helperFind(Tries *root,char* word,int idx){
		boolean res=false;
		Tries* t=NULL;
		Tries* node=root->children[word[idx]-'a'];
		if(node==NULL){
			res=false;
			return node;
		}
		if(word[idx+1]=='\0'){
			res=node->isEnd;
		    if(res){
		    	t=node;
		    }
		}
		else{
			t=helperFind(node,word, idx+1);
		}
		return t;
	}

	 Tries* find(Tries *root,char *word){

		if(*word=='\0') return NULL;
		return helperFind(root,word,0);
	}


