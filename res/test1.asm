ASSUME CS:codesg,DS:datasg
datasg segment
    a DB 1
datasg ends
codesg segment
  start:
    mov AX,datasg
    mov DS,AX
    mov BL,1
    mov a,BL
    mov AL,a
    CBW
    mov BL,10
    DIV BL
    mov BH,AH
    add BH,30H
    add AL,30H
    CMP AL,48
    JE stepC
  stepB:
    mov DL,AL
    mov AH,2
    int 21H
  stepC:
    mov DL,BH
    mov AH,2
    int 21H
    mov ax,4C00H
    int 21H
codesg ends
  end start
