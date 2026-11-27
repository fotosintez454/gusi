.section .idata$2,"aw"
.long ilt-0x400000,0,0,kernelname-0x400000,iat-0x400000
.section .idata$3,"aw"
.long 0,0,0,0,0
.section .idata$4,"aw"
ilt: .long name1-0x400000,0,name2-0x400000,0,0,0
.section .idata$5,"aw"
.global imp_LoadLibraryA
.global imp_GetProcAddress
iat:
imp_LoadLibraryA: .long name1-0x400000,0
imp_GetProcAddress: .long name2-0x400000,0
.quad 0
.section .idata$6,"aw"
name1: .short 0
.asciz "LoadLibraryA"
.balign 2
name2: .short 0
.asciz "GetProcAddress"
kernelname: .asciz "KERNEL32.dll"
