//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// PrimaryGenerator.hh
// Developer : Chao Gu, Weizhi Xiong
// History:
//   Mar 2017, C. Gu, Add for DRad configuration.
//   Apr 2017, W. Xiong, Add target thickness profile.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "ConfigParser.h"

#include "TFoamIntegrand.h"
#include "Math/Interpolator.h"

#include "G4VPrimaryGenerator.hh"

#include "G4String.hh"

#define MaxN 10

class G4Event;

class TTree;
class TFoam;
class TRandom2;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGenerator : public G4VPrimaryGenerator
{
public:
    PrimaryGenerator(G4String type, G4double e, G4double x, G4double y, G4double z, G4double theta, G4double phi, G4bool rec, G4String par);
    PrimaryGenerator(G4String type, G4double e, G4double thlo, G4double thhi, G4bool rec, G4String par);
    virtual ~PrimaryGenerator();

    virtual void GeneratePrimaryVertex(G4Event *);

protected:
    void Register(TTree *);

    void Print() const;
    void Clear();

    bool fRegistered;

    G4String fEventType;

    G4bool fRecoilOn;
    G4String fRecoilParticle;

    int fN;
    int fPID[MaxN];
    double fX[MaxN], fY[MaxN], fZ[MaxN];
    double fE[MaxN], fMomentum[MaxN];
    double fTheta[MaxN], fPhi[MaxN];

    G4bool fTargetInfo;
    G4double fTargetCenter, fTargetHalfL;

private:
    G4double fEBeam;
    G4double fBeamX, fBeamY, fBeamZ;
    G4double fBeamTheta, fBeamPhi;
    G4double fBeamThetaLo, fBeamThetaHi;

    G4double fTargetMass;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class PRadPrimaryGenerator;

class TargetProfileIntegrand : public TFoamIntegrand
{
public:
    TargetProfileIntegrand(PRadPrimaryGenerator *gen);

    double Density(int nDim, double *arg);

    ROOT::Math::Interpolator *fTargetProfile;
    double fZMin, fZMax;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PRadPrimaryGenerator : public PrimaryGenerator
{
    friend class TargetProfileIntegrand;

public:
    PRadPrimaryGenerator(G4String type, G4bool rec, G4String par); // DRadPrimaryGenerator uses this
    PRadPrimaryGenerator(G4String type, G4bool rec, G4String par, G4String path, G4String profile);
    virtual ~PRadPrimaryGenerator();

    virtual void GeneratePrimaryVertex(G4Event *);

protected:
    virtual double GenerateZ();

    ConfigParser fParser;

private:
    void LoadTargetProfile(const std::string &path);

    ROOT::Math::Interpolator *fTargetProfile;
    double fZMin, fZMax;

    TFoam *fZGenerator;
    TRandom2 *fPseRan;
    TargetProfileIntegrand *fFoamI;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DRadPrimaryGenerator : public PRadPrimaryGenerator
{
public:
    DRadPrimaryGenerator(G4String type, G4bool rec, G4String par, G4String path);
    virtual ~DRadPrimaryGenerator();

protected:
    virtual double GenerateZ();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
