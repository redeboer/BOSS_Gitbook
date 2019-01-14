#ifndef BOSS_Afterburner_Particle_H
#define BOSS_Afterburner_Particle_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TDatabasePDG.h"
	#include "TParticlePDG.h"
	#include "TString.h"

// * ======================= * //
// * ------- GLOBALS ------- * //
// * ======================= * //
/// @addtogroup BOSS_Afterburner_settings
/// @{

	inline const TDatabasePDG gPDG; //!< A `ROOT` `TDatabasePDG` object that contains all info of particles. Has to be constructed once, which is why it is a global.

/// @}
// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    Container for particle data. In essence an extended container of `ROOT`'s `TParticlePDG`.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     November 16th, 2018
	 * @details  This object describes fundamental properties of a particle. Most of this information is accessed through a reference to its equivalent `TParticlePDG` in the `TDatabasePDG`. The rest is additional information, like a LaTeX formated string that gives the sign for the particle.
	 * @remark   @b DEVELOPMENTAL
	 */
	class Particle
	{
	public:
		// * Constructor and destructors *
		Particle(int pdgCode);
		Particle(const char* pdgCode);

		// * Information *
		bool IsLoaded() const;

		// * Getters *
		TParticlePDG* GetParticlePDG() const;
		int GetPDGCode() const;
		double GetMass() const;
		const char* GetNameLaTeX() const;
		const char* GetName() const;

	protected:
		// * Data members *
		TParticlePDG* fParticlePDG;
		//!< A pointer to its corresponding particle in the PDG.
		TString fParticleNameLaTeX;
		//!< Particle name in LaTeX format.

		// * Private members
		void Initialize();
		TString& DetermineNameLaTeX();

	};


/// @}
#endif