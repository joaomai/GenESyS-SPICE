/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SPICECircuit.h
 * Author: bnmfw & joaomai
 *
 * Created on 10 de outubro de 2023, 23:00
 */

#pragma once

#include "../../kernel/simulator/ModelComponent.h"
#include "../data/SPICERunner.h"
#include "SPICENode.h"

/*!
 This component ...
 */
class SPICECircuit : public ModelComponent {
public: /// constructors
	SPICECircuit(Model* model, std::string name = "");
	virtual ~SPICECircuit() = default;

public: /// new public user methods for this components
	void init(SPICERunner* the_compiler, std::string description, unsigned int id, std::vector<std::string> the_params = {""}, std::string the_model = "", std::string the_model_file = "");
    void insertAtRank(int pin, SPICENode* node);
	void setRunner(SPICERunner* runner);
	SPICERunner* getRunner();

public: /// virtual public methods
	virtual std::string show();

public: /// static public methods that must have implementations (Load and New just the same. GetInformation must provide specific infos for the new component
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, PersistenceRecord *fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name = "");

protected: /// virtual protected method that must be overriden
	virtual bool _loadInstance(PersistenceRecord *fields);
	virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues);
	virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber); ///< This method is only for ModelComponents, not ModelDataElements

protected: /// virtual protected methods that could be overriden by derived classes, if needed
	/*! This method is called by ModelChecker during model check. The component should check itself to verify if user parameters are ok (ex: correct syntax for the parser) and everithing in its parameters allow the model too run without errors in this component */
	// virtual bool _check(std::string* errorMessage);
	/*! This method returns all changes in the parser that are needed by plugins of this ModelDatas. When connecting a new plugin, ParserChangesInformation are used to change parser source code, whch is after compiled and dinamically linked to to simulator kernel to reflect the changes */
	// virtual ParserChangesInformation* _getParserChangesInformation();
	/*! This method is called by ModelSimulation when initianting the replication. The model should set all value for a new replication (Ex: setting back to 0 any internal counter, clearing lists, etc. */
	// virtual void _initBetweenReplications();
	/*! This method is called by ModelChecker and is necessary only for those components that instantiate internal elements that must exist before simulation starts and even before model checking. That's the case of components that have internal StatisticsCollectors, since others components may refer to them as expressions (as in "TVAG(ThisCSTAT)") and therefore the modeldatum must exist before checking such expression */
	// virtual void _createInternalAndAttachedData(); /*< A ModelDataDefinition or ModelComponent that includes (internal) ou refers to (attach) other ModelDataDefinition must register them inside this method. */
	/*! This method is not used yet. It should be usefull for new UIs */
	// virtual void _addProperty(PropertyBase* property);

private: /// new private user methods
	// Spice subcircuit declaration, if one exists
    std::string spice_template;
    // Name of component
    std::string spice_name;
    // Params of the component, such as resistance or voltage
    std::vector<std::string> params;
    // Pin map of the component
    std::vector<std::string> pins;
    // Instace of the component in spice
    std::string *spice_instance;
    // Model of the component, if one exists (the model name should also be the name of the .cir file)
    std::string model;
    std::string model_file;
    // Spice compiler
    SPICERunner *compiler;

	static unsigned int counter;
    bool plain_circuit; 

protected:
    unsigned int id;
       

private: /// Attributes that should be loaded or saved with this component (Persistent Fields)

	/// Default values for the attributes. Used on initing, loading and saving
	const struct DEFAULT_VALUES {
		const std::string someString = "Test";
		const unsigned int someUint = 1;
	} DEFAULT;
	std::string _someString = DEFAULT.someString;
	unsigned int _someUint = DEFAULT.someUint;

private: /// internal DataElements (Composition)

private: /// attached DataElements (Agrregation)
	// ...
};

class Resistor : public SPICECircuit {
    public:
	Resistor(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, float resistance) {
		id = counter++;
		SPICECircuit::init(runner, "R a b", id, {uc(resistance)});
	}
	static PluginInformation* GetPluginInformation();	
    private:
    static unsigned int counter;
};

class Vsource : public SPICECircuit {
    public:
	Vsource(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, double vcc) {
		id = counter++;
		std::string s = uc(vcc);
		SPICECircuit::init(runner, "Vd a b", id, {s});
	}
	static PluginInformation* GetPluginInformation();
    private:
    static unsigned int counter;
};

class Vpulse : public SPICECircuit {
    public:
	Vpulse(Model* model, std::string name = ""): SPICECircuit(model, name) {

	}

	void init(SPICERunner* runner, double vcc, double freq, double slope = 1e-11) {
		id = counter++;
		SPICECircuit::init(runner, "Vp a b", id, {"PULSE (0 "+uc(vcc)+uc(freq/2)+uc(slope)+uc(slope)+uc(freq/2)+uc(freq)+")"});
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class Vsine : public SPICECircuit {
    public:
	Vsine(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, double vcc, double freq) {
		id = counter++;
		SPICECircuit::init(runner, "Vs a b", id, {"sin(0 "+uc(vcc)+uc(freq)+")"});
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class Capacitor : public SPICECircuit {
    public:
	Capacitor(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, float capacitance) {
		id = counter++;
		SPICECircuit::init(runner, "C a b", id, {uc(capacitance)});
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class Diode : public SPICECircuit {
    public:
	Diode(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "diode") {
		id = counter++;
		SPICECircuit::init(runner, "D a b", id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class PMOS: public SPICECircuit {
    public:
	PMOS(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, double width = 140e-9, double lenght = 32e-9, std::string model = "32nmCMOS") {
		id = counter++;
		SPICECircuit::init(runner, "Mp source gate drain bulk", id, {"w="+uc(width),"l="+uc(lenght)}, "pmos"+model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class NMOS: public SPICECircuit {
    public:
	NMOS(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, double width = 70e-9, double lenght = 32e-9, std::string model = "32nmCMOS") {
		id = counter++;
		SPICECircuit::init(runner, "Mn source gate drain bulk", id, {"w="+uc(width),"l="+uc(lenght)}, "nmos"+model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class NOT : public SPICECircuit {
    public:
	NOT(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			".subckt NOT vp a s\n"
			 "Mp1 vp a s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 s a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class NAND : public SPICECircuit {
    public:
	NAND(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			".subckt NAND vp vm a b s\n"
			 "Mp1 vp a s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp2 vp b s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 s a i1 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn2 i1 b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class AND : public SPICECircuit {
    public:
	AND(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			 ".subckt AND vp vm a b s\n"
			 "Mp1 vp a ns vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp2 vp b ns vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 ns a i1 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn2 i1 b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mp3 vp ns s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn3 s ns vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class NOR : public SPICECircuit {
    public:
	NOR(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			 ".subckt NOR vp vm a b s\n"
			 "Mp1 vp a i1 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp2 i1 b s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 s a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn2 s b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class OR : public SPICECircuit {
    public:
	OR(Model* model, std::string name = ""): SPICECircuit(model, name)  {

    }

	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			 ".subckt OR vp vm a b s\n"
			 "Mp1 vp a i1 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp2 i1 b ns vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 ns a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn2 ns b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mp3 vp ns s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn3 s ns vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class XOR : public SPICECircuit {
    public:
	XOR(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			 ".subckt XOR vp vm a b s\n"
			 "Mp1 vp a na vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 na a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 "Mp2 vp b nb vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn2 nb b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 "Mp3 vp na i1 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp4 i1 b s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp5 vp a i2 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp6 i2 nb s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn3 s nb i3 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn4 i3 na vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn5 s b i4 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn6 i4 a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};

class XNOR : public SPICECircuit {
    public:
	XNOR(Model* model, std::string name = ""): SPICECircuit(model, name) {

    }
	void init(SPICERunner* runner, std::string model = "32nmCMOS", double PMOSwidth = 140e-9, double PMOSlenght = 32e-9, double NMOSwidth = 70e-9, double NMOSlenght = 32e-9) {
		id = counter++;
		SPICECircuit::init(runner,
			 ".subckt XNOR vp vm a b s\n"
			 "Mp1 vp a na vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn1 na a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 "Mp2 vp b nb vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn2 nb b vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 "Mp3 vp na i1 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp4 i1 b ns vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp5 vp a i2 vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mp6 i2 nb ns vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn3 ns nb i3 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn4 i3 na vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn5 ns b i4 vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"
			 "Mn6 i4 a vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 "Mp7 vp ns s vp PMOS"+model+" w="+uc(PMOSwidth)+" l="+uc(PMOSlenght)+"\n"
			 "Mn7 s ns vm vm NMOS"+model+" w="+uc(NMOSwidth)+" l="+uc(NMOSlenght)+"\n"

			 ".ends\n",
			 id, {}, model, model);
	}
	static PluginInformation* GetPluginInformation();

    private:
    static unsigned int counter;
};


