sizemem 300000000
ff
setquantum 1000000000
srecl vpr.sr
--run "vpr net.in arch.in sim.placed.out sim.routed.out -nodisp -place_only -init_t 5 -exit_t 0.005 -alpha_t 0.9412 -inner_num 2"
run "vpr smred.net small.arch.in pip.sim.placed.out pip.sim.routed.out -nodisp -place_only -init_t 5 -exit_t 0.005 -alpha_t 0.9412 -inner_num 2"
on
