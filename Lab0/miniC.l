%{
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

FILE* outFile;

uint64_t numKeywords = 0;
uint64_t numConstants = 0;
uint64_t numIdentifiers = 0;
uint64_t numSeparators = 0;
uint64_t numOperators = 0;

%}

char [a-zA-Z_]
digit [0-9]

operators [!\+\*\-=\|<>\/]|\+\+|\-\-|==|!=|\+=|\-=|\*=|\/=|>=|<=

separators [\(\)\[\],;:]

keyword char|else|if|int|return|void|while

identifier {char}({char}|{digit}){0,254}

escape_sequences \\.
char_const \'([^\']|{escape_sequences})\'
string_const \"([^\"]|{escape_sequences})*\"
number_const {digit}+
float_const {digit}*\.{digit}+
constant {char_const}|{string_const}|{number_const}|{float_const}

multi_comment_start \/\*
multi_comment_end \*\/

single_comment \/\/.*$

whitespace [ \t]

%s COMMENT

%%

{multi_comment_start}         BEGIN(COMMENT);  /* {BEGIN(COMMENT); yymore();} */

<COMMENT>[^*\n]* {yymore();}
<COMMENT>\*+[^*/\n]* {yymore();}
<COMMENT>\n {yymore();}
<COMMENT>{multi_comment_end}       BEGIN(INITIAL);  /* {fprintf(outFile, "MULTI COMMENT: '%s'\n", yytext);BEGIN(INITIAL);} */

{single_comment} /* {fprintf(outFile, "SINGLE COMMENT,'%s'\n", yytext);} */

{operators} {fprintf(outFile, "<OP,%s>\n", yytext); numOperators++;}

{separators} {fprintf(outFile, "<SP,%s>\n", yytext); numSeparators++;}

{keyword} {fprintf(outFile, "<KY,%s>\n", yytext); numKeywords++;}

{identifier} {fprintf(outFile, "<ID,%s>\n", yytext); numIdentifiers++;}

{constant} {fprintf(outFile, "<CT,%s>\n", yytext);numConstants++;}

{whitespace} /* {fprintf(outFile, "WHITESPACE,'%s'\n", yytext);} */

. /* do nothing for anything else */


%%

int yywrap() {
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE* inFile = fopen(argv[1], "r");
        if (inFile) {
            yyin = inFile;
        }
    }
    outFile = fopen("output.txt", "w");
    fclose(stdout);
    yylex();
    uint64_t numTokens = numOperators + numSeparators + numKeywords + numIdentifiers + numConstants;
    fprintf(outFile, "Total number of operators in the above program is %lu\n", numOperators);
    fprintf(outFile, "Total number of separators in the above program is %lu\n", numSeparators);
    fprintf(outFile, "Total number of identifiers in the above program is %lu\n", numIdentifiers);
    fprintf(outFile, "Total number of keywords in the above program is %lu\n", numKeywords);
    fprintf(outFile, "Total number of constants in the above program is %lu\n", numConstants);
    fprintf(outFile, "Total number of tokens in the above program is %lu\n", numTokens);
    fclose(outFile);
    return 0;
}
