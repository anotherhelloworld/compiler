#ifndef COMPILER_OPTIMIZATOR_H
#define COMPILER_OPTIMIZATOR_H

#include "Generator.h"
#include <vector>
#include <set>

typedef bool (*TemplateFunc)(std::vector<AsmCommand*>* commands);

class Optimizator {
private:
    Generator* generator;
    std::vector <TemplateFunc> templates;
    void OptimizeCode(std::vector<AsmCommand*>*);
    void OptimizeFunc(AsmFunction*);
public:
    Optimizator(Generator*);
    void Optimize();
};


#endif //COMPILER_OPTIMIZATOR_H
