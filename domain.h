#include <exception>
#include <iostream>
#include <string>
#include <assert.h>
#include <map>
#include <vector>
#include <queue>

#define IDLENGTH 2

enum dwrType {LOCATION,PILE,ROBOT,CRANE,CONTAINER,NoDType};
enum predType {Adjacent,Attached,Belong,At,Free,Loaded,Unloaded,Holding,Empty,In,Top,On,NoPType};
enum actType {Move,Load,Unload,Take,Put,NoOp,NoAType};

class Action;

class Predicate {
private:
	const predType identity;
	dwrType p1type;
	std::string p1Name;
	dwrType p2type;
	std::string p2Name;
	//std::map<std::string,Action*> asPrecondition;
	std::vector<Action*> asPrecondition;
	//std::map<std::string,Action*> asEffect;
	std::vector<Action*> asEffect;
	mutable std::string keyVal;
public:
	predType getIdentity() const {
		return identity;
	}
	dwrType getTypeOne() const {
		return p1type;
	}
	dwrType getTypeTwo() const {
		return p2type;
	}
	std::string getNameOne() const {
		return p1Name;
	}
	void setNameOne(std::string s) {
		if(this->keyVal.length() > 0) {
			//bad time for changing things now that key has already been set
		} else {
			this->p1Name = s;
		}
	}
	std::string getNameTwo() const {
		return p2Name;
	}
	void setNameTwo(std::string s) {
		if(this->keyVal.length() > 0) {
			//bad time for changing things now that key has already been set
		} else {
			this->p2Name = s;
		}
	}
	Predicate(const predType id,std::string nom)
		: identity(id),p1Name(nom),p2type(dwrType::NoDType),p2Name(NULL)
	{
		switch(id) {
		case predType::Free:
			this->p1type = dwrType::LOCATION;
			break;
		case predType::Unloaded:
			this->p1type = dwrType::ROBOT;
		case predType::Empty:
			this->p1type = dwrType::CRANE;
			break;
		default:
			assert(false); //Predicate should have received a type!!
		}
	}
	Predicate(const predType id,std::string nom1,std::string nom2)
		: identity(id),p1Name(nom1),p2Name(nom2)
	{
		switch(id) {
		case predType::Adjacent:
			this->p1type = dwrType::LOCATION;
			this->p2type = dwrType::LOCATION;
			break;
		case predType::Attached:
			this->p1type = dwrType::PILE;
			this->p2type = dwrType::LOCATION;
			break;
		case predType::Belong:
			this->p1type = dwrType::CRANE;
			this->p2type = dwrType::LOCATION;
			break;
		case predType::At:
			this->p1type = dwrType::ROBOT;
			this->p2type = dwrType::LOCATION;
			break;
		case predType::Loaded:
			this->p1type = dwrType::ROBOT;
			this->p2type = dwrType::CRANE;
			break;
		case predType::Holding:
			this->p1type = dwrType::CRANE;
			this->p2type = dwrType::CONTAINER;
			break;
		case predType::In:
			this->p1type = dwrType::CONTAINER;
			this->p2type = dwrType::PILE;
			break;
		case predType::Top:
			this->p1type = dwrType::CONTAINER;
			this->p2type = dwrType::PILE;
			break;
		case predType::On:
			break;
			this->p1type = dwrType::CONTAINER;
			this->p2type = dwrType::CONTAINER;
		default:
			assert(false); //Predicate should always be supplied with a valid id
		}
	}
	void addResultant(Action* a) {
		this->asPrecondition.push_back(a);
		return;
	}
	void rememberParent(Action* a) {
		this->asEffect.push_back(a);
		return;
	}
	const std::string key(void) const;
	~Predicate() {}
};

class Action {
private:
	const actType identity;
	std::map<std::string,Predicate*> preLinks;
	std::map<std::string,Predicate*> posEffectLinks;
	//std::map<std::string,Predicate*> negEffectLinks;
	std::vector<Predicate> effects;
	std::queue<Predicate> emptyPreconditions;
	std::map<std::string,std::string> σ; //instantiation
	mutable std::string keyVal;

	void fillPredicates(); //fill effects and emptyPreconditions with dummy values
public:
	const std::string key(void) const;
	Action(const actType id) : identity(id) {
		assert(id != actType::NoAType);
	}
	Action(Predicate* staticPred) : identity(actType::NoOp) {
		preLinks[staticPred->key()] = staticPred;
		effects.push_back(Predicate(*staticPred)); //copy to output
	}
	Predicate nextMissingLink();
	bool submitPreLink(Predicate* pred);
	bool submitEffectLink(Predicate* pred);
	std::vector<Predicate> getEffects(bool onlyPositive=true); //replace the effect's names with real names, then return the list
	
	~Action() {}
};

class PredicateLayer {
private:
	std::map<std::string,Predicate*> predicates;
public:
	bool checkGoal(const std::vector<Predicate> &goalSet) const;
	bool checkμSet(const std::vector<Predicate> &goalSet) const;
	void addToLayer(const Predicate &pred);
	~PredicateLayer();
};

class ActionLayer {
private:
	std::map<std::string,Action*> actions;

public:
	ActionLayer(){}
	~ActionLayer();
};

class GraphPlanner {
private:
	std::vector<PredicateLayer> predLayers;
	std::vector<ActionLayer> actionLayers;
	const std::vector<Predicate> goalStates;
	std::vector<std::map<std::string,Predicate*>> noGood;

	std::vector<Predicate*> getOrphans(const ActionLayer &parent, ActionLayer &child);
	void applyActions(ActionLayer &actLayer,PredicateLayer &predLayer);
	void generateActions(ActionLayer &actLayer,PredicateLayer &predLayer);
	void expand();
public:
	GraphPlanner(PredicateLayer &initialState, const std::vector<Predicate> &goalSet);
	std::vector<Action> plan();
	~GraphPlanner();
};