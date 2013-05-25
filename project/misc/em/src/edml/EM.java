package edml;

import il2.inf.JointEngine;
import il2.model.BayesianNetwork;
import il2.model.Domain;
import il2.model.Table;
import il2.util.IntMap;

public class EM {
    Options opts;  // EM algorithm options
    Task task;     // learning task (network, data, prior)
    Monitor monitor;

    double logmap;

    // EM statistics
    double residual;
    double map_residual;
    int iterations;
    double learn_time;

    public EM(Options opts, Task task) {
        this.opts = opts;
        this.task = task;
        this.monitor = new Monitor(opts);
    }

    /**
     * main loop
     */
    public Table[] em() {
        JointEngine ie = task.startEngine(opts);
        Table[] expectation = null;
        double lastmap = Double.NEGATIVE_INFINITY;
        residual = Double.POSITIVE_INFINITY;

        task.logger.startLogger(task);
        iterations = 0;
        while ( iterations < opts.maxIters && residual > opts.threshold ) {
            expectation = emLoop(ie);
            task.logger.logData(logmap,iterations);
            lastmap = checkMap(lastmap,logmap);
            iterations++;
        }
        //task.logger.logData(expectation,iterations);
        learn_time = task.logger.stopLogger()*1e-6;

        monitor.iterations = iterations; // AC
        return expectation;
    }

    Table[] emLoop(JointEngine ie) {
        Domain d = task.getDomain();
        Table[] cpts = EmUtil.engineTables(ie);
        logmap = EmUtil.logPrior(cpts,task.prior);
        Table[] expectation = emptyTables(task.seed,task.prior);

        for (int di = 0; di < task.data.length; di++) {
            IntMap example = task.data[di];
            int count = task.useUnique ? task.counts[di] : 1;

            ie.setEvidence(example); // this invalidates engine
            double pe = ie.prEvidence();
            assert( pe > 0.0 ) : "P(e) = 0.0";
            logmap += count * Math.log(pe);

            for (int var = 0; var < d.size(); var++) {
                Table prxu = ie.tableConditional(var);
                accumulate(expectation[var],prxu,count);
            }
        }

        residual = 0.0;
        for (int var = 0; var < d.size(); var++) {
            Table t = expectation[var].makeCPT(var);
            setTable(ie,var,t);
            expectation[var] = t;
        }
        return expectation;
    }

    /**
     * sanity check EM updates:
     * updates should monotonically increase the posterior
     */
    double checkMap(double lastmap, double curmap) {
        double map_residual = Double.POSITIVE_INFINITY;
        if ( ! Double.isInfinite(lastmap) )
            map_residual = (curmap-lastmap)/Math.abs(lastmap);
        if ( map_residual < 0.0 && -map_residual > 1e-12 ) {
            System.err.println("cur map :" + curmap);
            System.err.println("prev map:" + lastmap);
            System.err.println("residual:" + map_residual);
            String errMsg = "EM Assertion failed : email Arthur";
            throw new IllegalStateException(errMsg);
        }
        return curmap;
    }

    protected void setTable(JointEngine ie, int var, Table t) {
        Table[] cpts = EmUtil.engineTables(ie);
        Table cpt = cpts[var];
        double[] new_vals = t.values();
        double[] old_vals = cpt.values();
        for (int i = 0; i < new_vals.length; i++) {
            double diff = Math.abs(new_vals[i] - old_vals[i]);
            if ( diff > residual ) residual = diff;
        }
        ie.setTable(var,t);
    }

    /**
     * Table t1 += t2
     */
    static void accumulate(Table t1, Table t2) {
        double[] v1 = t1.values();
        double[] v2 = t2.values();
        for (int i = 0; i < v1.length; i++)
            v1[i] += v2[i];
    }

    /**
     * Table t1 += c*t2
     */
    static void accumulate(Table t1, Table t2, int c) {
        double[] v1 = t1.values();
        double[] v2 = t2.values();
        for (int i = 0; i < v1.length; i++)
            v1[i] += c*v2[i];
    }

    static Table[] emptyTables(BayesianNetwork bn, Prior prior) {
        Table[] cpts = bn.cpts();
        Table[] empty = new Table[cpts.length];
        for (int var = 0; var < cpts.length; var++) {
            double[] vals = prior.getTablePsis(var).clone();
            for (int i = 0; i < vals.length; i++) {
                vals[i] -= 1;
            }
            empty[var] = new Table(cpts[var],vals);
        }
        return empty;
    }

}
