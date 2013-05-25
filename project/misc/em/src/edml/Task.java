package edml;

import il2.inf.JointEngine;
import il2.inf.PartialDerivativeEngine;
import il2.model.BayesianNetwork;
import il2.model.Domain;
import il2.util.IntMap;
import il2.util.IntSet;

/**
 * This class maintains a learning task for EM and EDML
 *
 * TODO:
 * - list of CPTs to learn (AC)
 */

public class Task {

    // network
    BayesianNetwork seed; // also used for structure
    int[] cptsToLearn;

    // data
    IntMap[] data;
    int[] counts;
    boolean useUnique;    // use compressed representation of data

    // prior
    Prior prior;

    // logging
    Logger logger;

    public Task(BayesianNetwork seed, IntMap[] data) {
        this(seed,data,null);
    }

    public Task(BayesianNetwork seed, IntMap[] data, int[] counts) {
        // Dirichlet prior with exponent 2, for all parameter set
        this(seed,data,counts,new Prior(seed.cpts(),2));
    }

    public Task(BayesianNetwork seed, 
                IntMap[] data, int[] counts, 
                Prior prior) {
        this.seed = seed;

        this.data = data;
        this.counts = counts;
        this.useUnique = (counts != null);

        this.prior = prior;
        this.logger = new Logger();
    }

    void initializeCptsToLearn() {
        cptsToLearn = new int[seed.size()];
        for (int i = 0; i < seed.size(); i++)
            cptsToLearn[i] = i;
    }

    public JointEngine startEngine(Options opts) {
        JointEngine ie;
        double maxClusterSize;
        if ( opts.pruneNetwork ) {
	    throw new UnsupportedOperationException();
        } else {
            ie = EmUtil.startInferenceEngine(seed.cpts());
            maxClusterSize = ie.getClusterStats().getNormalizedMax();
        }
        if (false) System.out.println("AC: " + maxClusterSize);
        return ie;
    }

    public PartialDerivativeEngine startPdEngine(Options opts) {
        return (PartialDerivativeEngine)startEngine(opts);
    }
    
    public Domain getDomain() { return seed.domain(); }
    public BayesianNetwork getSeed() { return seed; }
    public IntMap[] getData() { return data; }
    public int[] getCounts() { return counts; }
    public boolean getUseUnique() { return useUnique; }
    public Prior getPrior() { return prior; }
    public Logger getLogger() { return logger; }
    public int[] getCptsToLearn() { return cptsToLearn; }

    public void setCptsToLearn(int[] opt) { this.cptsToLearn = opt; }
    public void setLogger(java.io.PrintWriter writer, String name) {
        logger.setLogger(writer,name);
    }
}
