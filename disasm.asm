locals @@

.model small
.stack 100h

bufSize = 255

putc macro char
  mov ah, 2
  mov dl, char
  int 21h
endm

puts macro strPtr
  mov ah, 9
  lea dx, strPtr
  int 21h
endm

fputc macro char
  mov dl, char
  call filePutChar
endm

fputs macro strName
  lea si, strName
  call filePutString
endm

terminateWithErrorMsg macro
  mov ah, 9
  int 21h
  mov ax, 4C01h
  int 21h
endm

printWordInHex macro
  push dx

  mov dl, ah
  mov dh, 1
  call printByteInHex

  mov dl, al
  xor dh, dh
  call printByteInHex

  pop dx
endm

nje macro lbl
  local nojump
  jne nojump
  jmp lbl
  nojump:
endm

njb macro lbl
  local nojump
  jnb nojump
  jmp lbl
  nojump:
endm

njbe macro lbl
  local nojump
  jnbe nojump
  jmp lbl
  nojump:
endm

.data
crlf                    db 13,10,'$'
msgInfo                 db 'Gustas Zilinskas, PS 1k., 5 gr.',10,13,'Disasembleris (visos 8086 instrukcijos)',10,13,'$'

msgErrTooManyOpenFiles  db 'Atidaryta per daug failu!$'

msgErrInputFileNotFound db 'Ivesties failas neegzistuoja!$'
msgErrNoPathToInputFile db 'Ivesties failo kelias nepasiekiamas!$'
msgErrReadAccessDenied  db 'Nera teisiu ivesties failui skaityti!$'
msgErrInputFileGeneric  db 'Nepavyko atidaryti ivesties failo!$'

msgErrReadingFailure    db 'Klaida ivesties failo skaitymo metu!$'

inputFileName           db 128 dup (0)
inputFileHandle         dw ?
inputBuf                db bufSize dup (?)
bytesLeft               dw 0
bufPos                  dw 0
bufByte                 db ?
fip                     dw 100h

msgErrWrongOutPath      db 'Isvesties failo kelias nepasiekiamas!$'
msgErrNoWriteAcess      db 'Nera teisiu isvesties failui sukurti!$'
msgErrWriteGeneric      db 'Nepavyko sukurti isvesties failo!$'

msgErrWritingDenied     db 'Nera teisiu rasyti i isvesties faila!$'
msgErrFullDisk          db 'Nepavyko irasyti visu duomenu i isvesties faila. Patikrinkite, ar diske yra laisvos vietos.$'
msgErrWriting           db 'Klaida rasymo i rezultatu faila metu!$'

outputFileName          db 128 dup (0)
outputFileHandle        dw ?
outputBuf               db bufSize dup (?)
outCounter              dw 0

msgUnknownInstr         db 'Neatpazinta instrukcija!',10,13,'$'

instrItem struc
  insPtr    dw ?
  itemType  db 0
  insOp1    db ?
  insOp2    db ?
ends

include opcodes.inc

regAL db 'AL$'
regCL db 'CL$'
regDL db 'DL$'
regBL db 'BL$'
regAH db 'AH$'
regCH db 'CH$'
regDH db 'DH$'
regBH db 'BH$'

regAX db 'AX$'
regCX db 'CX$'
regDX db 'DX$'
regBX db 'BX$'
regSP db 'SP$'
regBP db 'BP$'
regSI db 'SI$'
regDI db 'DI$'

regES db 'ES$'
regSS db 'SS$'
regCS db 'CS$'
regDS db 'DS$'

label registers
byteRegs  dw regAL,regCL,regDL,regBL,regAH,regCH,regDH,regBH
wordRegs  dw regAX,regCX,regDX,regBX,regSP,regBP,regSI,regDI
segRegs   dw regES,regSS,regCS,regDS

bytePtr   db 'byte ptr $'
wordPtr   db 'word ptr $'

ovrVals enum {
  ovrByte,
  ovrWord,
  ovrReg,
  ovrNotSet
}

ea000   db 'BX+SI$'
ea001   db 'BX+DI$'
ea010   db 'BP+SI$'
ea011   db 'BP+DI$'
ea100   db 'SI$'
ea101   db 'DI$'
ea110   db 'BP$'
ea111   db 'BX$'

eaVals  dw ea000,ea001,ea010,ea011,ea100,ea101,ea110,ea111

prefix  db 0

mnem    dw ?
opcType db ?
op1     db ?
op2     db ?

modrm   db 0
mode    db ?
reg     db ?
rm      db ?

disp    dw ?
imm     dw ?

typeOvr db ?
segOvr  db 0

.code

skipSpaces proc ; ds:si - argv
  @@checkSpace:
    cmp byte ptr [si], ' '
    jne @@notSpace
    inc si
    loop @@checkSpace
  @@notSpace:
  ret
skipSpaces endp

getFilename proc ; ds:si - argv, es:di - failo vardas
  @@strcpy:
    movsb
    cmp byte ptr [si], ' '
    loopne @@strcpy
  ret
getFilename endp

prepareInputFile proc
  push ax dx

  mov ax, 3D00h
  lea dx, inputFileName
  int 21h
  jnc @@endSuccess

  cmp ax, 02h
  je  @@exitFileNotFound
  cmp ax, 03h
  je  @@exitWrongPath
  cmp ax, 04h
  je  @@exitTooManyOpenFiles
  cmp ax, 05h
  je  @@exitAccessDenied
  jmp @@exitGenericError

  @@exitFileNotFound:
  lea dx, msgErrInputFileNotFound
  jmp @@terminateWithErr

  @@exitWrongPath:
  lea dx, msgErrNoPathToInputFile
  jmp @@terminateWithErr

  @@exitTooManyOpenFiles:
  lea dx, msgErrTooManyOpenFiles
  jmp @@terminateWithErr

  @@exitAccessDenied:
  lea dx, msgErrReadAccessDenied
  jmp @@terminateWithErr

  @@exitGenericError:
  lea dx, msgErrInputFileGeneric

  @@terminateWithErr:
  terminateWithErrorMsg

  @@endSuccess:
  mov [inputFileHandle], ax

  pop dx ax
  ret
prepareInputFile endp

readInputByte proc
  push ax bx

  cmp [bytesLeft], 0
  jne @@readByte

  push cx dx

  mov ah, 3Fh
  mov bx, [inputFileHandle]
  mov cx, bufSize
  lea dx, inputBuf
  int 21h
  jnc @@resetBuf

  lea dx, msgErrReadingFailure
  terminateWithErrorMsg

  @@resetBuf:
  cmp ax, 0
  je @@endprog
  mov [bytesLeft], ax
  mov [bufPos], 0

  pop dx cx
  jmp @@readByte

  @@endProg:
  cmp [outCounter], 0
  je @@closeFiles

  call fwrite

  @@closeFiles:
  mov bx, [outputFileHandle]
  call fclose

  mov bx, [inputFileHandle]
  call fclose

  mov ax, 4C00h
  int 21h

  @@readByte:
  mov bx, [bufPos]
  mov al, inputBuf[bx]
  mov [bufByte], al
  dec [bytesLeft]
  inc [bufPos]
  inc [fip]

  pop bx ax
  ret
readInputByte endp

createOutputFile proc
  push ax cx dx

  mov ah, 3Ch
  xor cx, cx
  lea dx, outputFileName
  int 21h
  jnc @@endproc

  cmp ax, 03h
  je @@exitWrongPath
  cmp ax, 04h
  je @@exitTooManyOpenFiles
  cmp ax, 05h
  je @@exitAccessDenied
  jmp @@exitGenericError

  @@exitWrongPath:
  lea dx, msgErrWrongOutPath
  jmp @@terminateWithErr
  @@exitTooManyOpenFiles:
  lea dx, msgErrTooManyOpenFiles
  jmp @@terminateWithErr
  @@exitAccessDenied:
  lea dx, msgErrNoWriteAcess
  jmp @@terminateWithErr
  @@exitGenericError:
  lea dx, msgErrWriteGeneric

  @@terminateWithErr:
  terminateWithErrorMsg

  @@endproc:
  mov [outputFileHandle], ax

  pop dx cx ax
  ret
createOutputFile endp

fwrite proc
  push ax bx cx dx

  mov ah, 40h
  mov bx, [outputFileHandle]
  mov cx, [outCounter]
  lea dx, outputBuf
  int 21h
  jc @@writingError
  cmp ax, cx
  jb @@fullDiskError
  jmp @@writingSuccess

  @@writingError:
  cmp ax, 05h
  je @@exitNoWritePermission
  jmp @@exitUnknownWritingError

  @@fullDiskError:
  lea dx, msgErrFullDisk
  jmp @@terminateWithError

  @@exitNoWritePermission:
  lea dx, msgErrNoWriteAcess
  jmp @@terminateWithError
  @@exitUnknownWritingError:
  lea dx, msgErrWriting

  @@terminateWithError:
  mov bx, [outputFileHandle]
  call fclose

  mov bx, [inputFileHandle]
  call fclose

  terminateWithErrorMsg

  @@writingSuccess:
  mov [outCounter], 0

  pop dx cx bx ax
  ret
fwrite endp

filePutChar proc ; dl - isvedamas simbolis
  push bx

  cmp [outCounter], bufSize
  jb @@skipWrite

  call fwrite

  @@skipWrite:
  mov bx, [outCounter]
  mov outputBuf[bx], dl
  inc word ptr [outCounter]

  pop bx
  ret
filePutChar endp

filePutString proc ; si - adresas simboliu eilutes, uzbaigtos '$'
  @@nextc:
    mov dl, [si]
    call filePutChar
    inc si
    cmp byte ptr [si], '$'
    jne @@nextc

  ret
filePutString endp

fclose proc ; bx - failo deskriptorius
  push ax

  mov ah, 3Eh
  int 21h

  pop ax
  ret
fclose endp

printByteInHex proc ; dl - spausdinamas baitas, dh - ar prideti nuli, jei prasideda raide
  push ax cx dx

  mov ch, dh
  mov dh, dl

  mov cl, 4
  shr dl, cl
  cmp dl, 9
  jbe printhexdig0
  add dl, 7
  cmp ch, 0
  je printhexdig0
  mov al, dl
  fputc '0'
  mov dl, al
  printhexdig0:
	add dl, '0'
  call filePutChar

  mov dl, dh
  and dl, 0Fh
  cmp dl, 9
  jbe printhexdig1
  add dl, 7
  printhexdig1:
	add dl, '0'
  call filePutChar

	pop dx cx ax
  ret
printByteInHex endp

decodeOpc proc
  push ax bx

  mov ax, size instrItem
  mov bl, [bufByte]
  mul bl

  lea bx, instructionTable
  add bx, ax

  mov ax, [bx].insPtr
  mov [mnem], ax

  mov al, [bx].itemType
  mov [opcType], al

  mov al, [bx].insOp1
  mov [op1], al

  mov al, [bx].insOp2
  mov [op2], al

  pop bx ax
  ret
decodeOpc endp

applyWorkaround proc
  cmp [bufByte], 0D4h
  je skipByteD4D5
  cmp [bufByte], 0D5h
  jne readModrmD8DF

  skipByteD4D5:
  call readInputByte
  ret

  readModrmD8DF:
  push cx dx
  mov dh, 1

  mov dl, [bufByte]
  and dl, 7
  mov cl, 3
  shl dl, cl

  call decodeModrm
  add dl, [reg]

  call printByteInHex
  fputc 'h'

  mov [typeOvr], ovrReg

  pop dx cx
  ret
applyWorkaround endp

decodeModrm proc
  cmp [modrm], 0
  jne endModrmAnalysis

  push ax cx

  call readInputByte
  mov [modrm], 1

  mov al, [bufByte]
  and al, 0C0h
  mov cl, 6
  shr al, cl
  mov [mode], al

  mov al, [bufByte]
  and al, 38h
  mov cl, 3
  shr al, cl
  mov [reg], al

  mov al, [bufByte]
  and al, 7
  mov [rm], al

  pop cx ax

  endModrmAnalysis:
  ret
decodeModrm endp

decodeExtOpc proc
  push ax bx si

  mov al, [bufByte]
  push ax

  call decodeModrm

  ; Apskaiciuojame vardo indeksa isplestines komandos masyve
  xor bh, bh
  mov bl, [reg]

  pop ax
  cmp al, 0F6h
  jne notF6
  cmp bl, 0
  jne calculateIndex
  mov byte ptr [op2], opImm8
  jmp calculateIndex
  notF6:
  cmp al, 0F7h
  jne calculateIndex
  cmp bl, 0
  jne calculateIndex
  mov byte ptr [op2], opImm16

  ; Paimame adresa i komandos vardo eilute is apskaiciuotos vietos isplestiniu komandu vardu masyve
  calculateIndex:
  shl bl, 1
  mov si, [mnem]
  mov ax, [bx+si]
  mov [mnem], ax

  pop si bx ax
  ret
decodeExtOpc endp

readOpBytes proc
  push ax
  xor ax, ax

  cmp dl, opNone ; nera operando
  nje endRead
  cmp dl, opConst3 ; registras arba konstanta
  njbe endRead
  cmp dl, opReg8 ; reikalingas modrm
  jae readModrm

  readImm:
  call readInputByte
  mov al, [bufByte]
  mov [typeOvr], ovrByte
  cmp dl, opImm16
  jb storeImm
  call readInputByte
  mov ah, [bufByte]
  inc [typeOvr]

  storeImm:
  cmp dl, opMem
  je storeAsDisp
  mov [imm], ax
  cmp dl, opFar
  jne endRead

  call readInputByte
  mov al, [bufByte]
  call readInputByte
  mov ah, [bufByte]

  storeAsDisp:
  mov [disp], ax
  jmp endRead

  readModrm:
  call decodeModrm

  cmp dl, opRegMem8
  jb regMode ; jei, operandas yra registras, neskaitome poslinkio

  cmp [mode], 11b
  je regMode
  cmp [mode], 01b
  jae readDisp
  cmp [rm], 110b
  jne setupOverride
  jmp readDisp

  regMode:
  mov [typeOvr], ovrReg
  jmp endRead

  readDisp:
  call readInputByte
  mov al, [bufByte]

  cmp [mode], 01b
  je storeDisp

  call readInputByte
  mov ah, [bufByte]

  storeDisp:
  mov [disp], ax

  setupOverride:
  cmp [typeOvr], ovrReg
  je endRead

  mov [typeOvr], ovrByte
  cmp dl, opRegMem16
  jne endRead
  inc [typeOvr]

  endRead:
  pop ax
  ret
readOpBytes endp

printOperand proc
  cmp bl, opNone
  nje endprint
  cmp bl, opConst1
  njb printConstReg
  nje printConst1
  cmp bl, opConst3
  nje printConst3
  cmp bl, opImm8
  nje printImm8
  cmp bl, opShort
  nje printShort
  cmp bl, opImm16
  nje printImm16
  cmp bl, opNear
  nje printNear
  cmp bl, opMem
  nje printMem
  cmp bl, opFar
  nje printFar
  cmp bl, opRegMem8
  njb printReg

  cmp [mode], 11b
  jne eadressing

  mov al, bl
  xor bh, bh
  mov bl, [rm]
  cmp al, opRegMem8
  je printModrmReg
  add bl, 8

  printModrmReg:
  shl bl, 1
  mov si, registers[bx]
  call filePutString
  jmp endprint

  eadressing:
  cmp [typeOvr], ovrReg
  jae eaSegOverride
  cmp [typeOvr], ovrWord
  je printWordPtr
  fputs bytePtr
  jmp eaSegOverride

  printWordPtr:
  fputs wordPtr

  eaSegOverride:
  cmp [segOvr], 0
  je eaOpen

  xor bh, bh
  mov bl, [segOvr]
  shl bl, 1
  mov si, registers[bx]
  call filePutString
  fputc ':'
  mov [segOvr], 0

  eaOpen:
  fputc '['

  cmp [mode], 00b
  jne eaNormal
  cmp [rm], 110b
  je printDisplacement

  eaNormal:
  xor bh, bh
  mov bl, [rm]
  shl bl, 1
  mov si, eaVals[bx]
  call filePutString

  cmp [mode], 00b
  je eaClose
  fputc '+'

  printDisplacement:
  mov ax, [disp]

  cmp [mode], 01b
  jne printWordDisplacement
  mov dh, 1
  mov dl, al
  call printByteInHex
  jmp printHexSuffix

  printWordDisplacement:
  printWordInHex

  printHexSuffix:
  fputc 'h'

  eaClose:
  fputc ']'
  jmp endprint

  printConstReg:
  xor bh, bh
  shl bl, 1
  mov si, registers[bx]
  call filePutString
  ret

  printConst1:
  fputc '1'
  ret

  printConst3:
  fputc '3'
  ret

  printImm8:
  mov dx, [imm]
  mov dh, 1
  call printByteInHex
  fputc 'h'
  ret

  printShort:
  mov ax, [imm]
  cbw
  add ax, [fip]
  printWordInHex
  fputc 'h'
  ret

  printImm16:
  mov ax, [imm]
  printWordInHex
  fputc 'h'
  ret

  printNear:
  mov ax, [fip]
  mov bx, [imm]
  add ax, bx
  printWordInHex
  fputc 'h'
  ret

  printMem:
  fputc '['
  mov ax, [disp]
  printWordInHex
  fputc 'h'
  fputc ']'
  ret

  printFar:
  mov ax, [disp]
  printWordInHex
  fputc ':'
  mov ax, [imm]
  printWordInHex
  ret

  printReg:
  mov al, [reg]

  cmp bl, opReg8
  je loadRegName
  add al, 8
  cmp bl, opReg16
  je loadRegName
  add al, 8

  loadRegName:
  xor bh, bh
  mov bl, al
  shl bl, 1

  mov si, registers[bx]
  call filePutString
  ret

  endprint:
  ret
printOperand endp

__main__:

cmp byte ptr es:[80h], 0
jne readCmdArguments
jmp printProgInfo

readCmdArguments:
xor ch, ch
mov cl, es:[80h]

mov si, 81h

mov ax, @data
mov es, ax

call skipSpaces

lea di, inputFileName
call getFilename

call skipSpaces

cmp cx, 0
jne readOutputFilename
jmp printProgInfo

readOutputFilename:
lea di, outputFileName
call getFilename

mov ax, @data
mov ds, ax

call prepareInputFile
call createOutputFile

decodeNewOpc:
mov [modrm], 0
mov [imm], 0
mov [disp], 0
mov [typeOvr], 0

call readInputByte
call decodeOpc
cmp [opcType], tSegOvr
jne setupOffset

mov ax, [mnem]
mov [segOvr], al
jmp decodeNewOpc

setupOffset:
cmp [prefix], 1
je skipOffset
mov ax, [fip]
dec ax
cmp [segOvr], 0
je printOffset
dec ax

printOffset:
xor dh, dh
mov dl, ah
call printByteInHex
mov dl, al
call printByteInHex
fputc ':'
fputc ' '

skipOffset:
cmp [opcType], tUnknown
jne knownInstruction
fputs msgUnknownInstr
jmp decodeNewOpc

knownInstruction:
cmp [opcType], tExtOpc
jne printMnemonic
call decodeExtOpc

printMnemonic:
mov si, [mnem]
call filePutString
fputc ' '

cmp [opcType], tPrefix
jne analyzeOperands
mov [prefix], 1
jmp decodeNewOpc

analyzeOperands:
cmp [opcType], tCustom
jne skipWorkaround
call applyWorkaround

skipWorkaround:
mov dl, [op1]
call readOpBytes

mov dl, [op2]
call readOpBytes

mov bl, [op1]
call printOperand

cmp [op2], opNone
je endLine

fputc ','
fputc ' '

mov bl, [op2]
call printOperand

endLine:
fputs crlf
mov [prefix], 0
jmp decodeNewOpc

printProgInfo:
mov ax, @data
mov ds, ax

puts msgInfo

mov ax, 4C00h
int 21h

end __main__
