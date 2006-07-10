#ifndef MCF_lp_hpp
#define MCF_lp_hpp

#include "OsiSolverInterface.hpp"
#include "BCP_buffer.hpp"
#include "BCP_lp_user.hpp"
#include "BCP_parameters.hpp"
#include "MCF_par.hpp"
#include "MCF_var.hpp"
#include "MCF_data.hpp"

class MCF_lp : public BCP_lp_user
{
    OsiSolverInterface* cg_lp;
    BCP_parameter_set<MCF_par> par;
    MCF_data data;
    // the solution to the original formulation
    std::map<int,double>* flows;
    bool generated_vars;

public:
    MCF_lp() {}
    virtual ~MCF_lp() {
	delete[] flows;
	delete cg_lp;
    }

    virtual void unpack_module_data(BCP_buffer& buf);
    virtual void pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf) {
	MCF_pack_var(var, buf);
    }
    virtual BCP_var_algo* unpack_var_algo(BCP_buffer& buf) {
	return MCF_unpack_var(buf);
    }

    virtual OsiSolverInterface* initialize_solver_interface();

    virtual void
    initialize_new_search_tree_node(const BCP_vec<BCP_var*>& vars,
				    const BCP_vec<BCP_cut*>& cuts,
				    const BCP_vec<BCP_obj_status>& var_status,
				    const BCP_vec<BCP_obj_status>& cut_status,
				    BCP_vec<int>& var_changed_pos,
				    BCP_vec<double>& var_new_bd,
				    BCP_vec<int>& cut_changed_pos,
				    BCP_vec<double>& cut_new_bd);
    virtual void
    process_lp_result(const BCP_lp_result& lpres,
		      const BCP_vec<BCP_var*>& vars,
		      const BCP_vec<BCP_cut*>& cuts,
		      const double old_lower_bound,
		      double& true_lower_bound,
		      BCP_solution*& sol,
		      BCP_vec<BCP_cut*>& new_cuts,
		      BCP_vec<BCP_row*>& new_rows,
		      BCP_vec<BCP_var*>& new_vars,
		      BCP_vec<BCP_col*>& new_cols);
    virtual void
    vars_to_cols(const BCP_vec<BCP_cut*>& cuts,
		 BCP_vec<BCP_var*>& vars,
		 BCP_vec<BCP_col*>& cols,
		 const BCP_lp_result& lpres,
		 BCP_object_origin origin, bool allow_multiple);
    virtual BCP_branching_decision
    select_branching_candidates(const BCP_lp_result& lpres,
				const BCP_vec<BCP_var*>& vars,
				const BCP_vec<BCP_cut*>& cuts,
				const BCP_lp_var_pool& local_var_pool,
				const BCP_lp_cut_pool& local_cut_pool,
				BCP_vec<BCP_lp_branching_object*>& cands);
};

#endif