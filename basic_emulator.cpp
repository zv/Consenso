// This is an emulator based off the MIT 0613 Operating Systems Design Course

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


// Quit the emulator
void quit(const char *why,int value1=0,int value2=0) 
	{ fprintf(stderr, why, value1, value2); exit(0); }


class CPU {
  public:
    enum {n_registers = 16};
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

// Note from Zephyr
// As I see it, This basic emulator implements a reasonable immediate
// architecture for LEA, and is technically workable but to be perfectly
// honest I have no fucking clue how to write anything but the most 
// basic program with this istruction set

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

      // Jump to Address
    case 0xF: registers[0xf] = registers[j]; break;
   
    // hack
    case 0x9: registers[0xf] = 0x0; break;

    // Load Effective Address
		case 0x1: registers[i]=(inst&0xff); break; 

    // multiplication  
    case 0x2: registers[i]=registers[j]*registers[k]; break;

    // Addition 
		case 0xA: registers[i]=registers[j]+registers[k]; break; 

    // Increment Register i if J == K
		case 0xB: if (registers[j]==registers[k]) registers[i]++; break; 

    // Increment Register i if J >=  K
		case 0xC: if (registers[j]>=registers[k]) registers[i]++; break; 

    // Increment Register i if J > K
		case 0xD: if (registers[j]>registers[k]) registers[i]++; break; 
    
    // Emulator Call
		case 0xE:
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
