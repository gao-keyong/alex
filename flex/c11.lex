/*
 * c11.lex: An C11 scanner
 * 
 */

%option noyywrap

/* Some useful grammar */
O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\f]

%{
#include <stdio.h>

/* C token types */
#define KEYWORD_TOK 1
#define IDENTIFIER_TOK 2
#define I_CONSTANT_TOK 3
#define F_CONSTANT_TOK 4
#define C_CONSTANT_TOK 5
#define STRING_LITERAL_TOK 6
#define PUNCTUATOR_TOK 7

/* statistics */
int charCount = 0;
int lineCount = 1;
int columnCount = 1;
int keywordCount = 0;
int identifierCount = 0;
int integerConst = 0;
int floatingConst = 0;
int charConst = 0;
int stringCount = 0;
int punctuatorCount = 0;
int errorCount = 0;

/* handle errors */
void yyerror(const char *);

/* handle multi-line comments */
static void comment(void);
%}

%%
"\n"                    { lineCount++; columnCount=0; return -1; } /* wrap */
"/*"                    { comment(); return -1; }   /* multi-line comment begins */
"//".*                  { return -1; }  /* single-line comment */
"#".+                   { return -1; }  /* macro */

"auto"					{ return KEYWORD_TOK; } /* keywords */
"break"					{ return KEYWORD_TOK; }
"case"					{ return KEYWORD_TOK; }
"char"					{ return KEYWORD_TOK; }
"const"					{ return KEYWORD_TOK; }
"continue"				{ return KEYWORD_TOK; }
"default"				{ return KEYWORD_TOK; }
"do"					{ return KEYWORD_TOK; }
"double"				{ return KEYWORD_TOK; }
"else"					{ return KEYWORD_TOK; }
"enum"					{ return KEYWORD_TOK; }
"extern"				{ return KEYWORD_TOK; }
"float"					{ return KEYWORD_TOK; }
"for"					{ return KEYWORD_TOK; }
"goto"					{ return KEYWORD_TOK; }
"if"					{ return KEYWORD_TOK; }
"inline"				{ return KEYWORD_TOK; }
"int"					{ return KEYWORD_TOK; }
"long"					{ return KEYWORD_TOK; }
"register"				{ return KEYWORD_TOK; }
"restrict"				{ return KEYWORD_TOK; }
"return"				{ return KEYWORD_TOK; }
"short"					{ return KEYWORD_TOK; }
"signed"				{ return KEYWORD_TOK; }
"sizeof"				{ return KEYWORD_TOK; }
"static"				{ return KEYWORD_TOK; }
"struct"				{ return KEYWORD_TOK; }
"switch"				{ return KEYWORD_TOK; }
"typedef"				{ return KEYWORD_TOK; }
"union"					{ return KEYWORD_TOK; }
"unsigned"				{ return KEYWORD_TOK; }
"void"					{ return KEYWORD_TOK; }
"volatile"				{ return KEYWORD_TOK; }
"while"					{ return KEYWORD_TOK; }
"_Alignas"              { return KEYWORD_TOK; }
"_Alignof"              { return KEYWORD_TOK; }
"_Atomic"               { return KEYWORD_TOK; }
"_Bool"                 { return KEYWORD_TOK; }
"_Complex"              { return KEYWORD_TOK; }
"_Generic"              { return KEYWORD_TOK; }
"_Imaginary"            { return KEYWORD_TOK; }
"_Noreturn"             { return KEYWORD_TOK; }
"_Static_assert"        { return KEYWORD_TOK; }
"_Thread_local"         { return KEYWORD_TOK; }
"__func__"              {}

{L}{A}*					{ return IDENTIFIER_TOK; }  /* identifiers */

{HP}{H}+{IS}?			    	{ return I_CONSTANT_TOK; }  /* constants */
{NZ}{D}*{IS}?			    	{ return I_CONSTANT_TOK; }
"0"{O}*{IS}?		    		{ return I_CONSTANT_TOK; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ return C_CONSTANT_TOK; }

{D}+{E}{FS}?		    		{ return F_CONSTANT_TOK; }
{D}*"."{D}+{E}?{FS}?			{ return F_CONSTANT_TOK; }
{D}+"."{E}?{FS}?		    	{ return F_CONSTANT_TOK; }
{HP}{H}+{P}{FS}?		    	{ return F_CONSTANT_TOK; }
{HP}{H}*"."{H}+{P}{FS}?			{ return F_CONSTANT_TOK; }
{HP}{H}+"."{P}{FS}?		    	{ return F_CONSTANT_TOK; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ return STRING_LITERAL_TOK; }  /* strings literal */

"..."					{ return PUNCTUATOR_TOK; }  /* punctuators */
">>="					{ return PUNCTUATOR_TOK; }
"<<="					{ return PUNCTUATOR_TOK; }
"+="					{ return PUNCTUATOR_TOK; }
"-="					{ return PUNCTUATOR_TOK; }
"*="					{ return PUNCTUATOR_TOK; }
"/="					{ return PUNCTUATOR_TOK; }
"%="					{ return PUNCTUATOR_TOK; }
"&="					{ return PUNCTUATOR_TOK; }
"^="					{ return PUNCTUATOR_TOK; }
"|="					{ return PUNCTUATOR_TOK; }
">>"					{ return PUNCTUATOR_TOK; }
"<<"					{ return PUNCTUATOR_TOK; }
"++"					{ return PUNCTUATOR_TOK; }
"--"					{ return PUNCTUATOR_TOK; }
"->"					{ return PUNCTUATOR_TOK; }
"&&"					{ return PUNCTUATOR_TOK; }
"||"					{ return PUNCTUATOR_TOK; }
"<="					{ return PUNCTUATOR_TOK; }
">="					{ return PUNCTUATOR_TOK; }
"=="					{ return PUNCTUATOR_TOK; }
"!="					{ return PUNCTUATOR_TOK; }
";"			    		{ return PUNCTUATOR_TOK; }
("{"|"<%")				{ return PUNCTUATOR_TOK; }
("}"|"%>")				{ return PUNCTUATOR_TOK; }
","				    	{ return PUNCTUATOR_TOK; }
":"			    		{ return PUNCTUATOR_TOK; }
"="			    		{ return PUNCTUATOR_TOK; }
"("			    		{ return PUNCTUATOR_TOK; }
")"			    		{ return PUNCTUATOR_TOK; }
("["|"<:")				{ return PUNCTUATOR_TOK; }
("]"|":>")				{ return PUNCTUATOR_TOK; }
"."			    		{ return PUNCTUATOR_TOK; }
"&"			    		{ return PUNCTUATOR_TOK; }
"!"			    		{ return PUNCTUATOR_TOK; }
"~"				    	{ return PUNCTUATOR_TOK; }
"-"		    			{ return PUNCTUATOR_TOK; }
"+"			    		{ return PUNCTUATOR_TOK; }
"*"				    	{ return PUNCTUATOR_TOK; }
"/"	    				{ return PUNCTUATOR_TOK; }
"%"		    			{ return PUNCTUATOR_TOK; }
"<"			    		{ return PUNCTUATOR_TOK; }
">"				    	{ return PUNCTUATOR_TOK; }
"^"	    				{ return PUNCTUATOR_TOK; }
"|"		    			{ return PUNCTUATOR_TOK; }
"?"			    		{ return PUNCTUATOR_TOK; }

{WS}			   	    	    {return -1;}    /* white space */

{D}+({A}|\.)+                   { yyerror("illegal name"); return -1; } /* errors */
{SP}?\"([^"\\\n]|{ES})*{WS}*    { yyerror("unclosed string"); return -1; }
.	    		        		{ yyerror("unexpected character"); return -1; }

%%

void yyerror(const char *err)
{
    errorCount++;
    fprintf(stderr, "\033[1m%d:%d: \033[31mError: \033[32m%s\033[39m %s\n\033[0m", lineCount, columnCount, err, yytext);
}

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
    {
        charCount++;
        if (c == '\n')
            lineCount++;
        if (c == '*')
        {
            while ((c = input()) == '*')
            {
                charCount++;
            }
            charCount++;
            if (c == '\n')
            {
                lineCount++;
                columnCount = 1;
            }

            if (c == '/')
            {
                return;
            }

            if (c == 0)
                break;
        }
    }
    yyerror("unterminated comment");
}

int main()
{
    int code;
    while ((code = yylex()))    // code is the type of token
    {
        charCount += yyleng;    // character counts
        switch (code)
        {
        case KEYWORD_TOK:
            keywordCount++;
            printf("%d:%d: [Keyword: %s]\n", lineCount, columnCount, yytext);
            break;
        case IDENTIFIER_TOK:
            identifierCount++;
            printf("%d:%d: [Identifier: %s]\n", lineCount, columnCount, yytext);
            break;
        case I_CONSTANT_TOK:
            integerConst++;
            printf("%d:%d: [Integer: %s]\n", lineCount, columnCount, yytext);
            break;
        case C_CONSTANT_TOK:
            charConst++;
            printf("%d:%d: [Character: %s]\n", lineCount, columnCount, yytext);
            break;
        case F_CONSTANT_TOK:
            floatingConst++;
            printf("%d:%d: [Floating: %s]\n", lineCount, columnCount, yytext);
            break;
        case STRING_LITERAL_TOK:
            stringCount++;
            printf("%d:%d: [String: %s]\n", lineCount, columnCount, yytext);
            break;
        case PUNCTUATOR_TOK:
            punctuatorCount++;
            printf("%d:%d: [Punctuator: %s]\n", lineCount, columnCount, yytext);
            break;
        default:
            break;
        }
        columnCount += yyleng;  // cursor forward
    }
    printf("\nTotal characters:\t%d\n", charCount);
    printf("Total lines:\t%d\n", lineCount);
    printf("\n");
    printf("Keyword:\t%d\n", keywordCount);
    printf("Identifier:\t%d\n", identifierCount);
    printf("Integers:\t%d\n", integerConst);
    printf("Floatings:\t%d\n", floatingConst);
    printf("Characters:\t%d\n", charConst);
    printf("Strings:\t%d\n", stringCount);
    printf("Punctuators:\t%d\n", punctuatorCount);
    printf("\033[1mErrors:\t\t%d\n\033[0m", errorCount);
    return 0;
}