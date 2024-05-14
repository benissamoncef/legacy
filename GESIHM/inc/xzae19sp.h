/*E*/
/*  Fichier : $Id: xzae19sp.h,v 1.2 2008/12/02 14:40:13 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/12/02 14:40:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/10/94	: Creation
* JPL	02/12/08	: Degroupage de la procedure
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transmettre les droits d'enrichir des FMC au voisin
* sur le site local
-------------------------------------------------------
* Sequence d'appel
* SP	XZAE19_Transmettre_Tous_Droits_Enrichir_Au_Voisin
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
*
----------------------------------------------------*/

 
#define XZAE19_Transmettre_Tous_Droits_Enrichir_Au_Voisin	"XZAE19"
