//package test;

/** Import il1 classes. */
import edu.ucla.belief.*;
import edu.ucla.belief.inference.*;
import edu.ucla.belief.io.PropertySuperintendent;
import edu.ucla.belief.io.NetworkIO;

/** Import classes for loopy belief propagation. */
import edu.ucla.belief.approx.PropagationEngineGenerator;
import edu.ucla.belief.approx.BeliefPropagationSettings;
import edu.ucla.belief.approx.PropagationInferenceEngineImpl;
import java.math.BigInteger;


import edu.ucla.belief.BeliefNetwork;
import edu.ucla.belief.FiniteVariable;
import edu.ucla.belief.InferenceEngine;
import edu.ucla.belief.inference.*;

import il2.inf.JointEngine;
import il2.inf.PartialDerivativeEngine;
import il2.inf.structure.EliminationOrders;
import il2.model.BayesianNetwork;
import il2.util.IntList;
import il2.util.IntMap;

import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.*;

public class Test {
    BeliefNetwork bn1;    // edu.ucla.belief library
    BayesianNetwork bn2;  // il2 library

	public static void main(String[] args) {
		if ( args.length != 1 ) { 
			System.out.println("Usage: program .uai file");
			return;
		}
        String filename = args[0];

        BeliefNetwork bn1 = UaiConverter.uaiToBeliefNetwork(filename);
        BayesianNetwork bn2 = UaiConverter.uaiToBayesianNetwork(filename);
        Test test = new Test(bn1,bn2);
        test.run();
	}

    /************************************************************
     * CONSTRUCTOR
     ************************************************************/

	public Test(BeliefNetwork bn1, BayesianNetwork bn2) {
        this.bn1 = bn1;
        this.bn2 = bn2;
	}

    /************************************************************
     * TESTING STUFF
     ************************************************************/

    public void run() {
        InferenceEngine ie1 = startInferenceEngine1(bn1,bn2); //AC: need2fix
        JointEngine ie2 = startInferenceEngine2(bn2);

	//Add evidence
	IntMap evidence = new IntMap();
	evidence.putAtEnd(1,1);
	evidence.putAtEnd(2,1);
	ie2.setEvidence(evidence);


        System.out.printf("=== il1 inference engine ===\n");
        System.out.printf("Pr(e) = %.2f\n",ie1.probability());
        for (int i = 0; i < bn1.size(); i++) {
            FiniteVariable var = (FiniteVariable)bn1.forID("x"+i);
            edu.ucla.belief.Table t1 = ie1.familyConditional(var);
            System.out.println(t1);
            System.out.println();
        }
        System.out.println();

        System.out.printf("=== il2 inference engine ===\n");
        System.out.printf("Pr(e) = %.2f\n",ie2.prEvidence());
        for (int i = 0; i < bn2.domain().size(); i++) {
            il2.model.Table t2 = ie2.tableConditional(i);
            System.out.println(t2);
        }
        
    }

    /************************************************************
     * INFERENCE ENGINE STUFF
     ************************************************************/

    /**
     * get elimination order
     */
    static EliminationOrders.Record 
        getEliminationOrder(Collection<il2.model.Table> domains) {

        java.util.Random r = new java.util.Random(0);
        EliminationOrders.Record r_mf, r_ms, record;

        r_mf = EliminationOrders.minFill(domains,6,r);
        if (true) return r_mf; // AC: need to re-evalute the following
        r_ms = EliminationOrders.minSize(domains);

        int size = r_mf.order.size();
        IntList order = new IntList(size);
        for (int i = 0; i < size; i++) order.add(i);
        record = EliminationOrders.createRecord(domains,order);

        // AC: really want to test total size, not max size
        if ( record.size > r_mf.size ) record = r_mf;
        if ( record.size > r_ms.size ) record = r_ms;
        return record;
    }

    /**
     * get a jointree
     */
    static EliminationOrders.JT getJoinTree(BayesianNetwork bn) {
        il2.model.Table[] cpts = bn.cpts();
        Collection<il2.model.Table> sd = Arrays.asList(cpts);
        EliminationOrders.Record record = getEliminationOrder(sd);
        
        EliminationOrders.JT jt;
        if (true) // this JT construction has quadratic space complexity
            jt = EliminationOrders.traditionalJoinTree(sd,record.order);
        else      // this JT construction has linear space complexity
            jt = EliminationOrders.bucketerJoinTree(sd,record.order);
        return jt;
    }

    /**
     * starts il1 inference engine (Hugin)
     */
    public static InferenceEngine startInferenceEngine1(BeliefNetwork bn,
                                                        BayesianNetwork bn2) {
		edu.ucla.belief.inference.HuginEngineGenerator dynamator = 
			new edu.ucla.belief.inference.HuginEngineGenerator();
        edu.ucla.belief.inference.JoinTreeSettings settings = 
            dynamator.getSettings((edu.ucla.belief.io.PropertySuperintendent)bn,
                                  true);
        //il2.bridge.Converter converter = new il2.bridge.Converter();
        //bn2 = converter.convert(bn);
        EliminationOrders.JT jt = getJoinTree(bn2);
        settings.setJoinTree(jt.asJoinTreeIL1());
        InferenceEngine engine = dynamator.manufactureInferenceEngine(bn);

	/* Define evidence, by id. */
    	Map evidence = new HashMap(2);
   	FiniteVariable var = null;
    	var = (FiniteVariable) bn.forID( "x1" );
    	evidence.put( var, var.instance( "s1" ) );
    	var = (FiniteVariable) bn.forID( "x2" );
    	evidence.put( var, var.instance( "s1" ) );
	
	/* Set evidence. */
    	try{
      		bn.getEvidenceController().setObservations( evidence );
    	}catch( StateNotFoundException e ){
      		System.err.println( "Error, failed to set evidence: " + e );
    	};


        return engine;
    }

    /**
     * starts il2 inference engine (Hugin)
     */
    public static JointEngine startInferenceEngine2(BayesianNetwork bn) {
        il2.model.Table[] cpts = bn.cpts();
        EliminationOrders.JT jt = getJoinTree(bn);
        JointEngine ie;
        //ie = il2.inf.jointree.NormalizedZCAlgorithm.create(cpts,jt);
        //ie = il2.inf.jointree.UnindexedZCAlgorithm.create(cpts,jt);
        //ie = il2.inf.jointree.UnindexedSSAlgorithm.create(cpts,jt);
        ie = il2.inf.jointree.UnindexedHuginAlgorithm.create(cpts,jt);

        double max = ie.getClusterStats().getNormalizedMax();
		double total = ie.getClusterStats().getTotal().doubleValue();
		total = Math.log(total)/Math.log(2);

        //System.out.printf("MAX: %.2f\n",max);
        //System.out.printf("TOT: %.2f\n",total);
        return ie;
    }
}
