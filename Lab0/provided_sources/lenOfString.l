/*lex program to find the length of a string*/

%{ 
#include<stdio.h> 
int length; 
%} 

/* Rules Section*/
%% 
[a-z A-Z 0-9]+ {length=yyleng; } 
%% 

int main() 
{ 
yylex(); 
printf("length of given string is : %d", length); 
return 0; 
} 

