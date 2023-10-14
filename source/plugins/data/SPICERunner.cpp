/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   SPICERunner.cpp
 * Author: bnmfw & joaomai
 *
 * Created on 10 de outubro de 2023, 23:00
 */

#include "SPICERunner.h"

#ifdef PLUGINCONNECT_DYNAMIC

/// Externalize function GetPluginInformation to be accessible throught dynamic linked library
extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &SPICERunner::GetPluginInformation;
}
#endif

//
// public: /// constructors
//

SPICERunner::SPICERunner(Model* model, std::string name) : ModelDataDefinition(model, Util::TypeOf<SPICERunner>(), name) {
}


//
// public: /// new public user methods for this component
//

std::string SPICERunner::CompileSpiceFile() {
    std::string spicefile = "GenESyS generated circuit\n\n";//set spicebehavior=all\n\n";
    // Resolves all model dependencies
	for (std::string model: models) spicefile += ".include " + models_path + model + ".cir\n";
    spicefile += "\n";
    // Compiles all subcircuit templates
    for (std::string subcircuit: subcircuits) spicefile += subcircuit + "\n";
    // Compiles all circuit instances
    for (std::string* instance: instances) spicefile += *instance + "\n";
    spicefile += config;
    spicefile += ".end";
    return spicefile;
}

void SPICERunner::SendComponent(std::string *instance, std::string subcircuit, std::string model) {
    // Recieves component data
    instances.push_back(instance);
    if (subcircuit.size()) subcircuits.insert(subcircuit);
    if (model.size()) models.insert(model);
}

void SPICERunner::ConfigSim(double duration, double step, std::string plot) {
    config = "\n.control\n";
    config += "tran " + uc(step) + " " + uc(duration) + "\n";
    if (plot.size()) config+= "hardcopy plot.ps " + plot;
    config+= "\n.endc\n";
}

void SPICERunner::Run(std::string output) {
    std::string s;
    std::ofstream out("input.cir");
    out << output;
    out.close();

	int code = std::system("ngspice < input.cir 2> /dev/null");

    if (code == 0) {
        std::ifstream in("output");
        while (std::getline(in, s)) {
            std::cout << s << '\n';
        }
    }
}


// ...


//
// public: /// virtual methods
//

std::string SPICERunner::show() {
	return ModelDataDefinition::show();
}



//
// public: /// static methods that must have implementations (Load and New just the same. GetInformation must provide specific infos for the new component
//


PluginInformation* SPICERunner::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<SPICERunner>(), &SPICERunner::LoadInstance, &SPICERunner::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelDataDefinition* SPICERunner::LoadInstance(Model* model, PersistenceRecord *fields) {
	SPICERunner* newElement = new SPICERunner(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

ModelDataDefinition* SPICERunner::NewInstance(Model* model, std::string name) {
	return new SPICERunner(model, name);
}


//
// protected: /// virtual method that must be overriden
//

bool SPICERunner::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelDataDefinition::_loadInstance(fields);
	if (res) {
		try {
			this->_someString = fields->loadField("someString", DEFAULT.someString);
			this->_someUint = fields->loadField("someUint", DEFAULT.someUint);
		} catch (...) {
		}
	}
	return res;
}

void SPICERunner::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelDataDefinition::_saveInstance(fields, saveDefaultValues);
	fields->saveField("someUint", _someUint, DEFAULT.someUint);
	fields->saveField("someString", _someString, DEFAULT.someString);
}


