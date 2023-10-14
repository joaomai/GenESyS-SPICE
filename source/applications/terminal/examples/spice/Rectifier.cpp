/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "Rectifier.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/SPICECircuit.h"
#include "../../../../plugins/components/SPICENode.h"
#include "../../../../plugins/data/SPICERunner.h"
#include "../../../TraitsApp.h"

Rectifier::Rectifier() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Rectifier::main(int argc, char** argv) {

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
	SPICENode* vm = plugins->newInstance<SPICENode>(model);
	SPICENode* vp = plugins->newInstance<SPICENode>(model);

    double sim_time = 1, sim_step = 0.001;
    double voltage = 0.9;

	Vsine* vpu1 = plugins->newInstance<Vsine>(model);
	vpu1->init(runner, 12, 60);
	vpu1->insertAtRank(0, vdd);
	vpu1->insertAtRank(1, gnd);

	Diode* d1 = plugins->newInstance<Diode>(model);
	d1->init(runner);
	d1->insertAtRank(0, vm);
	d1->insertAtRank(1, vdd);

	Diode* d2 = plugins->newInstance<Diode>(model);
	d2->init(runner);
	d2->insertAtRank(0, vm);
	d2->insertAtRank(1, gnd);

	Diode* d3 = plugins->newInstance<Diode>(model);
	d3->init(runner);
	d3->insertAtRank(0, vdd);
	d3->insertAtRank(1, vp);

	Diode* d4 = plugins->newInstance<Diode>(model);
	d4->init(runner);
	d4->insertAtRank(0, gnd);
	d4->insertAtRank(1, vm);

	Capacitor* c1 = plugins->newInstance<Capacitor>(model);
	c1->init(runner, 0.01);
	c1->insertAtRank(0, vp);
	c1->insertAtRank(1, vm);

	Resistor* r1 = plugins->newInstance<Resistor>(model);
	r1->init(runner, 1000);
	r1->insertAtRank(0, vp);
	r1->insertAtRank(1, vm);

	runner->ConfigSim(sim_time, sim_step,
					  "v("+vp->getNodeName()+")-v("+vm->getNodeName()+")");
	runner->Run(runner->CompileSpiceFile());

	delete genesys;

	return 0;
};

