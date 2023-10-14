/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "Half_Adder.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/SPICECircuit.h"
#include "../../../../plugins/components/SPICENode.h"
#include "../../../../plugins/data/SPICERunner.h"
#include "../../../TraitsApp.h"

Half_Adder::Half_Adder() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Half_Adder::main(int argc, char** argv) {

	Simulator* genesys = new Simulator();
	genesys->getTracer()->setTraceLevel(TraitsApp<GenesysApplication_if>::traceLevel);
	setDefaultTraceHandlers(genesys->getTracer());
	PluginManager* plugins = genesys->getPlugins();
	plugins->autoInsertPlugins("autoloadplugins.txt");
	Model* model = genesys->getModels()->newModel();
	// create model
	SPICERunner* runner = plugins->newInstance<SPICERunner>(model);

	SPICENode* gnd = plugins->newInstance<SPICENode>(model, "gnd");
    SPICENode* vdd = plugins->newInstance<SPICENode>(model);
    SPICENode* a = plugins->newInstance<SPICENode>(model);
    SPICENode* b = plugins->newInstance<SPICENode>(model);
    SPICENode* sum = plugins->newInstance<SPICENode>(model);
    SPICENode* carry = plugins->newInstance<SPICENode>(model);

    double sim_time = 1, sim_step = 0.001;
    double voltage = 0.9;

	Vsource* v1 = plugins->newInstance<Vsource>(model);
	v1->init(runner, voltage);
    v1->insertAtRank(0, vdd);
    v1->insertAtRank(1, gnd);

	Vpulse* va = plugins->newInstance<Vpulse>(model);
	va->init(runner, voltage, sim_time);
    va->insertAtRank(0, a);
    va->insertAtRank(1, gnd);

	Vpulse* vb = plugins->newInstance<Vpulse>(model);
	vb->init(runner, voltage, sim_time/2);
    vb->insertAtRank(0, b);
    vb->insertAtRank(1, gnd);

	XNOR* xnor = plugins->newInstance<XNOR>(model);
	xnor->init(runner);
    xnor->insertAtRank(0, vdd);
    xnor->insertAtRank(1, gnd);
    xnor->insertAtRank(2, a);
    xnor->insertAtRank(3, b);
    xnor->insertAtRank(4, sum);

	NAND* nand = plugins->newInstance<NAND>(model);
	nand->init(runner);
    nand->insertAtRank(0, vdd);
    nand->insertAtRank(1, gnd);
    nand->insertAtRank(2, a);
    nand->insertAtRank(3, b);
    nand->insertAtRank(4, carry);

	runner->ConfigSim(sim_time, sim_step, "v("+sum->getNodeName()+")");
	runner->Run(runner->CompileSpiceFile());

	delete genesys;

	return 0;
};

