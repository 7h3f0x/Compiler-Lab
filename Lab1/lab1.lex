%{
#include <stdio.h>
%}


/* regular definitions */
char [a-zA-Z_]
digit [0-9]
escape_sequences \\.

/* needed regexes */
keyword auto|double|break|else|long|case|enum|char|extern|const|float|short|continue|for|signed|do|if|static|int|struct|switch|register|typedef|return|union|unsigned|void|while

identifier {char}({char}|{digit}){0,30}

char_const \'([^\']|{escape_sequences})\'
number_const {digit}+
float_const {digit}*\.{digit}+
constant {char_const}|{number_const}|{float_const}

string_const \"([^\"]|{escape_sequences})*\"

operators [!\+\-=\|<>\/]|\+\+|\-\-|==|!=|\+=|\-=|\*=|\/=|>=|<=

special_symbols [\.\(\)\[\]\{\}\*,;:=#]

whitespace [ \t]

newline \n

%%

{keyword} {printf("keyword ");}
{identifier} {printf("identifier ");}
{constant} {printf("constant ");}
{string_const} {printf("string ");}
{special_symbols} {printf("special character ");}
{operators} {printf("operator ");}

{newline} {putchar('\n');}

{whitespace} /* ignore */

%%
int yywrap() {
}

int main(void) {
    yyout = NULL;
    yylex();
    return 0;
}

