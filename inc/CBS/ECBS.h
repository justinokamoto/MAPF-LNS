#pragma once
#include "CBS.h"
#include "ECBSNode.h"


class ECBS : public CBS
{
public:
	ECBS(const Instance& instance, bool sipp, int screen) : CBS(instance, sipp, screen) {}
    ECBS(vector<SingleAgentSolver*>& search_engines,
         const PathTable& path_table,
         int screen) : CBS(search_engines, path_table, screen) {}
    ~ECBS();
	////////////////////////////////////////////////////////////////////////////////////////////
	// Runs the algorithm until the problem is solved or time is exhausted 
	bool solve(double time_limit, int cost_lowerbound = 0);

	ECBSNode* getGoalNode() { return goal_node; }
    void updatePaths(ECBSNode* curr);
	void clear();
	void printPaths() const;
private:
    //ECBSNode* dummy_start = nullptr;
    ECBSNode* goal_node = nullptr;

	vector<int> min_f_vals; // lower bounds of the cost of the shortest path
	vector< pair<Path, int> > paths_found_initially;  // contain initial paths found

	pairing_heap< ECBSNode*, compare<ECBSNode::compare_node_by_f> > cleanup_list; // it is called open list in ECBS
	pairing_heap< ECBSNode*, compare<ECBSNode::compare_node_by_inadmissible_f> > open_list; // this is used for EES
	pairing_heap< ECBSNode*, compare<ECBSNode::compare_node_by_d> > focal_list; // this is ued for both ECBS and EES

	void adoptBypass(ECBSNode* curr, ECBSNode* child, const vector<int>& fmin_copy);

	// node operators
	void pushNode(ECBSNode* node);
	ECBSNode* selectNode();
	bool reinsertNode(ECBSNode* node);
    void releaseNodes();

	 // high level search
	bool generateChild(ECBSNode* child, ECBSNode* curr);
	bool generateRoot();
	bool findPathForSingleAgent(ECBSNode*  node, int ag);
	void classifyConflicts(ECBSNode &node);
	void computeConflictPriority(shared_ptr<Conflict>& con, ECBSNode& node);
};