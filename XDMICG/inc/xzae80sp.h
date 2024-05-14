/*E*/
/*  Fichier : $Id: xzae80sp.h,v 1.2 2007/04/18 14:25:59 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/04/18 14:25:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae80sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth…se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	14/10/94 : Creation	V 1.1 
* JPL	14/10/94 : Renommage (lie a DEM 638) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'intitule d'une fiche main courante 
* 
* Sequence d'appel
* SP	XZAE80_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* char(2)	va_CleEvt_in
* char(10)	va_Type_in
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* char(48)	va_Intitule_out 
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* 
* Conditions d'utilisation
* Cette procedure est utilis≈e par la procedure XZAE;02
* Elle est interne a la base de donn≈e.
* 
* Fonction
* format de l'intitule : N%4s %2s %10s %4s pr %3s.%3s, %11s
* (NumEvt Site Type Autoroute pr Kilometres, Sens)
* 
-------------------------------------------------------*/

 
#define XZAE80_Intitule_FMC	"XZAE80"
