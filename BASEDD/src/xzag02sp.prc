/*E*/
/*Fichier :  $Id: xzag02sp.prc,v 1.15 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.15 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/95	: Creation	(V 1.1)
* C.T.	03/05/96	: supprimer la table MES_NIV	(V 1.2)
* P.N.	09/10/96	: ajouter la table MES_KCS, NEC et QPC	(dem/1227) (V 1.3)
* P.N.	10/10/96	: ajouter la table MES_MET	(dem/1232) (V 1.4)
* P.N.  23/02/97	: ajouter la table ZD_HEU	(dem/1396) v1.5
* C.T.	02/02/98	: Ajouter les actions sur ACT_PAL (V1.6)
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT et FMC_DAT (V1.7)
* P.C.	06/02/02	: courrection pour HIS  V1.8 
* AAZ   28/08/2007  : Ajout action domaine SIG, ECL, VEN, ENR, INC DEM 663
* JMG	19/09/11 : ajout ALT_LOC
* VR	13/12/11	: Ajout PRV (DEM/1016) V2
* JPL	13/09/12	: Ajouter la table FMC_INC (DEM 1037)  1.12
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.13 DEM1284
* ABE	17/12/20	: Ajout IMU DEM-SAE155 1.14
* ABE	07/04/21	: Ajout PAU SONO DEM-SAE244 1.15
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Vider la base SVG
* 
* Sequence d'appel
* SP	XZAG02_Vider_Bse_SVG
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: vidage realise
* XDC_KO	: effectue sur un district
* 
* Conditions d'utilisation
* 
* Fonction
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;02
as
#ifdef HIST
	/*A Effacer toutes les donnees actuellement dans la base sauvegarde */
	delete SVG..FMC_GEN
	delete SVG..FMC_HIS
	delete SVG..FMC_ACC
	delete SVG..FMC_VEH
	delete SVG..FMC_VLN
	delete SVG..FMC_BOU
	delete SVG..FMC_DEL
	delete SVG..FMC_BAS
	delete SVG..FMC_OPR
	delete SVG..FMC_MET
	delete SVG..FMC_TRF
	delete SVG..FMC_TRH
	delete SVG..FMC_MNF
	delete SVG..FMC_ECH
	delete SVG..FMC_CNC
	delete SVG..FMC_NAT
	delete SVG..FMC_INC
	delete SVG..FMC_COM
	delete SVG..FMC_DAT
	delete SVG..ACT_GEN
	delete SVG..ACT_APL
	delete SVG..ACT_TFM
	delete SVG..ACT_TXT_TFM
	delete SVG..ACT_PMV
	delete SVG..ACT_PRV
	delete SVG..ACT_PIC
	delete SVG..ACT_PAL
	delete SVG..ACT_EXP_ADA
	delete SVG..ACT_TFM_EXP
	delete SVG..ACT_PMV_ADA
	delete SVG..ACT_FRG
	delete SVG..ACT_RMQ
	delete SVG..ACT_NAV
	delete SVG..ACT_TUB
	delete SVG..ACT_ECH
	delete SVG..ACT_INT
	delete SVG..ACT_SIS
	delete SVG..ACT_DEP
	delete SVG..ACT_GAR
	delete SVG..ACT_FAX
	delete SVG..ALT_EXP
	delete SVG..ALT_LOC
	delete SVG..EQT_DSP
	delete SVG..MES_TRA
	delete SVG..MES_POI
	delete SVG..MES_KCS
	delete SVG..MES_NEC
	delete SVG..MES_QPC
	delete SVG..MES_MET
    delete SVG..ZDP_HEU
    /* AAZ 28/08/2007 Suppression action domaine */
	delete SVG..ACT_GTC_SIG
	delete SVG..ACT_GTC_EXP
	delete SVG..ACT_GTC_ECL
	delete SVG..ACT_GTC_VEN
	delete SVG..ACT_GTC_ENR
	delete SVG..ACT_GTC_INC
    /* AAZ */
        delete SVG..ACT_CFE
        delete SVG..ACT_IMU
        delete SVG..ACT_PAU

	return XDC_OK
#else
	return XDC_NOK
#endif
go
