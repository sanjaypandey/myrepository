/**
This file contains the main function

EX Input:
    logfilter 6.6.6.6 8080 -e "user= admin@nss_sushil.com &((reqsize=1080|reqsize=2018)&(respsize=61528))"


*/


#include "tries.c"
#include "expressionvaldEvl.c"


/**
Static values of the keys
*/
static char *s[10]={"user",
                     "reqsize",
                      "respsize",
                      "urlclass",
                       "location",
                       "department",
                        "reqmethod",
                        "respcode",
                        "useragent",
                        "hostname"
    };

/**
Structure for storing the log value of an user.
*/
typedef struct LogValue
{
  char user[N];
  char reqsize[N];
  char respsize[N];
  char urlclass[N];
  char location[N];
  char department[N];
  char reqmethod[N];
  char respcode[N];
  char useragent[N];
  char hostname[N];
  struct LogValue *next;
}LogValueUser;


/**
This function conforms the validity of the expression.
*/
int parse(char *expression)
{
    int i=0,j=-1,k=0,flag=1;
	char keystr[N],valuestr[N];
	char newstr[N];
	int key=1,value=-1;
    int expressionlen=strlen(expression);

	keystr[0]=valuestr[0]='\0';
	while(expression[i]!='\0')
	{

		if(expression[i]==' '){
   		 i++;
		  continue;
	  }



	  switch(expression[i])
	  {
		 case '=':
		 case '<':
		 case '>':
			      if(keystr[0]=='\0'||(value==0&&valuestr[0]=='\0')||(keystr[0]!='\0'&&valuestr[0]!='\0'))
				  {
				     return 0;
				  }
			      if(key==1&&value==-1){

                   value=0;
                   key=0;
				   keystr[j]='\0';

				    Tries *node;
                    node=find(&root,keystr);

					if(strlen(keystr)>10||node==NULL||!node->isEnd)
					 {
					    return 0;
					 }
				   j=-2;

		       }
	         flag=0;
		     break;
	   case '&':
	   case '|':
	   case '(':
	   case ')':
	   case '!':
	        if((i-1)>=0&&(expression[i]!=')'||expression[i]==')')&&expression[i-1]==')')
	        {
	            if(expression[i]!=')'&&(i+1)==expressionlen) return 0;
                newstr[k++]=expression[i];
                flag=0;
                break;
            }
            else if(expression[i]=='('&&(expression[i+1]==')'))
            return 0;
            if(expression[i]=='('||expression[i]=='!'){
                newstr[k++]=expression[i];
            }
            else{
            if(keystr[0]=='\0'||valuestr[0]=='\0')
             return 0;
		    if(key==0&&value==0)
		    {
		         newstr[k++]='1';
            }
			value=-1;
			key=1;
			j=-1;
		   keystr[0]='\0';
		   valuestr[0]='\0';
		   newstr[k++]=expression[i];
          }
          flag=0;
          break;
	   default :;
	  }
         if(flag){
		 {
			 if(key==1&&value==-1)
			 {
			  if(j==-1)
		       j=0;
		       keystr[j++]=expression[i];
			 }
			  else if(key==0&&value==0)
			  {
			     if(j==-2)
					 j=0;
				valuestr[j++]=expression[i];
			  }
			 else
				 return 0;
		 }
		 }
		 flag=1;
	    i++;
	  }

    if(keystr[0]!='\0'&&valuestr[0]=='\0')
       return 0;

    if(key==0&&value==0)
	{
	  newstr[k++]='1';
	}

	newstr[k]='\0';
    char postfix[N];
    if(is_valid_paraenthis(newstr))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/**
This Compares the given expression value to the the value current log , Whether it's conforming that expression or not.
*/
boolean isValideLog(char *expression)
{
   int i=0,j,k=0,key=1;
   j=0;
   char compchar='\0';
   char newstr[N],keystr[N],valuestr[N];
   while(expression[i]!='\0')
   {
      if(expression[i]==' ')
      {
         i++;
         continue;
       }
     else if(expression[i]=='='||expression[i]=='>'||expression[i]=='<')
     {
         compchar=expression[i];
         keystr[j]='\0';
         key=0;
         j=0;
     }
     else if(expression[i]=='&'||expression[i]=='|'||expression[i]=='!'||expression[i]=='('||expression[i]==')'
     ||expression[i]=='!')
    {
           if(expression[i]=='('||expression[i]=='!')
           {
               newstr[k++]=expression[i];
            }
           else if((i-1)>=0&&(expression[i]!=')'||expression[i]==')')&&expression[i-1]==')')
           {
                newstr[k++]=expression[i];

            }
           else{
            valuestr[j]='\0';
            Tries *node;
            node=find(&root,keystr);
            if(node==NULL)
            {
               newstr[k++]='0';
            }
            else if(!strcmp(keystr,s[1]))
            {
               int t=0;
               switch(compchar)
               {
                  case '=':
                           if(atoi(node->value)==atoi(valuestr))
                             t=1;
                            break;

                  case '<':if(atoi(node->value)<atoi(valuestr))
                             t=1;
                            break;
                  case '>':   if(atoi(node->value)>atoi(valuestr))
                             t=1;
                }
               if(t==1)
                newstr[k++]='1';
               else
                newstr[k++]='0';
            }

          else  if(!strcmp(keystr,s[2]))
            {
               int t=0;
               switch(compchar)
               {
                  case '=':
                           if(atoi(node->value)==atoi(valuestr))
                             t=1;
                            break;

                  case '<':if(atoi(node->value)<atoi(valuestr))
                             t=1;
                            break;
                  case '>':   if(atoi(node->value)>atoi(valuestr))
                             t=1;
                }
               if(t==1)
                newstr[k++]='1';
               else
                newstr[k++]='0';
            }

            else if(strcmp(node->value,valuestr))
            {
               newstr[k++]='0';
            }
            else
            {
                newstr[k++]='1' ;
            }
            newstr[k++]=expression[i];
            keystr[0]=valuestr[0]='\0';
            key=1;
            j=0;
         }
   }

      else
      {
			 if(key==1)
			 {
		       keystr[j++]=expression[i];
			 }
			  else
			  {
				 valuestr[j++]=expression[i];
			  }
		 }

         i++;
   }
     if(keystr[0]!='\0'&&valuestr[0]!='\0')
     {
            valuestr[j]='\0';
            Tries *node;
            node=find(&root,keystr);
            if(node==NULL)
               newstr[k++]='0';
            else if(!strcmp(keystr,s[1]))
            {
               int t=0;
               if(node){
               switch(compchar)
               {
                  case '=':
                           if(atoi(node->value)==atoi(valuestr))
                             t=1;
                            break;

                  case '<':if(atoi(node->value)<atoi(valuestr))
                             t=1;
                            break;
                  case '>':   if(atoi(node->value)>atoi(valuestr))
                             t=1;
                }
               }
               if(t==1)
                newstr[k++]='1';
               else
                newstr[k++]='0';
            }


          else  if(!strcmp(keystr,s[2]))
            {
               int t=0;
               if(node){
               switch(compchar)
               {
                  case '=':
                           if(atoi(node->value)==atoi(valuestr))
                             t=1;
                            break;

                  case '<':if(atoi(node->value)<atoi(valuestr))
                             t=1;
                            break;
                  case '>':   if(atoi(node->value)>atoi(valuestr))
                             t=1;
                }
               }
               if(t==1)
                newstr[k++]='1';
               else
                newstr[k++]='0';
            }

            else if(strcmp(node->value,valuestr))
            {
               newstr[k++]='0';
            }
            else
            {
                newstr[k++]='1' ;
            }
            newstr[k++]=expression[i];
            keystr[0]=valuestr[0]='\0';
            key=1;
            j=0;
     }
     newstr[k]='\0';
    char postfix[N];
    infix_to_postfix(newstr,postfix);
    if(eval_postfix(postfix))
    {
        return 1;
    }
    else
      return 0;
}


/**
This is used by the below method for copying..
*/
void fillLogUserStructure(LogValueUser *valideFilters)
{
   int i=0;
   Tries *node;
   for (i=0;i<10;i++)
   {
     node=find(&root,s[i]);
     switch(i)
     {
       case 0:
                if(node)
                strcpy(valideFilters->user,node->value);
                break;
       case 1:
                if(node)
                strcpy(valideFilters->reqsize,node->value);
                break;
       case 2:
                if(node)
                strcpy(valideFilters->respsize,node->value);
                break;
       case 3:
                if(node)
                strcpy(valideFilters->urlclass,node->value);
                break;
       case 4:
                if(node)
                strcpy(valideFilters->location,node->value);
                break;
       case 5:
                if(node)
                strcpy(valideFilters->department,node->value);
                break;
       case 6:
                if(node)
                strcpy(valideFilters->reqmethod,node->value);
                break;
       case 7:
                if(node)
                strcpy(valideFilters->respcode,node->value);
                break;
       case 8:
                if(node)
                strcpy(valideFilters->useragent,node->value);
                break;
       case 9:
                if(node)
                strcpy(valideFilters->hostname,node->value);

     }
   }
}

/**
This value copies the current values of keys present in the TRIE to the LogValueUser(structure.)
*/
void copyLogInfo(LogValueUser **valideFilters)
{
    if(*valideFilters==NULL)
    {
       *valideFilters=(LogValueUser *)malloc(sizeof(LogValueUser));
       (*valideFilters)->next=NULL;
       fillLogUserStructure(*valideFilters);
    }
    else
    {
        LogValueUser *prv=*valideFilters,*cur;
        cur=*valideFilters;
        while(cur->next)
        {
          prv=cur->next;
          cur=cur->next;
        }
        prv->next=(LogValueUser *)malloc(sizeof(LogValueUser));
        prv->next->next=NULL;
        fillLogUserStructure(prv->next);
    }
}

/**
this methods Print this logs values which conforms the expression. This is tle linked list of LogValueUser(structure).
*/
void printValidLogs(LogValueUser *logs)
{
   int i=0;
   LogValueUser *prv;
   for(i=0;i<4;i++)
   printf(":%-20s:",s[i]);
   while(logs)
   {
      printf("\n:%-20s:",logs->user);
      printf(":%-10s:",logs->reqsize);
      printf(":%-10s:",logs-> urlclass);
      printf(":%-5s:",logs->reqmethod);
      printf(":%-20s:",logs->hostname);
      logs=logs->next;
    }

}


/**
Start of the main function..
*/
int main(int argc,char **argv)
{


   char *expression;
   FILE *fp;
   if(argc<5)
   {
      printf("\nsome problem in the input");
      return 1;
   }
   expression=argv[argc-1];

   Triesnodeinit(&root);
    int i=0,j;
    for(i=0;i<10;i++)
    {
        addWordHelper(&root,s[i],0,s[i]);
    }

   if(parse(expression))
   {
        char ch;
        char keystr[N];
        char valuestr[N];
        int key,attributes,i,j;

        LogValueUser *valideFilters=NULL;

        fp=fopen("sample.txt","r");
        if(fp==NULL)
        {
          printf("\nError while opening the file.\n");
          exit(EXIT_FAILURE);
         }

         keystr[0]=valuestr[0]='\0';
         key=1;attributes=0;j=0;

         while( ( ch = fgetc(fp) ) != EOF ){
          if(ch==' ')
           continue;
         else if(ch=='=')
		 {
             key=0;
             keystr[j]='\0';
             j=0;
          }
		 else if(ch==',')
		 {
		    valuestr[j]='\0';
		    addWordHelper(&root,keystr,0,valuestr);
		    keystr[0]=valuestr[0]='\0';
		    key=1;
		    j=0;
          }
         else if(ch=='\n')
         {

           if(isValideLog(expression))
           {
                copyLogInfo(&valideFilters);
            }
         }
         else
		 {
			 if(key==1)
			 {
		       keystr[j++]=ch;
			 }
			  else if(key==0)
			  {
			     if(j==-2)
					 j=0;
				valuestr[j++]=ch;
			  }
		 }
      }
      if(valideFilters)
      printValidLogs(valideFilters);
      else
      {
          puts("No valid logs are found..");
        }
   }
   else
   {
     printf("\nexpression given by the user is wrong one.\n");
	 return 1;
   }
   return 0;
}

