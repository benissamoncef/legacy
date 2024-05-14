/*E*/
/*  Fichier : $Id: xzah22sp.h,v 1.1 1994/12/05 17:56:53 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/05 17:56:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  01/12/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Envoyer les etats des voies de l'analyseur demande
* dans le cas ou au moins une modification a ete 
* provoque par un evenement
*
* Sequence d'appel
* SP    XZAH22_Lire_Voies_Analyseur
*
* Arguments en entree
* XDY_Eqt		va_Numero_in	: numero analyseur
* XDY_Octet		va_Mode_in	: mode des enregistrement a lire
* XDY_Octet		va_NbVoies_in	: nombre de voies de l'analyseur
*
* Arguments en sortie
* XDY_Voie		va_VL_out
* XDY_Voie		va_VM1_out
* XDY_Voie		va_VM2_out
* XDY_Voie		va_VR_out
* XDY_Voie		va_BAU_out
* XDY_int		va_Resultat_out : indique si nouveaux etats de voie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_ARG_INV   : parametre d'entree null
*
* Conditions d'utilisation
* La procedure est utilisee en interne par XZAH;21
* La presence de la voie BAU est consideree toujours vrai s'il
* y a plus d'une voie surveillees par l'analyseur
*
* Fonction
-------------------------------------------------------*/

 
#define XZAH22_Lire_Voies_Analyseur	"XZAH;22" 
