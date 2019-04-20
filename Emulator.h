//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

    const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
    emulator() {
		for (int i = 0; i < MEMSZ;i++) {
			m_memory[i] = 0;
		}
		acceumulator = 0;
    }
    // Records instructions and data into VC3600 memory.
    bool InsertMemory( int a_location, int a_contents );
    
    // Runs the VC3600 program recorded in memory.
    bool RunProgram( );


	void Read(int a_location);
private:
	
    int m_memory[MEMSZ];       // The memory of the VC3600.
    int acceumulator;			// The accumulator for the VC3600
};

#endif