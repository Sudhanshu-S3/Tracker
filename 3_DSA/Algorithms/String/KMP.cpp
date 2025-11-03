// KMP Implementation

#include<bits/stdc++.h>

using namespace std;


    int strStr(string haystack, string needle) {

        if(needle.empty()) return 0;

        int needleSize = needle.size();
        int haystackSize = haystack.size();

      // Step 1: Building the LPS array.
        vector<int> LPS(needleSize , 0);

        int length = 0;
        int i = 1;

        while(i<needleSize){
            if(needle[i] == needle[length]){
                length++;
                LPS[i]= length;
                i++;
            }else{

                if(length != 0){
                    length = LPS[length-1];
                }
                else{
                    LPS[i]= 0;
                    i++;
                }
            }
        }
      
        // Step 2: Finding the Match.
      
        int Hptr = 0;
        int Nptr = 0;
        
        while(Hptr < haystackSize){
          
            if(haystack[Hptr]==needle[Nptr]){
                Hptr++;
                Nptr++;
            }
            if(Nptr == needleSize){
                return Hptr - Nptr;
            }
        
            else if(Hptr < haystackSize && haystack[Hptr] != needle[Nptr]){
                // MisMatch After Some Matches
                // Checking the Previous String matches or not.
                if(Nptr != 0){
                    Nptr = LPS[Nptr-1];
                }else{
                    Hptr++;
                }
            }
        }
        return -1;
    }


int main(){

  string haystack;
  string needle;
  cin>>haystack>>needle;
  cout<<"Index of needle in haystack is: " << strStr(haystack, needle);

  return 0;
}
