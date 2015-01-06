//Authors : Arjun M, Nagarjuna M, Varun K
//version 2.0

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

//-----------CONSTANTS------------

#define SERVERA_PORT 11111
#define SERVERB_PORT 11112
#define MAX_LINE 1024
#define MAX_PENDING 5

//The cards are stored as an array from 48 to 99. This is to reduce the
//effect of small numbers not encrypting normally
const int CARDS_START   =  48;
const int CARDS_END     = 99;

const int DEBUG   = 1;
const int INFO    = 2;
const int ERROR   = 3;

//-------GLOBAL VARIABLES---------
bool receiveInitialized = false, sendInitialized = false;
struct sockaddr_in server,client;
int sendSock=-1, receiveSock=-1; //Stores the socket values.
string connHost = "localhost";

//------FUNCTION DEFINITIONS------
//Logger module
void log(int mode, string msg) {
    switch(mode) {
    case DEBUG : cout << "DEBUG: " << msg << endl;          break;
    case INFO  : cout << "**INFO**: " << msg << endl;       break;
    case ERROR : cout << "!!!ERROR!!!: " << msg << endl;    break;
    }
}

//Split Function to tokenize the string s into Vector items.
void split(string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

//Initializes the Client (to send) on PORT and returns the Socket ID
//Author Varun
int initializeClient(int MYPORT) {
    //log(DEBUG, "Enter Client");
    int sockfd;
  //  struct sockaddr_in client; // connector's address information
    struct hostent *he;

    if ((he=gethostbyname(connHost.c_str())) == NULL) {  // get the host info
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    client.sin_family = AF_INET;     // host byte order
    client.sin_port = htons(MYPORT); // short, network byte order
    client.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(client.sin_zero), '\0', 8);  // zero the rest of the struct

    return sockfd;
}

//Sends the String to designated socket
//Author Varun
int send(string str, int sockId) {
    //struct sockaddr_in  client;
    char buf[MAX_LINE];
    int ret, n;

    //put into string
    for(unsigned int i = 0; i<MAX_LINE-1; i++) {
        buf[i] = str[i];
    }
    buf[MAX_LINE-1] = '\0';
    //log(DEBUG, "Ready to send:" + string(buf));
    //cout << "sockId:" << sockId << endl;
    if (sizeof(buf)>0) {

        n = strlen(buf);
        ret = sendto(sockId, buf, n, 0,(struct sockaddr *)&client, sizeof(client));
        if( ret != n) {
            log(ERROR,"Datagram Send error: " );
            cout << "ret:" << ret << endl;
            return -1;
        } else {
            //log(DEBUG, "Datagram Send Success: ");
            // cout << "ret:" << ret << endl;
        }
    }

    return 1;
}

//Initialize Server(to receive) on PORT and returns socket ID
//Author Arjun, Varun
int initializeServer(int MYPORT) {

    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server.sin_family = AF_INET;         // host byte order
    server.sin_port = htons(MYPORT);     // short, network byte order
    server.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(server.sin_zero), '\0', 8); // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&server,
        sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    //cout << "Server: receiveSock:" << s << endl;
    return sockfd;
}

//Generic receive on sockId
//Author Arjun, Varun
int receive(string &str, int sockId) {
    //log(DEBUG, "Enter Server");
    //cout << "sockId:" << sockId << endl;

    char buf[MAX_LINE];
    int ret;
    socklen_t length=0;
    length = sizeof(server);
    if(1) {
        ret = recvfrom(sockId, buf, 1024, 0, (struct sockaddr *)&server, &length);
        if( ret < 0 ) {
            log(ERROR,"Receive Error: " + ret);
            return -1;
        } else {
            //cout << endl << "Return value:" << ret << endl;
            //log(DEBUG, "Receive Success: " + ret);
        }
        buf[ret] = 0;
        //log( INFO, "Received[" + string(buf) + "]");
    }
    str = string(buf);
    //cout << "str:" << str << endl;

    return str.length();
}

//Called by B, to send a string to A
//Author Varun
int sendToA(string str) {
    //cout << "B sending on port " << SERVERA_PORT << endl;
    if(!sendInitialized){
        int ret = initializeClient(SERVERA_PORT);
        if(ret > 0){
            sendSock = ret;
            sendInitialized = true;
        }
    }
    return send(str, sendSock);
}

//Called by A, to send a string to B
//Author Varun
int sendToB(string str) {
    //cout << "A sending on port " << SERVERB_PORT << endl;
    if(!sendInitialized){
        int ret = initializeClient(SERVERB_PORT);
        if(ret > 0){
            sendSock = ret;
            sendInitialized = true;
        }
    }
    return send(str, sendSock);
}

//Receive String from B
//Author Arjun
int receiveFromA(string& str) {
    //cout << "B receiving on port " << SERVERB_PORT << endl;
    if(!receiveInitialized) {
        int ret = initializeServer(SERVERB_PORT);
        if(ret>0) {
            receiveSock = ret;
            receiveInitialized = true;
        }
    }
    //cout << "Ready to receive from A on receiveSock" << receiveSock << endl;
    return receive(str, receiveSock);
}

//Receive String from B
//Author Arjun
int receiveFromB(string& str) {
    //cout << "A receiving on port " << SERVERA_PORT << endl;
    if(!receiveInitialized) {
        int ret = initializeServer(SERVERA_PORT);
        if(ret>0) {
            receiveSock = ret;
            receiveInitialized = true;
        }
    }
    //cout << "Ready to receive from B on receiveSock" << receiveSock << endl;
    return receive(str, receiveSock);
}

//Display card detail - number and suite
//Author Arjun
string getCardDetail(int cardNum) {
    //log(DEBUG, "Enter getCardDetail");
    if(cardNum>CARDS_END || cardNum<CARDS_START) {
        cout << "Card number " << cardNum << " Invalid. Should be in between " << CARDS_START << " and " << CARDS_END << endl;
        return "ERROR";
    }

    string cardValue = "";
    int suite, num;

    //Mod 13 for number
    num = (cardNum-CARDS_START)%4;

    //log(DEBUG, "num:" + num);
    switch (num) {
    case 0: cardValue.append(" \u2667 \u2663 ");      break; //Clubs
    case 1: cardValue.append(" \u2662 \u2666 ");      break; //Diamonds
    case 2: cardValue.append(" \u2661 \u2665 ");      break; //Hearts
    case 3: cardValue.append(" \u2664 \u2660 ");      break; //Spades
    }

    //Divide by 13, quotient is suite
    suite = (cardNum-CARDS_START)/4;

    //log(DEBUG, "suite:" + suite);
    switch (suite) {
    case 0 : cardValue.append("  2  ");        break;
    case 1 : cardValue.append("  3  ");        break;
    case 2 : cardValue.append("  4  ");        break;
    case 3 : cardValue.append("  5  ");        break;
    case 4 : cardValue.append("  6  ");        break;
    case 5 : cardValue.append("  7  ");        break;
    case 6 : cardValue.append("  8  ");        break;
    case 7 : cardValue.append("  9  ");        break;
    case 8 : cardValue.append("  10 ");        break;
    case 9 : cardValue.append(" Jack");        break;
    case 10: cardValue.append("Queen");        break;
    case 11: cardValue.append(" King");        break;
    case 12: cardValue.append(" Ace ");        break;
    }

    switch (num) {
    case 0: cardValue.append(" \u2663 \u2667 ");      break; //Clubs
    case 1: cardValue.append(" \u2666 \u2662 ");      break; //Diamonds
    case 2: cardValue.append(" \u2665 \u2661 ");      break; //Hearts
    case 3: cardValue.append(" \u2660 \u2664 ");      break; //Spades
    }

    //log(DEBUG, "Exit getCardDetail");
    return cardValue;
}

//Generates GCD
//Author: Nag M, Varun
int getGCD(int primeN) {
    //Get encryption key K from user such that gcd(K,phi(n))=1 i.e. gcd(K,N-1)=1
    int x,y,m,i;
    x=primeN-1;
    int gcdnumbers[1000]={0};
    int index=0;
    //for(int p=1;p<primeN-1;p++){
    for(int p=primeN-2;p>1 && index < 10; p--){
        y=p;
        if(x>y) m=y;
        else    m=x;

        for(i=m;i>=1;i--)
            if(x%i==0&&y%i==0)
                break;

        if(i==1 && p!=1)
            gcdnumbers[index++]=p;
    }
    for(int k=0;k<index && k<10;k++){
        if(gcdnumbers[k]!=0){
            printf(" %d) %d\t",k+1,gcdnumbers[k]);
            if(k%5==4) printf("\n");
        }
    }

    while(true){
        srand(time(NULL));
        y = rand()%10;
        cout << "Random Choice : " << y+1;
        //scanf("%d",&y);
        if(y>=0 && y<10){
            y=gcdnumbers[y];
            cout << "\nMy Encryption Key is : " << y << endl;
            break;
        }else{
            cout << "\nEnter correct index: ";
        }
    }
    return y;
}

//Find modulo power of given card number
//Nagarjuna
int modpower(int cardnumber,int power,int modulo){

    int u=cardnumber%modulo;
    int y=1;
    while(power!=0){
        if(power%2==1){
            y=(y*u)%modulo;
        }
        power=floor(power/2);
        u=(u*u)%modulo;
    }
    return y;
}

//Calculate multipicate inversion of two numbers
//Nagarjuna
int multinverse(int n,int m){
    n %= m;
    for(int x = 1; x < m; x++)
        if((n*x) % m == 1)
            return x;
}

//Getting decryption key
//Nagarjuna
int getDecKey(int encKey, int primeN) {
    //Compute decryption key based on eK and N
    int deckey=multinverse(encKey,primeN-1);
    cout << "My Decryption key for "<< encKey << " is " << deckey <<endl;
    return deckey;
}

//Encrypting cards
//Nagarjuna
int encrypt(int msg, int key, int n) {
    //perform msg to the power key mod n and return result
    int msgencrypt=modpower(msg,key,n);
    //cout << "Your encrypt message: "<< msgencrypt <<endl;
    return msgencrypt;
}

//Decrypting cards
//Nagarjuna
int decrypt(int cipher, int key, int n) {
    //perform cipher to the power key mod n and return result
    //does same as encrypt but good to have a different function for readability
    int msgdecrypt=modpower(cipher,key,n);
    //cout << "Your Decrypt message: "<< msgdecrypt <<endl;
    return msgdecrypt;
}

//Picks a random prime number from a given set of Pre-generated Prime numbers
//Author: Varun KB.
int genRandomPrime()
{
    //Pregenerated Prime numbers from 101 to 1000
    int primes[] ={101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
                   167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269,
                   271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
                   389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
                   503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619,
                   631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751,
                   757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                   883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };

    int size = sizeof(primes)/sizeof(primes[0]);
    srand(time(NULL));
    //cout << "Count of Prime numbers[" << size << "]\n";
    int largePrime = primes[rand()%size];
    cout << "Randomly Selected Large Prime [" << largePrime << "]\n";
    return largePrime;
}

//Displays cards
//Author: Arjun
void displayAllCards(int cards[], int size, bool showValue) {
    if(!showValue) {
        for (int i = 0;  i < size; ){
            cout <<  i << ") " << cards[i++] << "\t";
            if(i%4 == 0)
                cout << endl;
        }
    } else {
        for (int i = 0;  i < size; ){
            cout <<  i << ") " << getCardDetail( cards[i++] ) << "\t\t";
            if(i%2 == 0)
                cout << endl;
        }
    }
}

//Shuffles the cards
//Author: Arjun
void shuffleCards(vector<int>& cards) {
    /*cout << "\nCards Before Shuffle: " << endl ;
    for (int i = 0; i < 52; i++) {
        //cout << cards[i];
        //if(i != 51) cout << ", ";
    }*/
    //cout << endl;
    srand ( time(NULL) );
    for (int i = 51; i >= 0; i--) {
        int j = rand() % (i+1);
        int temp = cards[j];
        cards[j] = cards[i];
        cards[i] = temp;
    }
    cout << "Cards After Shuffle: \n" ;
    for (int i = 0; i < 52; i++) {
        cout << cards[i];
        if(i != 51) cout << ", ";
    }
    cout << endl;
}

int mainA() {


    cout << "*****************************" << endl;
    cout << "********Mental Poker A*******" << endl;
    cout << "*****************************" << endl;

    cout << "\nNOTE: If you have not started B, Close this program, run B first and then run A" << endl << endl;
    usleep(3000000);

    int N, encryKeyA, decryKeyA;
    int allCards[52];
    vector<int> encryAllCards;
    int cardsForA[5];

    //Tasks for A
    //01) Generate Random Large Prime N
    N = genRandomPrime();
    //02) Send to B
    stringstream ss; ss << N;
    sendToB(ss.str());
    cout << "Sent the Large Prime Number [" << N << "] to B" << endl << endl;

    //receive ack from B that he has selected his encryption key.
    cout << "Waiting for B to choose his encryption key..."<<endl;
    string ack;
    int ack_ret = receiveFromB(ack);
    if(ack_ret)
    {
        if(0 == ack.compare("ACK"))
        {
            cout<<"B has chosen his encryption key. Now to choose one for myself."<<endl;
        }
    }
    usleep(1000000);
    //03) Generate GCD gcdA - Display range for user to choose Encryption Key - EKA

    cout << "Generating a set of Encryption Keys. Choose One from below." << endl;
    //Get encryption key A from user such that gcd(A,phi(n))=1
    encryKeyA = getGCD(N);

    //04) Pick and generate Decryption Key A - DKA
    decryKeyA = getDecKey(encryKeyA, N);
    usleep(1000000);
    //05) Encrypt 52 cards
    cout << endl << "Printing the Cards Deck:" << endl;
    //Initialize the 52 cards with 1 to 52 or 48 to 99
    for (unsigned int i=0 ; i < 52 ; i++) {
        allCards[i] = i+CARDS_START;
        cout << getCardDetail(allCards[i]); if(i%4==0) cout<<endl; if(i!=51) cout << ", ";
    }
    cout << endl << endl;
    usleep(1000000);
    cout << "Encrypting the cards:" << endl;
    for (unsigned int i=0 ; i < 52 ; i++) {
        int encrCard = encrypt(allCards[i], encryKeyA, N);
        cout << encrCard; if(i!=51) cout << ", ";
        encryAllCards.push_back(encrCard);
    }
    cout << endl;
    usleep(1000000);
    //06) Shuffle
    //pass address of array for shuffle
    shuffleCards(encryAllCards);
    usleep(1000000);
    //07) Send 52 cards to B
    //convert to string containing , separated elements and send
    string encryptedCardsStr;
    for(unsigned int i=0 ; i < 52 ; i++ ) {
        stringstream enCardStr;
        enCardStr << encryAllCards[i];
        encryptedCardsStr = i==0 ? "" : encryptedCardsStr.append(",") ;
        encryptedCardsStr = encryptedCardsStr.append(enCardStr.str());
    }
    sendToB(encryptedCardsStr);
    cout << "\nShuffled Encrypted Cards Deck is sent to B" << endl;

    cout << "\nWaiting to receive my 5 cards from B ..." << endl;
    //11) Receive 5 cards from B. This is A's cards.
    int encryCardsForA[5]={0};
    string cardsForAStr;
    int cardRecForA=receiveFromB(cardsForAStr);
    usleep(1000000);
    //split by , and load into encryCardsForA
    cout << "Cards Received from B. My cards are : ";
    if(cardRecForA) {
        //Split by , and store in array
        vector<string> tokens;
        split( cardsForAStr, ',' ,tokens);
        for (unsigned int i = 0;  i < tokens.size();   i++){
            encryCardsForA[i]=atoi(tokens[i].c_str());
            cout << encryCardsForA[i]; if(i!=(tokens.size()-1)) cout << ", ";
        }
        cout << endl;
    }
    usleep(1000000);
    cout << "My decrypted cards are : ";
    //12) Decrypt A's cards
    for(unsigned int i = 0; i < 5 ; i++) {
        cardsForA[i] = decrypt(encryCardsForA[i], decryKeyA, N);
        cout << getCardDetail(cardsForA[i]);
        if(i!=4) cout << ", ";
    }
    cout << endl;
    usleep(1000000);
    cout << "\nWaiting for B to send his 5 cards... " << endl;
    //13) Receive 5 cards from B. This is B's cards.
    string encryCardsForBStr;
    int cardRecB=receiveFromB(encryCardsForBStr);
    //split by , and load into encryCardsForB
    int cardsForB[5], encryCardsForB[5];
    if(cardRecB) {
        cout << "Received B's cards : ";
        //Split by , and store in array
        vector<string> tokens;
        split( encryCardsForBStr, ',' ,tokens);
        for (unsigned int i = 0;  i < tokens.size();   i++){
            encryCardsForB[i]=atoi(tokens[i].c_str());
            cout << encryCardsForB[i]; if(i!=(tokens.size()-1)) cout << ", ";
        }
        cout << endl;
    }

    cout << "Now Decrypting and Sending them back..." << endl;
    //14) Decrypt B's cards and send back to B
    for(unsigned int i = 0; i < 5 ; i++) {
        cardsForB[i] = decrypt(encryCardsForB[i], decryKeyA, N);
    }
    string cardsForBStr;
    for(unsigned int i=0 ; i < 5 ; i++ ) {
        stringstream enCardStr;
        enCardStr << cardsForB[i];
        cardsForBStr = i==0 ? "" : cardsForBStr.append(",") ;
        cardsForBStr = cardsForBStr.append(enCardStr.str());
    }
    cout << "Cards sending to B are : " << cardsForBStr << endl;
    usleep(1000000);
    sendToB(cardsForBStr);
    cout << "Cards Sent to B." << endl;
    //--Ready to Play--

    // Display A's cards
    cout << endl << endl << "*****READY TO PLAY*****" << endl << endl;
    cout <<  "**MY HAND**" << endl;
    sort(cardsForA, cardsForA + 5);
    displayAllCards(cardsForA, 5, true);

    int highCardA = cardsForA[4];
    cout << endl << "My high card is " << getCardDetail(highCardA) << endl;
    stringstream highCardAStr;
    highCardAStr << highCardA;

    string highCardBStr;
    receiveFromB(highCardBStr);
    int highCardB = atoi(highCardBStr.c_str());
    cout << endl << "B's high card is " << getCardDetail(highCardB) << endl;
    usleep(1000000);
    sendToB(highCardAStr.str());

    string winner, winnerFromB;
    if(highCardB > highCardA) {
        winner = "B";
        cout << endl << "I LOST!!!" << endl << endl;
    } else {
        winner = "A";
        cout << endl << "I WON!!!" << endl << endl;
    }

    receiveFromB(winnerFromB);

    if (winner == winnerFromB) {
        cout << "Verified Agreement on winner" << endl << endl;
    } else {
        cout << "Conflict in winner announcements" << endl << endl;
    }
    usleep(1000000);
    sendToB(winner);

    // Verification of non-cheating

    string decryKeyBStr;
    receiveFromB(decryKeyBStr);
    int decryKeyB = atoi(decryKeyBStr.c_str());
    cout << "Decryption key of B:" << decryKeyB << endl;

    stringstream decryKeyAStr;
    decryKeyAStr << decryKeyA;
    sendToA(decryKeyAStr.str());

    for (int i=0 ; i < 5 ; i++) {
        cardsForB[i] = decrypt(cardsForB[i], decryKeyB, N);
    }
    sort(cardsForB, cardsForB + 5);
    cout << "B's hand was" << endl;
    displayAllCards(cardsForB, 5, true);

    cout << endl << "B's high card was:" << getCardDetail(cardsForB[4]) << endl;

    if(highCardB==cardsForB[4]) {
        cout << "Cards Verified! B did not cheat." << endl;
    } else {
        cout << "B cheated. Take corrective measures." << endl;
    }

    cout << endl << endl << "Done Mental Poker A: Winner is " << winner << endl << endl;

    return 0;
}

int mainB() {

    cout << "*****************************" << endl;
    cout << "********Mental Poker B*******" << endl;
    cout << "*****************************" << endl;

    int N, encryKeyB, decryKeyB;

    //PUT MAIN under while(1) for Playing Again

    //Tasks for B
    //-----------
    //03) B receives N from A
    string largePrimeNStr;
    int largePrimeN;

    cout << "\nWaiting for A to send the large Prime number..." << endl;
    largePrimeN = receiveFromA(largePrimeNStr);
    largePrimeN = atoi(largePrimeNStr.c_str());
    N = largePrimeN;
    cout << "Prime number received from A : " << N <<endl;
    usleep(1000000);

    //04) Generate GCD gcdB - Display range for user to choose Encryption Key - EKB
        //Get encryption key B from user such that gcd(B,phi(n))=1
    cout << "Generating a set of Encryption Keys. Choose One from below" << endl;
    encryKeyB = getGCD(N);

    //05) Generate Decryption Key B - DKB
    decryKeyB = getDecKey(encryKeyB, N);

    usleep(1000000);
    string ackToA = "ACK";
    sendToA(ackToA);

    cout << "\nWaiting for A to send the encrypted cards Deck..." << endl;
    //08) Receive 52 Cards from A
    string allCardsStr;
    int allCardsRes;
    int cards[52]={0};
    allCardsRes = receiveFromA(allCardsStr);
    //cout << "Received Cards Str" << allCardsStr << endl;
    if(allCardsRes) {
        //Split by , and store in array
        vector<string> tokens;
        split( allCardsStr, ',' ,tokens);
        for (unsigned int i = 0;  i < tokens.size();   i++){
            //cout << "Card at " << i << " is " << atoi(tokens[i].c_str()) << endl;
            cards[i]=atoi(tokens[i].c_str());
        }
    }
    //09) Display all cards on screen for B to choose

    cout << "Cards Deck is received from A. Cards are:" << endl;
    displayAllCards(cards,52,false);
    //10) Choose 5 for A and Send
    cout << "\nChoose 5 cards for A. "<< endl;
    int cardsForA[5] = {0};

    int input = 0;
    string mystr;
    int checkIndexes[10]={0};
    for (int q=0;q<5;q++){
        cout << "Choose card "<<q+1<<" for A: ";
        while (true) {
            //getline (cin,mystr);
            //stringstream(mystr) >> input;
            srand(time(NULL));
            input = rand()%51 + 1;
            cout << "Random Choice : " << input << endl;
            usleep(1000000);
            //if(!mystr.empty()){
            if(true){
                bool exists = find(checkIndexes, checkIndexes+10, input) != (checkIndexes+10);
                //cout <<"exists "<<exists<<" checkIndexes "<<checkIndexes<<endl;
                if (exists || (input < 1 || input >52) )
                    cout << "Invalid entry or card already choosen, please try again card " <<q+1<<" for A:" << endl;
                else{
                    cardsForA[q]=cards[input-1];
                    checkIndexes[q]=input;
                    break;
                }
            }
        }
    }

    //choose

    //put into , separted string and send
    string cardsForAStr;
    //int cardsForA[5];
    for(unsigned int i=0 ; i < 5 ; i++ ) {
        stringstream enCardStr;
        enCardStr << cardsForA[i];
        cardsForAStr = i==0 ? "" : cardsForAStr.append(",") ;
        cardsForAStr = cardsForAStr.append(enCardStr.str());
    }
    sendToA(cardsForAStr);
    cout << "Sent A's cards to A: " << cardsForAStr << endl;

    //11) Choose 5 for self, Encrypt using Encryption Key EKB and Send
    mystr="";
    int encryCardsForB[5], cardsForB[5];
    cout << "\nChoose 5 cards for yourself (B) " << endl;

    for (int q=0;q<5;q++){
        cout << "Choose card "<<q+1<<" for B: ";
        while (true) {
            //getline (cin,mystr);
            //stringstream(mystr) >> input;
            srand(time(NULL));
            input = rand()%51 + 1;
            cout << "Random Choice : " << input << endl;
            usleep(1000000);
            //if(!mystr.empty()){
            if(true){
                bool exists = (find(checkIndexes, checkIndexes+10, input) != (checkIndexes+10));
                if (exists || (input < 1 || input >52) )
                    cout << "\nInvalid entry or card already choosen, please try again card " <<q+1<<" for B:" << endl;
                else{
                    cardsForB[q]=cards[input-1];
                    checkIndexes[q+5]=input;
                    break;
                }
            }
        }
    }
    //encrypt
    string encryCardsForBStr;
    for(unsigned int i = 0; i < 5 ; i++) {
        encryCardsForB[i] = encrypt(cardsForB[i], encryKeyB, N);
    }

    for(unsigned int i=0 ; i < 5 ; i++ ) {
        stringstream enCardStr;
        enCardStr << encryCardsForB[i];
        encryCardsForBStr = i==0 ? "" : encryCardsForBStr.append(",") ;
        encryCardsForBStr = encryCardsForBStr.append(enCardStr.str());
    }
    usleep(1000000);
    sendToA(encryCardsForBStr);
    cout << "Sent my cards to A: " << encryCardsForBStr << endl;

    cout << "\nWaiting to receive my 5 cards from A..." << endl;
    //15) Receive B's cards
    allCardsRes=receiveFromA(encryCardsForBStr);

    cout << "Received my 5 cards from A : ";
    //split by , and load into encryCardsForB
    if(allCardsRes) {
        //Split by , and store in array
        vector<string> tokens;
        split( encryCardsForBStr, ',' ,tokens);
        for (unsigned int i = 0;  i < tokens.size();   i++){
            //cout << "Card at " << i << " is " << atoi(tokens[i].c_str()) << endl;
            encryCardsForB[i]=atoi(tokens[i].c_str());
            cout << encryCardsForB[i]; if(i!=(tokens.size() -1)) cout << ", ";
        }
    }
    cout << endl;

    cout << "My decrypted cards are: ";
    //16) Decrypt B's cards
    for(unsigned int i = 0; i < 5 ; i++) {
        cardsForB[i] = decrypt(encryCardsForB[i], decryKeyB, N);
        cout<<getCardDetail(cardsForB[i]); if(i!=4) cout << ", ";
    }
    cout << endl;

    //--Ready to Play--

    // Display B's cards
    cout << endl << endl << "*****READY TO PLAY*****" << endl << endl;
    cout <<  "**MY HAND**" << endl;
    sort(cardsForB, cardsForB + 5);
    displayAllCards(cardsForB, 5, true);
    usleep(1000000);
    int highCardB = cardsForB[4];
    cout << endl << "My high card is " << getCardDetail(highCardB) << endl;
    stringstream highCardBStr;
    highCardBStr << highCardB;
    usleep(1000000);
    sendToA(highCardBStr.str());

    string highCardAStr;
    receiveFromA(highCardAStr);
    int highCardA = atoi(highCardAStr.c_str());
    cout << endl << "A's high card is " << getCardDetail(highCardA) << endl;

    string winner, winnerFromA;
    if(highCardB > highCardA) {
        winner = "B";
        cout << endl << "I WON!!" << endl << endl;
    } else {
        winner = "A";
        cout << endl << "I LOST!!" << endl << endl;
    }
    usleep(1000000);
    sendToA(winner);

    receiveFromA(winnerFromA);

    if (winner == winnerFromA) {
        cout << "Verified Agreement on winner" << endl << endl;
    } else {
        cout << "Conflict in winner announcements" << endl << endl;
    }

    //Verification of non-cheating
    stringstream decryKeyBStr;
    decryKeyBStr << decryKeyB;
    usleep(1000000);
    sendToA(decryKeyBStr.str());

    string decryKeyAStr;
    receiveFromA(decryKeyAStr);
    int decryKeyA = atoi(decryKeyAStr.c_str());

    cout << "Decryption key of A:" << decryKeyA << endl;

    for (int i=0 ; i < 5 ; i++) {
        cardsForA[i] = decrypt(cardsForA[i], decryKeyA, N);
    }
    sort(cardsForA, cardsForA + 5);
    cout << "A's hand was" << endl;
    displayAllCards(cardsForA, 5, true);

    cout << endl << "A's high card was:" << getCardDetail(cardsForA[4]) << endl;

    if(highCardA==cardsForA[4]) {
        cout << "Cards Verified! A did not cheat." << endl;
    } else {
        cout << "A cheated. Call an arbitrator." << endl;
    }
    cout << endl << endl << "Done Mental Poker B : Winner is " << winner  << endl << endl;

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc > 1) {
        //cout << "argc:" << argc << " argv[1]:" << argv[1] << endl;
        string programToRun(argv[1]);
        if (argc > 2) {
            string hostToConnectTo(argv[2]);
            connHost = hostToConnectTo;
        } else {
            connHost = "localhost";
        }
        if(programToRun == "A" || programToRun == "a") {
            cout << programToRun << " will connect to host:'" << connHost << "'" << endl;
            int ret = mainA();
            return ret;
        } else if(programToRun == "B" || programToRun == "b") {
            cout << programToRun << " will connect to host:'" << connHost << "'" << endl;
            int ret = mainB();
            return ret;
        } else {
            cout << "Wrong parameter: Run " << argv[0] << " A | B" << endl;
        }

    } else {
        //Comment out one of the below
        int ret;
        ret = mainA();
        //ret = mainB();
        return ret;
    }
}
