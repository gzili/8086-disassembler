; Data transfer
insMOV    db 'MOV$'
insPUSH   db 'PUSH$'
insPOP    db 'POP$'
insXCHG   db 'XCHG$'
insXLAT   db 'XLAT$'
insIN     db 'IN$'
insOUT    db 'OUT$'
; Address transfer
insLEA    db 'LEA$'
insLDS    db 'LDS$'
insLES    db 'LES$'
; Flag transfer
insLAHF   db 'LAHF$'
insSAHF   db 'SAHF$'
insPUSHF  db 'PUSHF$'
insPOPF   db 'POPF$'
; Addition
insADD    db 'ADD$'
insADC    db 'ADC$'
insINC    db 'INC$'
insAAA    db 'AAA$'
insDAA    db 'DAA$'
; Subtraction
insSUB    db 'SUB$'
insSBB    db 'SBB$'
insDEC    db 'DEC$'
insNEG    db 'NEG$'
insCMP    db 'CMP$'
insAAS    db 'AAS$'
insDAS    db 'DAS$'
; Multiplication
insMUL    db 'MUL$'
insIMUL   db 'IMUL$'
insAAM    db 'AAM$'
; Division
insDIV    db 'DIV$'
insIDIV   db 'IDIV$'
insAAD    db 'AAD$'
insCBW    db 'CBW$'
insCWD    db 'CWD$'
; Logical
insNOT    db 'NOT$'
insAND    db 'AND$'
insOR     db 'OR$'
insXOR    db 'XOR$'
insTEST   db 'TEST$'
; Shifts
insSHL    db 'SHL$'
insSHR    db 'SHR$'
insSAR    db 'SAR$'
; Rotates
insROL    db 'ROL$'
insROR    db 'ROR$'
insRCL    db 'RCL$'
insRCR    db 'RCR$'
; Repeat prefixes
insREP    db 'REP$'
insREPNE  db 'REPNE$'
; String instructions
insMOVSB  db 'MOVSB$'
insMOVSW  db 'MOVSW$'
insCMPSB  db 'CMPSB$'
insCMPSW  db 'CMPSW$'
insSCASB  db 'SCASB$'
insSCASW  db 'SCASW$'
insSTOSB  db 'STOSB$'
insSTOSW  db 'STOSW$'
insLODSB  db 'LODSB$'
insLODSW  db 'LODSW$'
; Unconditional control transfer
insCALL   db 'CALL$'
insRET    db 'RET$'
insRETF   db 'RETF$'
insJMP    db 'JMP$'
; Conditional control transfer
insJA     db 'JA$'
insJAE    db 'JAE$'
insJB     db 'JB$'
insJBE    db 'JBE$'
insJE     db 'JE$'
insJG     db 'JG$'
insJGE    db 'JGE$'
insJL     db 'JL$'
insJLE    db 'JLE$'
insJNE    db 'JNE$'
insJNO    db 'JNO$'
insJNP    db 'JNP$'
insJNS    db 'JNS$'
insJO     db 'JO$'
insJP     db 'JP$'
insJS     db 'JS$'
; Iteration control
insLOOP   db 'LOOP$'
insLOOPNE db 'LOOPNE$'
insLOOPE  db 'LOOPE$'
insJCXZ   db 'JCXZ$'
; Interrupts
insINT    db 'INT$'
insINTO   db 'INTO$'
insIRET   db 'IRET$'
; Flag operations
insSTC    db 'STC$'
insCLC    db 'CLC$'
insCMC    db 'CMC$'
insSTD    db 'STD$'
insCLD    db 'CLD$'
insSTI    db 'STI$'
insCLI    db 'CLI$'
; External synchronization
insHLT    db 'HLT$'
insWAIT   db 'WAIT$'
insLOCK   db 'LOCK$'
insESC    db 'ESC$'
; No operation
insNOP    db 'NOP$'

; Opcode types
opcTypes enum {
  tUnknown,
  tNormal,
  tExtOpc,
  tSegOvr,
  tPrefix,
  tCustom
}

; Enumerated operands structure
ops enum {
  ; Byte registers
  opAL,
  opCL,
  opDL,
  opBL,
  opAH,
  opCH,
  opDH,
  opBH,
  ; Word registers
  opAX,
  opCX,
  opDX,
  opBX,
  opSP,
  opBP,
  opSI,
  opDI,
  ; Segment registers
  opES,
  opSS,
  opCS,
  opDS,
  ; Constant numeric operands
  opConst1,
  opConst3,
  ; 1 byte operands
  opImm8,
  opShort,
  ; 2 byte operands
  opImm16,
  opNear,
  opMem,
  ; 4 byte operands
  opFar,
  ; Operands accompanied by MODRM byte
  opReg8,
  opReg16,
  opSegReg,
  opRegMem8,
  opRegMem16,
  ; Empty operand
  opNone
}

; Instruction decoding table
label instructionTable
instrItem<insADD,tNormal,opRegMem8,opReg8> ; 00
instrItem<insADD,tNormal,opRegMem16,opReg16> ; 01
instrItem<insADD,tNormal,opReg8,opRegMem8> ; 02
instrItem<insADD,tNormal,opReg16,opRegMem16> ; 03
instrItem<insADD,tNormal,opAL,opImm8> ; 04
instrItem<insADD,tNormal,opAX,opImm16> ; 05
instrItem<insPUSH,tNormal,opES,opNone> ; 06
instrItem<insPOP,tNormal,opES,opNone> ; 07
instrItem<insOR,tNormal,opRegMem8,opReg8> ; 08
instrItem<insOR,tNormal,opRegMem16,opReg16> ; 09
instrItem<insOR,tNormal,opReg8,opRegMem8> ; 0A
instrItem<insOR,tNormal,opReg16,opRegMem16> ; 0B
instrItem<insOR,tNormal,opAL,opImm8> ; 0C
instrItem<insOR,tNormal,opAX,opImm16> ; 0D
instrItem<insPUSH,tNormal,opCS,opNone> ; 0E
db (size instrItem)*1 dup (0) ; 0F (not used)
instrItem<insADC,tNormal,opRegMem8,opReg8> ; 10
instrItem<insADC,tNormal,opRegMem16,opReg16> ; 11
instrItem<insADC,tNormal,opReg8,opRegMem8> ; 12
instrItem<insADC,tNormal,opReg16,opRegMem16> ; 13
instrItem<insADC,tNormal,opAL,opImm8> ; 14
instrItem<insADC,tNormal,opAX,opImm16> ; 15
instrItem<insPUSH,tNormal,opSS,opNone> ; 16
instrItem<insPOP,tNormal,opSS,opNone> ; 17
instrItem<insSBB,tNormal,opRegMem8,opReg8> ; 18
instrItem<insSBB,tNormal,opRegMem16,opReg16> ; 19
instrItem<insSBB,tNormal,opReg8,opRegMem8> ; 1A
instrItem<insSBB,tNormal,opReg16,opRegMem16> ; 1B
instrItem<insSBB,tNormal,opAL,opImm8> ; 1C
instrItem<insSBB,tNormal,opAX,opImm16> ; 1D
instrItem<insPUSH,tNormal,opDS,opNone> ; 1E
instrItem<insPOP,tNormal,opDS,opNone> ; 1F
instrItem<insAND,tNormal,opRegMem8,opReg8> ; 20
instrItem<insAND,tNormal,opRegMem16,opReg16> ; 21
instrItem<insAND,tNormal,opReg8,opRegMem8> ; 22
instrItem<insAND,tNormal,opReg16,opRegMem16> ; 23
instrItem<insAND,tNormal,opAL,opImm8> ; 24
instrItem<insAND,tNormal,opAX,opImm16> ; 25
instrItem<opES,tSegOvr,opNone,opNone> ; 26
instrItem<insDAA,tNormal,opNone,opNone> ; 27
instrItem<insSUB,tNormal,opRegMem8,opReg8> ; 28
instrItem<insSUB,tNormal,opRegMem16,opReg16> ; 29
instrItem<insSUB,tNormal,opReg8,opRegMem8> ; 2A
instrItem<insSUB,tNormal,opReg16,opRegMem16> ; 2B
instrItem<insSUB,tNormal,opAL,opImm8> ; 2C
instrItem<insSUB,tNormal,opAX,opImm16> ; 2D
instrItem<opCS,tSegOvr,opNone,opNone> ; 2E
instrItem<insDAS,tNormal,opNone,opNone> ; 2F
instrItem<insXOR,tNormal,opRegMem8,opReg8> ; 30
instrItem<insXOR,tNormal,opRegMem16,opReg16> ; 31
instrItem<insXOR,tNormal,opReg8,opRegMem8> ; 32
instrItem<insXOR,tNormal,opReg16,opRegMem16> ; 33
instrItem<insXOR,tNormal,opAL,opImm8> ; 34
instrItem<insXOR,tNormal,opAX,opImm16> ; 35
instrItem<opSS,tSegOvr,opNone,opNone> ; 36
instrItem<insAAA,tNormal,opNone,opNone> ; 37
instrItem<insCMP,tNormal,opRegMem8,opReg8> ; 38
instrItem<insCMP,tNormal,opRegMem16,opReg16> ; 39
instrItem<insCMP,tNormal,opReg8,opRegMem8> ; 3A
instrItem<insCMP,tNormal,opReg16,opRegMem16> ; 3B
instrItem<insCMP,tNormal,opAL,opImm8> ; 3C
instrItem<insCMP,tNormal,opAX,opImm16> ; 3D
instrItem<opDS,tSegOvr,opNone,opNone> ; 3E
instrItem<insAAS,tNormal,opNone,opNone> ; 3F
instrItem<insINC,tNormal,opAX,opNone> ; 40
instrItem<insINC,tNormal,opCX,opNone> ; 41
instrItem<insINC,tNormal,opDX,opNone> ; 42
instrItem<insINC,tNormal,opBX,opNone> ; 43
instrItem<insINC,tNormal,opSP,opNone> ; 44
instrItem<insINC,tNormal,opBP,opNone> ; 45
instrItem<insINC,tNormal,opSI,opNone> ; 46
instrItem<insINC,tNormal,opDI,opNone> ; 47
instrItem<insDEC,tNormal,opAX,opNone> ; 48
instrItem<insDEC,tNormal,opCX,opNone> ; 49
instrItem<insDEC,tNormal,opDX,opNone> ; 4A
instrItem<insDEC,tNormal,opBX,opNone> ; 4B
instrItem<insDEC,tNormal,opSP,opNone> ; 4C
instrItem<insDEC,tNormal,opBP,opNone> ; 4D
instrItem<insDEC,tNormal,opSI,opNone> ; 4E
instrItem<insDEC,tNormal,opDI,opNone> ; 4F
instrItem<insPUSH,tNormal,opAX,opNone> ; 50
instrItem<insPUSH,tNormal,opCX,opNone> ; 51
instrItem<insPUSH,tNormal,opDX,opNone> ; 52
instrItem<insPUSH,tNormal,opBX,opNone> ; 53
instrItem<insPUSH,tNormal,opSP,opNone> ; 54
instrItem<insPUSH,tNormal,opBP,opNone> ; 55
instrItem<insPUSH,tNormal,opSI,opNone> ; 56
instrItem<insPUSH,tNormal,opDI,opNone> ; 57
instrItem<insPOP,tNormal,opAX,opNone> ; 58
instrItem<insPOP,tNormal,opCX,opNone> ; 59
instrItem<insPOP,tNormal,opDX,opNone> ; 5A
instrItem<insPOP,tNormal,opBX,opNone> ; 5B
instrItem<insPOP,tNormal,opSP,opNone> ; 5C
instrItem<insPOP,tNormal,opBP,opNone> ; 5D
instrItem<insPOP,tNormal,opSI,opNone> ; 5E
instrItem<insPOP,tNormal,opDI,opNone> ; 5F
db (size instrItem)*16 dup (0) ; 60-6F (not used)
instrItem<insJO,tNormal,opShort,opNone> ; 70
instrItem<insJNO,tNormal,opShort,opNone> ; 71
instrItem<insJB,tNormal,opShort,opNone> ; 72
instrItem<insJAE,tNormal,opShort,opNone> ; 73
instrItem<insJE,tNormal,opShort,opNone> ; 74
instrItem<insJNE,tNormal,opShort,opNone> ; 75
instrItem<insJBE,tNormal,opShort,opNone> ; 76
instrItem<insJA,tNormal,opShort,opNone> ; 77
instrItem<insJS,tNormal,opShort,opNone> ; 78
instrItem<insJNS,tNormal,opShort,opNone> ; 79
instrItem<insJP,tNormal,opShort,opNone> ; 7A
instrItem<insJNP,tNormal,opShort,opNone> ; 7B
instrItem<insJL,tNormal,opShort,opNone> ; 7C
instrItem<insJGE,tNormal,opShort,opNone> ; 7D
instrItem<insJLE,tNormal,opShort,opNone> ; 7E
instrItem<insJG,tNormal,opShort,opNone> ; 7F
instrItem<insExtOpc80_83,tExtOpc,opRegMem8,opImm8> ; 80
instrItem<insExtOpc80_83,tExtOpc,opRegMem16,opImm16> ; 81
instrItem<insExtOpc80_83,tExtOpc,opRegMem8,opImm8> ; 82
instrItem<insExtOpc80_83,tExtOpc,opRegMem16,opImm8> ; 83
instrItem<insTEST,tNormal,opRegMem8,opReg8> ; 84
instrItem<insTEST,tNormal,opRegMem16,opReg16> ; 85
instrItem<insXCHG,tNormal,opRegMem8,opReg8> ; 84
instrItem<insXCHG,tNormal,opRegMem16,opReg16> ; 85
instrItem<insMOV,tNormal,opRegMem8,opReg8> ; 88
instrItem<insMOV,tNormal,opRegMem16,opReg16> ; 89
instrItem<insMOV,tNormal,opReg8,opRegMem8> ; 8A
instrItem<insMOV,tNormal,opReg16,opRegMem16> ; 8B
instrItem<insMOV,tNormal,opRegMem16,opSegReg> ; 8C
instrItem<insLEA,tNormal,opReg16,opRegMem16> ; 8D
instrItem<insMOV,tNormal,opSegReg,opRegMem16> ; 8E
instrItem<insPOP,tNormal,opRegMem16,opNone> ; 8F
instrItem<insNOP,tNormal,opNone,opNone> ; 90
instrItem<insXCHG,tNormal,opAX,opCX> ; 91
instrItem<insXCHG,tNormal,opAX,opDX> ; 92
instrItem<insXCHG,tNormal,opAX,opBX> ; 93
instrItem<insXCHG,tNormal,opAX,opSP> ; 94
instrItem<insXCHG,tNormal,opAX,opBP> ; 95
instrItem<insXCHG,tNormal,opAX,opSI> ; 96
instrItem<insXCHG,tNormal,opAX,opDI> ; 97
instrItem<insCBW,tNormal,opNone,opNone> ; 98
instrItem<insCWD,tNormal,opNone,opNone> ; 99
instrItem<insCALL,tNormal,opFar,opNone> ; 9A
instrItem<insWAIT,tNormal,opNone,opNone> ; 9B
instrItem<insPUSHF,tNormal,opNone,opNone> ; 9C
instrItem<insPOPF,tNormal,opNone,opNone> ; 9D
instrItem<insSAHF,tNormal,opNone,opNone> ; 9E
instrItem<insLAHF,tNormal,opNone,opNone> ; 9F
instrItem<insMOV,tNormal,opAL,opMem> ; A0
instrItem<insMOV,tNormal,opAX,opMem> ; A1
instrItem<insMOV,tNormal,opMem,opAL> ; A2
instrItem<insMOV,tNormal,opMem,opAX> ; A3
instrItem<insMOVSB,tNormal,opNone,opNone> ; A4
instrItem<insMOVSW,tNormal,opNone,opNone> ; A5
instrItem<insCMPSB,tNormal,opNone,opNone> ; A6
instrItem<insCMPSW,tNormal,opNone,opNone> ; A7
instrItem<insTEST,tNormal,opAL,opImm8> ; A8
instrItem<insTEST,tNormal,opAX,opImm16> ; A9
instrItem<insSTOSB,tNormal,opNone,opNone> ; AA
instrItem<insSTOSW,tNormal,opNone,opNone> ; AB
instrItem<insLODSB,tNormal,opNone,opNone> ; AC
instrItem<insLODSW,tNormal,opNone,opNone> ; AD
instrItem<insSCASB,tNormal,opNone,opNone> ; AE
instrItem<insSCASW,tNormal,opNone,opNone> ; AF
instrItem<insMOV,tNormal,opAL,opImm8> ; B0
instrItem<insMOV,tNormal,opCL,opImm8> ; B1
instrItem<insMOV,tNormal,opDL,opImm8> ; B2
instrItem<insMOV,tNormal,opBL,opImm8> ; B3
instrItem<insMOV,tNormal,opAH,opImm8> ; B4
instrItem<insMOV,tNormal,opCH,opImm8> ; B5
instrItem<insMOV,tNormal,opDH,opImm8> ; B6
instrItem<insMOV,tNormal,opBH,opImm8> ; B7
instrItem<insMOV,tNormal,opAX,opImm16> ; B8
instrItem<insMOV,tNormal,opCX,opImm16> ; B9
instrItem<insMOV,tNormal,opDX,opImm16> ; BA
instrItem<insMOV,tNormal,opBX,opImm16> ; BB
instrItem<insMOV,tNormal,opSP,opImm16> ; BC
instrItem<insMOV,tNormal,opBP,opImm16> ; BD
instrItem<insMOV,tNormal,opSI,opImm16> ; BE
instrItem<insMOV,tNormal,opDI,opImm16> ; BF
db (size instrItem)*2 dup (0) ; C0-C1 (not used)
instrItem<insRET,tNormal,opImm16,opNone> ; C2
instrItem<insRET,tNormal,opNone,opNone> ; C3
instrItem<insLES,tNormal,opReg16,opRegMem16>
instrItem<insLDS,tNormal,opReg16,opRegMem16>
instrItem<insMOV,tNormal,opRegMem8,opImm8> ; C6
instrItem<insMOV,tNormal,opRegMem16,opImm16> ; C7
db (size instrItem)*2 dup (0) ; C8-C9 (not used)
instrItem<insRETF,tNormal,opImm16,opNone> ; CA
instrItem<insRETF,tNormal,opNone,opNone> ; CB
instrItem<insINT,tNormal,opConst3,opNone> ; CC
instrItem<insINT,tNormal,opImm8,opNone> ; CD
instrItem<insINTO,tNormal,opNone,opNone> ; CE
instrItem<insIRET,tNormal,opNone,opNone> ; CF
instrItem<insExtOpcD0_D3,tExtOpc,opRegMem8,opConst1> ; D0
instrItem<insExtOpcD0_D3,tExtOpc,opRegMem16,opConst1> ; D1
instrItem<insExtOpcD0_D3,tExtOpc,opRegMem8,opCL> ; D2
instrItem<insExtOpcD0_D3,tExtOpc,opRegMem16,opCL> ; D3
instrItem<insAAM,tCustom,opNone,opNone> ; D4 (has a constant second byte)
instrItem<insAAD,tCustom,opNone,opNone> ; D5 (has a constant second byte)
db (size instrItem)*1 dup (0) ; D6 (not used)
instrItem<insXLAT,tNormal,opNone,opNone> ; D7
instrItem<insESC,tCustom,opNone,opRegMem8> ; D8 (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; D9 (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DA (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DB (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DC (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DD (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DE (custom calculation for first operand)
instrItem<insESC,tCustom,opNone,opRegMem8> ; DF (custom calculation for first operand)
instrItem<insLOOPNE,tNormal,opShort,opNone> ; E0
instrItem<insLOOPE,tNormal,opShort,opNone> ; E1
instrItem<insLOOP,tNormal,opShort,opNone> ; E2
instrItem<insJCXZ,tNormal,opShort,opNone> ; E3
instrItem<insIN,tNormal,opAL,opImm8> ; E4
instrItem<insIN,tNormal,opAX,opImm16> ; E5
instrItem<insOUT,tNormal,opImm8,opAL> ; E6
instrItem<insOUT,tNormal,opImm8,opAX> ; E7
instrItem<insCALL,tNormal,opNear,opNone> ; E8
instrItem<insJMP,tNormal,opNear,opNone> ; E9
instrItem<insJMP,tNormal,opFar,opNone> ; EA
instrItem<insJMP,tNormal,opShort,opNone> ; EB
instrItem<insIN,tNormal,opAL,opDX> ; EC
instrItem<insIN,tNormal,opAX,opDX> ; ED
instrItem<insOUT,tNormal,opDX,opAL> ; EE
instrItem<insOUT,tNormal,opDX,opAX> ; EF
instrItem<insLOCK,tPrefix,opNone,opNone> ; F0
db (size instrItem)*1 dup (0) ; F1 (not used)
instrItem<insREPNE,tPrefix,opNone,opNone> ; F2
instrItem<insREP,tPrefix,opNone,opNone> ; F3
instrItem<insHLT,tNormal,opNone,opNone> ; F4
instrItem<insCMC,tNormal,opNone,opNone> ; F5
instrItem<insExtOpcF6_F7,tExtOpc,opRegMem8,opNone> ; F6
instrItem<insExtOpcF6_F7,tExtOpc,opRegMem16,opNone> ; F7
instrItem<insCLC,tNormal,opNone,opNone> ; F8
instrItem<insSTC,tNormal,opNone,opNone> ; F9
instrItem<insCLI,tNormal,opNone,opNone> ; FA
instrItem<insSTI,tNormal,opNone,opNone> ; FB
instrItem<insCLD,tNormal,opNone,opNone> ; FC
instrItem<insSTD,tNormal,opNone,opNone> ; FD
instrItem<insExtOpcFE_FF,tExtOpc,opRegMem8,opNone> ; FE
instrItem<insExtOpcFE_FF,tExtOpc,opRegMem16,opNone> ; FF

; Mnemonics for extended opcode instructions 80-83
label insExtOpc80_83
dw insADD
dw insOR
dw insADC
dw insSBB
dw insAND
dw insSUB
dw insXOR
dw insCMP

; Mnemonics for extended opcode instructions D0-D3
label insExtOpcD0_D3
dw insROL
dw insROR
dw insRCL
dw insRCR
dw insSHL
dw insSHR
dw insNOP ; not used
dw insSAR

; Mnemonics for extended opcode instructions F6-F7
label insExtOpcF6_F7
dw insTEST
dw insNOP ; not used
dw insNOT
dw insNEG
dw insMUL
dw insIMUL
dw insDIV
dw insIDIV

; Mnemonics for extended opcode instructions FE-FF
label insExtOpcFE_FF
dw insINC
dw insDEC
dw insCALL
dw insCALL
dw insJMP
dw insJMP
dw insPUSH
dw insNOP ; not used
