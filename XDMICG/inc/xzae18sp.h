/*E*/
/*  Fichier : $Id: xzae18sp.h,v 1.2 2009/03/03 11:09:30 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/03/03 11:09:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/10/94	: Creation
*			  remplace le poste operateur par le site 1.1
* JPL	14/01/2009	: Degroupage (lie a DEM 848) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajoute un evenement a la liste des evenements a traiter pour 
* le site concerne.
* 
* Sequence d'appel
* SP	XZAE18_Ajout_Liste_Evt_Traiter
* 
* Arguments en entree
* XDY_Site	va_Site_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	@va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insertion impossible,
*                 site local inconnu
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* Le resultat indique si l'ajout a ete realise ou non
* (XDC_OUI/XDC_NON)
* Un evenement de type operateur n'est pas ajoute dans la liste 
* des evenements a traiter s'il n'est pas sur le site local
* L'ajout est realise si l'evenement n'est pas deja dans la liste
* 
* Fonction
* Insert dans la table A_TRAITER de l'evenements pour le 
* site 
----------------------------------------------------------*/

 
#define XZAE18_Ajout_Liste_Evt_Traiter	"XZAE18" 
