/*E*/
/*  Fichier : $Id: xzas29sp.prc,v 1.1 1994/12/29 17:23:37 fournel Exp $     Release : $Revision: 1.1 $       Date : $Date: 1994/12/29 17:23:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas29sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  19/12/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recapitulatif des fax.
*
* Sequence d'appel
* SP    XZAS29_Recapitulatif_Fax
*
* Arguments en entree :
*
* Arguments en sortie :
*
* Liste retournee :
* Liste des FAX envoyes par site au cours du mois precedent.
* le numero du site
* heure de l'envoi
* l'utilisateur (emetteur du fax)
* le destinataire du fax
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
*
* Fonction
* Consultation des fax envoyes par site (PC, CI) au cours du mois precedent.
* Ces fax sont historises comme un fil de l'eau.
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;29
as
	declare	@vl_DateCourante	datetime /* Date courante */

	/*A
	** Recherche de la date courante.
	*/
	select @vl_DateCourante = getdate()

	/*A
	** Calcul de la date d'il y a un mois.
	*/
	select @vl_DateCourante = dateadd(month,-1,@vl_DateCourante)

	select 	GEN.sit,
		GEN.heure_succes,
		GEN.operateur,
		APL.interlocuteur
	from EXP..ACT_GEN GEN, EXP..ACT_APL APL
	where GEN.type = XDC_ACT_FAX
	and   GEN.heure_succes >= @vl_DateCourante
	and   APL.actio = GEN.numero
	group by GEN.sit


	/* Fin de la procedure */
 	return XDC_OK
go
