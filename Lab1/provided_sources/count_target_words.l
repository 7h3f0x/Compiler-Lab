/* LEX code to count the frequency 
of the given word in a file */

/* Definition section */
/* variable word indicates the word 
whose frequency is to be count */
/* variable count is used to store the 
frequency of the given word */

%{ 
#include<stdio.h> 
#include<string.h> 

char word [] = "geeks"; 
int count = 0; 


%} 

/* Rule Section */
/* Rule 1 compares the matched token with the 
word to count and increments the count variable 
on successful match */
/* Rule 2 matches everything other than string 
(consists of albhabets only ) and do nothing */

%% 
[a-zA-Z]+ { if(strcmp(yytext, word)==0) 
				count++; } 
. ;		 
%% 


int yywrap() 
{ 
	return 1; 
} 

/* code section */
int main() 
{ 
		extern FILE *yyin, *yyout; 
		
		/* open the input file 
		in read mode */
		yyin=fopen("input.txt", "r"); 
		yylex(); 
		
		printf("%d", count); 
	
} 

