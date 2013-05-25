package edml;

import il2.inf.JointEngine;
import il2.inf.PartialDerivativeEngine;
import il2.inf.structure.EliminationOrders;
import il2.model.BayesianNetwork;
import il2.model.Domain;
import il2.model.Table;
import il2.util.IntMap;
import il2.util.IntSet;

import java.util.Arrays;
import java.util.Random;

public class EmUtil {

    public static Table[] randomNetwork(BayesianNetwork bn, Random r) {
        Table[] cpts = bn.cpts();
        Table[] rand = new Table[cpts.length];
        for (int x = 0; x < cpts.length; x++) {
            Table cpt = cpts[x];
            double[] vals = new double[cpt.values().length];
            for (int i = 0; i < vals.length; i++)
                vals[i] = r.nextDouble();
            rand[x] = new Table(cpt,vals);
            rand[x] = rand[x].makeCPT(x);
        }
        return rand;
    }

    public static Table[] randomUndirectedNetwork(BayesianNetwork bn,
                                                  Random r) {
        Table[] cpts = bn.cpts();
        Table[] rand = new Table[cpts.length];
        for (int ti = 0; ti < cpts.length; ti++) {
            Table cpt = cpts[ti];
            double[] vals = new double[cpt.values().length];
            for (int i = 0; i < vals.length; i++)
                vals[i] = r.nextDouble();
            rand[ti] = new Table(cpt,vals);
        }
        return rand;
    }

    public static Table[] uniformNetwork(BayesianNetwork bn) {
        Table[] cpts = bn.cpts();
        Table[] unif = new Table[cpts.length];
        for (int x = 0; x < cpts.length; x++) {
            Table cpt = cpts[x];
            double[] vals = new double[cpt.values().length];
            Arrays.fill(vals,1.0);
            unif[x] = new Table(cpt,vals);
            unif[x] = unif[x].makeCPT(x);
        }
        return unif;
    }

    public static Table[] uniformUndirectedNetwork(BayesianNetwork bn) {
        Table[] cpts = bn.cpts();
        Table[] unif = new Table[cpts.length];
        for (int ti = 0; ti < cpts.length; ti++) {
            Table cpt = cpts[ti];
            double[] vals = new double[cpt.values().length];
            Arrays.fill(vals,1.0);
            unif[ti] = new Table(cpt,vals);
        }
        return unif;
    }

    static int familyChild(IntSet vars) {
        int size = vars.size();
        return vars.get(size-1);
    }

    static double[] cptColumn(double[] vals, int size, int uindex) {
        int usize = vals.length/size;

        double[] xvals = new double[size];
        for (int state = 0; state < size; state++)
            xvals[state] = vals[state*usize + uindex];
        return xvals;
    }

    static double[] cptColumn(Table cpt, int uindex) {
        int var = familyChild(cpt.vars());
        int size = cpt.domain().size(var);

        double[] vals = cpt.values();
        int usize = vals.length/size;

        double[] xvals = new double[size];
        for (int state = 0; state < size; state++)
            xvals[state] = vals[state*usize + uindex];
        return xvals;
    }

    static void setCptColumn(Table cpt, int uindex, double[] xvals) {
        int var = familyChild(cpt.vars());
        int size = cpt.domain().size(var);

        double[] vals = cpt.values();
        int usize = vals.length/size;

        for (int state = 0; state < size; state++)
            vals[state*usize + uindex] = xvals[state];
    }

    /** log likelihood computation **/

    static JointEngine startInferenceEngine(Table[] cpts) {
        java.util.Collection<Table> sd = java.util.Arrays.asList(cpts);
        Random r = new Random(0);
        EliminationOrders.Record record = EliminationOrders.minFill(sd,6,r);
        // this JT construction has quadratic space complexity
        EliminationOrders.JT jt = 
            EliminationOrders.traditionalJoinTree(sd,record.order);
        // this JT construction has linear space complexity
        //EliminationOrders.JT jt = 
        //    EliminationOrders.bucketerJoinTree(sd,record.order);
        //ie = il2.inf.jointree.NormalizedZCAlgorithm.create(cpts,jt);
        return il2.inf.jointree.UnindexedZCAlgorithm.create(cpts,jt);
    }

    static PartialDerivativeEngine startPartialEngine(Table[] cpts) {
        return (PartialDerivativeEngine)startInferenceEngine(cpts);
    }

    static Table[] engineTables(JointEngine ie) { //AC: make this cleaner
        if (ie instanceof il2.inf.jointree.JoinTreeAlgorithm)
            return ((il2.inf.jointree.JoinTreeAlgorithm)ie).getOriginalTables();
        else return null;
    }

    public static double logPrior(Table[] cpts, Prior prior) {
        Domain d = cpts[0].domain();
        double logp = 0.0;
        for (int var = 0; var < d.size(); var++) {
            Table cpt = cpts[var];
            int usize = cpt.values().length/d.size(var);
            for (int uindex = 0; uindex < usize; uindex++) {
                double[] pr = EmUtil.cptColumn(cpt,uindex);
                double[] psis = prior.getPsis(var,uindex);
                for (int x = 0; x < pr.length; x++) {
                    if ( psis[x] == 1.0 ) continue;
                    logp += (psis[x]-1) * Math.log(pr[x]);
                }
            }
        }
        return logp;
    }

}