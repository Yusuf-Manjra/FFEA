Overview {#overview}
=========

Fluctuating Finite Element Analysis is a new molecular modelling technique, built from the ground-up to support systems that are larger and more complex than those modelled by atomistic molecular dynamics. Instead of modelling biological systems as a collection of connected atoms, it models them as 3D volumes comprised of tetrahedrons. Unlike previous coarse-grained models, the models FFEA generates are viso-elastic continuum solids. Unlike other applications of Finite Element Analysis, these systems are subject to thermal fluctuations.

This technique has the potential to model large, complex systems, made of many molecules, and complex processes at the frontiers of molecular biology. As it does not not require an atomistic level of detail, it can also be used to simulate biological molecules that cannot be imaged using X-ray crystallography.


Features  {#whatever}
========

 * Protein Interactions:
  * A 6-12 [Lennard-Jones Potential](\ref ljPotential)
  * A repulsive potential that is proportional 
        to the [volume overlap](\ref sPotential).
  * Specific interactions defined using precomputed potentials.
        More documentation can be found [here](\ref fmApproach).
  * Coulombic interactions [EXPERIMENTAL].
 * [Kinetic state changes](\ref kineticApproach) can be simulated together with the continuum model to
    account for conformational changes and binding events.
 * Conversion of EM density data and atomistic structures into FFEA simulations
 * PyMOL visualisation plugin
 * Analysis tools (equilibration, euler characteristic, principal component analysis, gemoetric measurements) and Python API
 * Extensive test suite that checks FFEA's simulation output against analytical results



Videos
======

[video will go here]


Publications  {#publications}
============

   * Methodology
       * Oliver R., Read D. J., Harlen O. G. & Harris S. A. ["A Stochastic finite element model for the dynamics of globular macromolecules"](http://www.sciencedirect.com/science/article/pii/S0021999112007589) (2013) J. Comp. Phys. 239, 147-165.
       * Patargias G. N., Harris S. A. & Harding J. ["A demonstration of the inhomogeneity of the local dielectric response of proteins by molecular dynamics simulations."](https://www.ncbi.nlm.nih.gov/pubmed/20572740) (2010) J. Chem. Phys. 132, 235103.
   * Applications
       * Richardson R., Papachristos K., Read D. J., Harlen O. G., Harrison M. A., Paci E., Muench S. P. & Harris S. A ["Understanding the apparent stator-rotor connections in the rotary ATPase family using coarse-grained computer modelling"](https://www.ncbi.nlm.nih.gov/pubmed/25174610) (2014), Proteins: Struct., Funct., Bioinf., 82, 3298-3311.
   * Reviews
       * Gray A., Harlen O. G., Harris S. A., Khalid S., Leung Y. M., Lonsdale R., Mulholland A. J., Pearson A. R., Read D. J. & Richardson R. A. ["In pursuit of an accurate spatial and temporal model of biomolecules at the atomistic level: a perspective on computer simulation"](https://www.ncbi.nlm.nih.gov/pubmed/25615870), Acta Cryst. (2015) D71, 162-172.
       * Oliver R. , Richardson R. A., Hanson B., Kendrick K., Read D. J., Harlen O. G. & Harris S. A. ["Modelling the Dynamic Architecture of Biomaterials Using Continuum Mechanics"](http://link.springer.com/chapter/10.1007%2F978-3-319-09976-7_8), Protein Modelling, G. Náray-Szabó, Editor. (2014) Springer International Publishing. p. 175-197.
       * Hanson B., Richardson R., Oliver R., Read D. J., Harlen O. & Harris S. ["Modelling biomacromolecular assemblies with continuum mechanics"](https://www.ncbi.nlm.nih.gov/pubmed/25849915) Biochem. Soc. Trans. (2015), 43, 186-192.


Technology  {#technology}
============
 
   * [Boost](http://www.boost.org) (>=1.54.0).
   * [Eigen](http://eigen.tuxfamily.org) (>=3.2.1).   
     FFEA uses Eigen to calculate and solve linear approximations to the model i.e. Elastic / Dynamic Network Models.
   * [RngStreams](http://www.iro.umontreal.ca/~lecuyer/myftp/streams00/)<sup>[1](#RngStreams1)</sup><sup>,[2](#RngStreams2)</sup>
        is shipped with FFEA and used as Random Number Generator (RNG). RngStreams 
        allows the FFEA to **safely** generate random numbers when running 
        on a number of threads, as well as safe restarts, recovering the state 
        of the RNGs in the last saved time step.
   * [Doxygen](http://www.doxygen.org) (>= 1.8) [OPTIONAL]   
   * [PyMOL](https://www.pymol.org) (>=1.8) [OPTIONAL] can 
        be used, using the plugin we provide,
        to visualise FFEA systems and trajectories
        as well as molecular and EM systems. Alternatives 
        to visualise and work with molecular systems 
        include [Chimera](https://www.cgl.ucsf.edu/chimera/)
        and [VMD](http://www.ks.uiuc.edu/Research/vmd/).
   * [mtTkinter](http://tkinter.unpythonic.net/wiki/mtTkinter) v. 0.4 is shipped 
        with FFEA and used in the PyMOL plugin, allowing safe threading. 
   * [GTS](http://gts.sourceforge.net) (>=0.7.6)[OPTIONAL]. The
     GNU Triangulated Surface Libraries
     allowing the manipulation and coarsening of surface profiles.
   * [NETGEN](https://sourceforge.net/projects/netgen-mesher/) 
   or [TETGEN](http://wias-berlin.de/software/tetgen/) [OPTIONAL]. 
     Programs which convert surface profile into volumetric meshes 
        to be used by FFEA.
   * [pyPcazip](https://pypi.python.org/pypi/pyPcazip) [OPTIONAL]
     Some of the Python FFEA analysis tools interact with these 
     Principal Component Analysis library in order to generate the standard
     PCA output (eigensystems, projections, animations etc)
     obtained from standard from equivalent MD simulations.


<a name="RngStreams1">1</a>: P L'Ecuyer, "Good Parameter Sets for Combined Multiple Recursive Random Number Generators", (1999) Oper Res, 47, 1, 159-164 <br> 
<a name="RngStreams2">2</a>: P L'Ecuyer et al. "An Objected-Oriented Random-Number Package with Many Long Streams and Substreams", (2002) Oper Res, 50, 6, 1073-1075.


Contribute
==========

FFEA is maintained by a small but dedicated team at the University of Leeds. If you want to see where we can take FFEA, then you can:

   * Use the software for something cool
   * Send bug reports and feature requests to our [issue tracker](https://bitbucket.org/sohpc-ffea/ffea/issues)
   * [Fork us](https://bitbucket.org/sohpc-ffea/ffea/fork)


FFEA Team  {#FFEAteam}
==========

 * Code:
   * Albert Solernou
   * Ben Hanson
   * Robin Richardson
   * [Rob Welch](http://robwel.ch/)
 * Theory
   * [Oliver Harlen](https://www.maths.leeds.ac.uk/index.php?id=263&uid=1025)
   * [Sarah Harris](http://www.comp-bio.physics.leeds.ac.uk/)
   * Robin Oliver
   * [Daniel Read](http://www1.maths.leeds.ac.uk/~djread/)
   * Robin Richardson
   * Ben Hanson
   * Albert Solernou
 * Thanks
   * Stan Burgess
   * Stephan Muench



How to read this manual
=======================

Because the software is split between the FFEA runner (written in C++) 
  and the rest of FFEA tools (written mostly Python, with some C++ and C), 
  this manual is split also in these two sections: 
   [FFEA runner](\ref userManual) and [ffeatools](../../ffeamodules/html/index.html)
  to be used as reference pages.
However, the new user may want start reading the [tutorial](\ref Tutorial),
  and consult the reference pages later. 
