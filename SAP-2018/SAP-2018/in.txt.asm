.586
.model FLAT, C , stdcall
includelib libucrt.lib 
includelib StaticLibrary.lib 
includelib kernel32.lib 
ExitProcess PROTO : DWORD 
.stack 4096
outputWord PROTO : DWORD
outputLogic PROTO : DWORD
outputNumb PROTO : DWORD
subword PROTO : DWORD, : DWORD
wordlen PROTO : DWORD
factorial PROTO : DWORD
numbin PROTO : DWORD, : DWORD
.CONST
L1 DWORD 10
L2 DWORD 2
L3 DWORD 2018
L4 DWORD 5
L5 DWORD 3
L6 DB "stalmashenko", 0
L7 DB "sap-2018", 0
L8 DB "количество букв в слове: ", 0
L9 DB " ", 0
L10 DB "проверка на наличие подстроки в строке: ", 0
L11 DB "факториал числа 10: ", 0
L12 DB "вычисление значения степени числа: ", 0
L13 DB "результат работы функции math: ", 0
L14 DWORD 0
.data
mathfmath DWORD 0
mainb00lean DWORD 0
mainfres DWORD 0
mainfirst DWORD 0
mainsecond DWORD 0
mainthird DWORD 0
mainfuck DWORD 0
mainnumword DWORD 0
maininchislo DWORD 0
mainresult DWORD 0
mainsap2018 DB 255 dup(0)
mainlogin DB 255 dup(0)
.CODE
math PROC mathx : DWORD, mathy : DWORD
PUSH mathx
PUSH mathy
POP EBX
POP EAX
MOV EDX, 0
MUL EBX
MOV EBX, EAX
PUSH EBX
PUSH L1
PUSH L2
POP EBX
POP EAX
MOV EDX, 0
DIV EBX
MOV EBX, EAX
PUSH EBX
POP EDX
POP EBX
SUB EBX, EDX
PUSH EBX
PUSH L3
POP EDX
POP EBX
ADD EBX, EDX
PUSH EBX
POP mathfmath
MOV EAX, mathfmath
ret
math ENDP
main PROC
PUSH L4
PUSH L1
POP EDX
POP EBX
SUB EBX, EDX
PUSH EBX
POP mainfirst
PUSH L4
POP mainsecond
PUSH L5
POP mainthird
MOV ESI, OFFSET L6
MOV EDI, OFFSET mainlogin
MOV ECX, 13
REP MOVSB
MOV ESI, OFFSET L7
MOV EDI, OFFSET mainsap2018
MOV ECX, 9
REP MOVSB
PUSH OFFSET mainsap2018
PUSH OFFSET mainlogin
CALL subword
PUSH EAX
POP mainfres
PUSH mainsecond
PUSH mainfirst
CALL math
PUSH EAX
POP mainresult
PUSH OFFSET mainsap2018
CALL wordlen
PUSH EAX
POP mainnumword
PUSH L1
CALL factorial
PUSH EAX
POP mainfuck
PUSH mainsecond
PUSH mainfirst
CALL numbin
PUSH EAX
POP maininchislo
PUSH OFFSET mainsap2018
CALL outputWord
PUSH mainfirst
CALL outputNumb 
PUSH OFFSET L8
CALL outputWord
PUSH mainnumword
CALL outputNumb 
PUSH OFFSET L9
CALL outputWord
PUSH OFFSET L10
CALL outputWord
PUSH mainb00lean
CALL outputLogic
PUSH OFFSET L9
CALL outputWord
PUSH OFFSET L11
CALL outputWord
PUSH mainfuck
CALL outputNumb 
PUSH OFFSET L9
CALL outputWord
PUSH OFFSET L12
CALL outputWord
PUSH maininchislo
CALL outputNumb 
PUSH OFFSET L9
CALL outputWord
PUSH OFFSET L13
CALL outputWord
PUSH mainresult
CALL outputNumb 
MOV EAX, L14
PUSH 0
CALL ExitProcess
MAIN ENDP
end main
