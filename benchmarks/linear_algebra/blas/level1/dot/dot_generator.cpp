#include <isl/set.h>
#include <isl/union_map.h>
#include <isl/union_set.h>
#include <isl/ast_build.h>
#include <isl/schedule.h>
#include <isl/schedule_node.h>

#include <tiramisu/debug.h>
#include <tiramisu/core.h>
#include "benchmarks.h"

#include <string.h>
#include <Halide.h>

/* dot product between two vectors.

inputs:
--------
- y[]
- x[]

res[0] = 0;
for (i = 0; i < M; i++)
	res[0] = res[0] + y[i] * x[i];
*/

using namespace tiramisu;

int main(int argc, char **argv)
{
    // Set default tiramisu options.
    global::set_default_tiramisu_options();

    function dot("dot");

    constant M_CST("M", expr(SIZE), p_int32, true, NULL, 0, &dot);

#define B0 4

    // Inputs
    computation x("[M]->{x[j]: 0<=j<M}", tiramisu::expr(), false, p_float64, &dot);
    computation y("[M]->{y[j]: 0<=j<M}", tiramisu::expr(), false, p_float64, &dot);

    tiramisu::var j("j");
    computation res_init("[M]->{res_init[0]}",    tiramisu::expr((double) 0), true, p_float64, &dot);
    computation      mul("[M]->{mul[j]: 0<=j<M}", tiramisu::expr(x(j)*y(j)),    true, p_float64, &dot);
    computation      res("[M]->{res[j]: 0<=j<M}", tiramisu::expr(),    true, p_float64, &dot);

    res.set_expression(res(j) + mul(j));

    dot.set_context_set("[M]->{: M>0 and M%"+std::to_string(B0)+"=0}");

    // -----------------------------------------------------------------
    // Layer II
    // ----------------------------------------------------------------- 

    // Split (prepare for vectorization and split)
    mul.split(0, B0);
    res.split(0, B0);

    // Vectorization and unrolling
    mul.tag_vector_level(1, B0);
    res.tag_unroll_level(1);

    // Ordering
    mul.after_low_level(res_init,0);
    res.after_low_level(mul, 0);

    // ---------------------------------------------------------------------------------
    // Layer III
    // ---------------------------------------------------------------------------------
    buffer b_x("b_x", {tiramisu::expr(SIZE)}, p_float64, a_input, &dot);
    buffer b_y("b_y", {tiramisu::expr(SIZE)}, p_float64, a_input, &dot);
    buffer b_res("b_res", {tiramisu::expr((int) 0)}, p_float64, a_output, &dot);
    buffer b_mul("b_mul", {tiramisu::expr((int) B0)}, p_float64, a_temporary, &dot);

    x.set_access("{x[j]->b_x[j]}");
    y.set_access("{y[j]->b_y[j]}");
    res_init.set_access("{res_init[j]->b_res[0]}");
    mul.set_access("{mul[j]->b_mul[j%"+std::to_string(B0)+"]}");
    res.set_access("{res[j]->b_res[0]}");

    // ------------------------------------------------------------------
    // Generate code
    // ------------------------------------------------------------------
    dot.set_arguments({&b_x, &b_y, &b_res});
    dot.gen_time_space_domain();
    dot.gen_isl_ast();
    dot.gen_halide_stmt();
    dot.gen_halide_obj("generated_dot.o");

    return 0;
}