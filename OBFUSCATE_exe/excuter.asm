
.code


validate_asm PROC
  
    mov     rax, rcx           
    
  
    mov     r8, 123456789      
    

    xor     rdx, rdx           
    div     r8              

    xor     rax, rax           
    test    rdx, rdx         
    setz    al                 
    
    ret                      
    

validate_asm ENDP

END