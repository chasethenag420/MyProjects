#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
using namespace std;
char* data = NULL;

int size =0;
int ciphersize =0;
int count=0;
 int numberofciphers=0;
char **arrayWords=NULL;
char  result[1000]="\0";
int decodedvalue=0;
int trywithoutspace=0;
char finaltest[1000]="\0";
struct stu{
    int a=0;
    int index=0;
    char str[1000]="\0";
    int found=0;
};
struct bin{
    int abin=0;
    int indexbin=0;
};
struct readablecipher{
    int readable=0;
    char readableorNot[1000]="\0";
};
bin bn;
char  cipherxor12dup[1000]="\0";
char  cipherxor12_old[1000]="\0";
char  cipherxor13dup[1000]="\0";
char  cipherxor13_old[1000]="\0";
char  cipherxor14dup[1000]="\0";
char  cipherxor14_old[1000]="\0";
char tempguessword[1000]="\0";
int sizeofguess=0;

char* cipher000=NULL;
char* cipher001=NULL;
char* cipher002=NULL;
char* cipher003=NULL;

char  decode1[1000]={};
char  decode2[1000]={};
char temp[1000]="\0";
int trackIndex=0;
int arraywordtrackIndex=0;
int reset=1;
char  delimiterslimt[]=".?!\n,";
char temparrayvalue[20]="\0";


char arra[100][100]={"the ","be ","to ","of ","and ","a ","in ","that ","have ","I ","it ","for ","not ","on ","with ","he ","as ","you ","do ","at ","this ","but ","his ","by ","from ","they ","we ","say ","her ","she ","or ","an ","will ","my ","one ","all ","would ","there ","their ","what ","so ","up ","out ","if ","about ","who ","get ","which ","go ","me ","when ","make ","can ","like ","time ","no ","just ","him ","know ","take ","people ","into ","year ","your ","good ","some ","could ","them ","see ","other ","than ","then ","now ","look ","only ","come ","its ","over ","think ","also ","back ","after ","use ","two ","how ","our ","work ","first ","well ","way ","even ","new ","want ","because ","any ","these ","give ","day ","most ","us "};
char  backup[100][100]={"the ","be ","to ","of ","and ","a ","in ","that ","have ","I ","it ","for ","not ","on ","with ","he ","as ","you ","do ","at ","this ","but ","his ","by ","from ","they ","we ","say ","her ","she ","or ","an ","will ","my ","one ","all ","would ","there ","their ","what ","so ","up ","out ","if ","about ","who ","get ","which ","go ","me ","when ","make ","can ","like ","time ","no ","just ","him ","know ","take ","people ","into ","year ","your ","good ","some ","could ","them ","see ","other ","than ","then ","now ","look ","only ","come ","its ","over ","think ","also ","back ","after ","use ","two ","how ","our ","work ","first ","well ","way ","even ","new ","want ","because ","any ","these ","give ","day ","most ","us "};
//stu st;
stu expandSearch(char * string);

//method to release memory
void releasememory(char *str){
    if(str!=NULL){
        free(str);
        str=NULL;
    }
}
int testvalue=0;
// Find the user home directory
char * getHomeDir(){
    char *homedir = getenv("HOME");

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        //printf("Failed\n");
        exit(EXIT_FAILURE);
    }
    return homedir;
}
void copybinaryarr(char cipher[],char * XOR, int length){
    for (int i=0;i<length;i++){
        cipher[i]=XOR[i];
    }
    cipher[length]='\0';

}
int comparecipher(char * str1,char * str2, int length){
    for (int i=0;i<length;i++){
        if(str1[i]!=str2[i]){
            return 0;
        }
    }

}
//Print the cipher in hex code to screen

void printcipher(char * str, int length, char arr[20], char line[5]){
    //    //printf("hex code=\n");
    //    for (int i=0;i<length;i++){
    //        //printf("%x",str[i]);
    //        //printf("%c",str[i]);
    //    }
    //    //printf("\n");

}
// Print the decoded messages to screen
void printReadbleDecoded(char  readableOrNot[],char  readableOrNot12[],char  readableOrNot13[],char  readableOrNot14[]){
    readableOrNot[strlen(readableOrNot)-1]='\0';
    readableOrNot12[strlen(readableOrNot)]='\0';
    readableOrNot13[strlen(readableOrNot)]='\0';
    readableOrNot14[strlen(readableOrNot)]='\0';
    printf("\n=======================================\n");

        if(numberofciphers==4){

            printf("Decoded message 1 = %s\n",readableOrNot);
            printf("Decoded message 2 = %s\n",readableOrNot12);
            printf("Decoded message 3 = %s\n",readableOrNot13);
            printf("Decoded message 4 = %s\n",readableOrNot14);

        }
        if(numberofciphers==3){
            printf("Decoded message 1 = %s\n",readableOrNot);
            printf("Decoded message 2 = %s\n",readableOrNot12);
            printf("Decoded message 3 = %s\n",readableOrNot13);
        }
        if(numberofciphers==2){
            printf("Decoded message 1 = %s\n",readableOrNot);
            printf("Decoded message 2 = %s\n",readableOrNot12);

        }
         printf("\n=======================================\n");



}
void printReadbleDecodedFinal(char  readableOrNot[],char  readableOrNot12[],char  readableOrNot13[],char  readableOrNot14[]){

printf("\n=======================================\n");
    if(numberofciphers==4){

        printf("Decoded message 1 = %s\n",readableOrNot);
        printf("Decoded message 2 = %s\n",readableOrNot12);
        printf("Decoded message 3 = %s\n",readableOrNot13);
        printf("Decoded message 4 = %s\n",readableOrNot14);

    }
    if(numberofciphers==3){
        printf("Decoded message 1 = %s\n",readableOrNot);
        printf("Decoded message 2 = %s\n",readableOrNot12);
        printf("Decoded message 3 = %s\n",readableOrNot13);
    }
    if(numberofciphers==2){
        printf("Decoded message 1 = %s\n",readableOrNot);
        printf("Decoded message 2 = %s\n",readableOrNot12);
    }
     printf("\n=======================================\n");

}

//Get the substringusing position and length
char* substring(char *string, int position, int length)
{
    char temp[20]="\0";
    int c;
       for (c = 0 ; c < position -1 ; c++)
        string++;
    for (c = 0 ; c < length ; c++)
    {
        temp[c]= *string;
        string++;
    }
    temp[c]='\0';
    return temp;
}

//Trim any trailing spaces

char *trimwhitespace(char *str)
{
    char *end;
    // Trim leading space
    while(isspace(*str)) str++;
    if(*str == 0)  // All spaces?
        return str;
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    // Write new null terminator
    *(end+1) = 0;
    return str;
}
int numberofwords(char * tempe){
    int wordcount=0;
    for (int i = 0;tempe[i] != '\0';i++)

    {
        if (tempe[i] == ' ')

            wordcount++;

    }
    return wordcount;
}

// Check for allowed characters
int checkallowedchar(char * readableOrNot12,int sizeofguess){
    int readable=0;
    for(int y=0;y<sizeofguess;y++){
        char c=readableOrNot12[y];

        if(c==44 ||c==39||c==32 || c==33 || c==63 || c==46 || c==10 || (c>=65 &&c<=90)|| (c>=97&&c<=122))  {
            // if(c==32||(c>=65 &&c<=90)|| (c>=97&&c<=122))  {
            readable=1;
        }else{
            readable=0;
            break;
        }
    }
    return readable;
}

int validatewords(char* string,stu checkread){
    // Make a local copy of the string that we can manipulate.
    static const char *space_characters = " .?!\n";
    char copy[1000] = "";
    strcpy(copy,string);
    strcpy(copy,trimwhitespace(copy));
    if(string==NULL)
        return 0;
    int length=strlen(string);

    for(int k=0;k<length;k++){
        char delim=string[k];
        if(delim=='?' || delim=='!'|| delim=='\n'|| delim=='.'|| delim==','){
            if(k+1==length){

            }else if(k+1<length){
                if(string[k+1]==' '){
                    //pintf("return 1 231");
                    // return 1;
                }else{
                   // //printf("return 1 234");
                    testvalue=1;
                    return 0;
                }
            }
        }

    }




    if(checkread.a-1>=0 && checkread.a+1<count){
        //printf("validatewords arrayWords[checkread.a-1] = %s\n",arrayWords[checkread.a-1]);
        //printf("validatewords arrayWords[checkread.a] = %s\n",arrayWords[checkread.a]);
        //printf("validatewords arrayWords[checkread.a+1] = %s\n",arrayWords[checkread.a+1]);
        if(checkread.found !=0
                &&(
                    strcmp(checkread.str,substring(arrayWords[checkread.a-1],0,strlen(checkread.str)))!=0
                    &&strcmp(checkread.str,substring(arrayWords[checkread.a],0,strlen(checkread.str)))!=0
                    &&strcmp(checkread.str,substring(arrayWords[checkread.a+1],0,strlen(checkread.str)))!=0) ){
            //printf("return 0 203");

            return 0;
        }}
    return 1;

}
void removelastword(char * line1){

    // char line1[] = "This is a sentence with a last WoRd ";
    char line[1000]="\0";
    strcpy(line,line1);
    //printf("line %s\n",line);
    // //printf("test %s\n",test);
    char temp[1000]="\0";

    char *lastWord = NULL;
    char *token = strtok(line, " ");
    while (token != NULL)
    {
        lastWord = token;
        token = strtok(NULL, " ");
        //printf("token %s\n",token);
        if(token!=NULL){
            strcat(temp,lastWord);
            strcat(temp," ");
        }
        //printf("temp %s\n",temp);
    }
    temp[strlen(temp)]='\0';
    //    if(lastWord==NULL)
    //        return;
    //    while (*lastWord)
    //    {
    //        // //printf("%c", tolower(*lastWord++));
    //        //printf("lastWord %s\n",lastWord);
    //      //  //printf("line %s\n",line);

    //        //  //printf("lastWord %c\n",lastWord);
    //         *lastWord++;
    //        // return strcmp(lastWord,test);

    //    }
    //    * (lastWord)='\0';
    //printf("after chopping temp=,%s,\n",temp);

    strcpy(line1,temp);

    //printf("after chopping line1=,%s,\n",line1);

    //printf("strcmp lastword =%d\n",strcmp(temp,test));

}

int lastword(char* str,char* test){

    //char line[] = "This is a sentence with a last WoRd ";

    char temp[1000]="\0";
    char line[1000]="\0";
    strcpy(line,str);
    //printf("line %s\n",line);
    //printf("test %s\n",test);

    char *lastWord = NULL;
    char *token = strtok(line, " ");
    while (token != NULL)
    {
        lastWord = token;
        token = strtok(NULL, " ");
    }
    int i=0;
    while (*lastWord)
    {
        // //printf("%c", tolower(*lastWord++));
        // //printf("lastWord %c",*lastWord++);
        //  //printf("lastWord %c\n",lastWord);
        temp[i++]=*lastWord;
        *lastWord++;
        // return strcmp(lastWord,test);

    }
    //printf("lastword temp=%s\n",temp);
    //printf("strcmp lastword =%d\n",strcmp(temp,test));
    return strcmp(temp,test);

}

//Find the index of which string has last word of long length
int lastBigWord(char *string) {
    // Make a local copy of the string that we can manipulate.
    static const char *space_characters = " .?!\n";
    char copy[1000] = "";
    strcpy(copy,string);
    strcpy(copy,trimwhitespace(copy));
    if(string==NULL)
        return 0;

    stu checkread= expandSearch(string);
    //printf("string = ,%s,\n",string);
    //printf("checkread.found = %d\n",checkread.found);
    //printf("checkread.str = %s\n",checkread.str);
    //printf("checkread.index = %d\n",checkread.index);

    if(checkread.found!=0 && (checkread.index+ strlen(checkread.str))<=strlen(string)){
        //printf("checkread.value = %c\n",string[checkread.index+ strlen(checkread.str)]);
        char delim=string[checkread.index+ strlen(checkread.str)];
        //printf("delim = %c\n",delim);
        if(delim=='?' || delim=='|'|| delim=='\n'|| delim=='.'|| delim==','){
            //printf("return 0");
            return 0;

        }
    }

    if(checkread.a-1>=0 && checkread.a+1<count){
        //printf("arrayWords[checkread.a-1] = %s\n",arrayWords[checkread.a-1]);
        //printf("arrayWords[checkread.a] = %s\n",arrayWords[checkread.a]);
        //printf("arrayWords[checkread.a+1] = %s\n",arrayWords[checkread.a+1]);
        if(checkread.found !=0
                &&(
                    strcmp(checkread.str,substring(arrayWords[checkread.a-1],0,strlen(checkread.str)))!=0
                    &&strcmp(checkread.str,substring(arrayWords[checkread.a],0,strlen(checkread.str)))!=0
                    &&strcmp(checkread.str,substring(arrayWords[checkread.a+1],0,strlen(checkread.str)))!=0) ){
            //printf("return 0 187");

            return 0;
        }}


    if(strlen(copy)<strlen(string))
        return 0;
    char space[1000] ="";
    strcpy(space,copy);
    char *pch=NULL;

    char *str2 =NULL;
    // Find the next space in the string, and replace it with a newline.
    //while (space = strchr(space,' ')) *space = '\n';
    pch=strtok(space,space_characters);
    int length=0;
    while (pch != NULL)
    {

        size_t len = strlen(pch);
        char str2[1000] ="";
        //(char*) malloc(len + 1 + 1 ); /* one for extra char, one for trailing zero */
        strcpy(str2, (pch));
        str2[len] = '\0';
        length=strlen(str2);


        pch = strtok (NULL, space_characters);
        // free(str2);
    }

    // There are no more spaces in the string; print out our modified copy.
    //printf("%s\n", copy);
    // Free our local copy
    // free(copy);


    return length;
}

// Find the maximum of input array
int findMax(int a[],int length){
    int large=a[0];
    int index=0;
    for (int i=1;i<length;i++){
        if(a[i]>large){
            large=a[i];
            index=i;
        }
    }
    return index;
}

// Do XOR of strings
char* doXOR(char* str1,char * str2,int sizetoallocate){


    for(int i=0;i<sizetoallocate;i++){
        result[i]=str1[i]^str2[i];

    }
    result[sizetoallocate]='\0';
    return result;
}

// Read all the words from the words file from home directory
void readWordFile(){

    char * filename=(char*)malloc(1000);
    strcpy(filename,getHomeDir());
    strcat(filename,"/words");
    filename[strlen(filename)]='\0';
    FILE* myinput = fopen(filename,"rt");

    fseek(myinput,0,SEEK_END);

    size = ftell(myinput);

    fseek(myinput,0,SEEK_SET);

    data = (char*)malloc(size);

    arrayWords= (char**)malloc(size+4);


    fread(data,size,1,myinput);


    fclose(myinput);

    free(filename);
}
int checkAllValues(int b[],int length){
    int i=0;
    int readable=1;
    for (int k=0;k<length;k++){
        //printf("b[ %d ]= %d\n",i,b[k]);
        if(b[k]==0){
            i++;
        }
    }
    //printf("i =%d length=%d\n",i,length);
    if(i==length)
        readable=0;
    else
        readable=1;
    return readable;

}


// Check whether reached final decoded messages or not
readablecipher verifyreadablecipher( int readable12, int readable13, int readable14,
                                     char  cipherxor12[], char  cipherxor13[], char  cipherxor14[],
                                     char readableOrNot12[], char  readableOrNot13[], char  readableOrNot14[], int sizeofguess,char  tempguessword[]){
    int readable=0;
    char  readableOrNot[1000]="\0";
    int a[3]={readable12,readable13,readable14};
    if(a[0]==1 && a[1]==1 && a[2]==1){ //111
        copybinaryarr(readableOrNot,tempguessword,sizeofguess);

        readable=1;
        if(readable==1)
            printReadbleDecodedFinal(tempguessword,readableOrNot12,readableOrNot13,readableOrNot14);
    }
    readablecipher abc;
    abc.readable=readable;
    strcpy(abc.readableorNot,readableOrNot);
    return abc;
}

// Decision making API to check which string to be parsed to get the readable string based on XOR with other strings and find the best of them based on size of last word

readablecipher findreadablecipher( int readable12, int readable13, int readable14,
                                   char  cipherxor12[], char  cipherxor13[], char  cipherxor14[],
                                   char readableOrNot12[], char  readableOrNot13[], char  readableOrNot14[], int sizeofguess, char tmpguessword[]){
    int readable=0;
    char  readableOrNot[1000]="\0";
    char temp[1000]="\0";
    strcpy(temp,tmpguessword);
    int a[3]={readable12,readable13,readable14};
    if(a[0]==1 && a[1]==1 && a[2]==1){ //111


        readable=1;
        // tmpguessword[strlen(tmpguessword)-1]='\0';

        printReadbleDecoded(temp,readableOrNot12,readableOrNot13,readableOrNot14);
        // if(readable==1)
        //    readable= checkAllValues(b,3);

        stu verifyst1=expandSearch(temp);
        stu verifyst2=expandSearch(readableOrNot12);
        stu verifyst3=expandSearch(readableOrNot13);
        stu verifyst4=expandSearch(readableOrNot14);
        testvalue=0;
        int lastword1=lastword(temp,verifyst1.str);
        int lastword2=lastword(readableOrNot12,verifyst2.str);
        int lastword3=lastword(readableOrNot13,verifyst3.str);
        int lastword4=lastword(readableOrNot14,verifyst4.str);
        int validatest1=validatewords(temp,verifyst1);
        int validatest2=validatewords(readableOrNot12,verifyst2);
        int validatest3=validatewords(readableOrNot13,verifyst3);
        int validatest4=validatewords(readableOrNot14,verifyst4);
        //printf("verifyst1.found find= %d\n",verifyst1.found);
        //printf("verifyst2.found find= %d\n",verifyst2.found);
        //printf("verifyst3.found find= %d\n",verifyst3.found);
        //printf("verifyst4.found find= %d\n",verifyst4.found);
        //printf("validatest1 find= %d\n",validatest1);
        //printf("validatest2 find= %d\n",validatest2);
        //printf("validatest3 find= %d\n",validatest3);
        //printf("validatest4 find= %d\n",validatest4);
        char key1[1000]="\0";
        copybinaryarr(key1, doXOR( cipher000,temp,sizeofguess),sizeofguess);
        char key2[1000]="\0";
        copybinaryarr(key2, doXOR( cipher001,readableOrNot12,sizeofguess),sizeofguess);
        char key3[1000]="\0";
        copybinaryarr(key3, doXOR( cipher002,readableOrNot13,sizeofguess),sizeofguess);
        char key4[1000]="\0";
        copybinaryarr(key4, doXOR( cipher003,readableOrNot14,sizeofguess),sizeofguess);


        if( verifyst1.found!=0 && lastword1 !=0){
            readable=0;
        }
        if( verifyst2.found!=0 && lastword2!=0){
            readable=0;
        }
        if( verifyst3.found!=0  && lastword3!=0){
            readable=0;
        }
        if( verifyst4.found!=0 && lastword4!=0){
            readable=0;
        }
        //printf("readable find= %d\n",readable);

        //printf("readable after key= %d\n",readable);
        strcpy(readableOrNot,readableOrNot12);
        if(readable==0){
            if(comparecipher(key1,key2,sizeofguess)!=0 &&  comparecipher(key2,key3,sizeofguess)!=0 &&
                    comparecipher(key3,key4,sizeofguess)!=0 && comparecipher(key4,key1,sizeofguess)!=0){
                readable=1;

            }
        }
        if( testvalue==0&&readable==1 && (verifyst1.found==0||validatest1==1)&& (verifyst2.found==0||validatest2==1)&& (verifyst3.found==0||validatest3==1)&& (verifyst4.found==0||validatest4==1)){

            readable=1;
        }else{
            readable=0;
        }


    }
    readablecipher abc;
    abc.readable=readable;
    strcpy(abc.readableorNot,readableOrNot);
    return abc;
}


// Parse the words files and stores in arraywords array
void printWords(const char *string) {
    // Make a local copy of the string that we can manipulate.
    static const char *space_characters = " \t\n\r";
    char * const copy = strdup(string);
    char *space = copy;
    char *pch=NULL;
    // Find the next space in the string, and replace it with a newline.
    //while (space = strchr(space,' ')) *space = '\n';
    pch=strtok(space,space_characters);
    int i=0;
    while (pch != NULL)
    {
        size_t len = strlen(pch);
        char *str2 =(char*) malloc(len + 1 + 1 ); /* one for extra char, one for trailing zero */
        strcpy(str2, (pch));
        str2[len] = '\0';

        arrayWords[i]=str2;

        i++;
        pch = strtok (NULL, space_characters);
    }
    arrayWords[i]=".";
    arrayWords[i+1]="?";
    arrayWords[i+2]="!";
    arrayWords[i+3]="\n";
    count=i+4;

    // There are no more spaces in the string; print out our modified copy.
    //printf("%s\n", copy);
    // Free our local copy
    free(copy);
}
/* return a new string with every instance of ch replaced by repl */
char *replace(const char *s, char ch, const char *repl) {
    int count = 0;
    const char *t;
    for(t=s; *t; t++)
        count += (*t == ch);
    size_t rlen = strlen(repl);
    char *res = (char*)malloc(strlen(s) + (rlen-1)*count + 1);
    char *ptr = res;
    for(t=s; *t; t++) {
        if(*t == ch) {
            memcpy(ptr, repl, rlen);
            ptr += rlen;
        } else {
            *ptr++ = *t;
        }
    }
    *ptr = 0;
    return res;
}

// Convert string to hex code
char* string_to_hex (char *str)
{
    char *buffer=NULL;
    buffer=(char *)malloc((strlen(str)+1));
    memset(buffer,0,sizeof(buffer));
    int n,i=0;
    while (*str){
        n=sprintf (&buffer[i], "%x",*str );
        str++;
        i=i+2; //assuming each char will be converted to 2 chars in hex string
    }
    return buffer;
}

//Pad the extra spaces at the end
void pad(char *s, int length)
{
    int l;
    l = strlen(s); /* its length */
    while(l<length) {
        s[l] =' '; /* insert a space */
        l++;
    }
    s[l]= '\0'; /* strings need to be terminated in a null */
}

// Write the cipher text to file in home dir
void writeCipherToFile(char * filename,char * msg,char * key, char  p){

    char filename1[1000]="";
    strcpy(filename1,getHomeDir());
    strcat(filename1,"/");
    filename1[strlen(filename1)]='\0';
    size_t len = strlen(filename);
    char str2[1000] =""; /* one for extra char, one for trailing zero */
    strcpy(str2, filename);
    str2[len] = p;
    str2[len + 1] = '\0';
    char * cipher001=(char *)malloc((strlen(msg)+1));
    FILE * fp=fopen(strcat(filename1,str2),"wb");
    for(int i=0;i<strlen(msg);i++){
        cipher001[i]=msg[i]^key[i];
        fputc(cipher001[i],fp);

    }
    if(cipher001!=NULL){
        free(cipher001);
        cipher001=NULL;
    }
    //    if(str2!=NULL){
    //        free(str2);
    //        str2=NULL;
    //    }

    if(fp !=NULL){
        fclose(fp);
        fp =NULL;
    }
}

// Writes the final messages and key to out files
void write_data(const char *filename, const char *data)
{
    char * filename1=(char*)malloc(1000);
    strcpy(filename1,strdup(getHomeDir()));
    strcat(filename1,"/");
    filename1[strlen(filename1)]='\0';
    size_t len = strlen(filename);
    char *str2 =(char*) malloc(len + 1 ); /* one for extra char, one for trailing zero */
    strcpy(str2, strdup(filename));
    // str2[len] = p;
    str2[len + 1] = '\0';
    FILE *fp = fopen(strcat(filename1,str2),"w");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}

//  Read the cipher text from a file
char* readCipherFromFile(char * filename){
    int localsize=0;
    char * filename1=(char*)malloc(1000);
    strcpy(filename1,getHomeDir());
    strcat(filename1,replace(filename,'~',""));
    filename1[strlen(filename1)]='\0';
    FILE * fp=fopen(filename1,"rb");
    if( fp == NULL )
       {
          return NULL;
       }
    fseek(fp,0,SEEK_END);
    localsize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char *cipherdata = (char*)malloc(localsize);
    fread(cipherdata,localsize,1,fp);

    if(localsize>ciphersize)
        ciphersize=localsize;
    fclose(fp);
    free(filename1);
    return cipherdata;
}

// does a binary search and return a struct of found or not and index
bin binary_search(char **list_of_words, int size, char *target){
    int bottom= 0;
    int mid;
    int top = size - 1;
    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(list_of_words[mid], target) == 0){

            bn.abin=0;
            bn.indexbin=mid+1;
            return bn;
        } else if (strcmp(list_of_words[mid], target) > 0){
            top    = mid - 1;
        } else if (strcmp(list_of_words[mid], target) < 0){
            bottom = mid + 1;
        }
    }

    bn.abin=mid;
    bn.indexbin=mid;
    return bn;
}

// fins the index of last word in a string
int strstr_last( char* s1,  char* s2)
{
    char* ptr=strstr(s1,s2);
    return ptr-s1;
}

// Expand your and string to find the probable words from dictionary
stu expandSearch(char * string){
    // Make a local copy of the string that we can manipulate.

    static const char *space_characters = " \t\n\r";

    char   copy[1000] ="";
    strcpy(copy,string);


    char space[1000] ="";
    strcpy(space,copy);

    strcpy(space,trimwhitespace(space));

    char  delimiters[]=" ?.\n!,";

    for (int a=0;a<strlen(delimiters);a++){

        strcpy(space , replace(space,delimiters[a],"  "));

    }
    char *pch=NULL;


    // Find the next space in the string, and replace it with a newline.
    //while (space = strchr(space,' ')) *space = '\n';
    pch=strtok(space,space_characters);

    int i=0;
    stu st;
    while (pch != NULL)
    {
        size_t len = strlen(pch);
        char str2[20] ="\0"; /* one for extra char, one for trailing zero */
        strcpy(str2, (pch));

        bin fnd=binary_search(arrayWords,count,str2);
        int found=fnd.abin;
        st.a=fnd.indexbin;//track the word match index in word list
        st.found=found;

        st.index= strstr_last(string,str2)    ;

        strcpy(st.str,str2);


        if(found==0){//full word match

            i++;
            pch = strtok (NULL, space_characters);
        }else{//partial word match

            return st;
        }
    }

    // Free our local copy
    // free(copy);
    return st;
}
/**
 * This method recursively calls and decodes the messages. The algorithm is based on guessing a word at the start of any of the messages and do XOR with other to see which of them will be readable..if readable take the string whose last word is of longest length and expand it and see if you get a readable.. this method works on LOOK AHEAD methodology and see the possible words fits in then go XOR with other strings to see if they are readable
 * */

char * cribdrag(
        char  cipherxor12[],
        char  cipherxor13[],
        char  cipherxor14[],
        int sizeofcipher,
        char  guessword[],
        char decode1[],
        char decode2[])
{
    fflush(stdout);
    int readable12=0;
    int readable13=0;
    int readable14=0;
    int readable=0;
    int reset=0;
    //printcipher( cipherxor12,    sizeofcipher ,            "cipherxor12\0",            "914\0");
    //printcipher(cipherxor13,sizeofcipher ,"cipherxor13\0","914\0");
    //printcipher(cipherxor14,sizeofcipher ,"cipherxor14\0","914\0");
    //printcipher(cipherxor12,ciphersize ,"cipherxor12 ");
    if(cipherxor12==NULL||cipherxor13==NULL ||cipherxor14==NULL  ||guessword==NULL ){
        return NULL;
    }

    //    char loc_guessword[25]="\0";
    //    char  newguessword[25]="\0";
    char temp[1000]="\0";
    char  readableOrNot[1000]="\0";
    //printcipher(cipherxor12,sizeofguess ,"cipherxor12","944");
    //printcipher(cipherxor13,sizeofguess ,"cipherxor13","944");
    //printcipher(cipherxor14,sizeofguess ,"cipherxor14","944");

    char readableOrNot12[1000]="\0";
    char readableOrNot13[1000]="\0";
    char readableOrNot14[1000]="\0";
    char  verification1[1000]="\0";
    char  verification2[1000]="\0";
    char  verification3[1000]="\0";
    char  verifyreadableOrNot[1000]="\0";
    readablecipher readcipher;
    readablecipher verifybestmatchcipher;
    int verifiy1=0;
    int verifiy2=0;
    int verifiy3=0;
    int verifiy4=0;
    int verifyreadable=0;
    stu verifyst1;
    stu verifyst2;
    stu verifyst3;
    stu verifyst4;
    stu removelast;


    //strcpy(loc_guessword,guessword);

    sizeofguess=strlen(guessword);

    while(true){


        if(decode1!=NULL && strlen(decode1)!=0){
            strcpy(temp,tempguessword);
            copybinaryarr(tempguessword,decode1,decodedvalue);
            for (int j=0;j<strlen(temp);j++){

                tempguessword[decodedvalue+j]=temp[j];
            }
            tempguessword[decodedvalue+strlen(temp)]='\0';

        }

        sizeofguess=decodedvalue+strlen(temp);
        if(sizeofguess==0)
            sizeofguess=strlen(tempguessword);

        //printf("sizeofguess 1182 = %d\n",sizeofguess);
        //printf("tempguessword 1182 = %s\n",tempguessword);


        copybinaryarr(readableOrNot12,doXOR(cipherxor12,tempguessword,sizeofguess),sizeofguess);

        copybinaryarr(readableOrNot13,doXOR(cipherxor13,tempguessword,sizeofguess),sizeofguess);
        copybinaryarr(readableOrNot14,doXOR(cipherxor14,tempguessword,sizeofguess),sizeofguess);
        //printf("readableOrNot12 1182 = %s\n",readableOrNot12);
        //printf("readableOrNot13 1182 = %s\n",readableOrNot13);
        //printf("readableOrNot14 1182 = %s\n",readableOrNot14);
        //printf("tempguessword 1182 = %s\n",tempguessword);
        readable12=checkallowedchar(readableOrNot12,sizeofguess);
        readable13=checkallowedchar(readableOrNot13,sizeofguess);
        readable14=checkallowedchar(readableOrNot14,sizeofguess);
        //printf("readable12 978 = %d\n",readable12);
        //printf("readable13 978 = %d\n",readable13);
        //printf("readable14 978 = %d\n",readable14);

        readcipher= findreadablecipher(readable12,readable13,readable14,cipherxor12,cipherxor13,cipherxor14,readableOrNot12,readableOrNot13,readableOrNot14,sizeofguess,tempguessword);
        readable=readcipher.readable;
        strcpy(readableOrNot,readcipher.readableorNot);
        //printf("tempguessword 985 = ,%s,\n",tempguessword);
        //printf("arraywordtrackIndex 985 = %d\n",arraywordtrackIndex);
        //printf("arraywordtrackIndex 985 = %s\n",arrayWords[arraywordtrackIndex]);
        //printf("readable 985 = %d\n",readable);
        //printf("readableOrNot 985 = %s\n",readableOrNot);


        if(readable==0){//Non readable slide the word
            /*if(sizeofguess==ciphersize ){
            //printf("arraywordtrackIndex 1154 = %d\n",arraywordtrackIndex);
            //printf("arrayWords 1154 = %s\n",arrayWords[arraywordtrackIndex]);
            //printf("sizeofguess 1154 = %d\n",sizeofguess);
            //printf("ciphersize 1154 = %d\n",ciphersize);
            //printf("tempguessword[sizeofguess-1] 1154 = %c\n",tempguessword[sizeofguess-1]);
}*/


            if(sizeofguess==ciphersize && tempguessword[sizeofguess-1]==' '){
                strcpy(tempguessword,temp);
                tempguessword[strlen(temp)-1]='.';
            }else if(sizeofguess==ciphersize && tempguessword[sizeofguess-1]=='.'){
                strcpy(tempguessword,temp);
                tempguessword[strlen(temp)-1]='?';
            }else if(sizeofguess==ciphersize && tempguessword[sizeofguess-1]=='?'){
                strcpy(tempguessword,temp);
                tempguessword[strlen(temp)-1]='!';
            }else if(sizeofguess==ciphersize && tempguessword[sizeofguess-1]=='!'){
                strcpy(tempguessword,temp);
                tempguessword[strlen(temp)-1]='\n';
            }else if(++arraywordtrackIndex<count){

                strcpy(temparrayvalue,arrayWords[arraywordtrackIndex]);
                if(strcmp(temparrayvalue,".")!=0 && strcmp(temparrayvalue,"?")!=0 && strcmp(temparrayvalue,"!")!=0 && strcmp(temparrayvalue,"\n")!=0 ){

                    strcpy(tempguessword,arrayWords[arraywordtrackIndex]);

                    strcat(tempguessword," ");
                }else{
                    if(strcmp(temparrayvalue,".")==0 ){
                        decode1[decodedvalue-1]='.';

                    }else if(strcmp(temparrayvalue,"?")==0 ){
                        decode1[decodedvalue-1]='?';
                    }else if(strcmp(temparrayvalue,"!")==0 ){
                        decode1[decodedvalue-1]='!';
                    }else if(strcmp(temparrayvalue,"\n")==0 ){
                        decode1[decodedvalue-1]='\n';
                    }
                    tempguessword[0]=' ';
                    tempguessword[1]='\0';
                }


            }else{
                //printf("count =%d",count);

                //printf("decode1 =,%s,\n",decode1);
                decodedvalue=sizeofguess;

                if(decodedvalue>0&& decode1[decodedvalue-1]!=' '){
                    decode1[decodedvalue-1]=' ';
                    decode1[decodedvalue]='\0';
                }else{
                    decode1[decodedvalue]='\0';
                }
                //printf("decode1 =,%s,\n",decode1);

                //        if (numberofwords(tempguessword)!=1){
                //                removelastword(tempguessword);
                //        }
                removelast=expandSearch(tempguessword);
                removelastword(tempguessword);
                arraywordtrackIndex=removelast.a+1;
                sizeofguess=strlen(tempguessword);
                //printf("arraywordtrackIndex 985 = %d\n",arraywordtrackIndex);
                //printf("arrayword 985 = %s\n",arrayWords[arraywordtrackIndex]);

                copybinaryarr(decode1,tempguessword,sizeofguess);
                decodedvalue=sizeofguess;
                if(decodedvalue>0&& decode1[decodedvalue-1]!=' '){
                    decode1[decodedvalue-1]=' ';
                    decode1[decodedvalue]='\0';
                }else{
                    decode1[decodedvalue]='\0';
                }
                decode1[decodedvalue]='\0';
                strcpy(tempguessword,arrayWords[arraywordtrackIndex]);

                strcat(tempguessword," ");


                //printf("tempguessword 1415 = %s\n",tempguessword);
                //printf("decode1 1415 = %s\n",decode1);

                return NULL;

            }
          }
        else{//find the probable match
            //            //printf("length ofreadableOrNot 1054 = %d\n",strlen(readableOrNot));
            //            //printf("length tempguessword 1054 = %d\n",strlen(tempguessword));
            //            //printf("length ciphersize 1054 = %d\n",ciphersize);
            //            //printf(" tempguessword 1054 = %s\n",tempguessword);
            //            //printf(" readableOrNot 1054 = %s\n",readableOrNot);
            strcpy(finaltest,tempguessword);
            strcpy(finaltest,trimwhitespace(finaltest));
            //            //printf(" finaltest 1054 = %d\n",finaltest);
            //            //printf(" size finaltest 1054 = %d\n",strlen(finaltest));

            if((strlen(finaltest)== ciphersize)){

                copybinaryarr(verification1, doXOR(tempguessword,cipherxor12,ciphersize),sizeofguess);
                copybinaryarr(verification2,doXOR(tempguessword,cipherxor13,ciphersize),ciphersize);
                copybinaryarr(verification3,doXOR(tempguessword,cipherxor14,ciphersize),ciphersize);
                verifiy1=checkallowedchar(verification1,ciphersize);
                verifiy2=checkallowedchar(verification2,ciphersize);
                verifiy3=checkallowedchar(verification3,ciphersize);


                verifybestmatchcipher= verifyreadablecipher(verifiy1,verifiy2,verifiy3,cipherxor12,cipherxor13,cipherxor14,verification1,verification2,verification3,ciphersize,tempguessword);
                verifyreadable=verifybestmatchcipher.readable;
                copybinaryarr( verifyreadableOrNot,verifybestmatchcipher.readableorNot,ciphersize);

                copybinaryarr( verification1,doXOR(verifyreadableOrNot,cipherxor12,ciphersize),ciphersize);
                copybinaryarr( verification2,doXOR(verifyreadableOrNot,cipherxor13,ciphersize),ciphersize);
                copybinaryarr(verification3,doXOR(verifyreadableOrNot,cipherxor14,ciphersize),ciphersize);
                verifiy1=checkallowedchar(verification1,ciphersize);
                verifiy2=checkallowedchar(verification2,ciphersize);
                verifiy3=checkallowedchar(verification3,ciphersize);
                verifyst1=expandSearch(verifyreadableOrNot);
                verifyst2=expandSearch(verification1);
                verifyst3=expandSearch(verification2);
                verifyst4=expandSearch(verification3);

                if(verifyreadable==1 && verifyst1.found==0&& verifyst2.found==0&& verifyst3.found==0&& verifyst4.found==0){

                    printReadbleDecodedFinal(verifyreadableOrNot,verification1,verification2,verification3);
                    write_data("msg000",verifyreadableOrNot);
                    write_data("msg001",verification1);
                    if(numberofciphers!=2){
                    write_data("msg002",verification2);
                    }
                    if(numberofciphers!=3 && numberofciphers!=2){
                    write_data("msg003",verification3);
                    }
                    // //printf("Done!! Please press enter to exit");

                    return verifyreadableOrNot;
                }
            }
         strcat(tempguessword," ");
            if(readable){
                copybinaryarr(decode1,tempguessword,sizeofguess);
                decodedvalue=sizeofguess;
                decode1[decodedvalue]='\0';
            }
            arraywordtrackIndex=0;


            if(arraywordtrackIndex>=count){
                decodedvalue=sizeofguess;

                if(decode1[decodedvalue-1]!=' '){
                    decode1[decodedvalue]=' ';
                    decode1[++decodedvalue]='\0';
                }else{
                    decode1[decodedvalue]='\0';
                }

                //printf("count =%d",count);
                //printf("decode1 =,%s,\n",decode1);

                decodedvalue=sizeofguess;

                if(decodedvalue>0&& decode1[decodedvalue-1]!=' '){
                    decode1[decodedvalue-1]=' ';
                    decode1[decodedvalue]='\0';
                }else{
                    decode1[decodedvalue]='\0';
                }

                removelast=expandSearch(tempguessword);
                removelastword(tempguessword);
                arraywordtrackIndex=removelast.a+1;
                sizeofguess=strlen(tempguessword);


                copybinaryarr(decode1,tempguessword,sizeofguess);
                decodedvalue=sizeofguess;
                if(decodedvalue>0&& decode1[decodedvalue-1]!=' '){
                    decode1[decodedvalue-1]=' ';
                    decode1[decodedvalue]='\0';
                }else{
                    decode1[decodedvalue]='\0';
                }

                decode1[decodedvalue]='\0';
                strcpy(tempguessword,arrayWords[arraywordtrackIndex]);

                strcat(tempguessword," ");


                //printf("tempguessword 1741 = %s\n",tempguessword);
                //printf("decode1 1741 = %s\n",decode1);

                return NULL;
            }

            strcpy(temparrayvalue,arrayWords[arraywordtrackIndex]);
            if(strcmp(temparrayvalue,".")!=0 && strcmp(temparrayvalue,"?")!=0 && strcmp(temparrayvalue,"!")!=0 && strcmp(temparrayvalue,"\n")!=0 ){
                strcpy(tempguessword,arrayWords[arraywordtrackIndex++]);
                strcat(tempguessword," ");
            }else{
                if(strcmp(temparrayvalue,".")==0 ){
                    decode1[decodedvalue-1]='.';

                }else if(strcmp(temparrayvalue,"?")==0 ){
                    decode1[decodedvalue-1]='?';
                }else if(strcmp(temparrayvalue,"!")==0 ){
                    decode1[decodedvalue-1]='!';
                }else if(strcmp(temparrayvalue,"\n")==0 ){
                    decode1[decodedvalue-1]='\n';
                }
                tempguessword[0]=' ';
                tempguessword[1]='\0';
            }


        }
    }

}
void createTestCipher(){
    char*  message1="abandon add? absolve";
    char*  message2="hello there try best";
    char*  message3="the project is going";
    char*  message4="where is it going to";
    char*  key     ="32131454242890087678";


    int num_msgs=4;
    int ciphersize1=strlen(message1);
    char keytmp[1000]="";
    //  memset(keytmp,0,ciphersize1);
    char msgtmp1[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp1,0,ciphersize1);
    char msgtmp2[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp2,0,ciphersize1);
    char msgtmp3[1000]="";
    //(char*)malloc(ciphersize1);
    memset(msgtmp3,0,ciphersize1);
    char msgtmp4[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp4,0,ciphersize1);
    char keytmptest[1000]="";
    // (char*)malloc(ciphersize1);
    //memset(keytmptest,0,ciphersize1);
    char msgtmp1test[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp1test,0,ciphersize1);
    char msgtmp2test[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp2test,0,ciphersize1);
    char msgtmp3test[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp3test,0,ciphersize1);
    char msgtmp4test[1000]="";
    //(char*)malloc(ciphersize1);
    //memset(msgtmp4test,0,ciphersize1);
    strcpy(keytmptest,key);
    strcpy(msgtmp1test,message1);
    strcpy(msgtmp2test,message2);
    strcpy(msgtmp3test,message3);
    strcpy(msgtmp4test,message4);

    char* arr[]={msgtmp1test,msgtmp2test,msgtmp3test,msgtmp4test};

    for (int j=0;j<num_msgs;j++){
        char dig = (char)(((int)'0')+j);
        writeCipherToFile("cipher00",arr[j],keytmptest,dig);
    }


}
int main(int argc, char **argv)
{
    //Uncomment below line for generating test ciphers into a file which can be read back
    //  createTestCipher();

    //Read the word file into an array to use later
    readWordFile();

    //storing data read from file into arrayWord variable
    printWords(data);

    int args;
    numberofciphers=4;
    // Read all 4 ciphers from disk which are in home directory
//    printf("Enter an number of ciphers: ");
//    if (( args = scanf("%d", &numberofciphers)) == 0) {
//        //printf("Error: not an integer\n");
//    }else if(numberofciphers<2 || numberofciphers>4){
//        //printf("Not valid input\n");
//    } else {
//        //printf("Read in %d\n", numberofciphers);
//    }
   // numberofciphers=x;
            cipher000=readCipherFromFile("~/cipher000");
            cipher001=readCipherFromFile("~/cipher001");

            cipher002=readCipherFromFile("~/cipher002");
            if(cipher002==NULL){
                cipher002=(char*)malloc(ciphersize);
                 copybinaryarr(cipher002,cipher001,ciphersize);
                 numberofciphers=2;
            }
            cipher003=readCipherFromFile("~/cipher003");
            if(cipher003==NULL){
                cipher003=(char*)malloc(ciphersize);
                 copybinaryarr(cipher003,cipher002,ciphersize);
                 if(numberofciphers!=2)
                 numberofciphers=3;
            }

//    switch (numberofciphers){
//    case 4:
//        cipher000=readCipherFromFile("~/cipher000");
//        cipher001=readCipherFromFile("~/cipher001");
//        cipher002=readCipherFromFile("~/cipher002");
//        cipher003=readCipherFromFile("~/cipher003");
//        break;
//    case 3:
//        cipher000=readCipherFromFile("~/cipher000");
//        cipher001=readCipherFromFile("~/cipher001");
//        cipher002=readCipherFromFile("~/cipher002");
//        cipher003=(char*)malloc(ciphersize);
//        copybinaryarr(cipher003,cipher002,ciphersize);
//        break;
//    case 2:
//        cipher000=readCipherFromFile("~/cipher000");
//        cipher001=readCipherFromFile("~/cipher001");
//        cipher002=(char*)malloc(ciphersize);
//        cipher003=(char*)malloc(ciphersize);
//        copybinaryarr(cipher002,cipher001,ciphersize);
//        copybinaryarr(cipher003,cipher001,ciphersize);

//        break;
//    default :
//        cipher000=readCipherFromFile("~/cipher000");
//        cipher001=readCipherFromFile("~/cipher001");
//        cipher002=readCipherFromFile("~/cipher002");
//        cipher003=readCipherFromFile("~/cipher003");

//    }

    //XOR 1st cipher with other 3 ciphers and store
    char cipherxor12[1000]="\0";
    copybinaryarr(cipherxor12,doXOR(cipher000,cipher001,ciphersize),ciphersize);
    char cipherxor13[1000]="\0";
    copybinaryarr(cipherxor13,doXOR(cipher000,cipher002,ciphersize),ciphersize);
    char cipherxor14[1000]="\0";
    copybinaryarr(cipherxor14,doXOR(cipher000,cipher003,ciphersize),ciphersize);

    //XOR 1st cipher with other 3 ciphers and store to set back to original if needed.
    copybinaryarr(cipherxor12dup,cipherxor12,ciphersize);
    copybinaryarr(cipherxor13dup,cipherxor13,ciphersize);
    copybinaryarr(cipherxor14dup,cipherxor14,ciphersize);

    // Call to cribdrag method to do decryption
    char* message=NULL;
    //removelastword("avaca");
    while ( message==NULL){
        message=cribdrag(cipherxor12,cipherxor13,cipherxor14,ciphersize,arrayWords[0],decode1,decode2);

    }
    char * key=doXOR(message,cipher000,ciphersize);
    printf("Key is = %s\n",key);
    printf("\n=======================================\n");

    write_data("pad",key);



    free(data);
    free(arrayWords);
    fflush ( stdout );
   // getchar();
    return 0;
}
