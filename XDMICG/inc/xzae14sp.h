/*E*/
/*  Fichier : $Id: xzae14sp.h,v 1.2 2009/03/03 11:09:41 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/03/03 11:09:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	24/10/94	: Creation
* C.T.     	24/10/94	: Modif commentaire entete 1.1
* JPL		14/01/2009	: Degroupage de la procedure 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Acheve la fiche main courante (l'enleve de la liste 
* des fiches a traiter pour le poste operateur).
* 
* Sequence d'appel
* XZAE14_Achever_Fiche_MC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in
* 
* Arguments en sortie
* XDY_Entier	va_Resultat_out : XDC_OK si achevenement de l'evt est ok
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement inexistant, suppression de la liste des evts
*                         a traiter impossible, site local inconnu,
*                         positionner la date de cloture impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* XDC_REV_NON_DCL	: l'evenement a ete acheve mais pb de reveil
* 
* Conditions d'utilisation
* va_Resultat_out vaut :
*   XDC_OK
*   XZAEC_FMC_PAS_ENR	  : l'evenement ne peut pas etre modifie par cet operateur
*   XZAEC_FMC_ACT_NON_FIN : toutes les actions relatives a l'evenement ne sont 
*   	      		    terminees
*   XZAEC_FMC_A_TRT	  : evenement est a traiter dans un district
*   XZAEC_FMC_NON_FIN	  : evenement non termine
* 
* Fonction
* 	-verifier que toutes les actions du site liees 
* 	 a la FMC sont finies, sinon refuser
* 	-au district, si l'operateur a le droit d'enrichir, 
* 	 transmettre ce droit au CI
* 	-au CI, verifier que l'evenement est fini
* 	-supprimer la FMC de la liste des evenements a traiter
-------------------------------------------------------------*/

 
#define XZAE14_Achever_Fiche_MC	"XZAE14" 
