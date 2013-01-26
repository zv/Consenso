// ALL HAIL THE MIGHTY MACHINE CONSENSO
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <strings.h>

// I met a processor from an antique land
// Who saith: "Two vast and trunkless register of opcodes"
// Stand in the desert, Near them, on the sand,
// a half sunk and shattered visage lies, whose
// frown and sneer of cold command tell that it's 
// creators well read those passions which survive
// yet. 
//
// We know the hand that mocked them, the heart that fed.
//
// Upon this CPU, these words appears.
// "MY NAME IS CONSENSO, SYN OF ACKS
// LOOK UPON MY RUN ROUTINE YE MIGHTY 
// AND DESPAIR"
//
// Nothing besides him remains
//
// Around the decay of this colossal wreck, boundless
// and bare. 
//
// The lone and level sands stretch far away.


#define BYTE_SIZE 8

// Manipulation
#define LOADB  0x10
#define STOREB 0x11
// Arithmetic
#define CMP     0x20
#define ADD     0x3
#define MUL     0xA

// Logic
#define AND     0x4
#define ORN     0x5 
// Branches
#define BA      0x61
#define BNE     0x62
#define BE      0x63
#define BG      0x64
#define BLE     0x65
#define BGE     0x66
#define BL      0x67

// Stack Frame
#define CALL    0x70
#define SAVE    0x8
#define RESTORE 0x9

// Emulator Call
#define CALL_GATE 0xE

// Comparison Register
#define COMPARISON_REGISTER 0xE

// Quit the emulator
void quit(const char *why,int value1=0,int value2=0) 
	{ fprintf(stderr, why, value1, value2); exit(0); }


class CPU {
  public: enum {n_registers = 16};
    // 15 is reserved as an instruction pointer
    // 14 is reserved as a a status flag register
    // 13 -> 11 are saved program state registers so we can implement cooperative multitasking
    // Rest are GP
    
    int registers[n_registers]; ///< "registers"-- temporary working space of machine

    // When we're ready to get real enum {n_mem=65536};
    enum {n_mem = 1024};
    int mem[n_mem]; 
    
    CPU() { 
      // Sets the default value of a register 
      for (int r=0;r<n_registers;r++) registers[r]=0xABADBABE;
      // Sets the default value of a memory location
      for (int m=0;m<n_mem ;m++) mem [m]=0xCAFEBABE;
    }
    int run(void);
    void dump(int id); /* Print out registers */
  };

// here's the total of what I would like to implement
// Multiply (Through Discrete Fourier Transform)
// Swap two registers
// Load/Store a Byte
// Conditional Branch (Make it easy though)

int CPU::run(void) {
	while (1) {

    // i am glad jessy is here :) 
		int inst = mem[registers[0xf]++]; 

		int opcode=(inst >> 12) & 0xf, i=(inst>>8) & 0xf, 
		         j=(inst >> 4)  & 0xf, k=(inst>>0) & 0xf;

    // This routine decodes the instruction
		switch (opcode) { 

   /*  
    * Data Manipulation
    */
 
    // loadb
    // Load Byte (Signed) from Address
    case LOADB: registers[i] = mem[registers[j]]; break;

    // storeb
    // Store Byte (Signed) from Address
    case STOREB: mem[registers[i]] = registers[j]; break;

   /*
    * Arithmetic  
    */

    // cmp
    // Compare two registers and set a flag
    case CMP: if (registers[j]==registers[k])
                registers[COMPARISON_REGISTER] = 0; 
              else if (registers[j] > registers[k])
                registers[COMPARISON_REGISTER] = 1; 
              else if (registers[j] < registers[k])
                registers[COMPARISON_REGISTER] = -1; 
              break; 

    // mul
    // Multiply Two Numbers
    case MUL: registers[i]=registers[j]*registers[k]; break;

    // add
    // Add Two Numbers
		case ADD: registers[i]=registers[j]+registers[k]; break; 

    // Emulator Call
		case CALL_GATE:
			switch (i) {
      // Print the value of k (in decimal) & j
			case 0x0: printf("%d%c",registers[k]," \n\t,"[j]); break;

      // Assign a value to register i 
      // ex. 0x12FF would assign 0xFF to register 2
			case 0x1: printf("Enter a value:\n");  
				scanf("%i",&registers[k]); break; 

      // Dump :) 
			case 0xD: dump(inst & 0xFF); break; 
      
      // exit with  return code of k
			case 0xE: return registers[k]; 

      // Faulty Trap
			default: quit("Shit's getting crazy around 0x%X at 0x%X!\n",inst,registers[0xf]-1);
			};
			break;

    // Couldn't read the instruction
		default: quit("Couldn't read this noise @ 0x%X at 0x%X!\n",inst,registers[0xf]-1);
		};
	}
}

void CPU::dump(int id) {
	if (id==0xff) return; /* 0xEDFF is a disabled dump */
	printf("Dump %02x:\n",id);
	if (id<=0x80)
	  for (int m=0;m<n_mem;m++)
		if (mem[m]!=0)
			printf("Memory[%X]:0x%X \n",m,mem[m]);
			
	for (int i=0;i<16;i++)
		if (registers[i]!=0)
			printf("Register[%X]:0x%X  (or %d)\n",i,registers[i],registers[i]);
	printf("\n");
}

int main(int argc,char *argv[]) {
	CPU cpu;
	int initialization_location = 0x80; //  Location in memory where program is loaded 
	cpu.registers[0xf] = initialization_location; // Start at first instruction of program 
	
	int instruction, load_count=0;

	while (std::cin && std::cin >> std::hex >> instruction && instruction>=0) {
		cpu.mem[initialization_location+load_count++]=instruction;
		std::string comment; std::getline(std::cin,comment); 
	}
	cpu.dump(0x10); 
	
  // run 
  
	int v = cpu.run();
	quit("Execution Routine Finished. Return code: %d (0x%X)\n",v,v);
	return 0;
}
