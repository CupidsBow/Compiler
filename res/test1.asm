ASSUME CS:codesg,DS:datasg
datasg segment
    a DB 10
    b DB 20
    c DB 30
    d DB 0
    tmpB DB '?'
    tmpC DB '?'
datasg ends
codesg segment
  start:
    mov AX,datasg
    mov DS,AX
    mov BL,10
    mov a,BL
    mov BL,20
    mov b,BL
    mov BL,30
    mov c,BL
    mov BL,0
    mov d,BL
    mov AL,b
    mov BH,c
    mul BH
    mov BL,1
    div BL
    mov tmpB,AL
    mov BL,a
    sub BL,b
    mov tmpB,BL
    mov BL,tmpB
    mov c,BL
    mov AL,c
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
