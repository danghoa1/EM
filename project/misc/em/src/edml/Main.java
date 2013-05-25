package edml;

import il2.inf.JointEngine;
import il2.model.BayesianNetwork;
import il2.model.Domain;
import il2.model.Table;
import il2.util.IntMap;

public class Main {
    public static double logLikelihood(Table[] cpts, IntMap[] data) {
        JointEngine ie = EmUtil.startInferenceEngine(cpts);
        double ll = 0.0;
        for (int di = 0; di < data.length; di++) {
            ie.setEvidence(data[di]);
            ll += Math.log(ie.prEvidence());
        }
        return ll;
    }

    public static double logLikelihood(Table[] cpts, 
                                       IntMap[] data, int[] counts) {
        JointEngine ie = EmUtil.startInferenceEngine(cpts);
        double ll = 0.0;
        for (int di = 0; di < data.length; di++) {
            ie.setEvidence(data[di]);
            ll += counts[di]*Math.log(ie.prEvidence());
        }
        return ll;
    }

    public static double logPrior(Table[] cpts, Prior prior) {
        if (prior == null) return 0.0;
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

	public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("usage: main UAI-NETWORK CSV-DATASET\n");
        }

        String network_filename = args[0];
        String dataset_filename = args[1];

        // load Bayesian netework (seed for EM)
        Data gen = new Data(network_filename);
        BayesianNetwork bn = gen.getBayesianNetwork();

        // load dataset
        int var_count = bn.domain().size();
        int[][] data = UaiConverter.datToArrays(dataset_filename,var_count);

        // find unique data examples
        /*
        Data.UniqueData udata = Data.dataToUniqueData(data);
        IntMap[] evid = Data.dataToEvidence(udata.data);
        int[] counts = udata.counts;
        */
        IntMap[] evid = Data.dataToEvidence(data);
        int[] counts = null;

        //Prior prior = new Prior(seed,2); // laplace smoothing
        Prior prior = new Prior(bn.cpts(),1);   // no prior
        //Prior prior = null;

        Options opts = new Options();
        Task task = new Task(bn,evid,counts,prior);
        EM em = new EM(opts,task);
        Table[] ecpts = em.em();
	
	for (int i=0; i < var_count; i++)
		System.out.println(ecpts[i]);

        double score = Main.logLikelihood(ecpts,evid) + Main.logPrior(ecpts, prior);
        //double score = Main.logLikelihood(ecpts,evid,counts) + Main.logPrior(ecpts, prior);
        System.out.printf("%f\n",score);

	
	System.out.println("Iterations : " + em.iterations);
	System.out.println("Time : " + em.learn_time);
	}
}
