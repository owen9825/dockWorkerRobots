#include "domain.h"

//check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//necessary for debugger to display the line of the memory leak
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

//include these for Unicode
#include <io.h>
#include <fcntl.h>

#include <iostream>
#include <string.h>

int main(int argc, char** argv) {
	//define initial state
	PredicateLayer si;
	si.addToLayer(Predicate(predType::Adjacent,"l1","l2"));
	si.addToLayer(Predicate(predType::Adjacent,"l2","l1"));

	si.addToLayer(Predicate(predType::Attached,"p1","l1"));
	si.addToLayer(Predicate(predType::Attached,"q1","l1"));
	si.addToLayer(Predicate(predType::Attached,"p2","l2"));
	si.addToLayer(Predicate(predType::Attached,"q2","l2"));

	si.addToLayer(Predicate(predType::Belong,"k1","l1"));
	si.addToLayer(Predicate(predType::Belong,"k2","l2"));

	si.addToLayer(Predicate(predType::In,"ca","p1"));
	si.addToLayer(Predicate(predType::In,"cb","p1"));
	si.addToLayer(Predicate(predType::In,"cc","p1"));

	si.addToLayer(Predicate(predType::In,"cd","q1"));
	si.addToLayer(Predicate(predType::In,"ce","q1"));
	si.addToLayer(Predicate(predType::In,"cf","q1"));

	si.addToLayer(Predicate(predType::On,"ca","pallet"));
	si.addToLayer(Predicate(predType::On,"cb","ca"));
	si.addToLayer(Predicate(predType::On,"cc","cb"));

	si.addToLayer(Predicate(predType::On,"cd","pallet"));
	si.addToLayer(Predicate(predType::On,"ce","cd"));
	si.addToLayer(Predicate(predType::On,"cf","ce"));

	si.addToLayer(Predicate(predType::Top,"cc","p1"));
	si.addToLayer(Predicate(predType::Top,"cf","q1"));
	si.addToLayer(Predicate(predType::Top,"pallet","p2"));
	si.addToLayer(Predicate(predType::Top,"pallet","q2"));

	si.addToLayer(Predicate(predType::At,"r1","l1"));
	si.addToLayer(Predicate(predType::Unloaded,"r1"));
	si.addToLayer(Predicate(predType::Free,"l2"));

	si.addToLayer(Predicate(predType::Empty,"k1"));
	si.addToLayer(Predicate(predType::Empty,"k2"));

	std::vector<Predicate> goalSet;
	goalSet.push_back(Predicate(predType::In,"cb","q2"));
	goalSet.push_back(Predicate(predType::In,"cc","p2"));
	goalSet.push_back(Predicate(predType::In,"cd","q2"));
	goalSet.push_back(Predicate(predType::In,"ce","q2"));
	goalSet.push_back(Predicate(predType::In,"cf","q2"));

	GraphPlanner captain(si,goalSet);
	std::vector<Action> resultantPlan = captain.plan();
	return EXIT_SUCCESS;
}