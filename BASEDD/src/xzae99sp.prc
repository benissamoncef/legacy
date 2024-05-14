/*E*/
/*  Fichier : $Id: xzae99sp.prc,v 1.4 2011/09/23 09:27:13 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/09/23 09:27:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae99sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE  20/01/22: Creation AVA DEM-SAE367
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs cle et numéro d'une FMC à partir de l'id_AVA
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE99_Rechercher_NumFMC_par_idAVA
* 
* Arguments en entree
* XDY_Texte		va_id_ava_in
* 
* Arguments en sortie
* XDY_Entier	va_numEvt_out
* XDY_Octet	    va_cleEvt_out
* XDY_Entier	va_numEvtCause_out
* XDY_Octet	    va_cleEvtCause_out
* XDY_Entier    va_numAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Recherche dans FMC_VEH le numéro et la clé de la FMC à partir du numéro AVA
* 
* Fonction
* Select dans la table FMC_VEH
* where  id_ava
-------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAE99' and type = 'P')
	drop procedure XZAE99
go


create procedure XZAE99
	@va_id_ava_in	char(50) = null,
    @va_numEvt_out int = null output,
    @va_cleEvt_out tinyint = null output,
    @va_numEvtCause_out int = null output,
    @va_cleEvtCause_out tinyint = null output,
    @va_numAction_out int = null output
as	
	/*! transfo en parametres nulls bug applix */
	if @va_id_ava_in = XDC_CHAINE_VIDE select @va_id_ava_in = null

	/* test la presence des parametres obligatoires */
	if @va_id_ava_in = null
		return XDC_ARG_INV

	/*A lecture des informations sur complement vehicule */
    select 
        @va_numEvt_out = F.numero,
        @va_cleEvt_out = F.cle,
		@va_numEvtCause_out = F.cause,
        @va_cleEvtCause_out = F.cle_cause
        from EXP..FMC_VEH V, EXP..FMC_GEN F
    where V.id_ava = @va_id_ava_in and
	V.numero = F.numero and
    V.cle = F.cle

	/*A lecture des informations sur actions depannage */
    select 
        @va_numAction_out = A.numero
    from EXP..ACT_GEN A
    where A.evenement = @va_numEvt_out and
          A.cle = @va_cleEvt_out and
          A.type = XDC_ACT_DEPANNEUR

return XDC_OK
go