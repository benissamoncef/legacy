/*E*/
/*  Fichier :  $Id: xzaq02sp.prc,v 1.4 1995/09/21 09:58:36 gesconf Exp $      Release : $Revision: 1.4 $       Date : $Date: 1995/09/21 09:58:36 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaq02.sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* BASEDD 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  22/12/94        : Creation      (V 1.1)
* B.G.  21/09/95	: correction (met a jour les taches sans les mettre NOK) (1.3)
* B.G.  21/09/95        : correction pb nommage table (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mise a jour des etats de toute les taches lors de l'arret de la
* SUPERVISION ou avant un calcul d'indice qualite
*
* Sequence d'appel
* SP    XZAQ02_Arret_SUPERV
*
* Arguments en entree :
*
* Arguments en sortie :
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
*
* Conditions d'utilisation
*
* Fonction
* effectue le calcul du temps cumulé dans la table SYS_INQ.
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAQ;02
as
	declare @vl_Date_Courante  datetime /* Date courante */


	/*A
	** Pour chacune des taches de la table SYS_INQ les totaux sont mis a jour.
	*/

	/*A
	** Mise ajour de la date courante.
	*/
	select @vl_Date_Courante = getdate()

	/* Calcul du decompte de temps entre la derniere */
	/* mise a jour et l'heure actuelle.              */
	update HIS..SYS_INQ
	set totalNOK=totalNOK+abs(datediff(minute,DerniereDate,@vl_Date_Courante)),
	    DerniereDate=@vl_Date_Courante
	where DernierEtat=XDC_NOK

	update HIS..SYS_INQ
	set totalOK=totalOK+abs(datediff(minute,DerniereDate,@vl_Date_Courante)),
	    DerniereDate=@vl_Date_Courante
	where DernierEtat=XDC_OK
	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
