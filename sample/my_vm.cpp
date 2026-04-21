#include <iostream>
#include <vector>

class SimpleVM {
private:
    int regs[4];        
    int memory[256];    
    bool running;
    int flag_zero;      
    
public:
    SimpleVM() {
        for(int i = 0; i < 4; i++) regs[i] = 0;
        for(int i = 0; i < 256; i++) memory[i] = 0;
        running = true;
        flag_zero = 0;
    }
    
    void execute(const std::vector<uint8_t>& code) {
        size_t ip = 0;
        
        while(running && ip < code.size()) {
            uint8_t op = code[ip++];
            
            switch(op) {
                case 0x01: {  // MOV reg to reg
                    uint8_t dest = code[ip++];
                    uint8_t src = code[ip++];
                    regs[dest] = regs[src];
                    break;
                }
                case 0x02: {  // ADD
                    uint8_t dest = code[ip++];
                    uint8_t src = code[ip++];
                    regs[dest] = regs[dest] + regs[src];
                    break;
                }
                case 0x03: {  // SUB
                    uint8_t dest = code[ip++];
                    uint8_t src = code[ip++];
                    regs[dest] = regs[dest] - regs[src];
                    break;
                }
                case 0x04: {  // MOV immediate to reg
                    uint8_t dest = code[ip++];
                    int value = code[ip++];
                    regs[dest] = value;
                    break;
                }
                case 0x05: {  // MOV reg to memory
                    uint8_t mem_addr = code[ip++];
                    uint8_t reg_src = code[ip++];
                    memory[mem_addr] = regs[reg_src];
                    break;
                }
                case 0x06: {  // MOV memory to reg
                    uint8_t reg_dest = code[ip++];
                    uint8_t mem_addr = code[ip++];
                    regs[reg_dest] = memory[mem_addr];
                    break;
                }
                case 0x07: {  // CMP
                    uint8_t reg1 = code[ip++];
                    uint8_t reg2 = code[ip++];
                    flag_zero = (regs[reg1] == regs[reg2]) ? 1 : 0;
                    break;
                }
                case 0x08: {  // CMP immediate 
                    uint8_t reg = code[ip++];
                    int value = code[ip++];
                    flag_zero = (regs[reg] == value) ? 1 : 0;
                    break;
                }
                case 0x09: {  // JUMP 
                    uint8_t address = code[ip++];
                    ip = address;
                    break;
                }
                case 0x0A: {  // JE 
                    uint8_t address = code[ip++];
                    if(flag_zero == 1) {
                        ip = address;
                    }
                    break;
                }
                case 0x0B: {  // JNE 
                    uint8_t address = code[ip++];
                    if(flag_zero == 0) {
                        ip = address;
                    }
                    break;
                }
                case 0x0C: {  // DIV 
                    uint8_t dest = code[ip++];
                    uint8_t src = code[ip++];
                    if(regs[src] != 0) {
                        regs[dest] = regs[dest] / regs[src];
                    }
                    break;
                }
                case 0x0D: {  // MOD 
                    uint8_t dest = code[ip++];
                    uint8_t src = code[ip++];
                    if(regs[src] != 0) {
                        regs[dest] = regs[dest] % regs[src];
                    }
                    break;
                }
                case 0x0E: {  // INC
                    uint8_t reg = code[ip++];
                    regs[reg]++;
                    break;
                }
                case 0x0F: {  // DEC 
                    uint8_t reg = code[ip++];
                    regs[reg]--;
                    break;
                }
                case 0xFF: {
                    running = false;
                    break;
                }
                default:
                    std::cout << "Unknown opcode: " << std::hex << (int)op << std::endl;
                    running = false;
                    break;
            }
        }
    }
    
    void setReg(int reg, int value) {
        if(reg >= 0 && reg < 4) regs[reg] = value;
    }
    
    void setMemory(int addr, int value) {
        if(addr >= 0 && addr < 256) memory[addr] = value;
    }
    
    void runWithInput(int input, const std::vector<uint8_t>& code) {
        regs[0] = input;  
        execute(code);
    }
    
    int getReg(int i) { return regs[i]; }
    int getMemory(int addr) { return memory[addr]; }
    

};