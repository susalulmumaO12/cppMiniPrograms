#include<iostream>
#include<string>
#include<cstring>
#include <cctype>

using namespace std;
int main(){
   string s;                   //defining the text string
cout<<"\nENTER YOUR TEXT\n";
getline(cin,s);                //getting text from user with whitespaces
 int n = s.length();
cout<<"\n\nyour text in meme font is:\n";    //by meme font i mean lowercase with uppercase in the same time

for(int i=0;i<n;i++){
     char m = s[i];                  //a variable m that changes for each letter of the string
     if(i%2==0)                 //for even letters in the string 
     cout<<static_cast<char>(tolower(m));  //convert to lowercase
     else                       //for odd letters in the string
     cout<<static_cast<char>(toupper(m));  //convert to uppercase
}
cout<<endl;
    return 0;
}
