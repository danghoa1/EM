package edml;

public class Monitor {
    Options opts;

    // EM/EDML statistics
    double residual;
    int iterations;
    double learn_time;

    double last_log_map;
    double curr_log_map;

    // optimization statistics
    double opt_residual;
    int opt_calls;
    int opt_max_iters;
    int opt_evals;
    int opt_sum_iters;

    // other stats
    double sanity_check;

    long start_time;

    public Monitor(Options opts) {
        this.opts = opts;
        reset();
    }

    /**
     * clear all recorded statistics
     */
    public void reset() {
        // EM/EDML statistics
        this.residual = Double.POSITIVE_INFINITY;
        this.iterations = 0;
        this.learn_time = 0.0;

        this.last_log_map = Double.NEGATIVE_INFINITY;
        this.curr_log_map = Double.NEGATIVE_INFINITY;

        // optimization statistics
        this.opt_residual = 0.0;
        this.opt_calls = 0;
        this.opt_max_iters = 0;
        this.opt_evals = 0;
        this.opt_sum_iters = 0;

        this.start_time = System.nanoTime();
    }

    public void resetResidual() {
        this.residual = 0.0;
    }
    public void updateResidual(double res) {
        if (res > residual) residual = res;
    }
    public void incrIterations() {
        iterations++;
    }
    public void updateLogMap(double logmap) {
        last_log_map = curr_log_map;
        curr_log_map = logmap;
    }

    public void sanityCheck(double error) {
        sanity_check = error;
    }

    public void localUpdate(double residual, int iterations) {
        if (residual > opt_residual) opt_residual = residual;
        opt_calls++;
        if (iterations > opt_max_iters) opt_max_iters = iterations;
        opt_sum_iters += iterations;
    }

    public boolean hasConverged() {
        if ( iterations == 0 ) return false;

        boolean iteration_limit_reached = iterations >= opts.maxIters;

        boolean conv = false;
        switch (opts.convMode) {
        case CHANGE_IN_PARAM:
            conv = (residual <= opts.threshold);
            break;
        case CHANGE_IN_MAP:
            double delta = curr_log_map - last_log_map;
            if ( delta < 0 ) delta = -delta;
            conv = (delta <= opts.threshold);
            break;
        case MAP_LIMIT:
            conv = (curr_log_map >= opts.mapLimit);
            break;
        default:
            conv = true;
            break;
        }
        return iteration_limit_reached || conv;
    }

    public void stop() {
        this.learn_time = (System.nanoTime()-start_time)*1e-6;
    }

}
