package edml;

/**
 * This class maintains learning options for EM and EDML
 */

public class Options {
    public enum EdmlMode { PSETALT, PSET, CPT } // versions A/B/C
    public enum ConvMode { CHANGE_IN_PARAM, CHANGE_IN_MAP, MAP_LIMIT }

    // global algorithm parameters (EM/EDML)
    int maxIters;
    double threshold;
    double damp; // (EDML only)
    double mapLimit;

    // local algorithm parameters (EDML)
    int localMaxIters;
    double localThreshold;
    boolean filterSoftEvidence;

    // inference options
    boolean pruneNetwork;

    EdmlMode edmlMode;
    ConvMode convMode;

    // default options
    public Options() {
        this.maxIters = 128;
        this.threshold = 1e-4;
        this.damp = 0.5;
        this.mapLimit = Double.NEGATIVE_INFINITY;
        this.localMaxIters = 256;
        this.localThreshold = 1e-6;
        this.filterSoftEvidence = true;
        this.pruneNetwork = false;
        this.edmlMode = EdmlMode.PSET;
        this.convMode = ConvMode.CHANGE_IN_PARAM;
    }

    // default options
    public Options(Options opts) {
        this.maxIters           = opts.maxIters;
        this.threshold          = opts.threshold;
        this.damp               = opts.damp;
        this.mapLimit           = opts.mapLimit;
        this.localMaxIters      = opts.localMaxIters;
        this.localThreshold     = opts.localThreshold;
        this.filterSoftEvidence = opts.filterSoftEvidence;
        this.pruneNetwork       = opts.pruneNetwork;
        this.edmlMode           = opts.edmlMode;
        this.convMode           = opts.convMode;
    }

    public void setMaxIters(int opt) { maxIters = opt; }
    public void setThreshold(double opt) { threshold = opt; }
    public void setDamp(double opt) { damp = opt; }
    public void setLocalMaxIters(int opt) { localMaxIters = opt; }
    public void setLocalThreshold(double opt) { localThreshold = opt; }
    public void setFilterSE(boolean opt) { filterSoftEvidence = opt; }
    public void setPruneNetwork(boolean opt) { pruneNetwork = opt; }
    public void setEdmlMode(EdmlMode opt) { edmlMode = opt; }
    public void setConvMode(ConvMode opt) { convMode = opt; }

    public void setMapLimit(double opt) {
        mapLimit = opt;
        convMode = ConvMode.MAP_LIMIT;
    }
}
