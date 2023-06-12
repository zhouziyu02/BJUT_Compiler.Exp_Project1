%{
	#include<stdio.h>
	#include<stdlib.h>
	#define yywrap()  1
	#define IL -1
	#define ADD 1
	#define SUB 2
	#define MUL 3
	#define DIV 4
	#define GT 5
	#define LT 6
	#define EQ 7
	#define GE 8
	#define LE 9
	#define NEQ 10
	#define SLP 11
	#define SRP 12
	#define SEMI 13
	#define IF 14
	#define THEN 15
	#define ELSE 16
	#define WHILE 17
	#define DO 18
	#define BEGIN_N 19
	#define END 20
	#define ILOCT 21
	#define ILHEX 22
	#define IDN 23
	#define DEC 24
	#define OCT 25
	#define HEX 26
%}

%%
if                                        { return IF;}
then                                      { return THEN;}
else                                      { return ELSE;}
while                                     { return WHILE;}
do                                        { return DO;}
begin                                     { return BEGIN_N;}
end                                       { return END;}
[a-zA-Z][a-zA-Z0-9]*                      { return IDN;}
0|([1-9]+[0-9]*)                          { return DEC;}
0[0-7]+                                   { return OCT;}  
0[0-7]*[8-9]+[0-7]*                       { return ILOCT;}
0[xX][0-9a-fA-F]+                         { return HEX;} 
0[xX][0-9a-fA-F]*[g-zG-Z]+[0-9a-fA-F]*    { return ILHEX;}
\+                                        { return ADD;}
\-                                        { return SUB;}
\*                                        { return MUL;}
\/                                        { return DIV;}
\>                                        { return GT;}
\<                                        { return LT;}
\=                                        { return EQ;}
">="                                      { return GE;}
"<="                                      { return LE;}
"<>"                                      { return NEQ;}
\(                                        { return SLP;}
\)                                        { return SRP;}
\;                                        { return SEMI;}
[ \t\n]                                   { return IL;}
%%
	int yylex();
	int main(){
		int value;
		value = yylex();
		int hex;
		int oct;
		while(value!=0){
            if(value == IDN){
                printf("IDN  %s\n", yytext);
            }
            else if(value == DEC){
                printf("DEC  %s\n", yytext);
            }
            else if(value == OCT){
                oct = strtol(yytext, NULL, 8);
                printf("OCT  %d\n", oct);
            }
            else if(value == ILOCT){
                printf("ILOCT  -\n");
            }
            else if(value == HEX){
                hex = strtol(yytext, NULL, 16);
                printf("HEX  %d\n", hex);
            }
             else if(value == ILHEX){
                printf("ILHEX  -\n");
            }
             else if(value == ADD){
                 printf("ADD  -\n");
            }
            else if(value == SUB){
                 printf("SUB  -\n");
            }
            else if(value == MUL){
                 printf("MUL  -\n");
            }
            else if(value == DIV){
                 printf("DIV  -\n");
            }
            else if(value == GT){
                 printf("GT  -\n");
            }
            else if(value == LT){
                 printf("LT  -\n");
            }
            else if(value == EQ){
                 printf("EQ  -\n");
            }
            else if(value == GE){
                 printf("GE  -\n");
            }
            else if(value == LE){
                 printf("LE  -\n");
            }
            else if(value == NEQ){
                 printf("NEQ  -\n");
            }
            else if(value == SLP){
                 printf("SLP  -\n");
            }
            else if(value == SRP){
                 printf("SRP  -\n");
            }
            else if(value == SEMI){
                 printf("SEMI  -\n");
            }
            else if(value == IF){
                 printf("IF  -\n");
            }
            else if(value == THEN){
                 printf("THEN  -\n");
            }
            else if(value == THEN){
                 printf("THEN  -\n");
            }
            else if(value == ELSE){
                 printf("ELSE  -\n");
            }
            else if(value == WHILE){
                 printf("WHILE  -\n");
            }
            else if(value == DO){
                 printf("DO  -\n");
            }
            else if(value == BEGIN_N){
                 printf("BEGIN  -\n");
            }
            else if(value == END){
                 printf("END  -\n");
            }
            value = yylex();
		}
	}

