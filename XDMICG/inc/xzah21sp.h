/*E*/
/*  Fichier : $Id: xzah21sp.h,v 1.1 1994/12/05 17:56:48 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/05 17:56:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  30/11/94        : Creation      (V 1.1)
* C.T.  05/12/94        : Correction commentaire      (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Envoyer les etats des voies pour les radt
* et les dai pour le site local
*
* Sequence d'appel
* SP    XZAH21_Lire_Voies_Fmc
*
* Arguments en entree
* XDY_NomMachine	va_LocalTechnique_in
*
* Arguments en sortie
*
* liste des etats des voies des dai retournee :
* XDY_Eqt		va_Numero
* XDY_Octet		va_NumAnal
* XDY_Eqt		va_NumDAI
* XDY_Voie		va_BAU
* XDY_Voie		va_V1
* XDY_Voie		va_V2
* XDY_Voie		va_V3
* XDY_Voie		va_V4
* XDY_NomMachine	va_LocalTechnique
*
* liste des etats des voies des radt retournee :
* XDY_Eqt		va_Numero
* XDY_Sens		va_Sens
* XDY_Voie		va_BAU
* XDY_Voie		va_V1
* XDY_Voie		va_V2
* XDY_Voie		va_V3
* XDY_Voie		va_V4
* XDY_NomMachine	va_LocalTechnique
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null ou date demandee trop proche
*
* Conditions d'utilisation
* Si le nom du local technique n'est pas value alors seuls
* les etats des voies modifies sont retournes
* Sinon les etats de toutes les voies pour tous les dai et radt
* sont retournes
*
* Fonction
* select VOI_DAI et VOI_RAD
-------------------------------------------------------*/

 
#define XZAH21_Lire_Voies_Fmc	"XZAH;21" 
