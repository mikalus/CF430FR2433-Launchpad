; Register, Model, and Macro declarations - MSP430FR2433  ; M.Kalus April 2016

; Template was: CF430forth.h
; for TI MSP430F1611 by B. Rodriguez  3 Jan 09

; This file is used to build CF430FR2433 using the IAR workbench.

; ----------------------------------------------------------------------
; CamelForth for the Texas Instruments MSP430 
; (c) 2009 Bradford J. Rodriguez.
; 
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
; Commercial inquiries should be directed to the author at 
; 115 First St., #105, Collingwood, Ontario L9Y 4W3 Canada
; or via email to bj@camelforth.com

; ----------------------------------------------------------------------
; REVISION  HISTORY

; apr 2016 mk - added memory map of MSP430FR2433...
;  ... previous map deleted. 
;    dec 11 mk  - added memory map of MSP430FR5739.
;  (1 mar 09 bjr - added INFOSTART, changed FLASHSTART to be Main flash address
;  no flash in MSP430FR5739, we have FRAM now.) 
; 17 jan 09 bjr - changed IMMEDIATE flag from $00 to $FE to allow
;   use as a token field.

; ----------------------------------------------------------------------
; MEMORY MAP of the MSP430FR2433 (mk 4/2016 - SLAU445F 3/2016)
#define peri0  0000h  ; 0000-0FFF = peripherals (4 KB)
#define ddi0   0FF4h  ; 0FF4-0FF7 = device descriptor info 
; #define BSL0   1000h  ; 1000-17FF = bootstrap loader BSL0..3 (ROM 4x512 B) 
#define infoB0 1800h  ; 1800-19FF = info B (FRAM 128 B) 
#define ram0   2000h  ; 2000-2FFF = RAM (4 KB)
#define fram0  0C400h  ; C400-FFFF = code memory (FRAM 15KB) 
#define vecs0  0FF80h  ; FF80-FFFF = interrupt vectors (FRAM 127 B) 

; ----------------------------------------------------------------------
; FORTH MEMORY USAGE

; user area, stacks, tib, pad, holdarea have to fit in RAMSEG 
#define RAMSEG     1024

; INFO B,A (FRAM) unused so far.
#define INFOASEG    128
#define INFOBSEG    128
                       
; Reset service, MCU initialisation, Forth Dictionary
#define FRAMSEG   15743                        

; Vector area. Only RESET is used.                       
#define VECAREA     127

; ----------------------------------------------------------------------
; FORTH REGISTER USAGE

; Forth virtual machine
#define RSP SP
#define PSP R4
#define IP  R5
#define W   R6
#define TOS R7

; Loop parameters in registers
#define INDEX R8
#define LIMIT R9

; Scratch registers
#define X  R10
#define Y  R11
#define Q  R12
#define T  R13

; T.I. Integer Subroutines Definitions
#define IROP1   TOS
#define IROP2L  R10
#define IROP2M  R11
#define IRACL   R12
#define IRACM   R13
#define IRBT    W  


; ----------------------------------------------------------------------
; Routines of the Forth virtual machine

; INDIRECT-THREADED NEXT

NEXT    MACRO
        MOV @IP+,W      ; fetch word address into W
        MOV @W+,PC      ; fetch code address into PC, W=PFA
        ENDM

; BRANCH DESTINATION (RELATIVE BRANCH)
; For relative branch addresses, i.e., a branch is ADD @IP,IP

DEST    MACRO   label
        DW      label-$
        ENDM

; HEADER CONSTRUCTION MACROS

HEADER  MACRO   asmname,length,litname,action
        PUBLIC  asmname
        DW      link
        DB      0FFh       ; not immediate
link    SET     $
        DB      length
        DB      litname
        EVEN
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

HEADLESS  MACRO   asmname,action
        PUBLIC  asmname
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

IMMED   MACRO   asmname,length,litname,action
        PUBLIC  asmname
        DW      link
        DB      0FEh      ; immediate (LSB=0)
link    SET     $
        DB      length
        DB      litname
        EVEN
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

