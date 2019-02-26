// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "rhopi_pipigg/rhopi_pipigg.h"
	#include "VertexFit/KalmanKinematicFit.h"
	#include "VertexFit/VertexFit.h"
	#include <string>
	#include <utility>

	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif

	using CLHEP::Hep2Vector;
	using CLHEP::Hep3Vector;
	using CLHEP::HepLorentzVector;
	using namespace TSGlobals;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/// Constructor for the `rhopi_pipigg` algorithm, derived from `TrackSelector`.
	/// Here, you should declare properties: give them a name, assign a parameter (data member of `rhopi_pipigg`), and if required a documentation string. Note that you should define the paramters themselves in the header (rhopi_pipigg/rhopi_pipigg.h) and that you should assign the values in `share/jopOptions_rhopi_pipigg.txt`.
	rhopi_pipigg::rhopi_pipigg(const std::string &name, ISvcLocator* pSvcLocator) :
		/// * Construct base algorithm `TrackSelector`.
			TrackSelector(name, pSvcLocator),
		/// * Construct `NTuple::Tuple` containers used in derived classes.
			fNTuple_dedx_pi("dedx_pi", "dE/dx of the pions"),
			fNTuple_fit4c  ("fit4c",   "4-constraint fit information (CMS 4-momentum)"),
			fNTuple_fit5c  ("fit5c",   "5-constraint fit information (CMS 4-momentum + pi0 resonance)"),
			fNTuple_fit_mc ("fit_mc",  "Fake fit information according to MC truth"),
			fNTuple_photon ("photon",  "Kinematics of selected photons"),
		/// * Construct `CutObject`s. The `"cut_<parameter>_min/max"` properties determine cuts on certain parameters.
			fCut_GammaPhi  ("gamma_phi"),
			fCut_GammaTheta("gamma_theta"),
			fCut_GammaAngle("gamma_angle")
	{ PrintFunctionName("rhopi_pipigg", __func__); PostConstructor();
	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //


	/// (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	/// Define and load NTuples here.
	StatusCode rhopi_pipigg::initialize_rest()
	{ PrintFunctionName("rhopi_pipigg", __func__);
		/// <ol type="A">
		/// <li> `"mult_select"`: Multiplicities of selected particles
			/// <ol>
			if(fNTuple_mult_sel.DoWrite()) {
				fNTuple_mult_sel["NPhotons"]; /// <li> `"NPhotons"`: Number of \f$\gamma\f$'s.
				fNTuple_mult_sel["NPionNeg"]; /// <li> `"NPionNeg"`: Number of \f$\pi^-\f$.
				fNTuple_mult_sel["NPionPos"]; /// <li> `"NPionPos"`: Number of \f$\pi^+\f$.
			}
			/// </ol>

		/// <li> `"dedx_pi"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::AddNTupleItems_Dedx` for more info.
			AddNTupleItems_Dedx(fNTuple_dedx_pi);

		/// <li> `"fit4c"` and `"fit5c"`: results of the Kalman kinematic fit results. See `TrackSelector::AddNTupleItems_Fit` for more info.
			AddNTupleItems_Fit(fNTuple_fit4c);
			AddNTupleItems_Fit(fNTuple_fit5c);

		/// <li> `"photon"`: information of the selected photons

			/// <ol>
			if(fNTuple_photon.DoWrite()) {
				fNTuple_photon.AddItem("E");     /// <li> `"E"`:     Energy of the photon.
				fNTuple_photon.AddItem("phi");   /// <li> `"phi"`:   Smallest angle between the photon and the nearest charged pion.
				fNTuple_photon.AddItem("theta"); /// <li> `"theta"`: Smallest angle between the photon and the nearest charged pion.
				fNTuple_photon.AddItem("angle"); /// <li> `"angle"`: Smallest angle between the photon and the nearest charged pion.
			}
			/// </ol>

		/// </ol>
		return StatusCode::SUCCESS;
	}


	/// Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	StatusCode rhopi_pipigg::execute_rest()
	{ PrintFunctionName("rhopi_pipigg", __func__);
		/// <ol type="A">
		/// <li> Create selection charged tracks

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting particle selection:" << endmsg;
				fPIDInstance = ParticleID::instance();

			// * Clear vectors of selected particles *
				fPionNeg.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {
				/// <ol>
				/// <li> Initialise PID and skip if it fails:
					/// <ul>
					if(!InitializePID(
						/// <li> use <b>probability method</b>
						fPIDInstance->methodProbability(),
						/// <li> use \f$dE/dx\f$ and the three ToF detectors. Since BOSS 7.0.4, `ParticleID::useTofCorr()` should be used for ToF instead of e.g. `useTof1`.
						fPIDInstance->useDedx() |
						fPIDInstance->useTofCorr(),
						/// <li> identify only pions
						fPIDInstance->onlyPion(),
						/// <li> use \f$\chi^2 > 4.0\f$
						4.0
					)) continue;
					/// </ul>

				/// <li> @b Write Particle Identification information of all tracks
					WritePIDInformation();

				/// <li> Identify type of charged particle and add to related vector: (this package: only pions).
					fTrackKal = (*fTrackIterator)->mdcKalTrack();
					if(fCut_PIDProb.FailsMin(fPIDInstance->probPion())) continue; /// A cut is then applied on whether the probability to be a pion (or kaon) is at least `fCut_PIDProb_min` (see eventual settings in `rhopi_pipigg.txt`).
					RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); /// Finally, the particle ID of the `RecMdcKalTrack` object is set to pion
					if(fTrackKal->charge()>0) fPionPos.push_back(*fTrackIterator); /// and the pions (\f$\pm\f$) are added to the respective vector of pions.
					else                      fPionNeg.push_back(*fTrackIterator);

				/// </ol>
			}


		/// <li> Create selection neutral tracks (photons)

			// * Print log and clear vectors of selected particles *
			fLog << MSG::DEBUG << "Starting 'good' neutral track selection:" << endmsg;

			// * Loop over charged tracks *
			fPhotons.clear();
			for(fTrackIterator = fGoodNeutralTracks.begin(); fTrackIterator != fGoodNeutralTracks.end(); ++fTrackIterator) {

				/// <ol>
				/// <li> Get EM calorimeter info
					fTrackEMC = (*fTrackIterator)->emcShower();
					Hep3Vector emcpos = fTrackEMC->position();

				/// <li> Find angle differences with nearest charged pion
					fSmallestTheta = 1000.; // start value for difference in theta
					fSmallestPhi   = 1000.; // start value for difference in phi
					fSmallestAngle = 1000.; // start value for difference in angle (?)
					SetSmallestAngles(fPionNegIter, fPionNeg, emcpos); // check all differences with the pions
					SetSmallestAngles(fPionPosIter, fPionPos, emcpos); // check all differences with the pions

				/// <li> Apply angle cut
					fSmallestTheta = fSmallestTheta * 180 / (CLHEP::pi);
					fSmallestPhi   = fSmallestPhi   * 180 / (CLHEP::pi);
					fSmallestAngle = fSmallestAngle * 180 / (CLHEP::pi);

				/// <li> @b Write photon info (`"photon"` branch)
					if(fNTuple_photon.DoWrite()) {
						fNTuple_photon.at("E")     = fTrackEMC->energy();
						fNTuple_photon.at("phi")   = fSmallestTheta;
						fNTuple_photon.at("theta") = fSmallestPhi;
						fNTuple_photon.at("angle") = fSmallestAngle;
						fNTuple_photon.Write();
					}

				/// <li> Apply photon cuts (energy cut has already been applied in TrackSelector)
					if(
						fCut_GammaTheta.FailsMax(fabs(fSmallestTheta)) &&
						fCut_GammaPhi  .FailsMax(fabs(fSmallestPhi))) continue;
					if(fCut_GammaAngle.FailsMax(fabs(fSmallestAngle))) continue;

				/// <li> Add photon track to vector for gammas
					fPhotons.push_back(*fTrackIterator);

				/// </ol>
			}


		/// <li> Create selection of MC truth particles by looping over the collection of MC particles created in `TrackSelector::execute()`. See <a href="http://home.fnal.gov/~mrenna/lutp0613man2/node44.html">here</a> for a list of PDG codes.
			if(fEventHeader->runNumber()<0 && fNTuple_fit_mc.DoWrite()) {
				fMcPhotons.clear();
				fMcPionNeg.clear();
				fMcPionPos.clear();
				std::vector<Event::McParticle*>::iterator it;
				for(it=fMcParticles.begin(); it!=fMcParticles.end(); ++it) {
					switch((*it)->particleProperty()) {
						case   22 : fMcPhotons.push_back(*it); break;
						case -211 : fMcPionNeg.push_back(*it); break;
						case  211 : fMcPionPos.push_back(*it); break;
						default : fLog << MSG::DEBUG << "No switch case defined for McParticle " << (*it)->particleProperty() << endmsg;
					}
				}
			}


		/// <li> @b Write the multiplicities of the selected particles.
			fLog << MSG::DEBUG
				<< "N_\gamma = "  << fPhotons.size() << ", "
				<< "N_{\pi^-} = " << fPionNeg.size() << ", "
				<< "N_{\pi^+} = " << fPionPos.size() << endmsg;
			if(fNTuple_mult_sel.DoWrite()) {
				fNTuple_mult_sel.at("NPhotons") = fPhotons.size();
				fNTuple_mult_sel.at("NPionNeg") = fPionNeg.size();
				fNTuple_mult_sel.at("NPionPos") = fPionPos.size();
				fNTuple_mult_sel.Write();
			}


		/// <li> Apply a cut on the number of particles: <i>only events with one \f$\pi^-\f$, one \f$\pi^+\f$, and at least 2 photons</i>
			if(fPhotons.size()  < 2) return StatusCode::SUCCESS;
			if(fPionNeg.size() != 1) return StatusCode::SUCCESS;
			if(fPionPos.size() != 1) return StatusCode::SUCCESS;


		/// <li> @b Write \f$dE/dx\f$ PID information (`"dedx_*"` branchs)
			if(fNTuple_dedx.DoWrite()) {
				WriteDedxInfoForVector(fPionNeg, fNTuple_dedx_pi);
				WriteDedxInfoForVector(fPionPos, fNTuple_dedx_pi);
			}


		/// <li> Loop over MC truth of final decay products.
			for(fMcPhoton1Iter = fMcPhotons.begin(); fMcPhoton1Iter != fMcPhotons.end(); ++fMcPhoton1Iter)
			for(fMcPhoton2Iter = fMcPhotons.begin(); fMcPhoton2Iter != fMcPhotons.end(); ++fMcPhoton2Iter)
			for(fMcPionNegIter = fMcPionNeg.begin(); fMcPionNegIter != fMcPionNeg.end(); ++fMcPionNegIter)
			for(fMcPionPosIter = fMcPionPos.begin(); fMcPionPosIter != fMcPionPos.end(); ++fMcPionPosIter)
			{
				/// <ol>
				/// <li> Only continue if the two photons are different.
					if(fMcPhoton1Iter == fMcPhoton2Iter) continue;

				/// <li> Check topology: only consider that combination which comes from \f$J/\psi \rightarrow D^0\phi \rightarrow K^-\pi^+ K^-K^+\f$.
					if((!(*fMcPhoton1Iter)->primaryParticle()) && (*fMcPhoton1Iter)->mother().particleProperty() != 333) continue; // photon 1
					if((!(*fMcPhoton2Iter)->primaryParticle()) && (*fMcPhoton2Iter)->mother().particleProperty() != 421) continue; // photon 2
					if((!(*fMcPionNegIter)->primaryParticle()) && (*fMcPionNegIter)->mother().particleProperty() != 333) continue; // negative pion
					if((!(*fMcPionPosIter)->primaryParticle()) && (*fMcPionPosIter)->mother().particleProperty() != 421) continue; // positive pion

				/// <li> Write 'fake' fit results, that is, momenta of the particles reconstructed from MC truth.
					KKFitResult_rhopi_pipigg fitresult(
						*fMcPhoton1Iter,
						*fMcPhoton2Iter,
						*fMcPionNegIter,
						*fMcPionPosIter
					);
					WriteFitResults(&fitresult, fNTuple_fit_mc);
				/// </ol>
			}


		/// <li> Perform Kalman 4-constraint Kalman kinematic fit for all combinations
			if(fNTuple_fit4c.DoWrite()) {
				// * Reset best fit parameters * //
				KKFitResult_rhopi_pipigg bestKalmanFit;
				bestKalmanFit.fBestCompareValue = 1e9;
				// * Loop over all combinations * //
				for(fPhoton1Iter = fPhotons.begin(); fPhoton1Iter != fPhotons.end(); ++fPhoton1Iter)
				for(fPhoton2Iter = fPhoton1Iter+1;   fPhoton2Iter != fPhotons.end(); ++fPhoton2Iter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				{
					// * Get Kalman tracks
						RecMdcKalTrack *pimTrk = (*fPionNegIter)->mdcKalTrack();
						RecMdcKalTrack *pipTrk = (*fPionPosIter)->mdcKalTrack();

					// * Get W-tracks
						WTrackParameter wvpimTrk(gM_pi, pimTrk->getZHelix(), pimTrk->getZError());
						WTrackParameter wvpipTrk(gM_pi, pipTrk->getZHelix(), pipTrk->getZError());

					// * Initiate vertex fit * //
						HepPoint3D vx(0., 0., 0.);
						HepSymMatrix Evx(3, 0);
						double bx = 1E+6;
						double by = 1E+6;
						double bz = 1E+6;
						Evx[0][0] = bx*bx;
						Evx[1][1] = by*by;
						Evx[2][2] = bz*bz;
						VertexParameter vxpar;
						vxpar.setVx(vx);
						vxpar.setEvx(Evx);

					// * Test vertex fit * //
						VertexFit* vtxfit = VertexFit::instance();
						vtxfit->init();
						vtxfit->AddTrack(0, wvpimTrk);
						vtxfit->AddTrack(1, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) continue;
						vtxfit->Swim(0);

					// * Get Kalman kinematic fit for this combination and store if better than previous one
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, vtxfit->wtrk(0)); // pi-
						kkmfit->AddTrack(1, vtxfit->wtrk(1)); // pi+
						kkmfit->AddTrack(2, 0., (*fPhoton1Iter)->emcShower()); // gamma (1st occurrence)
						kkmfit->AddTrack(3, 0., (*fPhoton2Iter)->emcShower()); // gamma (2nd occurence)
						kkmfit->AddFourMomentum(0, gEcmsVec); // 4 constraints: CMS energy and 3-momentum
						if(kkmfit->Fit()) {
							/// <ol>
							/// <li> Apply max. \f$\chi^2\f$ cut (determined by `fCut_PIDChiSq_max`).
							if(fCut_PIDChiSq.FailsMax(kkmfit->chisq())) continue;
							/// <li> Construct fit result object for this combintation.
							KKFitResult_rhopi_pipigg fitresult(kkmfit);
							/// <li> @b Write results of the Kalman kinematic fit.
							WriteFitResults(&fitresult, fNTuple_fit4c);
							/// </ol>
						}
				}
			}


		/// <li> Perform Kalman 5-constraint Kalman kinematic fit for all combinations.
			if(fNTuple_fit5c.DoWrite()) {
				// * Reset best fit parameters * //
				double bestFitMeasure = 1e5;
				KalmanKinematicFit *bestKalmanFit = nullptr;
				// * Loop over all combinations of pi-, pi+, and gamma * //
				for(fPhoton1Iter = fPhotons.begin(); fPhoton1Iter != fPhotons.end(); ++fPhoton1Iter)
				for(fPhoton2Iter = fPhoton1Iter+1;   fPhoton2Iter != fPhotons.end(); ++fPhoton2Iter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				{
					// * Get Kalman tracks
						RecMdcKalTrack *pimTrk = (*fPionNegIter)->mdcKalTrack();
						RecMdcKalTrack *pipTrk = (*fPionPosIter)->mdcKalTrack();

					// * Get W-tracks
						WTrackParameter wvpimTrk(gM_pi, pimTrk->getZHelix(), pimTrk->getZError());
						WTrackParameter wvpipTrk(gM_pi, pipTrk->getZHelix(), pipTrk->getZError());

					// * Initiate vertex fit * //
						HepPoint3D vx(0., 0., 0.);
						HepSymMatrix Evx(3, 0);
						double bx = 1E+6;
						double by = 1E+6;
						double bz = 1E+6;
						Evx[0][0] = bx*bx;
						Evx[1][1] = by*by;
						Evx[2][2] = bz*bz;
						VertexParameter vxpar;
						vxpar.setVx(vx);
						vxpar.setEvx(Evx);

					// * Test vertex fit * //
						VertexFit* vtxfit = VertexFit::instance();
						vtxfit->init();
						vtxfit->AddTrack(0, wvpimTrk);
						vtxfit->AddTrack(1, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) continue;
						vtxfit->Swim(0);

					// * Get Kalman kinematic fit for this combination and store if better than previous one
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, vtxfit->wtrk(0)); // pi-
						kkmfit->AddTrack(1, vtxfit->wtrk(1)); // pi+
						kkmfit->AddTrack(2, 0., (*fPhoton1Iter)->emcShower()); // gamma (1st occurrence)
						kkmfit->AddTrack(3, 0., (*fPhoton2Iter)->emcShower()); // gamma (2nd occurence)
						kkmfit->AddResonance(0, gM_pi0, 2, 3); /// @remark 5th constraint: \f$\pi^0\f$ resonance
						kkmfit->AddFourMomentum(1, gEcmsVec); // 4 constraints: CMS energy and momentum
						if(kkmfit->Fit()) {
							/// <ol>
							/// <li> Apply max. \f$\chi^2\f$ cut (determined by `fCut_PIDChiSq_max`).
							if(fCut_PIDChiSq.FailsMax(kkmfit->chisq())) continue;
							/// <li> Construct fit result object for this combintation.
							KKFitResult_rhopi_pipigg fitresult(kkmfit);
							/// <li> @b Write results of the Kalman kinematic fit.
							WriteFitResults(&fitresult, fNTuple_fit5c);
							/// </ol>
						}
				}
			}


		/// </ol>
		return StatusCode::SUCCESS;
	}


	/// Currently does nothing. See `TrackSelector::finalize` for what else is done when finalising.
	StatusCode rhopi_pipigg::finalize_rest()
	{ PrintFunctionName("rhopi_pipigg", __func__);
		return StatusCode::SUCCESS;
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //


	/// Specification of what should be written to the fit `NTuple`. This function is called in `TrackSelector::WriteFitResults`.
	void rhopi_pipigg::SetFitNTuple(KKFitResult *fitresults, NTupleContainer &tuple)
	{
		/// -# Convert to the derived object of `KKFitResult` designed for this package. @remark This cast is required and cannot be solved using virtual methods, because of the specific structure of each analysis.
			KKFitResult_rhopi_pipigg* fit = dynamic_cast<KKFitResult_rhopi_pipigg*>(fitresults);

		/// -# Set the `NTuple::Item`s.
			tuple.at("mpi0")       = fit->fM_pi0;
			tuple.at("mrho0")      = fit->fM_rho0;
			tuple.at("mrho-")      = fit->fM_rhom;
			tuple.at("mrho+")      = fit->fM_rhop;
			tuple.at("mJpsi_rho0") = fit->fM_JpsiRho0;
			tuple.at("mJpsi_rho-") = fit->fM_JpsiRhom;
			tuple.at("mJpsi_rho+") = fit->fM_JpsiRhop;
			tuple.at("chisq")      = fit->fChiSquared;
	}


	/// This function encapsulates the `addItem` procedure for the fit branches.
	void rhopi_pipigg::AddNTupleItems_Fit(NTupleContainer &tuple)
	{
		if(!tuple) return;
		tuple.AddItem("mpi0");       /// * `"mpi0"`:       Invariant mass for \f$\pi^0 \rightarrow \gamma\gamma\f$ candidate.
		tuple.AddItem("mrho0");      /// * `"mrho0"`:      Invariant mass for \f$\rho^0 \rightarrow \pi^-\pi^+\f$ candidate.
		tuple.AddItem("mrho-");      /// * `"mrho-`":      Invariant mass for \f$\rho^- \rightarrow \pi^0\pi^-\f$ candidate.
		tuple.AddItem("mrho+");      /// * `"mrho+`":      Invariant mass for \f$\rho^+ \rightarrow \pi^0\pi^+\f$ candidate.
		tuple.AddItem("mJpsi_rho0"); /// * `"mJpsi_rho0"`: Invariant mass for \f$J/\psi \rightarrow \rho^0\pi^0\f$ candidate.
		tuple.AddItem("mJpsi_rho-"); /// * `"mJpsi_rho-`": Invariant mass for \f$J/\psi \rightarrow \rho^-\pi^+\f$ candidate.
		tuple.AddItem("mJpsi_rho+"); /// * `"mJpsi_rho+`": Invariant mass for \f$J/\psi \rightarrow \rho^+\pi^-\f$ candidate.
		tuple.AddItem("chisq");      /// * `"chisq"`:      \f$\chi^2\f$ of the Kalman kinematic fit.
	}


	/// Encapsulation of the procedure to write results of the Kalman kinematic fit (no matter how many constrains).
	HepLorentzVector rhopi_pipigg::ComputeGammaVector(EvtRecTrack* track)
	{
		fTrackEMC = track->emcShower();
		double eraw  = fTrackEMC->energy();
		double phi   = fTrackEMC->phi();
		double theta = fTrackEMC->theta();
		HepLorentzVector four_mom(
			eraw * sin(theta) * cos(phi),
			eraw * sin(theta) * sin(phi),
			eraw * cos(theta),
			eraw);
		// four_mom = four_mom.boost(-0.011, 0, 0); /// If necessary: Perform a boost to the CMS
		return four_mom;
	}


	/// Helper function that encapsulates the proces of finding the angle between some photon and the pions.
	void rhopi_pipigg::SetSmallestAngles(std::vector<EvtRecTrack*>::iterator &iter, std::vector<EvtRecTrack*> &vec, Hep3Vector &emcpos)
	{
		for(iter = vec.begin(); iter != vec.end(); ++iter) {
			/// * Get the extension object from MDC to EMC
			if(!(*iter)->isExtTrackValid()) continue;
			fTrackExt = (*iter)->extTrack();
			if(fTrackExt->emcVolumeNumber() == -1) continue;
			Hep3Vector extpos(fTrackExt->emcPosition());

			/// * Get angles
			// double cosTheta = extpos.cosTheta(emcpos);
			double angle  = extpos.angle(emcpos);
			double dTheta = extpos.theta() - emcpos.theta();
			double dPhi   = extpos.deltaPhi(emcpos);
			dTheta = fmod(dTheta + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
			dPhi   = fmod(dPhi   + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
			if(angle < fSmallestAngle){
				fSmallestAngle = angle;
				fSmallestTheta = dTheta;
				fSmallestPhi   = dPhi;
			}
		}
	}