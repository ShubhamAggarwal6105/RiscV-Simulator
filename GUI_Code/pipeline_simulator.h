// pipeline_simulator.h
#ifndef PIPELINE_SIMULATOR_H
#define PIPELINE_SIMULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include "pipeline_registers.h"
#include "knobs.h"
#include "branchprediction.h"
#include "control_unit.h"

namespace pipeline {

// register file and memory
extern unsigned int R[32];
extern std::unordered_map<unsigned int, unsigned char> MEM;
extern unsigned int PC;

// top‑level API
void    load_program_memory(const std::string& filename);
void    write_registers_to_file(const std::string& filename);
void    write_memory_to_file(const std::string& filename);

void    print_registers();
void    print_memory();

void resetPipelinedSimulator();
std::vector<int> stepPipelinedSimulator(KnobManager& knobs);
pair<std::vector<int>, map<std::uint32_t, std::vector<std::pair<bool, bool>>>> runPipelinedSimulator (KnobManager& knobs);

} // namespace pipeline

#endif // PIPELINE_SIMULATOR_H
