package edml;

import il2.model.Domain;
import il2.model.Index;
import java.util.Arrays;

public class Prior {
    Domain d;
    double[][] cpt_psis;
    double network_psi;

    /*
    public Prior(Domain d) {
        this(d,null,2);
    }
    */

    /** 
     * domains is an array of CPT domains (can be an array of Table)
     * psi represents a Dirichlet prior for all network parameter sets
     */
    public Prior(Index[] domains, double psi) {
        this(domains[0].domain(),null,psi);
        cpt_psis = new double[d.size()][];
        for (int i = 0; i < cpt_psis.length; i++) {
            cpt_psis[i] = new double[domains[i].sizeInt()];
            Arrays.fill(cpt_psis[i],psi);
        }
    }

    public Prior(Domain d, double[][] psis) {
        this(d,psis,2);
    }

    Prior(Domain d, double[][] psis, double psi) {
        this.d = d;
        this.cpt_psis = psis;
        this.network_psi = psi;
    }

    /**
     * returns the Dirichlet prior for variable v and parent
     * instantiation index uindex
     */
    double[] getPsis(int var, int uindex) {
        double[] psis;
        int size = d.size(var);
        if ( cpt_psis == null ) {
            psis = new double[size];
            Arrays.fill(psis,network_psi);
        } else {
            psis = EmUtil.cptColumn(cpt_psis[var],size,uindex);
        }
        return psis;
    }

    double[] getTablePsis(int var) {
        return cpt_psis[var];
    }
}
